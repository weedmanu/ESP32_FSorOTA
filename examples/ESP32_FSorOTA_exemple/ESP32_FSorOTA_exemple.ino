#include <ESP32_FSorOTA.h>

const char* ssid = "XXXX";             // your ssid
const char* password = "XXXX";         // your password

AsyncWebServer server(80);

void setup(void) {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Initialisation ...");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/html", "<button onclick=\"window.location.href = '/FSorOTA';\">Cliquez Ici</button>");
  });
  ESP32_FSorOTA.begin(&server);
  server.begin();
  Serial.println("HTTP server started");
  Serial.println("Initialisation OK");
}

void loop(void) {
}
