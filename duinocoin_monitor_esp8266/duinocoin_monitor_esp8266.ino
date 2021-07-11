#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#define ARDUINOJSON_USE_DOUBLE 1
#include <ArduinoJson.h>

const char* ssid = "";
const char* password = "";
const char* ducouser = "";
const char* host = "server.duinocoin.com";
const int httpsPort = 443;
const char* fingerprint = "72:D5:D1:06:8B:FA:8D:B3:0F:AF:3A:26:7C:F5:32:A2:A1:81:2A:28";
String url = "/users/" + String(ducouser);
const char* hostPrice = "server.duinocoin.com";
String urlPrice = "/api.json";

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
    WiFiClientSecure client;
    client.setInsecure();
    if (!client.connect(host, httpsPort)) {
      Serial.println("connection failed");
      return;
    }
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
    while (client.connected()) {
      String line = client.readStringUntil('\n');
      if (line == "\r") {
        display.clear();
        break;
      }
    }
    String jsonbalance = client.readStringUntil('\n');
    StaticJsonDocument<192> filter;
    filter["result"]["balance"]["balance"] = true;
    filter["result"]["miners"][0]["threadid"] = true;
    DynamicJsonDocument doc(2048);
    DeserializationError error = deserializeJson(doc, jsonbalance, DeserializationOption::Filter(filter));
    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.f_str());
      return;
    }
    JsonObject datadoc = doc.as<JsonObject>();
    String ducos = datadoc["result"]["balance"]["balance"];
    Serial.println(ducos);
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
    client.stop();

    // Price
    client.setInsecure();
    if (!client.connect(host, httpsPort)) {
      Serial.println("connection failed");
      return;
    }
    client.print(String("GET ") + urlPrice + " HTTP/1.1\r\n" +
               "Host: " + hostPrice + "\r\n" +
               "Connection: close\r\n\r\n");
    while (client.connected()) {
      String line = client.readStringUntil('\n');
      if (line == "\r") {
        display.clear();
        break;
      }
    }
    String jsonprice = client.readString();
    Serial.println(jsonprice);
    StaticJsonDocument<128> filterPrice;
    filterPrice["Duco price"] = true;
    DeserializationError errorPrice = deserializeJson(doc, jsonprice, DeserializationOption::Filter(filterPrice));
    if (errorPrice) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(errorPrice.f_str());
      price = '-';
    } else {
      JsonObject pricedoc = doc.as<JsonObject>();
      String euros = pricedoc["Duco price"];
      price = euros;
      Serial.println("Price: " + String(euros));
    }
    
    doc.clear();
    client.stop();

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
