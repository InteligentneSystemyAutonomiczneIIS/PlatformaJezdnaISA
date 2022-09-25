#pragma once

// Defines/constants for V3 chassis - based on Traxxas 4-tec 3.0 (Corvette Stingray body)

// TODO: check what is "better" #define or const 'type'
const int TRAXXAX_PWM_MICROSECONDS_MIN = 1000;
const int TRAXXAS_PWM_MICROSECONDS_MAX = 2000;
const int TRAXXAS_PWM_MICROSECONDS_ZERO = 1500;


const int watchdogTimer_maxTimeBetweenCommandsInMiliseconds = 100;
const int motorPin = 10; // Temporary
const int steeringPin = 11; //Temporary
const float speedPowerLevelConstraintForward = 1.0f; 
const float speedPowerLevelConstraintBackward = -1.0f; 
const float steeringSwingConstraintLeft = -1.0f; 
const float steeringSwingConstraintRight = 1.0f; 
