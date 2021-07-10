#include <WiFi.h>
#include <WiFiUdp.h>
#include <HTTPClient.h>
#define ARDUINOJSON_USE_DOUBLE 1
#include <ArduinoJson.h>

const char* ssid = "";
const char* password = "";
const char* ducouser = "";
String serverName = "https://server.duinocoin.com/users/";
String serverPrice = "https://server.duinocoin.com/api.json";
const char* root_ca= \
"-----BEGIN CERTIFICATE-----\n" \
"MIIFLTCCBBWgAwIBAgISBLrlR4aCBlmofGjUwxvNkJ9IMA0GCSqGSIb3DQEBCwUA\n" \
"MDIxCzAJBgNVBAYTAlVTMRYwFAYDVQQKEw1MZXQncyBFbmNyeXB0MQswCQYDVQQD\n" \
"EwJSMzAeFw0yMTA0MjAxMjAwMThaFw0yMTA3MTkxMjAwMThaMB8xHTAbBgNVBAMT\n" \
"FHNlcnZlci5kdWlub2NvaW4uY29tMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIB\n" \
"CgKCAQEAv7SZV8V8uikoSOnw9KVa9M3HQsyB6l3QdinCuyZz7sRckOdSrUKtYFK/\n" \
"g+RmBqWVrwRLgnVmLtUlulHI7/H7Rbz0Exiv0beyjakd+p40O8V7Y1kuhE9z8WT6\n" \
"gtDd4grLNBMtUkd2Y8MmYFed2SP5OUoIdySdcUuo50NiS2KH5j3DhZB+ZmZn0lXw\n" \
"E91O2eNgEaryrnDoMftlJAUOSGh3uuCS27y1SXm8IC1Cxkv+OUyjXu9HnZoIX5v0\n" \
"59cTAFzrSo5tF/XtJlZ7fJU7VC98NaurfWKrF1XWgoKHJcjsFgep1K6PRIb7g5xq\n" \
"wlWUOFYTlDIYMAocGYJabw6jZrSphQIDAQABo4ICTjCCAkowDgYDVR0PAQH/BAQD\n" \
"AgWgMB0GA1UdJQQWMBQGCCsGAQUFBwMBBggrBgEFBQcDAjAMBgNVHRMBAf8EAjAA\n" \
"MB0GA1UdDgQWBBSGBC/8G5lAbUvFVjt4wL7kzsnzDjAfBgNVHSMEGDAWgBQULrMX\n" \
"t1hWy65QCUDmH6+dixTCxjBVBggrBgEFBQcBAQRJMEcwIQYIKwYBBQUHMAGGFWh0\n" \
"dHA6Ly9yMy5vLmxlbmNyLm9yZzAiBggrBgEFBQcwAoYWaHR0cDovL3IzLmkubGVu\n" \
"Y3Iub3JnLzAfBgNVHREEGDAWghRzZXJ2ZXIuZHVpbm9jb2luLmNvbTBMBgNVHSAE\n" \
"RTBDMAgGBmeBDAECATA3BgsrBgEEAYLfEwEBATAoMCYGCCsGAQUFBwIBFhpodHRw\n" \
"Oi8vY3BzLmxldHNlbmNyeXB0Lm9yZzCCAQMGCisGAQQB1nkCBAIEgfQEgfEA7wB2\n" \
"APZclC/RdzAiFFQYCDCUVo7jTRMZM7/fDC8gC8xO8WTjAAABeO9eAZAAAAQDAEcw\n" \
"RQIgXLdccQKNKjfPXVV1dbVW8LPmCKb80E8FOVBNw6WswN8CIQDerRKSvtgx4PxF\n" \
"1lOq8sFWwlMvchIkpiELDpmSQ7/6iAB1AFzcQ5L+5qtFRLFemtRW5hA3+9X6R9yh\n" \
"c5SyXub2xw7KAAABeO9eAYAAAAQDAEYwRAIgAkQ5SNxBduS8ckP7z0wEMMLdcNmf\n" \
"rR76s3MD8KIG/UQCIB4txuVGP/citMxxQYKFan1H0l4l2dYJuV4IAizWJ5GHMA0G\n" \
"CSqGSIb3DQEBCwUAA4IBAQCGELdxTgyUsqnCgE6bgOkRyAiVZHcSqNMg17DWw+ek\n" \
"IqfXFMbfuTFAs3+VRRCZS2BUiCttsmNzdiPIGgZLvyyv20RLDEjg7Kq22mudGg3F\n" \
"i/koB5DlGWWv9t2Ng/qnM/4+y6kAwpbJ8R1v4P3NpZgGFIccYgP+N41IOMT+OIzT\n" \
"6PxYyUH9yQx58e0t2FNTRjJSwIPZKfRUdLVIYb4sjXHqvLA9s76SkrUebDlEt16O\n" \
"vjDNTcK0q6jXGQjjRwjzPPmze36jvzhhGBypU3tXJifciRSl/4766tlvLwszMaOe\n" \
"yx+m+83OOchAq+N7/QxbXkNhAXrEzIOXOoYmA6QpfndQ\n" \
"-----END CERTIFICATE-----\n" \
"-----BEGIN CERTIFICATE-----\n" \
"MIIEZTCCA02gAwIBAgIQQAF1BIMUpMghjISpDBbN3zANBgkqhkiG9w0BAQsFADA/\n" \
"MSQwIgYDVQQKExtEaWdpdGFsIFNpZ25hdHVyZSBUcnVzdCBDby4xFzAVBgNVBAMT\n" \
"DkRTVCBSb290IENBIFgzMB4XDTIwMTAwNzE5MjE0MFoXDTIxMDkyOTE5MjE0MFow\n" \
"MjELMAkGA1UEBhMCVVMxFjAUBgNVBAoTDUxldCdzIEVuY3J5cHQxCzAJBgNVBAMT\n" \
"AlIzMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAuwIVKMz2oJTTDxLs\n" \
"jVWSw/iC8ZmmekKIp10mqrUrucVMsa+Oa/l1yKPXD0eUFFU1V4yeqKI5GfWCPEKp\n" \
"Tm71O8Mu243AsFzzWTjn7c9p8FoLG77AlCQlh/o3cbMT5xys4Zvv2+Q7RVJFlqnB\n" \
"U840yFLuta7tj95gcOKlVKu2bQ6XpUA0ayvTvGbrZjR8+muLj1cpmfgwF126cm/7\n" \
"gcWt0oZYPRfH5wm78Sv3htzB2nFd1EbjzK0lwYi8YGd1ZrPxGPeiXOZT/zqItkel\n" \
"/xMY6pgJdz+dU/nPAeX1pnAXFK9jpP+Zs5Od3FOnBv5IhR2haa4ldbsTzFID9e1R\n" \
"oYvbFQIDAQABo4IBaDCCAWQwEgYDVR0TAQH/BAgwBgEB/wIBADAOBgNVHQ8BAf8E\n" \
"BAMCAYYwSwYIKwYBBQUHAQEEPzA9MDsGCCsGAQUFBzAChi9odHRwOi8vYXBwcy5p\n" \
"ZGVudHJ1c3QuY29tL3Jvb3RzL2RzdHJvb3RjYXgzLnA3YzAfBgNVHSMEGDAWgBTE\n" \
"p7Gkeyxx+tvhS5B1/8QVYIWJEDBUBgNVHSAETTBLMAgGBmeBDAECATA/BgsrBgEE\n" \
"AYLfEwEBATAwMC4GCCsGAQUFBwIBFiJodHRwOi8vY3BzLnJvb3QteDEubGV0c2Vu\n" \
"Y3J5cHQub3JnMDwGA1UdHwQ1MDMwMaAvoC2GK2h0dHA6Ly9jcmwuaWRlbnRydXN0\n" \
"LmNvbS9EU1RST09UQ0FYM0NSTC5jcmwwHQYDVR0OBBYEFBQusxe3WFbLrlAJQOYf\n" \
"r52LFMLGMB0GA1UdJQQWMBQGCCsGAQUFBwMBBggrBgEFBQcDAjANBgkqhkiG9w0B\n" \
"AQsFAAOCAQEA2UzgyfWEiDcx27sT4rP8i2tiEmxYt0l+PAK3qB8oYevO4C5z70kH\n" \
"ejWEHx2taPDY/laBL21/WKZuNTYQHHPD5b1tXgHXbnL7KqC401dk5VvCadTQsvd8\n" \
"S8MXjohyc9z9/G2948kLjmE6Flh9dDYrVYA9x2O+hEPGOaEOa1eePynBgPayvUfL\n" \
"qjBstzLhWVQLGAkXXmNs+5ZnPBxzDJOLxhF2JIbeQAcH5H0tZrUlo5ZYyOqA7s9p\n" \
"O5b85o3AM/OJ+CktFBQtfvBhcJVd9wvlwPsk+uyOy2HI7mNxKKgsBTt375teA2Tw\n" \
"UdHkhVNcsAKX1H7GNNLOEADksd86wuoXvg==\n" \
"-----END CERTIFICATE-----\n" \
"-----BEGIN CERTIFICATE-----\n" \
"MIIDSjCCAjKgAwIBAgIQRK+wgNajJ7qJMDmGLvhAazANBgkqhkiG9w0BAQUFADA/\n" \
"MSQwIgYDVQQKExtEaWdpdGFsIFNpZ25hdHVyZSBUcnVzdCBDby4xFzAVBgNVBAMT\n" \
"DkRTVCBSb290IENBIFgzMB4XDTAwMDkzMDIxMTIxOVoXDTIxMDkzMDE0MDExNVow\n" \
"PzEkMCIGA1UEChMbRGlnaXRhbCBTaWduYXR1cmUgVHJ1c3QgQ28uMRcwFQYDVQQD\n" \
"Ew5EU1QgUm9vdCBDQSBYMzCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEB\n" \
"AN+v6ZdQCINXtMxiZfaQguzH0yxrMMpb7NnDfcdAwRgUi+DoM3ZJKuM/IUmTrE4O\n" \
"rz5Iy2Xu/NMhD2XSKtkyj4zl93ewEnu1lcCJo6m67XMuegwGMoOifooUMM0RoOEq\n" \
"OLl5CjH9UL2AZd+3UWODyOKIYepLYYHsUmu5ouJLGiifSKOeDNoJjj4XLh7dIN9b\n" \
"xiqKqy69cK3FCxolkHRyxXtqqzTWMIn/5WgTe1QLyNau7Fqckh49ZLOMxt+/yUFw\n" \
"7BZy1SbsOFU5Q9D8/RhcQPGX69Wam40dutolucbY38EVAjqr2m7xPi71XAicPNaD\n" \
"aeQQmxkqtilX4+U9m5/wAl0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNV\n" \
"HQ8BAf8EBAMCAQYwHQYDVR0OBBYEFMSnsaR7LHH62+FLkHX/xBVghYkQMA0GCSqG\n" \
"SIb3DQEBBQUAA4IBAQCjGiybFwBcqR7uKGY3Or+Dxz9LwwmglSBd49lZRNI+DT69\n" \
"ikugdB/OEIKcdBodfpga3csTS7MgROSR6cz8faXbauX+5v3gTt23ADq1cEmv8uXr\n" \
"AvHRAosZy5Q6XkjEGB5YGV8eAlrwDPGxrancWYaLbumR9YbK+rlmM6pZW87ipxZz\n" \
"R8srzJmwN0jP41ZL9c8PDHIyh8bwRLtTcm1D9SZImlJnt1ir/md2cXjbDaJWFBM5\n" \
"JDGFoqgCWjBH4d1QB7wCCZAA62RjYJsWvIjJEubSfZGL+T0yjWW06XyxV3bqxbYo\n" \
"Ob8VZRzI9neWagqNdwvYkQsEjgfbKbYK7p2CNTUQ\n" \
"-----END CERTIFICATE-----\n";

