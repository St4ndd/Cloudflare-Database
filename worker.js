export default {
    async fetch(request, env) {
        try {
            const url = new URL(request.url);

            // 🟡 2️⃣ POST: Set a new variable
            if (url.pathname.startsWith("/setvar")) {
                let parts = url.pathname.split("/").filter(p => p); // Remove empty elements
                if (parts.length < 3) {
                    return new Response("Error: Key or value is missing", { status: 400 });
                }
                
                const key = parts[1];
                const value = parts[2];

                await env.DB.prepare("INSERT OR REPLACE INTO variables (key, value) VALUES (?, ?)")
                    .bind(key, value).run();
                
                return new Response(`Variable '${key}' set to '${value}'.`, { status: 200 });
            }
            // 🟢 1️⃣ GET: Retrieve a variable
            if (url.pathname.startsWith("/getvar")) {
                let parts = url.pathname.split("/").filter(p => p); // Remove empty elements
                if (parts.length < 2) {
                    return new Response("Error: Key is missing", { status: 400 });
                }

                const key = parts[1];
                let { results } = await env.DB.prepare("SELECT value FROM variables WHERE key = ?")
                    .bind(key).all();

                if (results.length > 0) {
                    return new Response(results[0].value, { status: 200 });
                } else {
                    return new Response("Variable not found", { status: 404 });
                }
            }

            return new Response("Hello from Cloudflare D1!", { status: 200 });

        } catch (error) {
            return new Response(`Error: ${error.message}`, { status: 500 });
        }
    }
};