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

bool lastUpState = 1;
bool lastDownState = 1;
bool lastSelectState = 1;
bool lastBackState = 1;
enum MenuState {
  STATE_MAIN_MENU,
  STATE_MENU_1,
  STATE_MENU_2,
  STATE_MENU_3,
  STATE_MENU_4
};

MenuState currentScreen = STATE_MAIN_MENU;
int menuIndex = 0;

void setup() {
  Serial.begin(115200);

  pinMode(PIN_UP, INPUT_PULLUP);
  pinMode(PIN_DOWN, INPUT_PULLUP);
  pinMode(PIN_SELECT, INPUT_PULLUP);
  pinMode(PIN_BACK, INPUT_PULLUP);
  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_POT, INPUT);
  pinMode(PIN_LED, OUTPUT);

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
  drawScreen();
}

void handleButtons() {
  bool currentUp = digitalRead(PIN_UP);
  bool currentDown = digitalRead(PIN_DOWN);
  bool currentSelect = digitalRead(PIN_SELECT);
  bool currentBack = digitalRead(PIN_BACK);

  if (lastBackState == 0 && currentBack == 1) {
    if (currentScreen != STATE_MAIN_MENU) {
      currentScreen = STATE_MAIN_MENU;
      menuIndex = 0;
    }
    delay(50);
  }
  lastBackState = currentBack;

  if (lastUpState == 0 && currentUp == 1) {
    menuIndex--;
    if (menuIndex < 0) {
      if (currentScreen == STATE_MAIN_MENU) menuIndex = 3;
      else if (currentScreen == STATE_MENU_1 && menuIndex < 1) menuIndex = 0;
      else if (currentScreen == STATE_MENU_2 && menuIndex < 1) menuIndex = 0;
    }
    delay(50);
  }
  lastUpState = currentUp;

  if (lastDownState == 0 && currentDown == 1) {
    menuIndex++;
    if (currentScreen == STATE_MAIN_MENU && menuIndex > 3) menuIndex = 0;
    else if (currentScreen == STATE_MENU_1 && menuIndex > 1) menuIndex = 0;
    else if (currentScreen == STATE_MENU_2 && menuIndex > 1) menuIndex = 0;

    delay(50);
  }
  lastDownState = currentDown;

  if (lastSelectState == 0 && currentSelect == 1) {
    executeSelection();
    delay(50);
  }
  lastSelectState = currentSelect;
}


void executeSelection() {
  switch (currentScreen) {
    case STATE_MAIN_MENU:
      if (menuIndex == 0) {
        currentScreen = STATE_MENU_1;
        menuIndex = 0;
      } else if (menuIndex == 1) {
        currentScreen = STATE_MENU_2;
        menuIndex = 0;
      } else if (menuIndex == 2) {
        currentScreen = STATE_MENU_3;
        menuIndex = 0;
      } else if (menuIndex == 3) {
        currentScreen = STATE_MENU_4;
        menuIndex = 0;
      }
      break;

    case STATE_MENU_1:
      if (menuIndex == 0) {
        digitalWrite(PIN_BUZZER, 1);
      } else if (menuIndex == 1) {
        digitalWrite(PIN_BUZZER, 0);
      }
      break;

    case STATE_MENU_2:
      if (menuIndex == 0) {
        digitalWrite(PIN_LED, 1);
      } else if (menuIndex == 1) {
        digitalWrite(PIN_LED, 0);
      }
      break;
  }
}

void drawScreen() {
  display.clearDisplay();
  display.setCursor(0, 0);
  switch (currentScreen) {
    case STATE_MAIN_MENU:
      display.println(F(" LED "));
      display.drawLine(0, 12, SCREEN_WIDTH, 12, SH110X_WHITE);
      drawItem(0, "1. BUZZER");
      drawItem(1, "2. LED");
      drawItem(2, "3. 7 Segment");
      drawItem(3, "4. POTENTIOMETER");
      break;

    case STATE_MENU_1:
      display.println(F(" BUZZER "));
      display.drawLine(0, 12, SCREEN_WIDTH, 12, SH110X_WHITE);
      drawItem(0, "1. ON");
      drawItem(1, "2. OFF");
      break;

    case STATE_MENU_2:
      display.println(F(" LED "));
      display.drawLine(0, 12, SCREEN_WIDTH, 12, SH110X_WHITE);
      drawItem(0, "1. ON");
      drawItem(1, "2. OFF");
      break;

    case STATE_MENU_3:
      display.println(F(" 7 SEGMENT "));
      display.drawLine(0, 12, SCREEN_WIDTH, 12, SH110X_WHITE);
      break;
    case STATE_MENU_4:
      display.println(F("POTENTIOMETER "));
      display.drawLine(0, 12, SCREEN_WIDTH, 12, SH110X_WHITE);
      drawLoadingBar(); break;
  }
  display.display();
}

void drawLoadingBar() {
  int filledWidth = map(analogRead(PIN_POT), 0, 1023, 0, 118);
  display.drawRect(5, 27, 118, 10, SH110X_WHITE);
  if (filledWidth > 4) {
    display.fillRect(5 + 2, 27 + 2, filledWidth - 4, 10 - 4, SH110X_WHITE);
  }
}

void drawItem(int index, String label) {
  display.setCursor(6, 20 + (index * 12));
  if (index == menuIndex) display.print(F("> "));
  else display.print(F("  "));
  display.println(label);
}