#define SDA_PIN 21
#define SCL_PIN 22
#define MAX_X 128
#define MAX_Y 64
#define screen_direction 0x3C
#include "SH1106.h"

SH1106 display(screen_direction, SDA_PIN, SCL_PIN);

uint32_t lastDrawTime;
uint32_t deauths = 0;       // deauth frames per second

int start;
int wait = 0;
int miners = 0;
float oldb = 0.0;
float userbalance;
float balance;
float ducomadesincesartdaily = 0.0;
float daily = 0;
String ducosmem;
String price;

void setup() {
  Serial.begin(115200);
  /* start screen */
  display.init();
  display.flipScreenVertically(); 
  display.setFont(ArialMT_Plain_10); 
  display.clear();
  display.drawString(6, 6, "Duinocoin Monitoring");
  display.drawString(24, 25, "Made by");
  display.drawString(29, 35, "tomaszafra.es");
  display.display();
  
  /* Connect */
  WiFi.begin(ssid, password); 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println(".");
    display.drawString(6, 54, "Connecting ...");
    display.display();
  }
  display.clear();
  display.setFont(ArialMT_Plain_10);
  Serial.println(WiFi.localIP());
  display.drawString(6, 6, "Connected. IP: ");
  display.drawString(6, 24, WiFi.localIP().toString());
  display.display();
  start = millis();
  delay(2000);
}

