#include <LiquidCrystal.h>
#include <stdlib.h>

// LCD1602 in 4-bit mode: RS, E, D4, D5, D6, D7.
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
const byte BUTTON_PIN = 8;  // Button connects this pin to GND.
const unsigned long DEBOUNCE_MS = 35;

const char *const BASE_NAMES[] = {"BIN", "OCT", "DEC", "HEX"};
const byte BASE_VALUES[] = {2, 8, 10, 16};
byte selectedBase = 0;
unsigned int number = 42;  // Must stay in the range 0..65535.

char inputBuffer[8];  // Five digits plus optional CR and terminator.
byte inputLength = 0;
bool inputTooLong = false;

bool lastButtonReading = HIGH;
bool stableButtonState = HIGH;
unsigned long lastButtonChange = 0;

void showNumber() {
  char digits[17];
  char row[17];

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Decimal: ");
  lcd.print(number);

  lcd.setCursor(0, 1);
  if (selectedBase == 0) {
    // 16 binary digits fill exactly one LCD row.
    for (byte bit = 0; bit < 16; ++bit) {
      lcd.print((number & (1U << (15 - bit))) ? '1' : '0');
    }
  } else {
    ultoa(number, digits, BASE_VALUES[selectedBase]);
    // Make hexadecimal letters uppercase for the LCD.
    for (byte i = 0; digits[i] != '\0'; ++i) {
      if (digits[i] >= 'a' && digits[i] <= 'f') digits[i] -= ('a' - 'A');
    }
    snprintf(row, sizeof(row), "%s: %s", BASE_NAMES[selectedBase], digits);
    lcd.print(row);
  }
}

void acceptInput() {
  if (inputLength == 0 && !inputTooLong) return;

  inputBuffer[inputLength] = '\0';
  unsigned long result = 0;
  bool valid = !inputTooLong && inputLength > 0;

  for (byte i = 0; valid && i < inputLength; ++i) {
    char c = inputBuffer[i];
    if (c < '0' || c > '9') {
      valid = false;
    } else {
      result = result * 10 + (c - '0');
      if (result > 65535UL) valid = false;
    }
  }

  if (valid) {
    number = (unsigned int)result;
    showNumber();
    Serial.print(F("Set value to "));
    Serial.println(number);
  } else {
    Serial.println(F("Enter a whole decimal number from 0 to 65535."));
  }

  inputLength = 0;
  inputTooLong = false;
}

void readSerialInput() {
  while (Serial.available() > 0) {
    char c = (char)Serial.read();
    if (c == '\n' || c == '\r') {
      acceptInput();  // Handles both NL and CR+NL line endings.
    } else if (inputLength < sizeof(inputBuffer) - 1) {
      inputBuffer[inputLength++] = c;
    } else {
      inputTooLong = true;
    }
  }
}

void readButton() {
  bool reading = digitalRead(BUTTON_PIN);
  if (reading != lastButtonReading) lastButtonChange = millis();

  if (millis() - lastButtonChange >= DEBOUNCE_MS && reading != stableButtonState) {
    stableButtonState = reading;
    if (stableButtonState == LOW) {
      selectedBase = (selectedBase + 1) % 4;
      showNumber();
    }
  }
  lastButtonReading = reading;
}

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  Serial.begin(9600);
  lcd.begin(16, 2);
  showNumber();
  Serial.println(F("Type a decimal number 0..65535 and press Enter."));
  Serial.println(F("Press the button to cycle BIN -> OCT -> DEC -> HEX."));
}

void loop() {
  readSerialInput();
  readButton();
}
