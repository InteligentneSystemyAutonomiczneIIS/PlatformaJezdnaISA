#pragma once
#include <Servo.h>
#include <Arduino.h>
#include <chassis_defines.h>
// #include <math.h>

class Steering
{
private:

    // Traxxas XL5 ESC requires signals to be in 1ms (min) to 2ms (max), where 1,5ms is zero
    int m_minimumPWMSignalLengthInMicroseconds = TRAXXAX_PWM_MICROSECONDS_MIN;
    int m_maximumPWMSignalLengtoInMicroseconds = TRAXXAS_PWM_MICROSECONDS_MAX;
    
    uint8_t m_PWMControlPin = -1;
    Servo m_steeringPWM;
    float m_SwingConstraintLeft = -1.0f;
    float m_SwingConstraintRight = 1.0f;
    bool m_isInitialized = false;


    float m_currentSetSwing = 0;

    void attachServo()
    {
        m_steeringPWM.attach(m_PWMControlPin, m_minimumPWMSignalLengthInMicroseconds, m_maximumPWMSignalLengtoInMicroseconds);
        m_steeringPWM.writeMicroseconds(TRAXXAS_PWM_MICROSECONDS_ZERO);
    }

    // converts from -100 to 100 range into 0-180 range
    int convertSwingToDegrees(int swing)
    {
        // OldRange = (OldMax - OldMin)  
        // NewRange = (NewMax - NewMin)  
        // NewValue = (((OldValue - OldMin) * NewRange) / OldRange) + NewMin

        //testing algorithms
        int deg = int(((swing - (-1.0f)) * (180 - 0)) / (1.0f - (-1.0f)) );
        return deg;
    }


public:

    Steering()
    {

    }

    Steering(uint8_t pin, float steeringLimitLeft = -1.0f, int steeringLimitRight= 1.0f)
    {
        Initialize(pin, steeringLimitLeft, steeringLimitRight);
    }

    void Initialize(uint8_t pin, float steeringLimitLeft=-1.0f,  float steeringLimitRight = 1.0f)
    {
        m_PWMControlPin = pin;
        m_SwingConstraintLeft = steeringLimitLeft;
        m_SwingConstraintRight  = steeringLimitRight;
        attachServo();
        m_isInitialized = true;

    }

    void SetSteering(int swing)
    {
        int speedConstrained = (swing < m_SwingConstraintLeft) ? m_SwingConstraintLeft : ((swing > m_SwingConstraintRight) ? m_SwingConstraintRight : swing);

        if (m_isInitialized)
        {
            m_steeringPWM.write(convertSwingToDegrees(swing));
            m_currentSetSwing = speedConstrained;
        }

    }

    // Send the "zero" speed signal to serwo.
    void SetNeutralSwing()
    {
        if (m_isInitialized)
        {
            m_steeringPWM.writeMicroseconds(TRAXXAS_PWM_MICROSECONDS_ZERO);
            m_currentSetSwing = 0;
        }
    }


    float GetCurrentSetSwing()
    {
        return m_currentSetSwing;
    }


};