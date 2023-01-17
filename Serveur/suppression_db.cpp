// Delete data where the timestamp is greater than a certain value
double timestamp = 1500000000;
std::string query = "DELETE FROM data WHERE timestamp > ?";
sqlite3_stmt *stmt;
int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
if (rc) {
    std::cerr << "Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
    sqlite3_close(db);
    return 1;
}
sqlite3_bind_double(stmt, 1, timestamp);

rc = sqlite3_step(stmt);
if (rc != SQLITE_DONE) {
    std::cerr << "Error deleting data: " << sqlite3_errmsg(db) << std::endl;
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return 1;
}
sqlite3_finalize(stmt);
