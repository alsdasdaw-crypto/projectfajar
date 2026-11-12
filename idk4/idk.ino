#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#define i2c_Address 0x3c  // initialize with the I2C addr 0x3C Typically eBay OLED's
#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels
#define OLED_RESET -1     // QT-PY / XIAO
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int PIN_UP = 3;
const int PIN_DOWN = 2;
const int PIN_SELECT = 4;
const int PIN_BACK = A3;
const int PIN_BUZZER = 13;
const int PIN_POT = A0;
const int PIN_LED = 10;

bool lastUpState = 0;
bool lastDownState = 0;
bool lastSelectState = 0;
bool lastBackState = 0;
const char* [MENU] = 

int CASE = 0;
int MENU = 0 void setup() {
  Serial.begin(115200);

  pinMode(PIN_UP, INPUT_PULLUP);
  pinMode(PIN_DOWN, INPUT_PULLUP);
  pinMode(PIN_SELECT, INPUT_PULLUP);
  pinMode(PIN_BACK, INPUT_PULLUP);

  delay(250);                        // wait for the OLED to power up
  display.begin(i2c_Address, true);  // Address 0x3C default
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(16, 25);
  display.print("XI TEI 2");
  display.display();
  delay(3000);
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(40, 25);
  display.print("DAVI");
  display.display();
  delay(2000);
  display.setTextSize(1);
}
void loop() {
  handleButtons();
  screen();
}


void handleButtons() {
  bool currentUp = digitalRead(PIN_UP);
  bool currentDown = digitalRead(PIN_DOWN);
  bool currentSelect = digitalRead(PIN_SELECT);
  bool currentBack = digitalRead(PIN_BACK);

  if (lastUpState == 0 && currentUp == 1) {
    if (CASE == 0) {
      CASE = 3;
    } else CASE--;
    delay(50);
  }
  lastUpState = currentUp;

  if (lastDownState == 0 && currentDown == 1) {
    if (CASE == 3) {
      CASE = 0;
    } else CASE++;
    delay(50);
  }
  lastDownState = currentDown;
}
void screen() {
  display.clearDisplay();
  if (MENU == 0) {
    switch (CASE) {
      case 0:

        display.setCursor(10, 10 * 1);
        display.print(">1. Buzzer");
        display.setCursor(10, 10 * 2);
        display.print(" 2. LED");
        display.setCursor(10, 10 * 3);
        display.print(" 3. 7 Segment");
        display.setCursor(10, 10 * 4);
        display.print(" 4. Sensor");
        break;
      case 1:

        display.setCursor(10, 10 * 1);
        display.print(" 1. Buzzer");
        display.setCursor(10, 10 * 2);
        display.print(">2. LED");
        display.setCursor(10, 10 * 3);
        display.print(" 3. 7 Segment");
        display.setCursor(10, 10 * 4);
        display.print(" 4. Sensor");
        break;
      case 2:

        display.setCursor(10, 10 * 1);
        display.print(" 1. Buzzer");
        display.setCursor(10, 10 * 2);
        display.print(" 2. LED");
        display.setCursor(10, 10 * 3);
        display.print(">3. 7 Segment");
        display.setCursor(10, 10 * 4);
        display.print(" 4. Sensor");
        break;
      case 3:

        display.setCursor(10, 10 * 1);
        display.print(" 1. Buzzer");
        display.setCursor(10, 10 * 2);
        display.print(" 2. LED");
        display.setCursor(10, 10 * 3);
        display.print(" 3. 7 Segment");
        display.setCursor(10, 10 * 4);
        display.print(">4. Sensor");
        break;
    }
  }

  else if (MENU != 0) {
    switch (MENU) {
      case 1:
        display.setCursor(0, 25);
        display.print ("BUZZER");
        display.setCursor(40, 25);
        display.print("buzzer    "0;
        display.print(buzzstate)
        break;
      case 2:

      case 3
      case 4
    }
  }
}
display.display();
}
