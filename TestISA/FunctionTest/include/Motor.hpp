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
    float m_speedConstraintForward = 1.0f;
    float m_speedConstraintReverse = -1.0f;
    bool m_isInitialized = false;


    float m_currentSetSpeed = 0;

    void attachServo()
    {
        m_motorPWM.attach(m_PWMControlPin, m_minimumPWMSignalLengthInMicroseconds, m_maximumPWMSignalLengtoInMicroseconds);
        m_motorPWM.writeMicroseconds(TRAXXAS_PWM_MICROSECONDS_ZERO);
    }

    // converts from -1 to 1 range into 0-180 range
    int convertSpeedToDegrees(float speed)
    {
        // OldRange = (OldMax - OldMin)  
        // NewRange = (NewMax - NewMin)  
        // NewValue = (((OldValue - OldMin) * NewRange) / OldRange) + NewMin

        //testing algorithms
        int deg = int(((speed - (-1.0f)) * (180 - 0)) / (1.0f - (-1.0f)));
        return deg;
    }


public:

    Motor()
    {

    }

    Motor(uint8_t pin, int speedConstraintForward = 1.0f, int SpeedConstraintReverse=-1.0f): 
        m_PWMControlPin{pin}, 
        m_speedConstraintForward{(int)(speedConstraintForward*100)}, 
        m_speedConstraintReverse{(int)(SpeedConstraintReverse*100)}
    {
        attachServo();
        m_isInitialized = true;

    }

    void Initialize(uint8_t pin, float speedConstraintForward = 1.0f, float SpeedConstraintReverse=-1.0f)
    {
        m_PWMControlPin = pin;
        m_speedConstraintForward = (int)(speedConstraintForward*100);
        m_speedConstraintReverse = (int)(SpeedConstraintReverse*100);
        attachServo();
        m_isInitialized = true;

    }

    void SetSpeed(int speed)
    {
        int speedConstrained = (speed < m_speedConstraintReverse) ? m_speedConstraintReverse : ((speed > m_speedConstraintForward) ? m_speedConstraintForward : speed);
        //speed = constrain(speed, m_speedConstraintReverse, m_speedConstraintForward);

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
            m_currentSetSpeed = 0.0f;
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