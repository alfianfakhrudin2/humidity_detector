// Define Blynk template and token
#define BLYNK_TEMPLATE_ID "TMPL6zm0n3ePF"
#define BLYNK_TEMPLATE_NAME "utsiot"
#define BLYNK_AUTH_TOKEN "rGplNxqTvSrkwr_6MpoE1Rp1x2mYezis"

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include "DHT.h"
#include <BlynkSimpleEsp32.h> 
#include <Keypad.h>

// Define Blynk serial and virtual pin
#define BLYNK_PRINT Serial 
#define BlynkV0 V0
#define BlynkV1 V1

// Define DHT sensor type and pin
#define DHTPIN 4     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

// Initialize WiFi SSID and password
char ssid[] = "ESPHotspot";
char pass[] = "alfian3241";

// Initialize the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Setup DHT
DHT dht(DHTPIN, DHTTYPE);

// Setup BlynkTimer object
BlynkTimer timer;

// Setup temperature and humidity variable
float temperature, humidity;
bool lcdIsDefault = true;

// Initialize keypad variable
const int ROW = 4;
const int COLUMN = 3;
char keys[ROW][COLUMN] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

// Keypad setup
byte rowPins[ROW] = {12, 14, 27, 26};
byte colPins[COLUMN] = {25, 33, 32}; 
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROW, COLUMN);

void setup() {
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  timer.setInterval(1000L, timerEvent);

  Serial.print("DHTxx test!\n");
  dht.begin();
  lcd.init();
  lcd.backlight();

  // Test custom characters
  lcd.setCursor(0,0);
  lcd.print("   Temperature");
  lcd.setCursor(0,1);
  lcd.print("     Monitor");
  delay(2000);
  lcd.clear();
}

void loop() {
  Blynk.run();
  timer.run();
  initLCD(lcdIsDefault);
  updateKeypad();
}

// Membaca Suhu dan Kelembaban, Pemeriksaan Kesalahan, Pencetakan Nilai, Pengiriman Data ke Blynk
void timerEvent() {
  temperature = dht.readTemperature();
  humidity = dht.readHumidity(); 

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.println("Temperature: " + String(temperature, 1) + "C");
  Serial.println("Humidity: " + String(humidity, 1) + "%");

  Blynk.virtualWrite(BlynkV0, temperature);
  Blynk.virtualWrite(BlynkV1, humidity);

  // delay(2000);
}


void initLCD(bool state) {
  if (state) {
    lcd.setCursor(0,0);
    lcd.print("Press 1: Temp");
    
    lcd.setCursor(0,1);
    lcd.print("Press 2: Humid");
  }
}

// untuk memperbarui tampilan LCD berdasarkan tombol yang ditekan pada keypad.
void updateLCD(char key) {
  lcdIsDefault = key == '1' || key == '2' ? false : true;
  lcd.clear();

  if (key == '1') {
    lcd.setCursor(2, 0);
    lcd.print("Temperature:");
    lcd.setCursor(4,1);
    lcd.scrollDisplayLeft();
    lcd.print(temperature);
    lcd.print("C");
  } else if (key == '2') {
    lcd.setCursor(3,0);
    lcd.print("Humidity:");
    lcd.setCursor(4,1);
    lcd.scrollDisplayLeft();
    lcd.print(humidity);
    lcd.print("%");
  }
}

// untuk membaca input dari keypad.
void updateKeypad() {
  char key = keypad.getKey();

  if (key != NO_KEY){
    Serial.println(key);
    updateLCD(key);
  }
}
