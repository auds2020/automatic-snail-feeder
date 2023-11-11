#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>
#include <Servo.h>  // Include the Servo library

const unsigned long fourDaysInMillis = 3.456 * pow(10, 8);  // 4 days in milliseconds
unsigned long previousMillis = 0;
Servo servo;  // Create a servo object

Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

#define OFF 0x0
#define ON 0x1
#define RED 0x1
#define YELLOW 0x3
#define GREEN 0x2
#define TEAL 0x6
#define BLUE 0x4
#define VIOLET 0x5
#define WHITE 0x7

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.setBacklight(WHITE);
  servo.attach(9);  // Attach the servo to pin 9 (adjust pin number as needed)
  previousMillis = millis();
}

void loop() {
  unsigned long currentMillis = millis();
  lcd.setCursor(0, 0);

  if (currentMillis - previousMillis >= fourDaysInMillis) {
    lcd.clear();
    lcd.print("Refill feeder &");
    lcd.setCursor(0, 1);
    lcd.print("hit reset button");
    
    // Rotate the servo to 0 degrees
    servo.write(0);
    
    while (true) {
      uint8_t buttons = lcd.readButtons();
      if (buttons & BUTTON_SELECT) {
        // Reset the timer and clear the LCD
        previousMillis = millis();
        lcd.clear();
        break;
      }
      delay(100);
    }
  } else {
    unsigned long elapsedTime = currentMillis - previousMillis;
    unsigned long timeLeft = fourDaysInMillis - elapsedTime;
    long hours = timeLeft / (1000UL * 60 * 60);
    long days = (hours + 23) / 24;  // Add 23 to round up if there's any fractional day

    lcd.print("Rotate servo in ");
    lcd.print(days);
    lcd.setCursor(0, 1);
    if (days == 1) {
      lcd.print("day");
    } else {
      lcd.print("days");
    }
    
    // Check if the servo is at or past 180 degrees
    if (servo.read() >= 180) {
      lcd.clear();
      lcd.print("Refill feeder &");
      lcd.setCursor(0, 1);
      lcd.print("hit reset button");
      servo.write(0);  // Reset servo to 0 degrees
    }
  }
}
