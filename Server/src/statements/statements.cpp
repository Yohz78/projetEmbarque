#include <sqlite3.h>

void afficheBME() {
    sqlite3 *db;
    int rc = sqlite3_open("data.db", &db);
    char *zErrMsg = 0;
    char **result;
    int rows, columns;

    // Execute the SELECT statement
    rc = sqlite3_get_table(db, "SELECT timestamp, temperature, pressure, humidity FROM sensor_data", &result, &rows, &columns, &zErrMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        for (int i = 1; i <= rows; i++) {
            std::cout << "le " << result[i * columns + 0] << " la temperature était de " << result[i * columns + 1] << " la pression de " << result[i * columns + 2] << " et l'humidité de " << result[i * columns + 3] << "%" << std::endl;
        }
    }
    sqlite3_free_table(result);
}

void afficheHCSR() {
    sqlite3 *db;
    int rc = sqlite3_open("data.db", &db);
    char *zErrMsg = 0;
    char **result;
    int rows, columns;

    // Execute the SELECT statement
    rc = sqlite3_get_table(db, "SELECT mvt FROM sensor_data", &result, &rows, &columns, &zErrMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        for (int i = 1; i <= rows; i++) {
            std::cout << "mvt : " << result[i * columns] << std::endl;
        }
    }
    sqlite3_free_table(result);
}

void afficheHMC() {
    sqlite3 *db;
    int rc = sqlite3_open("data.db", &db);
    char *zErrMsg = 0;
    char **result;
    int rows, columns;

    // Execute the SELECT statement
    rc = sqlite3_get_table(db, "SELECT x, y, z FROM sensor_data", &result, &rows, &columns, &zErrMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        for (int i = 1; i <= rows; i++) {
            std::cout << "x: " << result[i * columns] << " y: " << result[i * columns + 1] << " z: " << result[i * columns + 2] << std::endl;
        }
    }
    sqlite3_free_table(result);
}

void afficheAllData() {
    sqlite3 *db;
    int rc = sqlite3_open("data.db", &db);
    char *zErrMsg = 0;
    char **result;
    int rows, columns;

    // Execute the SELECT statement
    rc = sqlite3_get_table(db, "SELECT timestamp, temperature, pressure, humidity, mvt, x, y, z FROM sensor_data", &result, &rows, &columns, &zErrMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        for (int i = 1; i <= rows; i++) {
            std::cout << "le " << result[i * columns + 0] << " la température était de " << result[i * columns + 1] << " la pression de " << result[i * columns + 2] << " l'humidité de " << result[i * columns + 3] << " mvt : "<< result[i * columns + 4] << " x : "<< result[i * columns + 5] << " y : "<< result[i * columns + 6] << " z : "<< result[i * columns + 7] << std::endl;
        }
    }
    sqlite3_free_table(result);
}