#include <SPI.h>
#include <MFRC522.h>
#include "ArduinoGraphics.h"
#include "Arduino_LED_Matrix.h"

ArduinoLEDMatrix matrix;

#define RST_PIN 9
#define SS_PIN 10

MFRC522 mfrc522(SS_PIN, RST_PIN);
String MasterTag = "D30F5010";
String tagID = "";

void setup() {
  Serial.begin(9600);
  while (!Serial);
  SPI.begin();
  mfrc522.PCD_Init();
  delay(4);
  mfrc522.PCD_DumpVersionToSerial();
  Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
  matrix.begin();
}

void loop() {
  while (getID()) {
    if (tagID == MasterTag) {
      Serial.println("Access granted!");
      mfrc522.PICC_DumpToSerial(&(mfrc522.uid));

      // LED Matrix Scroll
      matrix.beginDraw();
      matrix.stroke(0xFFFFFFFF);
      matrix.textScrollSpeed(50);

      const char text[] = "    Hello World!    ";
      matrix.textFont(Font_5x7);
      matrix.beginText(0, 1, 0xFFFFFF);
      matrix.println(text);
      matrix.endText(SCROLL_LEFT);

      matrix.endDraw();
    } else {
      Serial.println("Access denied!");
      matrix.beginDraw();
      matrix.stroke(0xFFFFFFFF);

      const char text[] = "UNO";
      matrix.textFont(Font_4x6);
      matrix.beginText(0, 1, 0xFFFFFF);
      matrix.println(text);
      matrix.endText();

      matrix.endDraw();

      delay(2000);
    }
  }
}

boolean getID() {
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return false;
  }

  if (!mfrc522.PICC_ReadCardSerial()) {
    return false;
  }

  tagID = "";
  for (uint8_t i = 0; i < mfrc522.uid.size; i++) {
    tagID += String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
    tagID += String(mfrc522.uid.uidByte[i], HEX);
  }

  tagID.toUpperCase();
  mfrc522.PICC_HaltA();
  return true;
}
