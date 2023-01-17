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

    // Check if table exist and if not, create table
    char *table_name = "sensor_data";
    char *db_name = "main";
    char *column_name = NULL;
    rc = sqlite3_table_column_metadata(db, db_name, table_name, column_name, NULL, NULL, NULL, NULL, NULL);
    if(rc == SQLITE_OK) {
        printf("Table already exists\n");
    }
    else {
    char *sql = "CREATE TABLE sensor_data (timestamp TEXT,~
                                             temperature REAL,
                                              pressure REAL,
                                               humidity REAL,
                                                x INTEGER,
                                                 y INTEGER,
                                                  z INTEGER,
                                                   mvt INTEGER)";
    rc = sqlite3_exec(db, sql, NULL, 0, &zErrMsg);
    }

    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else {
        fprintf(stdout, "Table created successfully\n");
    }

    sqlite3_close(db);
}