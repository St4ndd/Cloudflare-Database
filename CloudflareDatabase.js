const sqlite3 = require('sqlite3').verbose();
const http = require('http');

const db = new sqlite3.Database('cloudflare.db');

const requestListener = (req, res) => {
    try {
        const url = new URL(req.url, `http://${req.headers.host}`);

        if (url.pathname.startsWith("/setvar")) {
            const parts = url.pathname.split('/').filter(p => p);
            if (parts.length < 3) {
                res.writeHead(400);
                res.end("Error: Key or Value missing");
                return;
            }
            const [ , key, value ] = parts;
            db.run("INSERT OR REPLACE INTO variables (key, value) VALUES (?, ?)", [key, value], (err) => {
                if (err) {
                    res.writeHead(500);
                    res.end(`Error: ${err.message}`);
                } else {
                    res.writeHead(200);
                    res.end(`Variable '${key}' set to '${value}'.`);
                }
            });
        } else if (url.pathname.startsWith("/getvar")) {
            const parts = url.pathname.split('/').filter(p => p);
            if (parts.length < 2) {
                res.writeHead(400);
                res.end("Error: Key missing");
                return;
            }
            const key = parts[1];
            db.get("SELECT value FROM variables WHERE key = ?", [key], (err, row) => {
                if (err) {
                    res.writeHead(500);
                    res.end(`Error: ${err.message}`);
                } else if (row) {
                    res.writeHead(200);
                    res.end(row.value);
                } else {
                    res.writeHead(404);
                    res.end("Variable not found");
                }
            });
        } else {
            res.writeHead(200);
            res.end("Hello from Cloudflare D1!");
        }
    } catch (error) {
        res.writeHead(500);
        res.end(`Error: ${error.message}`);
    }
};

const server = http.createServer(requestListener);
server.listen(3000, () => {
    console.log("Server running at http://localhost:3000/");
});
