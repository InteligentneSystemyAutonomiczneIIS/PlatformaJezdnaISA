// Definition of a chassis
// Collection of steering, motors and sensors
#include <Motor.hpp>
#include <Steering.hpp>
#include "TeensyTimerTool.h"
#include <chrono>

// TODO: 
// 1. Read configuration from a txt file on an SD card
// 2. Test watchdog timers (communication); try out software vs hardware timers under long computations
// 

class Chassis
{
private:
    Motor motor;
    Steering steering;
    TeensyTimerTool::OneShotTimer m_CommunicationWatchdogTimer;
    // TeensyTimerTool::PeriodicTimer m_watchdogTimer;

    std::chrono::milliseconds m_maxTimeBetweenCommands{watchdogTimer_maxTimeBetweenCommandsInMiliseconds};


public:
    Chassis()
    {

        
    }

    void Initialize()
    {
        motor.Initialize(motorPin,motorPowerLevelDefaultConstraintForward, motorPowerLevelDefaultConstraintBackward);
        steering.Initialize(steeringPin, steeringSwingDefaultConstraintLeft, steeringSwingDefaultConstraintRight);

        InitializeWatchdogTimers();
    }

    void SetNeutral()
    {
        motor.StopMotor();
        steering.SetNeutralSwing();
        ResetWatchdogTimers();
    }

    void SetSpeed(float speed)
    {
        motor.SetSpeed(speed);
        ResetWatchdogTimers();
    }

    void SetSpeed(int speed)
    {
        motor.SetSpeed(speed);
        ResetWatchdogTimers();
    }

    void SetSteering(float swing)
    {
        steering.SetSteering(swing);
        ResetWatchdogTimers();
    }

    void SetSteering(int swing)
    {
        steering.SetSteering(swing);
        ResetWatchdogTimers();
    }


protected:
    void InitializeWatchdogTimers()
    {   
        m_CommunicationWatchdogTimer = TeensyTimerTool::OneShotTimer(TeensyTimerTool::TCK32); // Software timer (TCK), read manual
        m_CommunicationWatchdogTimer.begin([this]{this->CommunicationTimerCallback();});

    }

    void CommunicationTimerCallback()
    {
        motor.StopMotor();
        steering.SetNeutralSwing(); 
        Serial.println("CHASSIS; Watchdog timer callback executed");
    }


    void ResetWatchdogTimers()
    {
        m_CommunicationWatchdogTimer.stop();
        m_CommunicationWatchdogTimer.trigger(this->m_maxTimeBetweenCommands);
    }
};