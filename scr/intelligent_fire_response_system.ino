#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

//LCD Setup
LiquidCrystal_I2C lcd(0x27,16,2);

//DHT Setup
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN,DHTTYPE);

//Pin Definitions
#define MQ2_PIN A0
#define FLAME_PIN 3
#define GREEN_LED 8
#define ORANGE_LED 9
#define RED_LED 10
#define PASSIVE_BUZZER 11

//Threshold Values
int gasThreshold = 400;
float tempThreshold = 50.00;

void setup() {
  Serial.begin(9600);

  dht.begin();
  lcd.init();
  lcd.backlight();

  pinMode(FLAME_PIN,INPUT);
  pinMode(GREEN_LED,OUTPUT);
  pinMode(ORANGE_LED,OUTPUT);
  pinMode(RED_LED,OUTPUT);
  pinMode(PASSIVE_BUZZER,OUTPUT);
  digitalWrite(GREEN_LED, HIGH); // all LEDs HIGH initially serves as a sign that they have been connected and working properly
  digitalWrite(RED_LED, HIGH);
  digitalWrite(ORANGE_LED, HIGH);

  lcd.setCursor(0,0);
  lcd.print("I.F.R.S");
  delay(2000);
  lcd.clear();
}

void loop() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  int gasValue = analogRead(MQ2_PIN);
  int flameDetected = digitalRead(FLAME_PIN);

  Serial.print("Temp:");
  Serial.print(temperature);
  Serial.print("Gas:");
  Serial.print(gasValue);
  Serial.print("Flame:");
  Serial.println(flameDetected);

  lcd.clear();

  //SAFE CONDITION(Low gas, Low temperature, No flame)
  if(gasValue < gasThreshold && temperature < tempThreshold && flameDetected == HIGH) {
    lcd.setCursor(0,0);
    lcd.print("Status: SAFE");
    lcd.setCursor(0,1);
    lcd.print("Temp:");
    lcd.print(temperature);
    safeCondition();
  } 
  
  //WARNING CONDITION(Gas and Smoke Detected)
  else if (gasValue >= gasThreshold && flameDetected == HIGH) {
    lcd.setCursor(0,0);
    lcd.print("Status: WARNING");
    lcd.setCursor(0,1);
    lcd.print("Gas:");
    lcd.print(gasValue);
    warningCondition();
    } 
    
  //DANGER CONDITION(High temperature + gas)  
  else if(temperature >= tempThreshold && gasValue > gasThreshold) {
    lcd.setCursor(0,0);
    lcd.print("Status: DANGER!");
    lcd.setCursor(0,1);
    lcd.print("Temp:");
    lcd.print(temperature);
    dangerCondition();
  }

  //CRITICAL CONDITION(Flame Detected)
  if (flameDetected == LOW) {
    lcd.setCursor(0,0);
    lcd.print("Status: CRITICAL");
    lcd.setCursor(0,1);
    lcd.print("Flame Detected");
    criticalCondition();
  }
  delay(2000);
}

void safeCondition() {
  digitalWrite(GREEN_LED,HIGH);
  digitalWrite(ORANGE_LED,LOW);
  digitalWrite(RED_LED,LOW);
  noTone(PASSIVE_BUZZER);
}

void warningCondition() {
  digitalWrite(GREEN_LED,LOW);
  digitalWrite(RED_LED,LOW);
  digitalWrite(ORANGE_LED,HIGH);
  delay(500);
  digitalWrite(ORANGE_LED,LOW);
  delay(500);
  digitalWrite(ORANGE_LED,HIGH);
  delay(500);
  digitalWrite(ORANGE_LED,LOW);
  delay(500);
  digitalWrite(ORANGE_LED,HIGH);
  delay(500);
  digitalWrite(ORANGE_LED,LOW);
  delay(500);
  noTone(PASSIVE_BUZZER);
}

void dangerCondition() {
digitalWrite(GREEN_LED,LOW);
digitalWrite(ORANGE_LED,LOW);
digitalWrite(RED_LED,HIGH);
delay(500);
digitalWrite(RED_LED,LOW);
delay(500);
digitalWrite(RED_LED,HIGH);
delay(500);
digitalWrite(RED_LED,LOW);
delay(500);
digitalWrite(RED_LED,HIGH);
delay(500);
digitalWrite(RED_LED,LOW);
delay(500);

tone(PASSIVE_BUZZER,1000);
delay(150);
tone(PASSIVE_BUZZER,800);
delay(150);
tone(PASSIVE_BUZZER,1000);
delay(150);
tone(PASSIVE_BUZZER,800);
delay(150);
tone(PASSIVE_BUZZER,1000);
delay(150);
tone(PASSIVE_BUZZER,800);
delay(150);
}

void criticalCondition() {
digitalWrite(GREEN_LED,LOW);
digitalWrite(ORANGE_LED,LOW);
digitalWrite(RED_LED,HIGH);
delay(500);
digitalWrite(RED_LED,LOW);
delay(500);
digitalWrite(RED_LED,HIGH);
delay(500);
digitalWrite(RED_LED,LOW);
delay(500);
digitalWrite(RED_LED,HIGH);
delay(500);
digitalWrite(RED_LED,LOW);
delay(500);

tone(PASSIVE_BUZZER,1000);
delay(500);
noTone(PASSIVE_BUZZER);
delay(100);
tone(PASSIVE_BUZZER,1000);
delay(500);
noTone(PASSIVE_BUZZER);
delay(100);
tone(PASSIVE_BUZZER,1000);
delay(500);
noTone(PASSIVE_BUZZER);
delay(100);
}
