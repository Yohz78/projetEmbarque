#include <sqlite3.h>
#include <jsoncpp/json/json.h>
#include <fstream>
#include <string>



/**
 * @brief This function import the data from the json file into the db
 *        The name of the json file is hard-coded (even if in reality you might to avoid that)
 *        Same for the database name
 */
void import_json() {
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
        char *sql = "CREATE TABLE sensor_data (timestamp TEXT,~
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
    }

    // Insert data into the table
    for (auto item : data) {
        std::string timestamp = item["timestamp"].asString();
        double temperature = item["temperature"].asDouble();
        double pressure = item["pressure"].asDouble();
        double humidity = item["humidity"].asDouble();
        int x = item["x"].asInt();
        int y = item["y"].asInt();
        int z = item["z"].asInt();
        int mvt = item["mvt"].asInt();
        char *sql = sqlite3_mprintf("INSERT INTO data (timestamp, temperature, pressure, humidity, x, y, z, mvt) VALUES (%s, %f, %f, %f, %d, %d, %d, %d)", timestamp, temperature, pressure, humidity, x, y, z, mvt);
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