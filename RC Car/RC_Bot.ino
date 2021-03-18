#include <Wire.h>

int slave_address =9;
int RFIn1 = 6;
int RFIn2 = 7;
int LFIn3 = 8;
int LFIn4 = 9;

int RBIn3 = 4;
int RBIn4 = 5;
int LBIn1 = 2;
int LBIn2 = 3;
 
class Wheel{
  private:
    int in1;
    int in2;
  public:
    Wheel(int in1, int in2){
      this -> in1 = in1;
      this -> in2 = in2;
    }
    void start_up(void){
      pinMode(in1, OUTPUT);
      pinMode(in2, OUTPUT);
    }
    void reset(void){
      //Turns off the wheel
      digitalWrite(in1, LOW);
      digitalWrite(in2, LOW);
    }
    void forward(void){
      // Turns this wheel forward
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW); 
    }
    void backward(void){
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
    }
};


class RCBot{
  private:
    Wheel RF = Wheel(1, 2);
    Wheel RB = Wheel(1, 2);
    Wheel LF = Wheel(1, 2);
    Wheel LB = Wheel(1, 2);
  
  public:
    RCBot(void){
      RF = Wheel(RFIn1, RFIn2);
      RB = Wheel(RBIn3, RBIn4);
      LF = Wheel(LFIn3, LFIn4);
      LB = Wheel(LBIn1, LBIn2);
      RF.start_up();
      RB.start_up();
      LF.start_up();
      LB.start_up(); 
    }
    
    void tick(void){
      // Tick used at every iteration of code
      RF.reset();
      RB.reset();
      LF.reset();
      LB.reset();
    }
    void straight_forward(void){
      // Runs the Car Straight Forward
      RF.forward();
      RB.forward();
      LF.forward();
      LB.forward();
    }
    void straight_backward(void){
      RF.backward();
      RB.backward();
      LF.backward();
      LB.backward();
    }
    void turn_left(void){
      // Turns the Car Left
      RF.reset();
      RB.reset();
      LF.backward();
      LB.backward();
    }
    void turn_right(void){
      // Turns the Car Right
      LF.reset();
      LB.reset();
      RF.backward();
      RB.backward();
    }
    
};
  
RCBot car = RCBot();
int action = 0;
void Receive(int bytes){
  action = Wire.read();
  //Serial.println("Received: ");
}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Wire.begin(slave_address);
  Wire.onReceive(Receive);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(action);
  if (action == 0){
    car.tick();
  }
  else if (action == 1){
    car.turn_left();
  }
  else if (action == 2){
    car.turn_right();
  }
  else if (action == 3){
    car.straight_forward();
  }
  else if (action == 4){
    car.straight_backward();
  }
  delay(100);
}
