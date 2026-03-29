
# Automated Conveyor Inspection System using Sensor-Based Fault Diagnosis

Arduino-based conveyor system for real-time fault detection using ultrasonic sensors and automatic rejection mechanism.

## Overview

This project implements an automated conveyor inspection system using Arduino Uno. It replaces manual inspection by using ultrasonic sensors to measure product dimensions and detect faulty items in real time.

## Features

* Real-time height and length measurement using ultrasonic sensors
* Threshold-based fault detection
* Automatic rejection of defective products
* Conveyor stop mechanism during fault detection
* Buzzer alert for operator notification
* Automatic conveyor restart after rejection

## Components Used

* Arduino Uno
* 3 Ultrasonic Sensors (2 sides + 1 top)
* MG996R Servo Motor (conveyor control)
* SG90 Servo Motor (rejection arm)
* Buzzer
* External power supply

## Working Principle

The system uses three ultrasonic sensors to measure product dimensions:

* Top sensor → Height
* Side sensors → Length

If dimensions exceed threshold limits, the product is identified as defective.

### Fault Handling:

* Conveyor stops
* Buzzer alerts
* Rejection arm removes product
* Conveyor restarts automatically

## Formula Used

* Height = Reference Height – Measured Distance
* Length = Distance between two side sensors

Condition:

* If (Height > H_limit OR Length > L_limit) → Fault detected

## How to Run

1. Upload code to Arduino
2. Connect sensors and servos
3. Start conveyor
4. Observe automatic inspection and rejection

## Project Demo



## Applications

* Industrial quality inspection
* Smart manufacturing
* Automated sorting systems
