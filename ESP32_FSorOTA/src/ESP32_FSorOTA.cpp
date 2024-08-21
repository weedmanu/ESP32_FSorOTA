#include "ESP32_FSorOTA.h"
#include "pageWeb.h"

ESP32_FSorOTAClass ESP32_FSorOTA;

String listFiles() {
  String fileList = "";
  File root = LittleFS.open("/");
  File file = root.openNextFile();
  while (file) {
    fileList += String(file.name()) + " Size: " + humanReadableSize(file.size()) + "\n";
    file = root.openNextFile();
  }
  return fileList;
}

String humanReadableSize(const size_t bytes) {
  if (bytes < 1024) return String(bytes) + " B";
  else if (bytes < (1024 * 1024)) return String(bytes / 1024.0) + " KB";
  else if (bytes < (1024 * 1024 * 1024)) return String(bytes / 1024.0 / 1024.0) + " MB";
  else return String(bytes / 1024.0 / 1024.0 / 1024.0) + " GB";
}

String cleanLittleFS() {
  File root = LittleFS.open("/");
  String result;
  if (!root) {
      result = "Échec de l'ouverture du répertoire racine";
      Serial.println(result);
      return result;
  }
  if (!root.isDirectory()) {
      result = "Le répertoire racine n'est pas un répertoire";
      Serial.println(result);
      return result;
  }

  File file = root.openNextFile();
  while (file) {
      String fileName = file.name();
      if (fileName) {
          Serial.print("Suppression du fichier : ");
          Serial.println(fileName);
          LittleFS.remove(fileName);
      }
      file = root.openNextFile();
  }
  result = "Nettoyage de LittleFS terminé";
  Serial.println(result);
  return result;
}

void ESP32_FSorOTAClass::begin(AsyncWebServer *server){
    if (!LittleFS.begin()) {
      Serial.println("Erreur de montage LittleFS");
    } else {
      Serial.println("LittleFS monté avec succès");
      Serial.print("LittleFS Disponible: "); Serial.println(humanReadableSize((LittleFS.totalBytes() - LittleFS.usedBytes())));
      Serial.print("LittleFS Utilisé: "); Serial.println(humanReadableSize(LittleFS.usedBytes()));
      Serial.print("LittleFS Total: "); Serial.println(humanReadableSize(LittleFS.totalBytes()));
    }
    _server = server;

    _server->on("/FSorOTA", HTTP_GET, [](AsyncWebServerRequest *request) {
      request->send(200, "text/html", HTML);
    });

    _server->on("/effacer", HTTP_GET, [](AsyncWebServerRequest *request) {
      if (request->hasParam("file")) {
        String str = String(request->getParam("file")->value());
        int i1 = str.indexOf(' ');
        String filename = str.substring(0, i1);
        String filepath = "/" + filename;
        //filename += request->getParam("file")->value();
        if (LittleFS.remove(filepath)) {
          Serial.printf("%s effacé ", filename.c_str());
        } else {
          Serial.printf("%s non trouvé ", filename.c_str());
        }
        request->redirect("/FSorOTA");
      }
    });

    _server->on("/list", HTTP_GET, [&](AsyncWebServerRequest *request) {
      String fileList = listFiles();
      request->send(200, "text/plain", fileList);
    });

    _server->on("/size", HTTP_GET, [&](AsyncWebServerRequest *request) {
      String taille = "LittleFS Free: ";
      taille += humanReadableSize((LittleFS.totalBytes() - LittleFS.usedBytes())); taille += "\n";
      taille += "LittleFS Used: ";
      taille += humanReadableSize(LittleFS.usedBytes());  taille += "\n";
      taille += "LittleFS Total: ";
      taille += humanReadableSize(LittleFS.totalBytes());  taille += "\n";
      taille += "\n";
      request->send(200, "text/plain", taille);
    });

    _server->on("/rawsize", HTTP_GET, [&](AsyncWebServerRequest *request) {
      request->send(200, "text/plain", String(LittleFS.totalBytes() - LittleFS.usedBytes()));
    });

    _server->on("/clean", HTTP_GET, [&](AsyncWebServerRequest *request) {
      request->send(200, "text/plain", cleanLittleFS());
    });

    _server->on("/upload", HTTP_POST, [&](AsyncWebServerRequest *request) {},
      [&](AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final) {

        static File file;
        static size_t totalSize = 0;

        if (!index) {
          Serial.printf("Upload Start: %s\n", filename.c_str());
          totalSize = request->contentLength();
          if (filename.endsWith(".bin")) {
            if (!Update.begin(UPDATE_SIZE_UNKNOWN)) {
              Update.printError(Serial);
              return;
            }
          } else {
            filename = "/" + filename;
            file = LittleFS.open(filename, "w");
            if (!file) {
              Serial.println("Erreur d'ouverture du fichier");
              return;
            }
          }
        }

        if (filename.endsWith(".bin")) {
          if (Update.write(data, len) != len) {
            Update.printError(Serial);
            return;
          }
        } else {
          if (file) {
            if ((LittleFS.totalBytes() - LittleFS.usedBytes()) >= len) {
              file.write(data, len);
            } else {
              Serial.println("Not enough space");
              return;
            }
          }
        }

        float progress = (float)(index + len) / (float)totalSize * 100;
        Serial.printf("Progress: %.2f%%\n", progress);

        if (final) {
          if (filename.endsWith(".bin")) {
            if (Update.end(true)) {
              Serial.printf("Update Success: %u bytes, Reboot...\n", index + len);
              request->send(200);
              delay(5000);
              ESP.restart();
            } else {
              Update.printError(Serial);
              request->send(500, "text/plain", "erreur de chargement");
            }
          } else {
            if (file) {
              file.close();
            }
            Serial.printf("Upload Success: %s\n", filename.c_str());
            request->send(200);
          }
        }
      });
}
