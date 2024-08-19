#include <Stepper.h>
#include <IRremote.h>
#include <uRTCLib.h>
#include <LiquidCrystal.h>
#include <dht.h>

#define DHT11PIN 8 //DHT11 sensor connected to pin 8
dht DHT11; //declares DHT as DHT11

uRTCLib rtc(0x68); //rTC module

const int RECV_PIN = 7; //IR receiver pin
IRrecv irrecv(RECV_PIN);
decode_results results;
unsigned long key_value = 0; //stores the last received IR value

#define STEPS_PER_REV 2000 //2000 steps per revolution
Stepper myStepper(2000, 8, 10, 9, 11); //stepper motor pins

int currentSoundLevel = 0; //variable to store the current sound level

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2; //LCD pins
LiquidCrystal lcd(rs, en, d4, d5, d6, d7); //initializes LCD

void setup() {
  Serial.begin(9600); // Starts the serial monitor
  delay(300);
  URTCLIB_WIRE.begin();
  rtc.set(45, 23, 18, 5, 13, 1, 22);
  irrecv.enableIRIn();
  irrecv.blink13(true); 
  myStepper.setSpeed(10); 
  lcd.begin(16, 2); // Initializes LCD display
}

void loop() {
  if (irrecv.decode(&results)) {
    if (results.value == 0XFFFFFFFF) {
      results.value = key_value;
    }

    switch (results.value) {
      case 0x22D912BB: // Vol Up Pressed
        Serial.println("Clockwise");
        myStepper.step(200);
        break;
      case 0x776C6E7A: // Vol Down Pressed
        Serial.println("Counterclockwise");
        myStepper.step(-200);
        break;
      case 0x898FDF7A : // 0 pressed
        updateSoundLevel(0);
        break;
      case 0x6BC6597B: // 1 pressed
        updateSoundLevel(10);
        break;
      case 0x735B797E: // 2 pressed
        updateSoundLevel(20);
        break;
      case 0x1EC81DBF: // 3 pressed
        updateSoundLevel(30);
        break;
      case 0x450753D6: // 4 pressed
        updateSoundLevel(40);
        break;
      case 0xBA0F4EDF: // 5 pressed
        updateSoundLevel(50);
        break;
      case 0x4AC4DA9A: // 6 pressed
        updateSoundLevel(60);
        break;
      case 0xF6317EDB: // 7 pressed
        updateSoundLevel(70);
        break;
      case 0xF9000E7E: // 8 pressed
        updateSoundLevel(80);
        break;
      case 0xC7291B77: // 9 pressed
        updateSoundLevel(90);
        break;
      case 0x61B73107: // "." pressed
        displaySoundLevel();
        break;
      case 0xD4BE4E37: // Last CH pressed
        displayTempAndTime();
        break;
      default:
        Serial.println("Unknown command");
        break;
    }
    key_value = results.value; //store the last received IR value
    irrecv.resume(); //ready to receive the next signal
  }
}

void updateSoundLevel(int targetLevel) {
  int stepDifference = targetLevel - currentSoundLevel;
  int stepsToMove = stepDifference * 15;
  myStepper.step(stepsToMove);
  currentSoundLevel = targetLevel;
  Serial.print("Sound Level: ");
  Serial.println(currentSoundLevel);
  displaySoundLevel(); //display the updated sound level
}

void displaySoundLevel() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Sound Level:");
  lcd.setCursor(0, 1);
  lcd.print(currentSoundLevel);
}

void displayTempAndTime() {
  int chk = DHT11.read11(DHT11PIN); //reads data from the DHT11 sensor
  
  float temperatureF = (((float)DHT11.temperature) * 9 / 5) + 32;
  int humidity = DHT11.humidity;

  rtc.refresh();
  int hour = rtc.hour();
  int minute = rtc.minute();
  int second = rtc.second();

  // Display on LCD
  lcd.clear();
  lcd.setCursor(6, 0);
  lcd.print("Hum.:");
  lcd.print(humidity, 0);
  lcd.print("%");

  lcd.setCursor(0, 0);
  lcd.print(temperatureF, 0);
  lcd.write(223);
  lcd.print("F");

  lcd.setCursor(0, 1);
  if(hour < 10) lcd.print("0");
  lcd.print(hour);
  lcd.print(":");
  
  if(minute < 10) lcd.print("0");
  lcd.print(minute);
  lcd.print(":");
  
  if(second < 10) lcd.print("0");
  lcd.print(second);

  // Print to Serial Monitor
  Serial.print("Temperature: ");
  Serial.print(temperatureF, 0);
  Serial.println("F");

  Serial.print("Humidity: ");
  Serial.print(humidity, 0);
  Serial.println("%");

  Serial.print("Time: ");
  if(hour < 10) Serial.print("0");
  Serial.print(hour);
  Serial.print(":");
  if(minute < 10) Serial.print("0");
  Serial.print(minute);
  Serial.print(":");
  if(second < 10) Serial.print("0");
  Serial.println(second);
}
