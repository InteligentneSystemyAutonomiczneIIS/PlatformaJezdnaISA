#pragma once
#include <Servo.h>
#include <Arduino.h>
#include <chassis_defines.h>
// #include <math.h>

class Steering
{
private:

    // Traxxas servo - 1ms (min) to 2ms (max), where 1,5ms is zero
    int m_minimumPWMSignalLengthInMicroseconds = TRAXXAX_PWM_MICROSECONDS_MIN;
    int m_maximumPWMSignalLengtoInMicroseconds = TRAXXAS_PWM_MICROSECONDS_MAX;
    int m_zeroPWMSignalInMicroseconds = TRAXXAS_PWM_MICROSECONDS_ZERO;
    
    uint8_t m_PWMControlPin = -1;
    Servo m_steeringPWM;
    float m_SwingConstraintLeft = -1.0f;
    float m_SwingConstraintRight = 1.0f;
    bool m_isInitialized = false;


    float m_currentSetSwing = 0.0f;

    void attachServo()
    {
        m_steeringPWM.attach(m_PWMControlPin, m_minimumPWMSignalLengthInMicroseconds, m_maximumPWMSignalLengtoInMicroseconds);
        m_steeringPWM.writeMicroseconds(m_zeroPWMSignalInMicroseconds);
    }

    // converts from -1 to 1 range into 0-180 range
    int convertSwingToDegrees(int swing)
    {
        // OldRange = (OldMax - OldMin)  
        // NewRange = (NewMax - NewMin)  
        // NewValue = (((OldValue - OldMin) * NewRange) / OldRange) + NewMin

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

    void SetSteering(float swing)
    {
        if (m_isInitialized)
        {
            int swingConstrained = (swing < m_SwingConstraintLeft) ? m_SwingConstraintLeft : ((swing > m_SwingConstraintRight) ? m_SwingConstraintRight : swing);

            m_steeringPWM.write(convertSwingToDegrees(swing));
            m_currentSetSwing = swingConstrained;
        }

    }

    void SetSteering(int swing)
    {
        if (m_isInitialized)
        {
            int swingConstrained = constrain(swing, -100, 100);
            float swingConverted =((float)swing - (-100.0f)) * (1.0f - (-1.0f)) / (100.0f - (-100.0f))  + (-1.0f);
            SetSteering(swingConverted);
            
        }

    }

    // Send the "zero" speed signal to serwo.
    void SetNeutralSwing()
    {
        if (m_isInitialized)
        {
            m_steeringPWM.writeMicroseconds(m_zeroPWMSignalInMicroseconds);
            m_currentSetSwing = 0;
        }
    }


    float GetCurrentSetSwing()
    {
        return m_currentSetSwing;
    }


};