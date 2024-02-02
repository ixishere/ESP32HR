// Define the pin connected to the pulse sensor
int pulsePin = 4;

// Set the threshold for detecting a heartbeat
int threshold = 2100;

// Variables to track heartbeat timing and rate
unsigned long lastBeatTime = 0;
unsigned long beatInterval = 60000; // Default interval for one beat (in milliseconds)
int bpm = 0; // Heartbeats per minute
bool beatDetected = false; // Flag to indicate if a beat is currently detected

// Include the BluetoothSerial library for Bluetooth communication
#include "BluetoothSerial.h"

// Check if Bluetooth is enabled, throw an error if not
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` and enable it
#endif

// Create a BluetoothSerial object named SerialBT
BluetoothSerial SerialBT;

// Setup function runs once at the beginning
void setup() {
  // Start the serial communication with a baud rate of 115200
  Serial.begin(115200);

  // Start the Bluetooth serial communication with the given name
  SerialBT.begin("ESP32 - Heart Rate");
}

// Loop function runs repeatedly
void loop() {
  // Read the analog signal from the pulse sensor
  int Signal = analogRead(pulsePin);

  // Reset variables if the sensor signal is zero
  if (Signal == 0) {
    lastBeatTime = 0;
    beatInterval = 60000;
    bpm = 0;
    beatDetected = false;
  }

  // Detect a beat if the signal is above the threshold and a beat is not already detected
  if (Signal > threshold && !beatDetected) {
    unsigned long currentTime = millis();

    // Calculate beat interval and BPM
    if (lastBeatTime != 0) {
      beatInterval = currentTime - lastBeatTime;
      bpm = 60000 / beatInterval;
    }

    // Update the last beat time and set the flag
    lastBeatTime = currentTime;
    beatDetected = true;
  }

  // Reset beat detection flag when the signal is below the threshold
  if (Signal <= threshold && beatDetected) {
    beatDetected = false;
  }

  // Check if BPM is too high or too low and print to Serial and Bluetooth
  if (bpm > 230) {
    Serial.println(0);
    SerialBT.println(0);
  } else if (bpm < 30) {
    Serial.println(0);
    SerialBT.println(0);
  } else {
    Serial.println(bpm);
    SerialBT.println(bpm);
  }

  // Delay for a short period to control the loop frequency
  delay(10);
}
