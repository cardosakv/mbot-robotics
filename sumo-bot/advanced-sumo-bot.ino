#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>

#include <MeMCore.h>

MeDCMotor motor_9(9);
MeDCMotor motor_10(10);
void move(int direction, int speed)
{
      int leftSpeed = 0;
      int rightSpeed = 0;
      if(direction == 1){
        	leftSpeed = speed;
        	rightSpeed = speed;
      }else if(direction == 2){
        	leftSpeed = -speed;
        	rightSpeed = -speed;
      }else if(direction == 3){
        	leftSpeed = -speed;
        	rightSpeed = speed;
      }else if(direction == 4){
        	leftSpeed = speed;
        	rightSpeed = -speed;
      }
      motor_9.run((9)==M1?-(leftSpeed):(leftSpeed));
      motor_10.run((10)==M1?-(rightSpeed):(rightSpeed));
}
double angle_rad = PI/180.0;
double angle_deg = 180.0/PI;
void get_UltrasonicSensor_Distance();
double UltraSonic_Distance;
MeUltrasonicSensor ultrasonic_3(3);
void get_LineFollower_State();
double LineFollower_State;
MeLineFollower linefollower_2(2);
void generate_RandomScan_Direction();
double RandomScan_Direction;
void scan_Enemy();
double Last_SumoRingContact_LFState;
double Scan_Speed;
void skill_Blur();
void skill_Dispel();
void skill_Charge();
double Charge_Speed;
void skill_Duel();
double Duel_Speed;
double Base_Speed;
double EnemyDetected_Threshold_Distance;
double Duel_Threshold_Distance;
double Blur_Threshold_Distance;
double Scan_Counter;
double EnemyDistance_Previous;
double EnemyDistance_Current;
double EnemyDistance_Difference;
double currentTime = 0;
double lastTime = 0;
double getLastTime(){
    	return currentTime = millis()/1000.0 - lastTime;
}

void get_UltrasonicSensor_Distance()
{
    UltraSonic_Distance = ultrasonic_3.distanceCm();
}

void get_LineFollower_State()
{
    LineFollower_State = linefollower_2.readSensors();
}

void generate_RandomScan_Direction()
{
    RandomScan_Direction = random(1,(2)+1);
}

void scan_Enemy()
{
    if((((Last_SumoRingContact_LFState)==(1))) || (((RandomScan_Direction)==(1)))){
        motor_9.run((9)==M1?-(Scan_Speed):(Scan_Speed));
        motor_10.run((10)==M1?-((Scan_Speed) * (-1)):((Scan_Speed) * (-1)));
    }else{
        if((((Last_SumoRingContact_LFState)==(2))) || (((RandomScan_Direction)==(2)))){
            motor_9.run((9)==M1?-((Scan_Speed) * (-1)):((Scan_Speed) * (-1)));
            motor_10.run((10)==M1?-(Scan_Speed):(Scan_Speed));
        }else{
            if((((Last_SumoRingContact_LFState)==(3))) && (((RandomScan_Direction)==(1)))){
                motor_9.run((9)==M1?-(Scan_Speed):(Scan_Speed));
                motor_10.run((10)==M1?-((Scan_Speed) * (-1)):((Scan_Speed) * (-1)));
            }else{
                motor_9.run((9)==M1?-((Scan_Speed) * (-1)):((Scan_Speed) * (-1)));
                motor_10.run((10)==M1?-(Scan_Speed):(Scan_Speed));
            }
        }
    }
}

void skill_Blur()
{
    if((((Last_SumoRingContact_LFState)==(1))) || (((RandomScan_Direction)==(1)))){
        motor_9.run((9)==M1?-(-255):(-255));
        motor_10.run((10)==M1?-(-50):(-50));
    }else{
        if((((Last_SumoRingContact_LFState)==(2))) || (((RandomScan_Direction)==(2)))){
            motor_9.run((9)==M1?-(-50):(-50));
            motor_10.run((10)==M1?-(-255):(-255));
        }
    }
}

void skill_Dispel()
{
    if(((RandomScan_Direction)==(1))){
        motor_9.run((9)==M1?-(-255):(-255));
        motor_10.run((10)==M1?-(0):(0));
    }else{
        if(((RandomScan_Direction)==(2))){
            motor_9.run((9)==M1?-(0):(0));
            motor_10.run((10)==M1?-(-255):(-255));
        }
    }
}

