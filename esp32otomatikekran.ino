#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>

#include <SPI.h>
#include <GxEPD2_BW.h>
#include <Fonts/FreeMonoBold9pt7b.h>

// ===== ESP32 PINLER =====
static const int8_t PIN_SCK  = 18;
static const int8_t PIN_MISO = 19;
static const int8_t PIN_MOSI = 23;

static const int8_t PIN_CS   = 5;
static const int8_t PIN_DC   = 21;
static const int8_t PIN_RST  = 22;
static const int8_t PIN_BUSY = 4;

// ===== Display driver =====
GxEPD2_BW<GxEPD2_213_B74, GxEPD2_213_B74::HEIGHT> display(
  GxEPD2_213_B74(PIN_CS, PIN_DC, PIN_RST, PIN_BUSY)
);

// ===== WiFi =====
const char* ssid = "internetadi";
const char* password = "internetsifresi";

IPAddress local_IP(192, 168, 1, 184);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

WebServer server(80);

// ===== Ürün verileri =====
String urunBarkod = "";
String urunIsim = "";
float urunFiyat = 0.0;
float urunIndirimMiktar = 0.0;
float urunIndirimli = 0.0;

bool veriGeldi = false;
wl_status_t lastWifiStatus = WL_IDLE_STATUS;

// ====== EKRANA ÇİZ ======
void ekraniCiz()
{
  display.setRotation(1);
  display.setFullWindow();

  display.firstPage();
  do {
    display.fillScreen(GxEPD_WHITE);
    display.setTextColor(GxEPD_BLACK);

    // WiFi ikon 
    int boxX = display.width() - 18;
    int boxY = 2;
    int boxW = 16;
    int boxH = 16;
    if (WiFi.status() == WL_CONNECTED) display.fillRect(boxX, boxY, boxW, boxH, GxEPD_BLACK);
    else                               display.drawRect(boxX, boxY, boxW, boxH, GxEPD_BLACK);

    // Ürün adı 
    display.setFont(&FreeMonoBold9pt7b);
    display.setCursor(10, 20);
    display.print(urunIsim);

    // Barkod 
    display.setCursor(10, display.height() - 10);
    display.print("Barkod: ");
    display.print(urunBarkod);

    // Fiyat bölümü
    if (urunIndirimMiktar == 0.0f) {
      display.setCursor(10, 70);
      display.setTextSize(2);
      display.print(urunFiyat, 2);
      display.print(" TL");
      display.setTextSize(1);
    } else {
      display.setTextSize(1);
      display.setCursor(10, 40);
      display.print("INDIRIM: ");
      display.print(urunIndirimMiktar, 2);
      display.print(" TL");

      display.setCursor(10, 60);
      display.print("Eski: ");
      display.print(urunFiyat, 2);
      display.print(" TL");
      display.drawLine(10, 64, 140, 64, GxEPD_BLACK);

      display.setTextSize(2);
      display.setCursor(10, 95);
      display.print(urunIndirimli, 2);
      display.print(" TL");
      display.setTextSize(1);
    }

  } while (display.nextPage());
}

void setup() {
  Serial.begin(115200);

  // SPI + ePaper init
  SPI.begin(PIN_SCK, PIN_MISO, PIN_MOSI, PIN_CS);
  display.init(115200, true, 2, false);  // Waveshare HAT reset
  display.setTextWrap(false);

  // WiFi
  WiFi.config(local_IP, gateway, subnet);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("WiFi baglantisi bekleniyor...");
  }
  Serial.println("WiFi bagli!");
  Serial.println(WiFi.localIP());
  lastWifiStatus = WiFi.status();

  // İlk ekran
  urunIsim = "Hazir";
  urunBarkod = "-";
  urunFiyat = 0;
  urunIndirimMiktar = 0;
  urunIndirimli = 0;
  ekraniCiz();

  // Endpoint
  server.on("/update", HTTP_POST, []() {
    if (!server.hasArg("plain")) {
      server.send(400, "text/plain", "Eksik veri");
      return;
    }

    DynamicJsonDocument doc(1024);
    DeserializationError err = deserializeJson(doc, server.arg("plain"));
    if (err) {
      server.send(400, "text/plain", "Gecersiz JSON");
      return;
    }

    urunBarkod = doc["urunBarkod"].as<String>();
    urunIsim = doc["urunIsim"].as<String>();
    urunFiyat = doc["urunFiyat"].as<float>();
    urunIndirimMiktar = doc["urunIndirimMiktar"].as<float>();
    urunIndirimli = doc["urunIndirimli"].as<float>();

    Serial.println("Veri alindi, ekran guncelleniyor...");
    veriGeldi = true;

    server.send(200, "text/plain", "OK");
  });

  server.begin();
}

void loop() {
  server.handleClient();

  if (veriGeldi) {
    ekraniCiz();
    veriGeldi = false;
  }

  wl_status_t st = WiFi.status();
  if (st != lastWifiStatus) {
    lastWifiStatus = st;
    ekraniCiz();
  }

  delay(10);
}
