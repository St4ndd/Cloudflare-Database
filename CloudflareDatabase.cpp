#include <iostream>
#include <sqlite3.h>
#include <string>

void handleRequest(const std::string& request)
{
    sqlite3* db;
    char* errMsg = 0;
    int rc = sqlite3_open("cloudflare.db", &db);

    if (rc != SQLITE_OK) {
        std::cerr << "Cannot open database: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    if (request.find("/setvar") != std::string::npos) {
        std::string key, value;
        size_t startPos = request.find("/setvar/") + 8;
        size_t endPos = request.find("/", startPos);
        key = request.substr(startPos, endPos - startPos);
        value = request.substr(endPos + 1);

        std::string sql = "INSERT OR REPLACE INTO variables (key, value) VALUES ('" + key + "', '" + value + "');";
        rc = sqlite3_exec(db, sql.c_str(), 0, 0, &errMsg);
        if (rc != SQLITE_OK) {
            std::cerr << "SQL error: " << errMsg << std::endl;
            sqlite3_free(errMsg);
        } else {
            std::cout << "Variable '" << key << "' set to '" << value << "'." << std::endl;
        }
    }
    else if (request.find("/getvar") != std::string::npos) {
        std::string key;
        size_t startPos = request.find("/getvar/") + 8;
        key = request.substr(startPos);

        std::string sql = "SELECT value FROM variables WHERE key = '" + key + "';";
        sqlite3_stmt* stmt;
        rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);

        if (rc != SQLITE_OK) {
            std::cerr << "Failed to fetch value: " << sqlite3_errmsg(db) << std::endl;
        } else {
            if (sqlite3_step(stmt) == SQLITE_ROW) {
                std::cout << "Value for " << key << ": " << sqlite3_column_text(stmt, 0) << std::endl;
            } else {
                std::cout << "Variable not found" << std::endl;
            }
        }
        sqlite3_finalize(stmt);
    } else {
        std::cout << "Hello from Cloudflare D1!" << std::endl;
    }

    sqlite3_close(db);
}

int main() {
    // Simulate a request
    handleRequest("/setvar/myKey/myValue");
    handleRequest("/getvar/myKey");
    return 0;
}
