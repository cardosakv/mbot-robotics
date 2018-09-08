#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>

#include <MeMCore.h>

MeDCMotor motor1(9);
MeDCMotor motor2(10);		
MeLineFollower lineFollower(2);
		
double Kp;
double Ki;
double Kd;
double targetError;
double targetSpeed;
double lastError;
double currentError;
double currentLFState;
double turnDirection;
double rotationSpeedM1;
double rotationSpeedM2;
double proportion;
double integral;
double derivative;
double turn;
double speedMotor1;
double speedMotor2;
double currentTime = 0;
double lastTime = 0;

double getLastTime()
{
    return currentTime = millis()/1000.0 - lastTime;
}

void setup() 
{
    Kp = 25;
    Ki = 2;
    Kd = 20;
    proportion = 0;
    integral = 0;
    derivative = 0; 
    targetError = 0;
    targetSpeed = 215;
    lastError = 0;
    
    pinMode(A7,INPUT);
    
    while(!((0^(analogRead(A7)>10?0:1))))
    {
        _loop();
    }
}

void loop() 
{
    currentLFState = lineFollower.readSensors();
		
    if ((currentLFState)==(0))
    {
        currentError = 0;
    } 
    else if ((currentLFState)==(1))
    {
        currentError = -1;
    } 
    else if ((currentLFState)==(2))
    {
        currentError = 1;
    } 
    else if ((currentLFState)==(3)) 
    {
        if ((lastError)==(1)) 
	{
            currentError = 2;
        } 
	else if ((lastError)==(-1)) 
	{
            currentError = -2;
        } 
	else if ((lastError)==(0)) 
	{					
            turnDirection = random(1,2);
	    
	    if ((turnDirection)==(1))
	    {
	    	rotationSpeedM1 = 220;
                rotationSpeedM2 = -220;
            }
	    else
	    {
                rotationSpeedM1 = -220;
                rotationSpeedM2 = 220;
            }
	    
	    lastTime = millis()/1000.0;
	    
            while ((currentLFState)==(3))
            {
                _loop();
		currentLFState = lineFollower.readSensors();
		
                motor_9.run((9)==M1?-(rotationSpeedM1):(rotationSpeedM1));
		motor_10.run((10)==M1?-(rotationSpeedM2):(rotationSpeedM2));
            }
	    
	    if((getLastTime()) > (1))
	    {
                lastTime = millis()/1000.0;
                                    
		while (((currentLFState)==(3)) && ((getLastTime()) > (1)))
                {
		    _loop();
                    currentLFState = linefollower_2.readSensors();
		    
                    motor_9.run((9)==M1?-((rotationSpeedM1) * (-1)):((rotationSpeedM1) * (-1)));
                    motor_10.run((10)==M1?-((rotationSpeedM2) * (-1)):((rotationSpeedM2) * (-1)));
                }
            }
        }
    }
    
    proportion = (currentError) - (targetError)
		
    if ((proportion) = (targetError))
    {
        lastError = 0;
        integral = 0;
        derivative = 0;
    }
		
    integral = (integral) + (proportion);
    derivative = (proportion) - (lastError);
    turn = ((Kp) * (proportion)) + ((Ki) * (integral)) + ((Kd) * (derivative));
		
    speedMotor1 = (targetSpeed) + (turn);
    speedMotor2 = (targetSpeed) - (turn);
		
    if ((speedMotor1) > (255))
    {
        speedMotor1 = 255;
    }
    else if ((speedMotor1) < (-255))
    {
        speedMotor1 = -255;
    }
    
    if ((speedMotor2) > (255))
    {
        speedMotor2 = 255;
    }
    else if ((speedMotor2) < (-255))
    {
        speedMotor2 = -255;
    }
    
    motor1.run((9)==M1?-(speedMotor1):(speedMotor1));
    motor2.run((10)==M1?-(speedMotor2):(speedMotor2));
		
    lastError = currentError;
		
    _loop(); 
}

void _delay(float seconds)
{
    long endTime = millis() + seconds * 1000;
    while(millis() < endTime)_loop();
}
    
void _loop()
{
}
