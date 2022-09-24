#pragma once
#include <Servo.h>
#include <Arduino.h>
#include <chassis_defines.h>
// #include <math.h>

class Motor
{
private:

    // Traxxas XL5 ESC requires signals to be in 1ms (min) to 2ms (max), where 1,5ms is zero
    int m_minimumPWMSignalLengthInMicroseconds = TRAXXAX_PWM_MICROSECONDS_MIN;
    int m_maximumPWMSignalLengtoInMicroseconds = TRAXXAS_PWM_MICROSECONDS_MAX;
    
    uint8_t m_PWMControlPin = -1;
    Servo m_motorPWM;
    int m_speedConstraintForward = 100;
    int m_speedConstraintReverse = -100;
    bool m_isInitialized = false;


    int m_currentSetSpeed = 0;

    void attachServo()
    {
        m_motorPWM.attach(m_PWMControlPin, m_minimumPWMSignalLengthInMicroseconds, m_maximumPWMSignalLengtoInMicroseconds);
        m_motorPWM.writeMicroseconds(TRAXXAS_PWM_MICROSECONDS_ZERO);
    }

    // converts from -100 to 100 range into 0-180 range
    int convertSpeedToDegrees(int speed)
    {
        // OldRange = (OldMax - OldMin)  
        // NewRange = (NewMax - NewMin)  
        // NewValue = (((OldValue - OldMin) * NewRange) / OldRange) + NewMin

        //testing algorithms
        int deg = int(((speed - (-100)) * (180 - 0)) / (100 - (-100))) + (-100);
        // int deg2 = map(speed, -100, 100, 0, 180);
        return deg;
    }


public:

    Motor()
    {

    }

    Motor(uint8_t pin, int speedConstraintForward = 100, int SpeedConstraintReverse=-100): 
        m_PWMControlPin{pin}, 
        m_speedConstraintForward{speedConstraintForward}, 
        m_speedConstraintReverse{SpeedConstraintReverse}
    {
        attachServo();
        m_isInitialized = true;

    }

    void Initialize(uint8_t pin, int speedConstraintForward = 100, int SpeedConstraintReverse=-100)
    {
        m_PWMControlPin = pin;
        m_speedConstraintForward = speedConstraintForward;
        m_speedConstraintReverse = SpeedConstraintReverse;
        attachServo();
        m_isInitialized = true;

    }

    void SetSpeed(int speed)
    {
        int speedConstrained = (speed < -100) ? -100 : ((speed > 100) ? 100 : speed);
        //speed = constrain(speed, -100, 100);

        if (m_isInitialized)
        {
            m_motorPWM.write(convertSpeedToDegrees(speed));
            m_currentSetSpeed = speedConstrained;
        }

    }

    // Send the "zero" speed signal to motor. Not the same as braking!
    void StopMotor()
    {
        if (m_isInitialized)
        {
            m_motorPWM.writeMicroseconds(TRAXXAS_PWM_MICROSECONDS_ZERO);
            m_currentSetSpeed = 0;
        }
    }

    //TODO: figure out how to do it
    void Brake()
    {   


    }

    int GetCurrentSetSpeed()
    {
        return m_currentSetSpeed;
    }


};