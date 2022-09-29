#pragma once

// Defines/constants for V3 chassis - based on Traxxas 4-tec 3.0 (Corvette Stingray body)

// TODO: check what is "better" #define or const 'type'
const int TRAXXAX_PWM_MICROSECONDS_MIN = 1000;
const int TRAXXAS_PWM_MICROSECONDS_MAX = 2000;
const int TRAXXAS_PWM_MICROSECONDS_ZERO = 1500;
const int steeringServoTrimInMicroseconds = 75;


const int watchdogTimer_maxTimeBetweenCommandsInMiliseconds = 500;
const int motorPin = 20; // Temporary - Yaw
const int steeringPin = 21; //Temporary - Pitch
const float motorPowerLevelDefaultConstraintForward = 1.0f; 
const float motorPowerLevelDefaultConstraintBackward = -1.0f; 
const float steeringSwingDefaultConstraintLeft = -1.0f; 
const float steeringSwingDefaultConstraintRight = 1.0f; 



