#include <Adafruit_RGBLCDShield.h>

const unsigned long fourDaysInMillis = 345600000;  // 4 days in milliseconds
unsigned long previousMillis = 0;
long lastDisplayedDays = -1; // Start with an invalid value

Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

void setup() {
  lcd.begin(16, 2);
  lcd.setBacklight(0x7); // WHITE
  previousMillis = millis();
}

void displayDays(long days) {
  if (days != lastDisplayedDays) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Feed snails in ");
    lcd.print(days);
    lcd.setCursor(0, 1);
    lcd.print(days == 1 ? "day" : "days");
    lastDisplayedDays = days;
  }
}

void loop() {
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= fourDaysInMillis) {
        lcd.clear();
        lcd.print("FEED SNAILS &");
        lcd.setCursor(0, 1);
        lcd.print("HIT RESET BUTTON");
        
        while (true) {
            if (lcd.readButtons() & BUTTON_SELECT) {
                previousMillis = currentMillis; // Reset the timer
                lastDisplayedDays = -1; // Reset displayed days
                break;
            }
            delay(100);
        }
    }
    else {
      unsigned long timeLeft = fourDaysInMillis - (currentMillis - previousMillis);
      long hours = timeLeft / (1000UL * 60 * 60);
      long days = (hours + 23) / 24;  // Add 23 to round up if there's any fractional day
      displayDays(days);
    }
}
