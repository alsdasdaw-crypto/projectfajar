#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

// --- DISPLAY SETUP ---
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define i2c_Address 0x3C 

Adafruit_SH1106G display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// --- BUTTON PINS ---
const int PIN_UP = 3;
const int PIN_DOWN = 2;
const int PIN_SELECT = 4;
const int PIN_BACK = A3; // <--- 4th Button for Back

// --- HARDWARE PINS ---
const int PIN_LED = 9;
const int PIN_BUZZER = 13;
bool ledState = false;
bool buzzerState = false;

// --- MENU STATES ---
enum MenuState {
  STATE_MAIN_MENU,
  STATE_MENU_1,      // LED & Buzzer toggles
  STATE_MENU_2,      // Custom String action
  STATE_MENU_3       // Predefined words
};

MenuState currentScreen = STATE_MAIN_MENU;
int menuIndex = 0; 

// --- MENU 3 PREDEFINED WORDS ---
const int totalWords = 3;
const char* componentWords[totalWords] = {"Resistor", "Capacitor", "Diode"};

void setup() {
  Serial.begin(115200);

  pinMode(PIN_UP, INPUT_PULLUP);
  pinMode(PIN_DOWN, INPUT_PULLUP);
  pinMode(PIN_SELECT, INPUT_PULLUP);
  pinMode(PIN_BACK, INPUT_PULLUP); // <--- Initialize 4th button
  
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);

  if(!display.begin(i2c_Address, true)) { 
    Serial.println(F("SH110X allocation failed"));
    while(1);
  }

  display.display();
  delay(1000);
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
}

void loop() {
  handleButtons();
  drawScreen();
}

void handleButtons() {
  bool btnUp = (digitalRead(PIN_UP) == LOW);
  bool btnDown = (digitalRead(PIN_DOWN) == LOW);
  bool btnSelect = (digitalRead(PIN_SELECT) == LOW);
  bool btnBack = (digitalRead(PIN_BACK) == LOW); // <--- Read 4th button

  if (!btnUp && !btnDown && !btnSelect && !btnBack) return; 

  // --- 4TH BUTTON: INSTANT BACK ---
  if (btnBack) {
    if (currentScreen != STATE_MAIN_MENU) {
      currentScreen = STATE_MAIN_MENU;
      menuIndex = 0; // Reset index back to top of main menu
    }
    delay(250); // Debounce
    return;
  }

  // Navigation logic (Up/Down) with clean bounds
  if (btnUp) {
    menuIndex--;
    if (menuIndex < 0) {
      if (currentScreen == STATE_MAIN_MENU) menuIndex = 2;
      else if (currentScreen == STATE_MENU_1) menuIndex = 1; // 2 items (0 to 1)
      else if (currentScreen == STATE_MENU_2) menuIndex = 0; // 1 item (0)
      else if (currentScreen == STATE_MENU_3) menuIndex = totalWords - 1; // 3 words (0 to 2)
    }
    delay(200); 
  }

  if (btnDown) {
    menuIndex++;
    if (currentScreen == STATE_MAIN_MENU && menuIndex > 2) menuIndex = 0;
    else if (currentScreen == STATE_MENU_1 && menuIndex > 1) menuIndex = 0;
    else if (currentScreen == STATE_MENU_2 && menuIndex > 0) menuIndex = 0;
    else if (currentScreen == STATE_MENU_3 && menuIndex >= totalWords) menuIndex = 0;
    delay(200); 
  }

  // Selection logic (Enter/OK)
  if (btnSelect) {
    executeSelection();
    delay(300); 
  }
}

void executeSelection() {
  switch (currentScreen) {
    case STATE_MAIN_MENU:
      if (menuIndex == 0) { currentScreen = STATE_MENU_1; menuIndex = 0; }
      else if (menuIndex == 1) { currentScreen = STATE_MENU_2; menuIndex = 0; }
      else if (menuIndex == 2) { currentScreen = STATE_MENU_3; menuIndex = 0; }
      break;

    case STATE_MENU_1: 
      if (menuIndex == 0) {
        ledState = !ledState;
        digitalWrite(PIN_LED, ledState ? HIGH : LOW);
      } else if (menuIndex == 1) {
        buzzerState = !buzzerState;
        digitalWrite(PIN_BUZZER, buzzerState ? HIGH : LOW);
      }
      break;

    case STATE_MENU_2: 
      if (menuIndex == 0) {
        // --- YOUR CUSTOM STRING ACTION HERE ---
        display.clearDisplay();
        display.setCursor(13, 30);
        display.setTextSize(2);
        String myCustomString = "XI TEI 2";
        display.println(myCustomString);
        Serial.println(myCustomString);
        
        display.display();
        display.setTextSize(1);
        delay(2000); 
      }
      break;

    case STATE_MENU_3: 
      if (menuIndex < totalWords) {
        display.clearDisplay();
        display.setTextSize(2);
        display.setCursor(13, 30);
        display.println(componentWords[menuIndex]);
        Serial.println(componentWords[menuIndex]);
        
        display.display();
        display.setTextSize(1);
        delay(2000);
      }
      break;
  }
}

void drawScreen() {
  display.clearDisplay();
  display.setCursor(0, 0);

  switch (currentScreen) {
    case STATE_MAIN_MENU:
      display.println(F("=== MAIN MENU ==="));
      display.drawLine(0, 12, SCREEN_WIDTH, 12, SH110X_WHITE);
      
      drawItem(0, "1. LED & Buzzer");
      drawItem(1, "2. Custom String");
      drawItem(2, "3. Pick Component");
      break;

    case STATE_MENU_1:
      display.println(F("=== LED & BUZZER ==="));
      display.drawLine(0, 12, SCREEN_WIDTH, 12, SH110X_WHITE);
      
      drawItemStatus(0, "LED:   ", ledState ? "[ON]" : "[OFF]");
      drawItemStatus(1, "Buzzer:", buzzerState ? "[ON]" : "[OFF]");
      
      display.setCursor(0, 52);
      display.print(F("[Press Back to Return]"));
      break;

    case STATE_MENU_2:
      display.println(F("=== CUSTOM STRING ==="));
      display.drawLine(0, 12, SCREEN_WIDTH, 12, SH110X_WHITE);
      
      drawItem(0, "Run Text Action");
      
      display.setCursor(0, 52);
      display.print(F("[Press Back to Return]"));
      break;

    case STATE_MENU_3:
      display.println(F("=== PICK COMPONENT ==="));
      display.drawLine(0, 12, SCREEN_WIDTH, 12, SH110X_WHITE);
      
      for (int i = 0; i < totalWords; i++) {
        drawItem(i, componentWords[i]);
      }
      
      display.setCursor(0, 52);
      display.print(F("[Press Back to Return]"));
      break;
  }

  display.display();
}

// Helper for standard items
void drawItem(int index, String label) {
  display.setCursor(6, 20 + (index * 12));
  if (index == menuIndex) display.print(F("> "));
  else display.print(F("  "));
  display.println(label);
}

// Helper for items with status tags like [ON]/[OFF]
void drawItemStatus(int index, String label, String status) {
  display.setCursor(6, 20 + (index * 12));
  if (index == menuIndex) display.print(F("> "));
  else display.print(F("  "));
  display.print(label);
  display.println(status);
}
