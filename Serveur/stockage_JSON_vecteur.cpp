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
    std::cout << "timestamp: " << e["timestamp"] << std::endl;
    std::cout << "humidity: " << e["humidity"] << std::endl;
    std::cout << "temp: " << e["temp"] << std::endl;
    std::cout << "pression: " << e["pression"] << std::endl;
    std::cout << "x: " << e["x"] << std::endl;
    std::cout << "y: " << e["y"] << std::endl;
    std::cout << "z: " << e["z"] << std::endl;
    std::cout << "mvt: " << e["mvt"] << std::endl;
}
sqlite3* db;
int rc;
rc = sqlite3_open("data.db", &db);
if (rc) {
    std::cout << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
    sqlite3_close(db);
    return 1;
}
char* sql = "CREATE TABLE IF NOT EXISTS data (id INTEGER PRIMARY KEY, timestamp DATE, humidity INTEGER, temp INTEGER, pression INTEGER, x INTEGER, y INTEGER, z INTEGER, mvt BOOLEAN);";
rc = sqlite3_exec(db, sql, NULL, 0, &sql);
if (rc != SQLITE_OK) {
    std::cout << "SQL Error: " << sqlite3_errmsg(db) << std::endl;
    sqlite3_free(sql);
    sqlite3_close(db);
    return 1;
}
sqlite3_stmt* stmt;
sql = "INSERT INTO data (timestamp, humidity, temp, pression, x, y, z, mvt) VALUES (?,?,?,?,?,?,?,?);";
rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
if (rc != SQLITE_OK) {
    std::cout << "SQL Error: " << sqlite3_errmsg(db) << std::endl;
    sqlite3_free(sql);
    sqlite3_close(db);
    return 1;
}
sqlite
