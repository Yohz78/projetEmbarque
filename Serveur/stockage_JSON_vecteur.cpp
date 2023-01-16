#include <iostream>
#include <vector>
#include <map>

int main() {
    std::vector<std::map<std::string, double>> data;

    std::map<std::string, double> entry;
    entry["timestamp"] = 0;
    entry["humidity"] = 0;
    entry["temp"] = 0;
    entry["pression"] = 0;
    entry["x"] = 0;
    entry["y"] = 0;
    entry["z"] = 0;
    entry["mvt"] = 0;

    data.push_back(entry);

    std::cout << "Data stored in vector: " << std::endl;
    for (const auto &e : data) {
        std::cout << "timestamp: " << entry["timestamp"] << std::endl;
        std::cout << "humidity: " << entry["humidity"] << std::endl;
        std::cout << "temp: " << entry["temp"] << std::endl;
        std::cout << "pression: " << entry["pression"] << std::endl;
        std::cout << "x: " << entry["x"] << std::endl;
        std::cout << "y: " << entry["y"] << std::endl;
        std::cout << "z: " << entry["z"] << std::endl;
        std::cout << "mvt: " << entry["mvt"] << std::endl;
    }
    return 0;
}