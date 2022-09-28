// Definition of a chassis
// Collection of steering, motors and sensors
#include <Motor.hpp>
#include <Steering.hpp>
#include "TeensyTimerTool.h"
#include <chrono>

class Chasis
{
private:
    Motor motor;
    Steering steering;
    TeensyTimerTool::OneShotTimer m_CommunicationWatchdogTimer;
    TeensyTimerTool::PeriodicTimer m_watchdogTimer;

    std::chrono::milliseconds m_maxTimeBetweenCommands{watchdogTimer_maxTimeBetweenCommandsInMiliseconds};


public:
    Chasis()
    {

        
    }

    void Initialize()
    {
        motor.Initialize(motorPin,speedPowerLevelConstraintForward, speedPowerLevelConstraintBackward);
        steering.Initialize(steeringPin, steeringSwingConstraintLeft, steeringSwingConstraintRight);

        InitializeWatchdogTimers();
    }

    void SetSpeed(float speed)
    {
        motor.SetSpeed(speed);
        ResetWatchdogTimers();
    }

    void SetSteering(float swing)
    {
        steering.SetSteering(swing);
        ResetWatchdogTimers();
    }


protected:
    void InitializeWatchdogTimers()
    {   
        m_CommunicationWatchdogTimer = TeensyTimerTool::OneShotTimer(TeensyTimerTool::TCK32);
        m_CommunicationWatchdogTimer.begin([this]{this->TimerCallback();});

    }

    void TimerCallback()
    {
        motor.StopMotor();
        steering.SetNeutralSwing(); 
    }


    void ResetWatchdogTimers()
    {
        m_CommunicationWatchdogTimer.trigger(this->m_maxTimeBetweenCommands);
    }
};