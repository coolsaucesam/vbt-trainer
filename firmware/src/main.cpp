#include <Arduino.h> // Core: pinMode, digitalRead, millis, Serial
#include <ESP32Encoder.h> // Harware pulse counter -- performs better than interrupts
#include <Wire.h> //I2C protocol -- needed for OLED
#include <NimBLEDevice.h> // Bluetooth Low Energy
#include <config.h> //imports custom pin definitions and constants

ESP32Encoder encoder;

// Velocity Tracking
float velocityBuffer[SMOOTHING_WINDOW]; //Circular buffer for moving average
int bufferIndex = 0; //Index for tracking buffer position
float smoothVelocity = 0.0; //Value of the smoothed velocity
float peakVelocity = 0.0; //Highest velocity in the calculated rep window
long lastPulseCount = 0; //Encoder count at last velocity sample
unasigned long lastSampleTime = 0; //Timestamp of last velocity sample (ms)

// Rep detection
bool repInProgress = false; // Checks to see if we are currently in a rep
int RepCount = 0; // Total reps in the session
float repStartVelocity = 0.0; // Velocity at the start of the rep 
float meanConcentricVelo = 0.0; //Mean Concentric Velocity for the completed repetition 
// **Mean Concentric Velocity (MCV)** Average velocity of the barbell during the rep
float velocitySumThisRep = 0.0; // Sum of the velocity samples during concentric phase
int samplesThisRep = 0; // Number of sampels taken during the concentric phase of the rep
unasigned long repStartTime = 0; // When this rep started (ms)

// Session Tracking
float rep1Velocity = 0.0; // MCV of first rep (to calculate velocity loss %)
float velocityLoss = 0.0; // Signed percent change from first to last rep

// OLED 

// Bluetooth LE 
NimBLEServer bleServer = nullptr;
NimBLECharacteristic* bleCharVelocity = nullptr;
bool bleClientConnected = false;
unsigned long lastBLEUpdate = 0; // timestamp of last BLE broadcast


