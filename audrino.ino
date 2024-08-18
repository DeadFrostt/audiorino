#include <Stepper.h>
#include <IRremote.h>

const int RECV_PIN = 7; //IR receiver pin
IRrecv irrecv(RECV_PIN);
decode_results results;
unsigned long key_value = 0; //stores the last received IR value

#define STEPS_PER_REV 2000 //2000 steps per revolution
Stepper myStepper(2000, 8, 10, 9, 11); //stepper motor pins

int currentSoundLevel = 0; //variable to store the current sound level

void setup(){
  Serial.begin(9600); //starts the serial monitor
  irrecv.enableIRIn(); //starts the IR receiver
  irrecv.blink13(true); //blinks when IR signal is received
  myStepper.setSpeed(10); //sets stepper speed
}

void loop(){
  if (irrecv.decode(&results)){
    if (results.value == 0XFFFFFFFF) {
      results.value = key_value;
    }

    switch (results.value) {
      case 0x22D912BB:
        Serial.println("Clockwise");
        myStepper.step(200);
        break;
      case 0x776C6E7A:
        Serial.println("Counterclockwise");
        myStepper.step(-200);
        break;
      case 0xFF6897:
        updateSoundLevel(0);
        break;
      case 0xFF30CF: 
        updateSoundLevel(10);
        break;
      case 0xFF18E7:
        updateSoundLevel(20);
        break;
      case 0xFF7A85:
        updateSoundLevel(30);
        break;
      case 0xFF10EF:
        updateSoundLevel(40);
        break;
      case 0xFF38C7:
        updateSoundLevel(50);
        break;
      case 0xFF5AA5:
        updateSoundLevel(60);
        break;
      case 0xFF42BD:
        updateSoundLevel(70);
        break;
      case 0xFF4AB5:
        updateSoundLevel(80);
        break;
      case 0xFF52AD: 
        updateSoundLevel(90);
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
  int stepsToMove = stepDifference * 200;
  myStepper.step(stepsToMove);
  currentSoundLevel = targetLevel;

  //display the current sound level in the console
  Serial.print("Sound Level: ");
  Serial.println(currentSoundLevel);
}
