#include <sqlite3.h>
#include <jsoncpp/json/json.h>
#include <fstream>
#include <string>
#include <vector>
#include <iostream>


/**
 * @brief This function import the data from a JSON vector into the db
 *        Same for the database name
 * @param jsonWrite 
 */
void import_json_vector(std::vector<Json::Value> &jsonWrite) {
    // Open the JSON file
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;

    // Connect to the SQLite database
    rc = sqlite3_open("data.db", &db);

    if(rc){
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(0);
    }

    // Check if the table exists
    const char *table_name = "sensor_data";
    const char *db_name = "main";
    const char *column_name = NULL;
    rc = sqlite3_table_column_metadata(db, db_name, table_name, column_name, NULL, NULL, NULL, NULL, NULL);
    if(rc == SQLITE_OK) {
        printf("Table already exists\n");
    }
    else {
        // Create table
        char *sql = "CREATE TABLE sensor_data (date TEXT,temperature REAL,pressure REAL,humidity REAL,x INTEGER,y INTEGER,z INTEGER,mvt INTEGER)";
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
    for (auto item : jsonWrite) {
        sqlite3_stmt *stmt;
        std::string date = item["date"].asString();
        double temperature = item["BME"]["temperature"].asDouble();
        double pressure = item["BME"]["pressure"].asDouble();
        double humidity = item["BME"]["humidity"].asDouble();
        int x = item["HMC"]["x"].asInt();
        int y = item["HMC"]["y"].asInt();
        int z = item["HMC"]["z"].asInt();
        int mvt = item["HCSR"]["mvt"].asInt();
        const char *sql = "INSERT INTO sensor_data (date, temperature, pressure, humidity, x, y, z, mvt) VALUES (?,?,?,?,?,?,?,?)";
        rc = sqlite3_prepare_v2(db, sql, -1, &stmt,NULL);
        if( rc != SQLITE_OK ){
            std::cout << "stderr, Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        }

        sqlite3_bind_text(stmt, 1, date.c_str() , -1, NULL);
        sqlite3_bind_double(stmt, 2,temperature );
        sqlite3_bind_double(stmt, 3,pressure);
        sqlite3_bind_double(stmt, 4,humidity);
        sqlite3_bind_int(stmt, 5,x );
        sqlite3_bind_int(stmt, 6,y );
        sqlite3_bind_int(stmt, 7,z );
        sqlite3_bind_int(stmt, 8,mvt);

        rc = sqlite3_step(stmt);
        if (rc != SQLITE_DONE) {
        fprintf(stderr, "Failed to insert object: %s\n", sqlite3_errmsg(db));
        exit(0);
    }

        sqlite3_finalize(stmt);
    }
    sqlite3_close(db);
}