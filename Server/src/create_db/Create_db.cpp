#include <sqlite3.h>

void createDatabase(const std::string &dbName) {
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;

    // Connect to the SQLite database
    rc = sqlite3_open(dbName.c_str(), &db);

    if(rc){
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }
    char *sql = "CREATE TABLE IF NOT EXISTS sensor_data (timestamp TEXT,
                                                        temperature REAL,
                                                        pressure REAL,
                                                        humidity REAL,
                                                        x INTEGER,
                                                        y INTEGER,
                                                        z INTEGER,
                                                        mvt INTEGER)";
    rc = sqlite3_exec(db, sql, NULL, 0, &zErrMsg);
    
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else {
        fprintf(stdout, "Table created successfully\n");
    }

    sqlite3_close(db);
}