void skill_Charge()
{
    motor_9.run((9)==M1?-((Charge_Speed) + (2)):((Charge_Speed) + (2)));
    motor_10.run((10)==M1?-((Charge_Speed) + (2)):((Charge_Speed) + (2)));
}

void skill_Duel()
{
    motor_9.run((9)==M1?-(Duel_Speed):(Duel_Speed));
    motor_10.run((10)==M1?-(Duel_Speed):(Duel_Speed));
}

void setup(){
    Base_Speed = 220;
    Scan_Speed = 215;
    Duel_Speed = 255;
    Charge_Speed = 235;
    EnemyDetected_Threshold_Distance = 100;
    Duel_Threshold_Distance = 5;
    Blur_Threshold_Distance = 10;
    RandomScan_Direction = 1;
    pinMode(A7,INPUT);
    while(!((0^(analogRead(A7)>10?0:1))))
    {
        _loop();
    }
    _delay(5);
}

void loop(){
    get_UltrasonicSensor_Distance();
    get_LineFollower_State();
    if(!(((LineFollower_State)==(0)))){
        motor_9.run((9)==M1?-((Base_Speed) * (-1)):((Base_Speed) * (-1)));
        motor_10.run((10)==M1?-((Base_Speed) * (-1)):((Base_Speed) * (-1)));
        Last_SumoRingContact_LFState = LineFollower_State;
    }
    if((!(((UltraSonic_Distance)==(EnemyDetected_Threshold_Distance)))) || (!((UltraSonic_Distance) < (EnemyDetected_Threshold_Distance)))){
        if((Scan_Counter) > (5)){
            lastTime = millis()/1000.0;
            while(!((getLastTime()) > (1)))
            {
                _loop();
                skill_Dispel();
            }
        }else{
            scan_Enemy();
            Scan_Counter = (Scan_Counter) + (0.5);
        }
    }else{
        EnemyDistance_Previous = UltraSonic_Distance;
        _delay(0.5);
        get_UltrasonicSensor_Distance();
        EnemyDistance_Current = UltraSonic_Distance;
        EnemyDistance_Difference = (EnemyDistance_Current) - (EnemyDistance_Previous);
        generate_RandomScan_Direction();
        Scan_Counter = 0;
        if((EnemyDistance_Difference) > (2)){
            while(!(((!(((UltraSonic_Distance)==(EnemyDetected_Threshold_Distance)))) || (!((UltraSonic_Distance) < (EnemyDetected_Threshold_Distance)))) || (((LineFollower_State)==(3)))))
            {
                _loop();
                get_UltrasonicSensor_Distance();
                get_LineFollower_State();
                skill_Charge();
            }
        }else{
            if((EnemyDistance_Difference) < (-2)){
                if(((EnemyDistance_Current) < (Blur_Threshold_Distance)) && ((EnemyDistance_Current) > (Duel_Threshold_Distance))){
                    lastTime = millis()/1000.0;
                    while(!((getLastTime()) > (0.8)))
                    {
                        _loop();
                        skill_Blur();
                    }
                }else{
                    if(((EnemyDistance_Current) < (Duel_Threshold_Distance)) && ((EnemyDistance_Current) > (1))){
                        lastTime = millis()/1000.0;
                        while(!(((!(((UltraSonic_Distance)==(EnemyDetected_Threshold_Distance)))) || (!((UltraSonic_Distance) < (EnemyDetected_Threshold_Distance)))) || (((LineFollower_State)==(3)))))
                        {
                            _loop();
                            get_UltrasonicSensor_Distance();
                            get_LineFollower_State();
                            skill_Duel();
                            if((getLastTime()) > (10)){
                                lastTime = millis()/1000.0;
                                while(!((getLastTime()) > (0.8)))
                                {
                                    _loop();
                                    skill_Dispel();
                                }
                            }
                        }
                    }else{
                        motor_9.run((9)==M1?-(0):(0));
                        motor_10.run((10)==M1?-(0):(0));
                    }
                }
            }else{
                while(!(((!(((UltraSonic_Distance)==(EnemyDetected_Threshold_Distance)))) || (!((UltraSonic_Distance) < (EnemyDetected_Threshold_Distance)))) || (((LineFollower_State)==(3)))))
                {
                    _loop();
                    get_UltrasonicSensor_Distance();
                    get_LineFollower_State();
                    skill_Charge();
                }
            }
        }
        Last_SumoRingContact_LFState = 0;
    }
    _loop();
}

void _delay(float seconds){
    long endTime = millis() + seconds * 1000;
    while(millis() < endTime)_loop();
}

void _loop(){
}
