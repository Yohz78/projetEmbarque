// Select data where the temperature is greater than a certain value
double temp = 25;
std::string query = "SELECT * FROM data WHERE temp > ?";
sqlite3_stmt *stmt;
int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
if (rc) {
    std::cerr << "Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
    sqlite3_close(db);
    return 1;
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
    return 1;
}
sqlite3_finalize(stmt);
