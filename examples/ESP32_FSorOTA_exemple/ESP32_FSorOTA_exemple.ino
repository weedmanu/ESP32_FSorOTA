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
  Serial.print("Connexion au réseau : ");
  Serial.println(ssid);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connecté à ");
  Serial.println(ssid);
  Serial.print("Adresse IP : ");
  Serial.println(WiFi.localIP());
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/html", "<button onclick=\"window.location.href = '/FSorOTA';\">Cliquez Ici</button>");
  });
  ESP32_FSorOTA.begin(&server);
  server.begin();
  Serial.print("Serveur HTTP démarré : http://");
  Serial.println(WiFi.localIP());
  Serial.println("Initialisation OK");
}

void loop(void) {
}
