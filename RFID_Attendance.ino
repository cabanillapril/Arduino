#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include <MFRC522.h>
#include <RTClib.h>  // Include RTC library for time functionality

// OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// RFID
#define SS_PIN 10
#define RST_PIN 9
MFRC522 rfid(SS_PIN, RST_PIN);

// Ultrasonic
#define TRIG_PIN 7
#define ECHO_PIN 6

// Buzzer
#define BUZZER_PIN 8

// RTC (Real-Time Clock)
RTC_DS3231 rtc;

// Function to get name from UID
String getNameFromUID(String uid) {
  if (uid == "7371FD2C") return "April Cabanilla";
  if (uid == "A1B2C3D4") return "Jane Smith";
  return "Unknown";  // Return "Unknown" if UID is not found
}

void setup() {
  Serial.begin(9600);
  delay(100); // Give OLED a moment to power up

  // Init OLED FIRST!
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("OLED not found"));
    while (true);  // Halt if OLED not detected
  }

  // Display startup message
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(F("Initializing..."));
  display.display();
  delay(1000);  // Show message for a sec

  // Now init other components
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  SPI.begin();
  rfid.PCD_Init();

  if (!rtc.begin()) {
    Serial.println(F("RTC not found!"));
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println(F("RTC NOT FOUND"));
    display.display();
    while (true);
  }

  if (rtc.lostPower()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  display.clearDisplay();
  display.setCursor(0, 0);
  display.println(F("Ready to scan..."));
  display.display();
}

void loop() {
  // Read ultrasonic distance
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH);
  float distance = duration * 0.034 / 2;

  // Filter noise or no reading
  if (distance <= 0 || distance > 300) {
    distance = 999;  // Safe placeholder
  }

  if (distance < 50 && distance > 0) {
    // Person detected
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(2);
    display.println("Scan your card...");
    display.display();

    // RFID check
    if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
      // Get UID
      String uidStr = "";
      for (byte i = 0; i < rfid.uid.size; i++) {
        uidStr += String(rfid.uid.uidByte[i] < 0x10 ? "0" : "");
        uidStr += String(rfid.uid.uidByte[i], HEX);
      }
      uidStr.toUpperCase();

      String name = getNameFromUID(uidStr);  // Get the name from UID
      DateTime now = rtc.now();

      // Display attendance info or "ID not registered"
      display.clearDisplay();
      display.setTextSize(1);
      display.setCursor(0, 0);
      if (name == "Unknown") {
        display.println("ID is not registered");
      } else {
        display.println("Attendance Recorded");
        display.print("Name: ");
        display.println(name);
        display.print("Date: ");
        display.print(now.year());
        display.print('/');
        display.print(now.month());
        display.print('/');
        display.println(now.day());
        display.print("Time: ");
        display.print(now.hour());
        display.print(':');
        display.print(now.minute());
        display.print(':');
        display.println(now.second());
      }
      display.display();

      Serial.print("Scanned UID: "); Serial.println(uidStr);
      Serial.print("Name: "); Serial.println(name);
      Serial.print("Date: ");
      Serial.print(now.year()); Serial.print("/");
      Serial.print(now.month()); Serial.print("/");
      Serial.println(now.day());
      Serial.print("Time: ");
      Serial.print(now.hour()); Serial.print(":");
      Serial.print(now.minute()); Serial.print(":");
      Serial.println(now.second());

      if (name != "Unknown") {
      // Successful scan: 1 beep
      tone(BUZZER_PIN, 1000);
      delay(300);
      noTone(BUZZER_PIN);
    } else {
      // Unsuccessful scan: 2 quick beeps
      for (int i = 0; i < 2; i++) {
        tone(BUZZER_PIN, 1500);
        delay(150);
        noTone(BUZZER_PIN);
        delay(100);
      }
    }
      delay(3000);  // Show attendance message longer
      rfid.PICC_HaltA();
      rfid.PCD_StopCrypto1();
    }
  } else {
    // No one detected
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(2);
    display.println("Attendance");
    display.println("System");
    display.display();
  }

  delay(500);  // Small delay before checking again
}
