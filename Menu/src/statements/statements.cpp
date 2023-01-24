#include <sqlite3.h>
#include <iostream>
#include <iomanip>

/**
 * @brief This function does a sqlite3 Select for the BME data
 * 
 */
void afficheBME() {
    sqlite3 *db;
    int rc = sqlite3_open("../Server/data.db", &db);
    char *zErrMsg = 0;
    char **result;
    int rows, columns;

    // Execute the SELECT statement
    rc = sqlite3_get_table(db, "SELECT date, temperature, pressure, humidity FROM sensor_data", &result, &rows, &columns, &zErrMsg);
    if (rc != SQLITE_OK) {
        std::cout << "stderr, SQL error: " <<  zErrMsg << std::endl;
        sqlite3_free(zErrMsg);
    } else {
        for (int i = 1; i <= rows; i++) {
            std::cout << "DATE:" << result[i * columns] << std::endl;
            std::cout << "---------> temperature : " <<  std::fixed << std::setprecision(2) << result[i * columns + 1] << std::endl;
            std::cout << "------------> pression : " <<  std::fixed << std::setprecision(2) << result[i * columns + 2] << std::endl;
            std::cout << "------------> humidité : " << result[i * columns + 3] << "%" << std::endl;
        }
    }
    sqlite3_free_table(result);
}

/**
 * @brief This function does a sqlite3 Select for the HCSR data
 * 
 */
void afficheHCSR() {
    sqlite3 *db;
    int rc = sqlite3_open("../Server/data.db", &db);
    char *zErrMsg = 0;
    char **result;
    int rows, columns;

    // Execute the SELECT statement
    rc = sqlite3_get_table(db, "SELECT date, mvt FROM sensor_data", &result, &rows, &columns, &zErrMsg);
    if (rc != SQLITE_OK) {
        std::cout << "stderr, SQL error: " <<  zErrMsg << std::endl;
        sqlite3_free(zErrMsg);
    } else {
        for (int i = 1; i <= rows; i++) {
            std::cout << "DATE:" << result[i * columns] << std::endl;
            std::cout << "mvt : " << result[i * columns + 1] << std::endl;
        }
    }
    sqlite3_free_table(result);
}

/**
 * @brief This function does a sqlite3 Select for the HMC data
 * 
 */
void afficheHMC() {
    sqlite3 *db;
    int rc = sqlite3_open("../Server/data.db", &db);
    char *zErrMsg = 0;
    char **result;
    int rows, columns;

    // Execute the SELECT statement
    rc = sqlite3_get_table(db, "SELECT date, x, y, z FROM sensor_data", &result, &rows, &columns, &zErrMsg);
    if (rc != SQLITE_OK) {
        std::cout << "stderr, SQL error: " <<  zErrMsg << std::endl;
        sqlite3_free(zErrMsg);
    } else {
        for (int i = 1; i <= rows; i++) {
            std::cout << "DATE:" << result[i * columns] << std::endl;
            std::cout << "x: " << result[i * columns+ 1] << std::endl;
            std::cout << "y: " << result[i * columns + 2] << std::endl;
            std::cout << "z: " << result[i * columns + 3] << std::endl;
        }
    }
    sqlite3_free_table(result);
}

/**
 * @brief This function does a sqlite3 Select for all the data
 * 
 */
void afficheAllData() {
    sqlite3 *db;
    int rc = sqlite3_open("../Server/data.db", &db);
    char *zErrMsg = 0;
    char **result;
    int rows, columns;

    // Execute the SELECT statement
    rc = sqlite3_get_table(db, "SELECT date, temperature, pressure, humidity, mvt, x, y, z FROM sensor_data", &result, &rows, &columns, &zErrMsg);
    if (rc != SQLITE_OK) {
        std::cout << "stderr, SQL error: " <<  zErrMsg << std::endl;
        sqlite3_free(zErrMsg);
    } else {
        for (int i = 1; i <= rows; i++) {
            std::cout << "DATE: " << result[i * columns + 0] << std::endl
            << " || température : " << result[i * columns + 1] 
            << " || pression : " << result[i * columns + 2] 
            << " || humidité : " << result[i * columns + 3] << std::endl
            << " || mvt : "<< result[i * columns + 4] << std::endl
            << " || x : "<< result[i * columns + 5] 
            << " || y : "<< result[i * columns + 6] 
            << " || z : "<< result[i * columns + 7] 
            << std::endl;
        }
    }
    sqlite3_free_table(result);
}