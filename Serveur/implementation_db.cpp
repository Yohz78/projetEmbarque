#include <iostream>
#include <vector>
#include <map>
#include <sqlite3>

int main() {
    std::vector<std::map<std::string, double>> data;

    // Fill the data vector with your data
    // ...

    sqlite3 *db;
    int rc = sqlite3_open("data.db", &db);
    if (rc) {
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
        return 1;
    }

    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, "INSERT INTO data (timestamp, humidity, temp, pression, x, y, z, mvt) VALUES (?, ?, ?, ?, ?, ?, ?, ?)", -1, &stmt, nullptr);
    if (rc) {
        std::cerr << "Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return 1;
    }

    for (const auto &entry : data) {
        sqlite3_bind_double(stmt, 1, entry["timestamp"]);
        sqlite3_bind_double(stmt, 2, entry["humidity"]);
        sqlite3_bind_double(stmt, 3, entry["temp"]);
        sqlite3_bind_double(stmt, 4, entry["pression"]);
        sqlite3_bind_double(stmt, 5, entry["x"]);
        sqlite3_bind_double(stmt, 6, entry["y"]);
        sqlite3_bind_double(stmt, 7, entry["z"]);
        sqlite3_bind_double(stmt, 8, entry["mvt"]);

        rc = sqlite3_step(stmt);
        if (rc != SQLITE_DONE) {
            std::cerr << "Error inserting data: " << sqlite3
        }
};