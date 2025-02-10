import sqlite3
from urllib.parse import urlparse

def handle_request(request):
    conn = sqlite3.connect('cloudflare.db')
    cursor = conn.cursor()

    if request.startswith("/setvar"):
        parts = request.split('/')[1:]
        if len(parts) < 2:
            print("Error: Key or Value missing")
            return
        key, value = parts[0], parts[1]
        cursor.execute("INSERT OR REPLACE INTO variables (key, value) VALUES (?, ?)", (key, value))
        conn.commit()
        print(f"Variable '{key}' set to '{value}'.")

    elif request.startswith("/getvar"):
        parts = request.split('/')[1:]
        if len(parts) < 1:
            print("Error: Key missing")
            return
        key = parts[0]
        cursor.execute("SELECT value FROM variables WHERE key = ?", (key,))
        row = cursor.fetchone()
        if row:
            print(row[0])
        else:
            print("Variable not found")
    else:
        print("Hello from Cloudflare D1!")

    conn.close()

# Example usage
handle_request("/setvar/myKey/myValue")
handle_request("/getvar/myKey")
