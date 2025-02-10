#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

void handle_request(const char *request)
{
    sqlite3 *db;
    char *err_msg = 0;
    int rc = sqlite3_open("cloudflare.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        return;
    }

    if (strstr(request, "/setvar") != NULL) {
        // Parse and set the variable
        char key[256], value[256];
        sscanf(request, "/setvar/%255s/%255s", key, value);

        char sql[512];
        snprintf(sql, sizeof(sql), "INSERT OR REPLACE INTO variables (key, value) VALUES ('%s', '%s');", key, value);
        
        rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "SQL error: %s\n", err_msg);
            sqlite3_free(err_msg);
        } else {
            printf("Variable '%s' set to '%s'.\n", key, value);
        }
    }
    else if (strstr(request, "/getvar") != NULL) {
        // Parse and get the variable
        char key[256];
        sscanf(request, "/getvar/%255s", key);

        char *sql = sqlite3_mprintf("SELECT value FROM variables WHERE key = '%s';", key);
        sqlite3_stmt *stmt;
        rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "Failed to fetch value: %s\n", sqlite3_errmsg(db));
        } else {
            if (sqlite3_step(stmt) == SQLITE_ROW) {
                printf("Value for %s: %s\n", key, sqlite3_column_text(stmt, 0));
            } else {
                printf("Variable not found\n");
            }
        }
        sqlite3_finalize(stmt);
    }
    else {
        printf("Hello from Cloudflare D1!\n");
    }

    sqlite3_close(db);
}

int main() {
    // Simulate request handling
    handle_request("/setvar/myKey/myValue");
    handle_request("/getvar/myKey");
    return 0;
}
