#include <Stepper.h>//include stepper librarry
#include <IRremote.h>


const int RECV_PIN = 7;// stores reciever pin number
IRrecv irrecv(RECV_PIN);//assigns IR reciever to a pin on the arduino board
decode_results results;// decodes the results under results variable
unsigned long key_value = 0; // no fucking clue what this is (creates and stores a value that equals the signal that is repeated when a key is held in)


#define STEPS_PER_REV 2000 //200 steps per revolution
Stepper myStepper(2000,8,10,9,11); //assigns pins to stepper motor


void setup(){
  Serial.begin(9600);// starts the serial monitor with a delay of 9600 ms
  irrecv.enableIRIn();//starts the IR reciever
  irrecv.blink13(true);//IR receiver blinks when arduino recieves signal
  myStepper.setSpeed(10);// sets stepper speed
}

void loop(){
  if (irrecv.decode(&results)){
        //determines if a HEX code was successfully recieved and decoded

        if (results.value == 0XFFFFFFFF)
          results.value = key_value;
          //stores the code in results.value and ing

        switch(results.value){
          case 0x22D912BB:
          Serial.println("clockwise");
          myStepper.step(200);
          break;
          case 0x776C6E7A:
          Serial.println("counterclockwise");
          myStepper.step(200);
          break;
          case 0xFFE21D:
          Serial.println("CH+");
          break;
          case 0xFF22DD:
          Serial.println("|<<");
          break;
          case 0xFF02FD:
          Serial.println(">>|");
          break ;  
          case 0xFFC23D:
          Serial.println(">|");
          break ;               
          case 111111:
          Serial.println("-");
          break ;  
          case 0xFFA857:
          Serial.println("+");
          break ;  
          case 222222:
          Serial.println("EQ");
          break ;  
          case 0xFF6897:
          Serial.println("0");
          break ;  
          case 0xFF9867:
          Serial.println("100+");
          break ;
          case 0xFFB04F:
          Serial.println("200+");
          break ;
          case 0xFF30CF:
          Serial.println("1");
          break ;
          case 0xFF18E7:
          Serial.println("2");
          break ;
          case 0xFF7A85:
          Serial.println("3");
          break ;
          case 0xFF10EF:
          Serial.println("4");
          break ;
          case 0xFF38C7:
          Serial.println("5");
          break ;
          case 0xFF5AA5:
          Serial.println("6");
          break ;
          case 0xFF42BD:
          Serial.println("7");
          break ;
          case 0xFF4AB5:
          Serial.println("8");
          break ;
          case 0xFF52AD:
          Serial.println("9");
          break ;      
        }
        key_value = results.value;
        irrecv.resume(); 
  }
}