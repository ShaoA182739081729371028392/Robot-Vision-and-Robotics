// Import dependencies
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
// Define necessary Ports
#define JoyStick_x A0
#define JoyStick_y A1

#define RightButton 10
#define DownButton 9
#define UpButton 8
#define LeftButton 7

// Initialize the Wireless Transmitter
#define CE 5
#define CSN 6

RF24 radio(CE, CSN);
const byte address[5] = {'R','x','A','A','A'};

class Joystick{
  private:
    int pin_x;
    int pin_y;
  public:
    Joystick(int pin_x, int pin_y){
      this -> pin_x = pin_x;
      this -> pin_y = pin_y;
    }
    int read_x(void){
      return analogRead(pin_x);
    }
    int read_y(void){
      return analogRead(pin_y);
    }
};
class Button{
  private:
   int pin;
  public: 
    Button(int pin){
      this -> pin = pin;
      pinMode(this -> pin, INPUT);
    }
    bool is_pushed(void){
      int val = digitalRead(this -> pin);
      if (val == HIGH){
        return true;
      }
      return false;
    }
};
// Initialize Buttons
Button up(UpButton);
Button down(DownButton);
Button left(LeftButton);
Button right(RightButton);
// Initialize JoyStick
Joystick joystick(JoyStick_x, JoyStick_y);

void setup() {
  // put your setup code here, to run once:
  // Initialize Transmitter
  radio.begin();
  radio.setAutoAck(false);
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
 
}

void loop() {
  // put your main code here, to run repeatedly:
  // Read X value from joystick
  int x = joystick.read_x();
  int y = joystick.read_y();
  // Car can only perform one action at a time, so preference is (left = 1, right = 2, up = 3, down = 4, idle = 0)
  int action = 0; // Idle
  
  if (x < 200){
    action = 1; // Left
  }
  else if (x > 900){
    action = 2; // Right
  }
  else if (y < 200){
    action = 3; // Forward
  }
  else if(y > 900){
    action = 4; // Backward
  }
  radio.write(&action, sizeof(int));
  delay(100);
}
