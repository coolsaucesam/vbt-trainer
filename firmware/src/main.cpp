#include <Arduino.h> // Core: pinMode, digitalRead, millis, Serial
#include <ESP32Encoder.h> // Harware pulse counter -- performs better than interrupts
#include <Wire.h> //I2C protocol -- needed for OLED
#include <NimBLEDevice.h> // Bluetooth Low Energy
#include "config.h" //imports custom pin definitions and constants

ESP32Encoder encoder;

// Velocity Tracking
float velocityBuffer[SMOOTHING_WINDOW]; //Circular buffer for moving average
int bufferIndex = 0; //Index for tracking buffer position
float smoothVelocity = 0.0; //Value of the smoothed velocity
float peakVelocity = 0.0; //Highest velocity in the calculated rep window
long lastPulseCount = 0; //Encoder count at last velocity sample
unsigned long lastSampleTime = 0; //Timestamp of last velocity sample (ms)

// Rep detection
bool repInProgress = false; // Checks to see if we are currently in a rep
int repCount = 0; // Total reps in the session
float repStartVelocity = 0.0; // Velocity at the start of the rep 
float meanConcentricVelo = 0.0; //Mean Concentric Velocity for the completed repetition 
// **Mean Concentric Velocity (MCV)** Average velocity of the barbell during the rep
float velocitySumThisRep = 0.0; // Sum of the velocity samples during concentric phase
int samplesThisRep = 0; // Number of sampels taken during the concentric phase of the rep
unsigned repStartTime = 0; // When this rep started (ms)

// Session Tracking
float rep1Velocity = 0.0; // MCV of first rep (to calculate velocity loss %)
float velocityLoss = 0.0; // Signed percent change from first to last rep

// OLED 

// Bluetooth LE 
NimBLEServer* bleServer = nullptr;
NimBLECharacteristic* bleCharVelocity = nullptr;
bool bleClientConnected = false;
unsigned long lastBLEUpdate = 0; // timestamp of last BLE broadcast

// HELPER FUNCTIONS ---------------------
// calculateVelocity: Returns raw velocity in m/s when called every 200Hz/5ms in loop()
float calculateVelocity() {
    unsigned long now = millis(); // Gets current time in ms
    long nowCount = encoder.getCount(); // Gets the current encoder pulse count
    unsigned long dt_ms = now - lastSampleTime; // Time delta between last sample (ms)
    long dp = nowCount - lastPulseCount; // Pulses since last sample
    // Update "last" values for next function call
    lastSampleTime = now;
    lastPulseCount = nowCount;
    //Safeguard: if not time has passed, avoide devide-by-zero error
    if (dt_ms == 0) return 0.0;
    //Convert pulses to mm and milliseconds to seconds to get velocity
    //Calculation is (meters/second) = pulses * mm/pulse * 1 meter / 1000 mm * 1000 ms/1s * 1/dt_ms
    float displacement_mm = dp * MM_PER_PULSE; //total mm moved
    float velocity_ms = (displacement_mm/1000.0)/(dt_ms/1000.0); //mm -> meters, ms -> seconds (meters/second)
    //we want concentric or upward velocity, so we take the absolute value of velocity
    return abs(velocity_ms);
}

// logToSerial: logs the timestamp (ms), rawVelocity value, and smoothVel value in csv format
void logToSerial(unsigned long timestamp, float rawVelocity, float smoothVel, int rep) {
    // Format: timestamp_ms, raw_velocity, smooth_velocity, rep_number
    Serial.print(timestamp);
    Serial.print(",");
    Serial.print(rawVelocity, 4);   // 4 decimal places
    Serial.print(",");
    Serial.print(smoothVel, 4);
    Serial.print(",");
    Serial.println(rep);            // println adds newline — Python reads line by line
}


// smoothVelocityReading: takes the float of the newest velocity sample "newSample" 
// and returns a new smoothed average given the defined SMOOTHING_WINDOW
float smoothVelocityReading(float newSample){
    velocityBuffer[bufferIndex] = newSample; // Stores new sample
    bufferIndex = (bufferIndex + 1) % SMOOTHING_WINDOW; // Advances buffer index 
    // % (modulus) wraps back to 0
    // Calculate the average of all samples in buffer
    float sum = 0.0;
    for (int i = 0; i<SMOOTHING_WINDOW; i++){
        sum += velocityBuffer[i];
    }
    return sum/SMOOTHING_WINDOW;
}

void setup(){
    // SERIAL 
    Serial.begin(115200);
    Serial.println("VBT Trainer starting...");
    // ENCODER
    ESP32Encoder::useInternalWeakPullResistors = puType::up; //enable internat pull-up resistors
    encoder.attachHalfQuad(ENCODER_PIN_A, ENCODER_PIN_B);
    //NOTE!! Currently using 2 ppr from config.h
    encoder.setCount(0); // Zero the counter when starting up
    Serial.println("Encoder initialized on GPIO " + String(ENCODER_PIN_A) + "and" + String(ENCODER_PIN_B));
    //Initialized velocity buffer to be zero
    for (int i=0; i<SMOOTHING_WINDOW; i++){
        velocityBuffer[i] = 0.0;
    }
}


void loop() {
    unsigned long now = millis(); // Get current time once per loop iteration
    //TASK 1: Calculate velocity at 200Hz
    if (now - lastSampleTime >= (1000/SAMPLE_RATE_HZ)){
        float rawVel = calculateVelocity();
        smoothVelocity = smoothVelocityReading(rawVel);
        logToSerial(now, rawVel, smoothVelocity, repCount);
    }
    //Task 1a: 

    //[Incomplete] Task 2: Send BLE update at 10Hz
}