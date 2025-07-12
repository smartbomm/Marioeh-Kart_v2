package CarraraV;

import com.sun.net.httpserver.HttpServer;
import com.sun.net.httpserver.HttpExchange;
import com.sun.net.httpserver.HttpHandler;

import java.io.IOException;
import java.io.OutputStream;
import java.net.InetSocketAddress;

public class CsvHostServer {

    public static void main(String[] args) throws IOException {
        int port = 8000;

        HttpServer server = HttpServer.create(new InetSocketAddress(port), 0);
        System.out.println("Time Server running at http://localhost:" + port);
        server.createContext("/t", new TimeHandler());
        server.setExecutor(null);
        server.start();
    }

    static class TimeHandler implements HttpHandler {
        @Override
        public void handle(HttpExchange exchange) throws IOException {
            if (!"GET".equals(exchange.getRequestMethod())) {
                exchange.sendResponseHeaders(405, -1);
                return;
            }

            String response = Long.toString(System.currentTimeMillis());
            byte[] data = response.getBytes();

            exchange.getResponseHeaders().set("Content-Type", "text/plain");
            exchange.sendResponseHeaders(200, data.length);

            try (OutputStream os = exchange.getResponseBody()) {
                os.write(data);
                System.out.println(data);            
            }
        }
    }
}
