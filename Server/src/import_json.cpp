#include <sqlite3.h>
#include <jsoncpp/json/json.h>
#include <fstream>

int main() {
    // Open the JSON file
    std::ifstream jsonFile("data.json");
    Json::Value data;
    jsonFile >> data;

    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;

    // Connect to the SQLite database
    rc = sqlite3_open("data.db", &db);

    if(rc){
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return(1);
    }

    // Check if the table exists
    char *table_name = "sensor_data";
    char *db_name = "data.db";
    char *column_name = NULL;
    rc = sqlite3_table_column_metadata(db, db_name, table_name, column_name, NULL, NULL, NULL, NULL, NULL);
    if(rc == SQLITE_OK) {
        printf("Table already exists\n");
    }
    else {
        // Create table
        char *sql = "CREATE TABLE data (field1 TEXT, field2 INTEGER, field3 REAL)";
        rc = sqlite3_exec(db, sql, NULL, 0, &zErrMsg);

        if( rc != SQLITE_OK ){
            fprintf(stderr, "SQL error: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
        }
        else {
            fprintf(stdout, "Table created successfully\n");
        }
    }

    // Insert data into the table
    for (auto item : data) {
        std::string field1 = item["field1"].asString();
        int field2 = item["field2"].asInt();
        double field3 = item["field3"].asDouble();
        char *sql = sqlite3_mprintf("INSERT INTO data (field1, field2, field3) VALUES ('%q', %d, %f)", field1.c_str(), field2, field3);
        rc = sqlite3_exec(db, sql, NULL, 0, &zErrMsg);
        if( rc != SQLITE_OK ){
            fprintf(stderr, "SQL error: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
        }
        sqlite3_free(sql);
    }
    sqlite3_close(db);
    return 0;
}