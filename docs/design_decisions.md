# Design Decisions Log

A running log of non-obvious choices made during the VBT build.

---

## Sensor: Linear transducer vs IMU

**Decision:** Linear position transducer (spool + rotary encoder)

**Reason:** IMUs require double-integration of acceleration to get position, which introduces significant drift. Direct position measurement via encoder gives cleaner velocity signal with no drift. Same method used by GymAware.

## Encoder: Signswise 600 PPR

**Decision:** 600 PPR incremental quadrature encoder

**Reason:** At 600 PPR with ~30mm spool, resolution is ~0.16mm per pulse. This is sufficient for barbell velocities (0.1–2.0 m/s). Higher PPR unnecessary and increases ESP32 interrupt load.

## Voltage: Pull-ups to 3.3V

**Decision:** 10kΩ pull-up resistors tied to 3.3V rail, not 5V 

**Reason:** ESP32 GPIO pins are not 5V tolerant. Pulling up to 3.3V keeps output signal within safe range without needing a level shifter.

## Smoothing: 5-sample moving average

**Decision:** Moving average with window size 5

**Reason:** TBD — will tune empirically during calibration testing.
