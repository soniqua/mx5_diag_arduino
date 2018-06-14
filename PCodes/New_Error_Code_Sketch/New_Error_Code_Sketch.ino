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
  pinMode(11, OUTPUT);

  //pwm pin test
  analogWrite(11, 150);

  //set ints
  int_count = 0;
  ten_count = 0;
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

  //the below only works for old cars (two digit error codes).

  //I need four digit codes which work as follows:

  //4 codes are displayed [10,12,15,14]
  //they are combined by trimming the tens digit
  //the above is error code 0254

  //need to use count from 0 to 3 to obtain 4 codes
  //then group and clear (wait for timeout)

  duration = pulseIn(pin, HIGH, code_break);
  delay(50);
  //check duration
  //no pulse detected - end of error code
  Serial.print(duration);
  Serial.print(int_count);
  Serial.print(ten_count);
  if ((duration == 0) && (int_count == 0 && ten_count >= 0)) Serial.print("No error code found");
  //check for small pulse
  if (0 < duration && duration < long_puls_thresh) {
    int_count++;
    Serial.print("Short Pulse");
  }
  //check for long pulse
  if (long_puls_thresh < duration && duration < code_break) {
    //if ten_count=1 already then need new code
    switch (ten_count) {
      case 0:
        thou = int_count * 1000;
        int_count = 0;
        break;
      case 1:
        hun = int_count * 100;
        int_count = 0;
        break;
      case 2:
        ten = int_count * 10;
        int_count = 0;
        break;
      case 3:
        //at this point, last digit of error should be known, and should be followed by a 4s timeout
        err_code = thou + hun + ten + int_count;
        break;
    }
    ten_count++;
    Serial.print("Long Pulse");
  }
  //check for no pulse, with content in either int_count or ten_count
  if ((duration == 0) && (int_count >= 0 || ten_count > 0)) {
    ten_count = ten_count * 10;
    err_code = ten_count + int_count;
    Serial.println();
    Serial.print("Error Code is: P");
    Serial.print(err_code);
    Serial.println();
    Serial.print("End of Code");
    ten_count = 0;
    int_count = 0;
  }
  Serial.println();
}
