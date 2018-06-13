#include <LiquidCrystal.h>
#include <Wire.h>

//define pulse lengths
const unsigned long long_puls_thresh = 1300000;
const unsigned long code_break  = 3900000;

//define counters
int int_count;
int ten_count;
int err_code;
unsigned long duration;

//define IO
int pin = 7;

//define LCD items
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  // basic code designed to data log from MX5 diagnostic port

  //diag structure:
  //a pulse of under 1 second indicates an integer (ones)
  //a pulse of 1.4 seconds indicates an integer (tens)
  //no pulse for 4 seconds indicates end of error code

  //init serial
  Serial.begin(9600);

  //initialise pins
  //set pin (7) as input
  pinMode(pin, INPUT);

  //lcd init
  lcd.begin(16, 2);
  lcd.setCursor(0,0);
  lcd.print("Ready");
}

void loop() {
  //psuedocode
  //when pin goes high (use pulseINLong(pin,mode,timeout)
  //if duration <1.4 then short pulse
  //add one to int counter
  //back to top
  //if long_puls_thresh<duration>code_break then long pulse
  //add one to tens counter
  //back to top
  //if timeout triggered, error code has ended
  //save data and output
  //reset counters

  duration = pulseInLong(pin, HIGH, code_break);
  //check duration
  //no pulse detected - end of error code
  if ((duration == 0) && (int_count = 0 && ten_count > 0)) Serial.print("No error code found");
  //check for small pulse
  if (0 < duration && duration < long_puls_thresh) {
    int_count++;
    Serial.print("Short Pulse");
  }
  //check for long pulse
  if (long_puls_thresh < duration && duration < code_break) {
    ten_count++;
    Serial.print("Long Pulse");
  }
  //check for no pulse, with content in either int_count or ten_count
  if ((duration == 0) && (int_count > 0 || ten_count > 0)) {
    ten_count = ten_count * 10;
    err_code = ten_count + int_count;
    Serial.print("End of Code");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Error:");
    lcd.setCursor(0, 1);
    lcd.print(err_code);
    ten_count = 0;
    int_count = 0;
  }
}
