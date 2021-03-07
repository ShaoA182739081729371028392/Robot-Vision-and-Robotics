#include <LiquidCrystal.h>

//Define PinIn and PinOut

class Button{
  private:
    int pin;
    bool is_pushed = false;
  public:
    Button(int pin_in){
      pin = pin_in;
      pinMode(pin_in, INPUT);
    }
    bool is_button_pushed(void){
      is_pushed = digitalRead(pin);
      if (is_pushed == HIGH){
       return true;
      }
      return false;
    }
    
};
class timer{
  private:
    int minutes_remaining = 5;
    int seconds_remaining = 0;
    bool started = false;
    bool ended = false;
    int cur_time = millis();
  public:
    timer(void){
      
    }
    bool is_started(void){
      return started;
    }
    int get_mins(void){
      return this -> minutes_remaining;
    }
    int get_secs(void){
      return this -> seconds_remaining;
    }
    void decrease_time(void){
      if (!ended && millis() - cur_time >=100){
        cur_time = millis();
        seconds_remaining--;
        if (seconds_remaining < 0 && minutes_remaining == 0){
          seconds_remaining = 0;
        }
        else if (seconds_remaining < 0){
          seconds_remaining = 59;
          minutes_remaining--;
        }
      }
    }
    void increase_time(void){
      if (!ended && millis() - cur_time >= 100){
        cur_time = millis();
        seconds_remaining++;
        if (seconds_remaining > 59){
          minutes_remaining++;
          seconds_remaining = 0;
        }
        
      }
    }
    void reset(void){
      this -> minutes_remaining = 5;
      this -> seconds_remaining = 0;
      this -> started = false;
      this -> ended = false;
    }
    void change_start(void){
      this -> started = !this ->started;   
    }
    bool is_ended(void){
      return this -> ended;
    }
    
    void tick(void){
      if (started and millis() - cur_time >= 1000){
        seconds_remaining--;
        cur_time = millis();
        if (seconds_remaining < 0 && minutes_remaining == 0) {
          ended = true;
          return;
        }
        else if (seconds_remaining < 0){
          minutes_remaining--;
          seconds_remaining = 59;
        }
        
      }
    }
    
};
//Global Variables 
LiquidCrystal lcd(2,3, 4, 5, 6, 7);
int start = 8;
int reset = 9;
int up = 10;
int down = 11;

Button button_start = Button(start);
Button button_reset = Button(reset);
Button button_up = Button(up);
Button button_down = Button(down);
//PiezoBuzzer buzzer = 
timer timer_obj = timer();
void setup() {
  // put your setup code here, to run once:
  // Initialize Liquid Crystal Display
  lcd.begin(16, 2);
  lcd.print("Time Remaining: ");
  lcd.setCursor(0, 1);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  lcd.setCursor(0, 1);
  if (button_start.is_button_pushed()){
    timer_obj.change_start();
  }
  if (button_reset.is_button_pushed() && !timer_obj.is_started()){
    timer_obj.reset();
  }
  if (button_up.is_button_pushed() && !timer_obj.is_started()){
    timer_obj.increase_time();
  }
  if (button_down.is_button_pushed() && !timer_obj.is_started()){
    timer_obj.decrease_time();
  }
  if (!timer_obj.is_ended()){
    timer_obj.tick();  
  }
  if (timer_obj.is_ended()){
    
  }
  lcd.print("Mins:");
  lcd.print(timer_obj.get_mins());
  lcd.print(" Secs:");
  lcd.print(timer_obj.get_secs());

}
