// trying to figure out how pulsein works
// going to pulse a high onto pin 7 thru a push switch
// pin 7 is pulled low thru 10k when switch open
// so a press/release is a low-high-low

int pin = 7;
unsigned long duration; //how long a press

void setup()
{
 pinMode(pin, INPUT); //going to be reading it
 pinMode(13, OUTPUT); //use the led as a marker
 pinMode(12,OUTPUT);
 digitalWrite(13, LOW);
 Serial.begin(9600); //going to output duration to monitor
 analogWrite(12,150);
}

void loop()
{
// two quick flashes on 13 to show we at top of loop
digitalWrite(13, HIGH);
 delay(500);
digitalWrite(13, LOW);
 delay(500);
digitalWrite(13, HIGH);
 delay(500);
digitalWrite(13, LOW);
// now measure a high pulse on our pin, with a 2sec timeout
duration = pulseIn(pin, HIGH, 2000000); //2000000microsec = 2 sec
// should the program stop? or should code here get run?
//
delay(5000); //give ample time for switch press or timeout
//if no delay, worried that it runs thru the loop again
//.... and restarts the pulsein "loop"
Serial.println(duration);
// always returns a 0....

}
