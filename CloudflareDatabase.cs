using System;
using System.Net;
using System.Threading.Tasks;
using Microsoft.Data.Sqlite;

public class CloudflareDatabase
{
    public async Task HandleRequest(HttpListenerRequest request, HttpListenerResponse response)
    {
        try
        {
            var url = new Uri(request.Url.ToString());

            // POST: Set variable
            if (url.AbsolutePath.StartsWith("/setvar"))
            {
                var parts = url.AbsolutePath.Split('/').Where(p => !string.IsNullOrEmpty(p)).ToArray();
                if (parts.Length < 3)
                {
                    response.StatusCode = 400;
                    await response.OutputStream.WriteAsync(System.Text.Encoding.UTF8.GetBytes("Error: Key or Value missing"));
                    return;
                }

                string key = parts[1];
                string value = parts[2];

                using (var connection = new SqliteConnection("Data Source=cloudflare.db"))
                {
                    connection.Open();
                    var command = connection.CreateCommand();
                    command.CommandText = "INSERT OR REPLACE INTO variables (key, value) VALUES (@key, @value)";
                    command.Parameters.AddWithValue("@key", key);
                    command.Parameters.AddWithValue("@value", value);
                    command.ExecuteNonQuery();
                }

                response.StatusCode = 200;
                await response.OutputStream.WriteAsync(System.Text.Encoding.UTF8.GetBytes($"Variable '{key}' set to '{value}'."));
            }
            // GET: Get variable
            else if (url.AbsolutePath.StartsWith("/getvar"))
            {
                var parts = url.AbsolutePath.Split('/').Where(p => !string.IsNullOrEmpty(p)).ToArray();
                if (parts.Length < 2)
                {
                    response.StatusCode = 400;
                    await response.OutputStream.WriteAsync(System.Text.Encoding.UTF8.GetBytes("Error: Key missing"));
                    return;
                }

                string key = parts[1];
                string value = null;

                using (var connection = new SqliteConnection("Data Source=cloudflare.db"))
                {
                    connection.Open();
                    var command = connection.CreateCommand();
                    command.CommandText = "SELECT value FROM variables WHERE key = @key";
                    command.Parameters.AddWithValue("@key", key);
                    using (var reader = command.ExecuteReader())
                    {
                        if (reader.Read())
                        {
                            value = reader.GetString(0);
                        }
                    }
                }

                if (value != null)
                {
                    response.StatusCode = 200;
                    await response.OutputStream.WriteAsync(System.Text.Encoding.UTF8.GetBytes(value));
                }
                else
                {
                    response.StatusCode = 404;
                    await response.OutputStream.WriteAsync(System.Text.Encoding.UTF8.GetBytes("Variable not found"));
                }
            }
            else
            {
                response.StatusCode = 200;
                await response.OutputStream.WriteAsync(System.Text.Encoding.UTF8.GetBytes("Hello from Cloudflare D1!"));
            }
        }
        catch (Exception e)
        {
            response.StatusCode = 500;
            await response.OutputStream.WriteAsync(System.Text.Encoding.UTF8.GetBytes($"Error: {e.Message}"));
            
        }
    }
}
