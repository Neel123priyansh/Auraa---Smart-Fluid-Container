# Auraa -- Smart Water Bottle 

> A compact, connected water bottle that monitors water quality, tracks hydration behavior, and provides personalized health-driven hydration guidance.

![alt text](https://github.com/Neel123priyansh/Auraa---Smart-Fluid-Container/blob/master/img/Screenshot%202024-12-21%20165510.png/?raw=true)

---

## Table of Contents

* [Overview](#overview)
* [Features](#features)
* [Hardware Components](#hardware-components)
* [Circuit & Wiring Summary](#circuit--wiring-summary)
* [Software & Algorithms](#software--algorithms)
* [Installation & Usage](#installation--usage)
* [Calibration](#calibration)
* [Performance & Limitations](#performance--limitations)
* [Cost Estimate](#cost-estimate)
* [Safety & Compliance](#safety--compliance)
* [Future Work](#future-work)
* [Author](#author)

---

## Overview

The Smart IoT Hydration & Health Bottle is an integrated system that combines water-quality sensing (turbidity, pH, temperature), water-level monitoring, user activity detection, and vitals measurement (via an oximeter). The device provides real-time feedback through a mobile/web dashboard and offers automated UV-C sanitization to keep the drinking area hygienic.

---

## Features

* **Water quality checks:** turbidity, pH, temperature readings and a clear/cloudy/dirty classification.
* **Accurate water-level tracking:** contactless ultrasonic measurement with motion-assisted filtering (MPU6050).
* **Drink detection:** combines tilt (accelerometer + gyroscope) and level drop to detect drinking events and estimate consumed volume.
* **Vitals integration:** MAX30100/102 oximeter for BPM and SpO₂ — used to personalize hydration reminders.
* **Automated UV-C sanitization:** cap-mounted UV-C LED (with MOSFET control) that runs a safe timed sanitization cycle.
* **Low power strategy:** selective sensor wake-up and deep-sleep modes; gyroscope runs with minimal draw.
* **Wireless charging:** keeps the bottle waterproof while simplifying charging.

---

## Hardware Components

A non-exhaustive list used in prototypes:

* Microcontroller: **NodeMCU (ESP8266)** or **ESP32**
* Ultrasonic sensor: AJ-SR04M (waterproof single-probe) or HC-SR04 for bench testing
* Motion: **MPU6050 (accelerometer + gyroscope)**
* Water sensors: **Turbidity**, **pH probe**, **DS18B20** (temperature)
* Vitals: **MAX30100 / MAX30102** (pulse oximeter)
* UV-C LED array + MOSFET (IRLZ44N / 2N7002 alternative for logic-level gating)
* Battery: 3.7V Li-ion + charging management + wireless charging coil
* Misc: PCBs, connectors, seals, enclosure (copper inner + polycarbonate outer in premium builds)

---

## Circuit & Wiring Summary

* **Power system:** Li-ion battery → power management → 3.3V regulator for MCU & sensors; 5V rail (boost) for ultrasonic and other peripherals if needed.
* **I²C bus:** MPU6050 and MAX3010x connect via SDA/SCL to the MCU.
* **Ultrasonic:** TRIG → digital out; ECHO → digital in (use voltage divider if sensor is a 5V part).
* **Turbidity & pH:** analog outputs → MCU ADC (A0). Use stable Vref and buffering for accuracy.
* **UV-C LED:** driven by MOSFET; MCU GPIO → gate. Include current limiting and thermal management.
* **Reed/magnet or timer:** cap-close detection can use a reed switch (GPIO + INPUT_PULLUP) or a simple countdown trigger for testing.

## Refer to the below circuit diagram for bottle's body.
![alt text](https://github.com/Neel123priyansh/Auraa---Smart-Fluid-Container/blob/master/img/Screenshot%202025-11-21%20145924.png/?raw=true)
## Refer to the below circuit diagram for bottle's cap.
![alt text](https://github.com/Neel123priyansh/Auraa---Smart-Fluid-Container/blob/master/img/Screenshot%202025-11-21%20145924.png/?raw=true)

---

## Software & Algorithms

**Main functions:**

1. **Water-level averaging:** take 5 readings, reject outliers, compute mean.
2. **Drink detection:** on a tilt event (tiltAngle > threshold), read water level before and after — if level decreased beyond the threshold, register a drink event and estimate volume consumed.
3. **Water quality classification:** map turbidity voltage to `Clear / Cloudy / Dirty` using calibrated thresholds.
4. **Vitals-driven hydration logic:** use BPM & SpO₂ trends to nudge hydration frequency; detect anomalies and recommend action.
5. **Power management:** sensor multiplexing and deep-sleep for NodeMCU between events.

**Example deploy:** microcontroller runs sensor loop and exposes a `/data` JSON endpoint for the React dashboard; backend (optional) stores time-series for model training.
![alt text](https://github.com/Neel123priyansh/Auraa---Smart-Fluid-Container/blob/master/img/image.png/?raw=true)

---

## Installation & Usage

1. Flash the provided firmware to NodeMCU (ESP8266) or ESP32.
2. Connect sensors according to the wiring summary.
3. Power the board and open Serial Monitor at `115200` baud to view logs.
4. For testing: use the 3 → 2 → 1 countdown to start a measurement cycle.
5. Use the React dashboard (or ThingSpeak) to visualize vitals and water-quality graphs.

---

## Calibration

* **Turbidity:** run known NTU samples and record ADC voltages to fit a curve; update thresholds in firmware.
* **pH:** use buffer solutions (pH 4, 7, 10) and map sensor voltages to pH values in code.
* **Ultrasonic distance:** verify empty/full distances and compute geometry-based volume conversions (bottle shape matters).
* **Oximeter:** ensure finger placement and run sample tests to tune filtering/windowing in the pulse algorithm.

---

## Performance & Limitations

* **Ultrasonic accuracy:** ±3 mm in ideal conditions; performance degrades with angled surfaces and strong agitation.
* **Turbidity classification:** depends on calibration; sensor struggles with coloured but particle-free water.
* **UV-C sanitization:** cap-mounted UV is energy-efficient but provides limited volumetric penetration — full-coverage arrays are more effective but costlier.
* **Battery life:** depends on UV usage frequency. Selective sensor wake/sleep greatly extends uptime.

---

## Cost Estimate (Prototype)

* Estimated BOM per unit (prototype): **₹3000 - ₹3500**
* Major contributors: oximeter, sensors, battery, wireless charging, polycarbonate + copper lining in premium units.

---

## Safety & Compliance

* **UV-C warning:** UV-C light can be hazardous to eyes and skin; design interlocks so UV runs only when the cap is closed.
* **Waterproofing:** ensure gaskets and assembly pass basic IP testing; wireless charging avoids exposed ports.
* **Food safety:** use food-grade inner liners and avoid exposed solder/pads in liquid-contact areas.

---

## Future Work

* On-device LSTM or lightweight ML for personalized hydration prediction.
* Federated learning for privacy-preserving model improvements.
* Mobile app with user profiles, reminders, and cloud analytics.
