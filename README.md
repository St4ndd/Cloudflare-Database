# Simple-Cloudflare-D1-Database


## 1. Create a Cloudflare Worker

1. **Log in to your Cloudflare account.**
   - Navigate to the Cloudflare dashboard.

2. **Go to the "Workers" section:**
   - From the sidebar, click on "Workers & Pages."
   - Click the "Create a Worker" button.

3. **Paste the worker.js Code:**
   - Copy and paste the provided Cloudflare Worker code (the one that handles `/setvar/{key}/{value}` and `/getvar/{key}` endpoints) into the online editor.
   - This worker code uses the D1 Database binding to store and retrieve variables.


## 2. Configure the Cloudflare Worker with D1 Database

1. **Set up a D1 Database:**
   - Follow the [Cloudflare D1 documentation](https://developers.cloudflare.com/d1/) to create a D1 database.

2. **Create a binding to your D1 Database:**
   - In the Cloudflare Workers dashboard, under the "Environment" section, create a binding for your D1 database.  
   - Name the binding `DB`.


## 3. Deploy the Worker

1. **Deploy your worker:**
   - Once your worker is set up with the code, click "Save and Deploy."

2. **Access the Worker:**
   - Your worker will be accessible at `https://your-worker-name.workers.dev`.


## 4. Testing the Worker

- **Set a Variable:**  
  `POST https://your-worker-name.workers.dev/setvar/{key}/{value}`

- **Get a Variable:**  
  `GET https://your-worker-name.workers.dev/getvar/{key}`

That's it! You now have a fully functional Cloudflare Worker that handles setting and retrieving variables using Cloudflare D1 Database.


# Various Guides for Coding Languages

Below are example snippets to interact with the worker from different languages.


## C# Guide

### 1️⃣ Initialize the Class
Create an instance of the `CloudflareDatabase` class in your HTTP handling code.

```
// Example usage in an HTTP listener context:
string url = "http://localhost:3000/setvar/myKey/myValue";
HttpWebRequest request = (HttpWebRequest)WebRequest.Create(url);
// Use CloudflareDatabase.HandleRequest(request, response) to process the request.
```
2️⃣ Set a Variable
```
// Call the handler for a POST request:
await cloudflareDatabase.HandleRequest(request, response);
```
3️⃣ Get a Variable
```
// Call the handler for a GET request:
await cloudflareDatabase.HandleRequest(request, response);
```



## C Guide
1️⃣ Initialize the Database Storage
```
// Example usage:
handle_request("/setvar/myKey/myValue");
handle_request("/getvar/myKey");
```

## C++ Guide
1️⃣ Initialize the Database Storage
```
// Example usage:
handleRequest("/setvar/myKey/myValue");
handleRequest("/getvar/myKey");
```
## PHP Guide
1️⃣ Initialize the Database Storage
```
// Example usage:
handleRequest("/setvar/myKey/myValue");
handleRequest("/getvar/myKey");
```
JavaScript (Node.js) Guide
1️⃣ Initialize the Database Storage
```
// Example: The server automatically listens on port 3000.
// To set a variable, send a POST request to /setvar/myKey/myValue.
// To get a variable, send a GET request to /getvar/myKey.
```
## Python Guide
1️⃣ Initialize the Database Storage
```
# Example usage:
handle_request("/setvar/myKey/myValue")
handle_request("/getvar/myKey");
```
