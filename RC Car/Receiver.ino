#include <Wire.h>
#include <Servo.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// Define Pins
#define CE 9
#define CSE 10
int slave_address = 9;
#define Trig 8
#define Echo 7
#define Servo_Control 6
Servo servo;
RF24 radio(CE, CSE);
const byte address[5] = {'R','x','A','A','A'};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  radio.begin();
  radio.setAutoAck(false);
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
  Wire.begin();
  // Setup SR04 Module
  pinMode(Trig, OUTPUT);
  pinMode(Echo, INPUT);
  servo.attach(Servo_Control);
  servo.write(90);
}
int read_dist(void){
  digitalWrite(Trig, LOW);
  delayMicroseconds(2);
  digitalWrite(Trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(Trig, LOW);

  //Read
  long duration = pulseIn(Echo, HIGH);
  int distance = duration * 0.034/2;
  return distance;
}
class State{
  private:
    int override_action;
    int count_down;
  public:
    State(void){
      override_action = 0;
      count_down = 0;
    }
    void update_vals(void){
      if (count_down > 0){
        count_down--;
      }
      else{
        override_action = 0;
      }
      
    }
    void update_state(int action){
      if (count_down > 0){
        count_down--;
      }
      else{
        override_action = action;
      }
    }
    void update_autonomous_state(int action){
      override_action = action;
      count_down = 15; 
    }
    int get_state(void){
      return override_action;
    }
    int get_countdown(void){
      return count_down;
    }
};
void send_data(int process){
  Wire.beginTransmission(slave_address);
  Wire.write(process);
  Wire.endTransmission();
}
State state;
void loop() {
  // put your main code here, to run repeatedly:
  // Read from SR04 first(Override) 
  int process = 0;
  int distance = read_dist();
  state.update_vals();
  Serial.println(distance);
  int count_down = state.get_countdown();
  if (distance < 5){
    // Too Close, so check left and right for opening to turn
    //Check Left
    servo.write(0);
    delay(500);
    //Read Distance Again
    process = 4;
    distance = read_dist();
    if (distance > 5){
      process = 2;
    }
    servo.write(180);
    delay(500);
    distance = read_dist();
    if (distance > 5){
      process = 1;
    }
    servo.write(90);
    delay(500);
    state.update_autonomous_state(process);
    process = state.get_state();
    Serial.println(process);
    send_data(process);
    
    
    
  }
  // Check if Bytes Available
  else if (radio.available()){
    radio.read(&process, sizeof(int));
    process = 3; // If you want it to be fully autonomous and not controllable.
    state.update_state(process);
    process = state.get_state();
    Serial.println(process);
    send_data(process);
  }
  
  
}
