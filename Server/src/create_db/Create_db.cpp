#include <sqlite3.h>
#include <string>
#include <iostream>

void createDatabase(const std::string &dbName) {
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;

    // Connect to the SQLite database
    rc = sqlite3_open(dbName.c_str(), &db);

    if(rc){
        std::cout << "stderr, Can't open database: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return;
    }
    std::string sql_string = "CREATE TABLE IF NOT EXISTS sensor_data (timestamp TEXT,temperature REAL,pressure REAL,humidity REAL,x INTEGER,y INTEGER,z INTEGER,mvt INTEGER)";
    char *sql = sql_string.c_str();
    rc = sqlite3_exec(db, sql, NULL, 0, &zErrMsg);
    
    if( rc != SQLITE_OK ){
        std::cout << "stderr, SQL error:" <<  zErrMsg <<  std::endl;
        sqlite3_free(zErrMsg);
    }
    else {
        std::cout << "stdout, Table created successfully\n" << std::endl;
    }

    sqlite3_close(db);
}