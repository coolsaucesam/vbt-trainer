#pragma once

// ── Encoder ──────────────────────────────────────────

#define ENCODER_PIN_A     18      // GPIO pin for encoder channel A

#define ENCODER_PIN_B     19      // GPIO pin for encoder channel B

#define ENCODER_PPR       600     // Pulses per revolution

#define DIRECTION_CW 0              // defines a clockwise direction

#define DIRECTION_CCW 1             // defines a counterclockwise direction

// ── Spool geometry ───────────────────────────────────

#define SPOOL_DIAMETER_MM 30.0    // Measure your actual spool and update this

#define SPOOL_CIRCUMFERENCE_MM (SPOOL_DIAMETER_MM * 3.14159)

//#define MM_PER_PULSE (SPOOL_CIRCUMFERENCE_MM / (ENCODER_PPR * 4)) // x4 for quadrature

#define MM_PER_PULSE (SPOOL_CIRCUMFERENCE_MM / (ENCODER_PPR * 2)) // x2 for 2 PPR

// ── Velocity calculation ──────────────────────────────

#define SAMPLE_RATE_HZ    200     // How often velocity is calculated

#define SMOOTHING_WINDOW  5       // Moving average window size (samples)

// ── Rep detection ─────────────────────────────────────

#define REP_START_VELOCITY  0.08  // m/s — velocity above this = concentric started

#define REP_MIN_DURATION_MS 150   // ms  — ignore spikes shorter than this

#define REP_END_VELOCITY    0.05  // m/s — velocity below this = rep ended

// ── BLE ───────────────────────────────────────────────

#define BLE_DEVICE_NAME   "VBT-Trainer"

#define BLE_BROADCAST_HZ  10      // How often to send BLE packet

// ── Display ───────────────────────────────────────────

#define OLED_SDA_PIN      21

#define OLED_SCL_PIN      22

#define OLED_ADDRESS      0x3C
