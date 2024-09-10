#include <WiFi.h>
#include <WebServer.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <ArduinoJson.h> 

// tft ekran pin tanımlaması
#define TFT_CS     5   // Chip Select
#define TFT_DC     21  // Data/Command
#define TFT_RST    22  // Reset

// tft ekran başlatması
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

// wifi bağlantısı
const char* ssid = "internet adı";
const char* password = "şifre";

WebServer server(80);

String urunBarkod = "";
String urunIsim = "";
float urunFiyat = 0.0;
float urunIndirimMiktar = 0.0;
float urunIndirimli = 0.0;

// veri alındıktan sonra ekranda gösterilmesini sağlıyor veri alındığında true dönecek
bool veriGeldi = false;

void setup() {
  // seri port başlatma serial monitorden kontrol sağlanabilir
  Serial.begin(115200);

  // tft ekranı ayarları
  tft.initR(INITR_BLACKTAB);
  tft.fillScreen(ST7735_WHITE);
  tft.setRotation(1); // ekran yönünü yatay kullandım

  // wifiye bağlanması ve kontrolü
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("WiFi bağlantısı bekleniyor...");
  }
  Serial.println("WiFi bağlı!");

  // web sunucusundan veri alma
  server.on("/update", HTTP_POST, []() {
    if (server.hasArg("plain")) { // json içeriğine bak
      String body = server.arg("plain");
      
      // json verisinin çözümlemesi
      DynamicJsonDocument doc(1024);
      DeserializationError error = deserializeJson(doc, body);
      if (error) {
        Serial.println("JSON deserialization hatası: " + String(error.c_str()));
        server.send(400, "text/plain", "Geçersiz JSON");
        return;
      }
      
      urunBarkod = doc["urunBarkod"].as<String>();
      urunIsim = doc["urunIsim"].as<String>();
      urunFiyat = doc["urunFiyat"].as<float>();
      urunIndirimMiktar = doc["urunIndirimMiktar"].as<float>();
      urunIndirimli = doc["urunIndirimli"].as<float>();

      Serial.println("Veri alındı:");
      Serial.println("Ürün Barkod: " + urunBarkod);
      Serial.println("Ürün İsim: " + urunIsim);
      Serial.print("Ürün urunIndirimMiktar : ");
      Serial.println(urunIndirimMiktar);
      Serial.print("Ürün Fiyat: ");
      Serial.println(urunFiyat);
      Serial.print("Ürün İndirimli Fiyat : ");
      Serial.println(urunIndirimli);
      
      server.send(200, "text/plain", "Veri alındı!");

      // veri alındığında bayrak true olrak ayarlanıyo döndürülmesi sağlanıyor
      veriGeldi = true;
    } else {
      Serial.println("Eksik veri");
      server.send(400, "text/plain", "Eksik veri");
    }
  });

  server.begin();
}

void loop() {
  server.handleClient();  // web sunucusu loop içinde dinliyo

  if (veriGeldi) {
    // ekranı temizleyip yenisi geçiriliyo
    tft.fillScreen(ST7735_WHITE);

    // barkod
    tft.setTextColor(ST7735_BLACK);
    tft.setTextSize(1);
    tft.setCursor(0, 10);
    tft.print("Barkod: ");
    tft.print(urunBarkod);

    // isim
    tft.setCursor(0, 30);
    tft.print("Isim: ");
    tft.print(urunIsim);

    // fiyat
    tft.setCursor(0, 50);
    tft.print("Fiyat: ");
    tft.print(urunFiyat);
    tft.print(" TL");

    // urunIndirim boşmu yada sıfırmı bak
    if (urunIndirimMiktar > 0) {
      // Display Indirim Oranı
      tft.setCursor(0, 70);
      tft.print("Indirim Orani: ");
      tft.print(urunIndirimMiktar);
      tft.print(" TL");

      // indirimli fiyat
      tft.setCursor(0, 90);
      tft.print("Indirimli Fiyat: ");
      tft.print(urunIndirimli);
      tft.print(" TL");
    } 

    // veri gelince bayrağı sıfırlıyoruz
    veriGeldi = false;
  }

  // döngünün dönme süresi bu sayede sunucuyu dinlemeye devam ediyo
  delay(1000);
} 
