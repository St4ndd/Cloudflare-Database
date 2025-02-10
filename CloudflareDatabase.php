<?php
function handleRequest($request) {
    $db = new SQLite3('cloudflare.db');

    if (strpos($request, '/setvar') !== false) {
        // Set variable
        preg_match('/\/setvar\/([^\/]+)\/([^\/]+)/', $request, $matches);
        if (count($matches) < 3) {
            echo "Error: Key or Value missing";
            return;
        }

        $key = $matches[1];
        $value = $matches[2];

        $stmt = $db->prepare('INSERT OR REPLACE INTO variables (key, value) VALUES (:key, :value)');
        $stmt->bindValue(':key', $key, SQLITE3_TEXT);
        $stmt->bindValue(':value', $value, SQLITE3_TEXT);
        $stmt->execute();

        echo "Variable '$key' set to '$value'.";
    }
    elseif (strpos($request, '/getvar') !== false) {
        // Get variable
        preg_match('/\/getvar\/([^\/]+)/', $request, $matches);
        if (count($matches) < 2) {
            echo "Error: Key missing";
            return;
        }

        $key = $matches[1];
        $stmt = $db->prepare('SELECT value FROM variables WHERE key = :key');
        $stmt->bindValue(':key', $key, SQLITE3_TEXT);
        $result = $stmt->execute();

        $row = $result->fetchArray(SQLITE3_ASSOC);
        if ($row) {
            echo $row['value'];
        } else {
            echo "Variable not found";
        }
    } else {
        echo "Hello from Cloudflare D1!";
    }
}

// Example Usage
handleRequest("/setvar/myKey/myValue");
handleRequest("/getvar/myKey");
?>
