#include <iostream>
#include <vector>
#include <map>
#include <sqlite3.h>

void perform_database_operation(int operation)
{
    std::vector<std::map<std::string, double>> data;

    // Fill the data vector with your data
    // ...

    sqlite3 *db;
    int rc = sqlite3_open("data.db", &db);
    if (rc) {
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    sqlite3_stmt *stmt;
    switch (operation) {
        case 1: 
            // Insert data into the database
            rc = sqlite3_prepare_v2(db, "INSERT INTO data (timestamp, humidity, temp, pression, x, y, z, mvt) VALUES (?, ?, ?, ?, ?, ?, ?, ?)", -1, &stmt, nullptr);
            if (rc) {
                std::cerr << "Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
                sqlite3_close(db);
                return;
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
                    std::cerr << "Error inserting data: " << sqlite3_errmsg(db) << std::endl;
                    sqlite3_finalize(stmt);
                    sqlite3_close(db);
                    return;
                }
                sqlite3_reset(stmt);
            }
            sqlite3_finalize(stmt);
            break;

        case 2: 
            // Delete data where the timestamp is greater than a certain value
            double timestamp = 1500000000;
            std::string query = "DELETE FROM data WHERE timestamp > ?";
            rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
            if (rc) {
                std::cerr << "Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
                sqlite3_close(db);
                return;
            }
            sqlite3_bind_double(stmt, 1, timestamp);

            rc = sqlite3_step(stmt);

            if (rc != SQLITE_DONE) {
                std::cerr << "Error deleting data: " << sqlite3_errmsg(db) << std::endl;
                sqlite3_finalize(stmt);
                sqlite3_close(db);
                return;
            }
            sqlite3_finalize(stmt);
            break;

        case 3: 
            // Select data where the temperature is greater than a certain value
            double temp = 25;
            std::string query = "SELECT * FROM data WHERE temp > ?";
            rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
            if (rc) {
                std::cerr << "Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
                sqlite3_close(db);
                return;
            }
            sqlite3_bind_double(stmt, 1, temp);

            while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
                double timestamp = sqlite3_column_double(stmt, 0);
                double humidity = sqlite3_column_double(stmt, 1);
                double temp = sqlite3_column_double(stmt, 2);
                double pression = sqlite3_column_double(stmt, 3);
                double x = sqlite3_column_double(stmt, 4);
                double y = sqlite3_column_double(stmt, 5);
                double z = sqlite3_column_double(stmt, 6);
                double mvt = sqlite3_column_double(stmt, 7);

                // Do something with the selected data
                // ...
            }
            if (rc != SQLITE_DONE) {
                std::cerr << "Error selecting data: " << sqlite3_errmsg(db) << std::endl;
                sqlite3_finalize(stmt);
                sqlite3_close(db);
                return;
            }
            sqlite3_finalize(stmt);
            break;
        }
}  