void loop() {
  if ((WiFi.status() == WL_CONNECTED)) {
    HTTPClient http;
    http.setTimeout(6000);
    http.begin(serverName + ducouser, root_ca);
    int httpCode = http.GET();
    display.clear();
    while(httpCode < 0) {
      httpCode = http.GET();
    }
    if (httpCode > 0) {
      DynamicJsonDocument doc(9216);
      String jsonbalance = http.getString();
      deserializeJson(doc, jsonbalance);
      JsonObject datadoc = doc.as<JsonObject>();
      String ducos = datadoc["result"]["balance"]["balance"];
      ducosmem = ducos;
      userbalance = ducos.toFloat();
      Serial.println("Ducos: " + String(ducos));

      // Miners
      miners = 0;
      for (JsonObject elem : datadoc["result"]["miners"].as<JsonArray>()) {
        miners++;
      }
      Serial.println("Miners: " + String(miners));
      doc.clear();
    } else {
      ducosmem = "-";
      Serial.println("Error http get: " + String(httpCode));
    }
    http.end();
    
    // Price
    http.begin(serverPrice, root_ca);
    int httpCodePrice = http.GET();
    if (httpCodePrice > 0) {
      DynamicJsonDocument doc(7168);
      String jsonprice = http.getString();
      deserializeJson(doc, jsonprice);
      JsonObject pricedoc = doc.as<JsonObject>();
      String euros = pricedoc["Duco price"];
      price = euros;
      Serial.println("Price: " + String(euros));
      doc.clear();
    } else {
      price = "-";
      Serial.println("Error http get: " + String(httpCodePrice));
    }
    http.end();
    
    if(wait == 0){
        oldb = userbalance;
    } if(wait == 5){
        wait = 0;
        calcule_daily();
    } else{
        wait++;
    }
    
    display.clear();
    display.drawString(6, 6, "Ducos: " + String(ducosmem));
    display.drawString(6, 20, "Miners: " + String(miners));
    display.drawString(6, 34, "Daily: " + String(daily));
    display.drawString(6, 48, "Price: " + price);
    display.display();
  } else {
    display.clear();
    display.setFont(ArialMT_Plain_10);
    display.drawString(24, 34, "Connection lost");
    display.drawString(29, 44, "Reconnecting");
    display.display();
    WiFi.begin(ssid, password); 
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.println(".");
      display.drawString(32, 6, ".");
      display.display();
    }
  }
  delay(15000);
}

void calcule_daily() {
  float ducomadein = userbalance - oldb;
  float dayduco = ducomadein * 960;
  daily = dayduco * 100 / 100;
  int ducomadesincestart = userbalance - balance;
  int secondssincestart = millis() - start;
  start = millis();
  ducomadesincesartdaily = ((86400/secondssincestart)*ducomadesincestart)*10 / 10;
}
