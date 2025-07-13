#include <SPI.h>
#include <DMD2.h>
//#include <fonts/SystemFont5x7.h>
#include <fonts/Arial14.h>  

const int WIDTH = 2;
const uint8_t *FONT = Arial14; // Best clarity
SoftDMD dmd(WIDTH, 1);
DMD_TextBox box(dmd);

String scrollMessage = "Student Achievement Display with IOT Tech. by CSE dept. KBPCOE, Satara...  ";
bool isReceiving = false;
String serialBuffer = "";

void setup() {
  Serial.begin(9600);
  dmd.begin();
  dmd.setBrightness(200);
  dmd.selectFont(FONT);
}

unsigned long lastSerialTime = 0;
const unsigned long messageTimeout = 40000; // 40 seconds

void loop() {
  dmd.scanDisplay();  // Keep refreshing display

  // Serial reception
  while (Serial.available()) {
    char ch = Serial.read();
    if (!isReceiving) {
      isReceiving = true;
      serialBuffer = "";
      dmd.clearScreen();
      box.clear();
    }
    if (ch == '\n') {
      scrollMessage = serialBuffer;
      isReceiving = false;
      lastSerialTime = millis();  // Update last received time
      break;
    } else {
      serialBuffer += ch;
    }
  }

  // Scroll if no reception ongoing
  if (!isReceiving && scrollMessage.length() > 0) {
    const char *msg = scrollMessage.c_str();
    while (*msg) {
      box.print(*msg++);
      delay(150);
      dmd.scanDisplay();
    }
    box.clear();

    // If timeout exceeded, repeat scroll
    if (millis() - lastSerialTime > messageTimeout) {
      lastSerialTime = millis();  // Restart timer for looping
    }
  }
}
