#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

int calibrationTime = 10;    //the time when the sensor outputs a low impulse 
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin
int ledState = LOW;          // ledState used to set the LED
boolean lockLow = true; 

const int buttonPin = 2;  //pushbutton attached to pin 2
const int pirPin = 3;     //PIR motion sensor output attached to pin 3
const int buzPin = 4;     //buzzer attached to pin 4
const int ledPin = 13;    //led attached to pin 13

unsigned long lastDebounceTime = 0;   // the last time the output pin was toggled
unsigned long debounceDelay = 50;     // the debounce time; increase if the output flickers
unsigned long previousMillis = 0;     // will store last time LED was updated
const long interval = 1000;           // interval at which to blink (milliseconds)

LiquidCrystal_I2C lcd(0x20,16,2);

void setup()
{
  
  pinMode(pirPin, INPUT); 
  pinMode(ledPin, OUTPUT); 
  pinMode(buzPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  digitalWrite(pirPin, LOW); 
  
  lcd.init();
  lcd.backlight();
  lcd.setCursor(1,0);
  lcd.print("Alarm Security");
  lcd.setCursor(5,1);
  lcd.print("System");
  delay (1000);
  lcd.clear();
  lcd.setCursor(5,0);
  lcd.print("LOADING");
  
  //give the sensor some time to calibrate 
  for(int i = 0; i < calibrationTime; i++)
  { 
    lcd.setCursor(i+3,1);
    lcd.print("*");
    delay(100); 
  }
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("Alarm Activated");
} 
  
void loop()
{
  if (digitalRead (pirPin)==HIGH)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Motion Detected");
    lcd.setCursor(0,1);
    lcd.print("Alarm Triggered");
    
    while (lockLow==true)
    {
      unsigned long currentMillis = millis();
      
      if (currentMillis - previousMillis >= interval)    
      {
        previousMillis = currentMillis;      // save the last time you blinked the LED
                                             // if the LED is off turn it on and vice-versa
      if (ledState == LOW) 
      {
        ledState = HIGH;
      } 
      
      else 
      {
        ledState = LOW;
      }
                                         
      digitalWrite(ledPin, ledState);      // set the LED with the ledState of the variable
      digitalWrite(buzPin, ledState);      // set the buzzer with the ledState of the variable
    }
    
    int reading = digitalRead(buttonPin);
    // check to see if you just pressed the button
    // (i.e. the input went from LOW to HIGH), and you've waited long enough
    // since the last press to ignore any noise:
    // If the switch changed, due to noise or pressing:
    
    if (reading != lastButtonState)  
    {// reset the debouncing timer
      lastDebounceTime = millis();
    }

    if ((millis() - lastDebounceTime) > debounceDelay) 
    {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:
    // if the button state has changed:
      if (reading != buttonState) 
      {
        buttonState = reading;

        if (buttonState == HIGH) 
        {
          lockLow=false;
          digitalWrite(ledPin, LOW);
          digitalWrite(buzPin, LOW);
          delay(50);
        }
      }
    }
    // save the reading. Next time through the loop, it'll be the lastButtonState:
    lastButtonState = reading;
   }
  }
  
  if (digitalRead(buttonPin)==HIGH && digitalRead(pirPin)==LOW)
  {
    lcd.clear();      
    lcd.setCursor(3,0);      
    lcd.print("Alarm Reset");
    delay(1000);
    lcd.clear();
    lcd.setCursor(1,0);
    lcd.print("Alarm Activated");     
    lockLow = true;
   }         
}

  

