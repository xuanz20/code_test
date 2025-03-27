#include<algorithm>
#include<fstream>
#include<iostream>
#include<sstream>
#include<map>
#include<vector>

using namespace std;

struct Trade {
    uint64_t timestamp;
    string symbol;
    int64_t quantity;
    int64_t price;
};

struct Data {
    uint64_t max_time_gap = 0;
    uint64_t last_timestamp = 0;
    int64_t total_volume = 0;
    int64_t total_price = 0;
    int64_t max_price = 0;
};

void process(const string& input_file, const string& output_file) {
    map<string, Data> symbol_data;
    ifstream input(input_file);
    string line;

    while (getline(input, line)) {
        istringstream ss(line);
        Trade trade;

        getline(ss, line, ','); 
        trade.timestamp = stoull(line);
        getline(ss, trade.symbol, ',');
        getline(ss, line, ','); 
        trade.quantity = stoll(line);
        getline(ss, line); 
        trade.price = stoll(line);

        auto& data = symbol_data[trade.symbol];
        
        // update max time gap
        if (data.last_timestamp > 0) {
            data.max_time_gap = max(data.max_time_gap, trade.timestamp - data.last_timestamp);
        }

        data.total_volume += trade.quantity;
        data.total_price += trade.quantity * trade.price;
        data.max_price = max(data.max_price, trade.price);
        data.last_timestamp = trade.timestamp;
    }

    ofstream output(output_file);
    for (const auto& [symbol, data] : symbol_data) {
        int64_t weighted_avg_price = data.total_price / data.total_volume;

        output << symbol << ','
               << data.max_time_gap << ','
               << data.total_volume << ','
               << weighted_avg_price << ','
               << data.max_price << '\n'; 
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << "<input.csv> <output.csv>" << std::endl;
        return 1;
    }

    process(argv[1], argv[2]);
    return 0;
}