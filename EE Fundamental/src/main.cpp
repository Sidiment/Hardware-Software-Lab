#include <Arduino.h>

// Define analog pins
const int VOUT1_PIN = A2; // Analog pin A2
const int VOUT2_PIN = A0; // Analog pin A6

void setup() {
  // Start serial communication
  Serial.begin(115200);

  // Attach ADC pins
  adcAttachPin(VOUT1_PIN);
  adcAttachPin(VOUT2_PIN);

  // Set ADC attenuation
  // ADC_ATTEN_DB_0    -> 0 to 1.1V
  // ADC_ATTEN_DB_2_5  -> 0 to 1.5V
  // ADC_ATTEN_DB_6    -> 0 to 2.2V
  // ADC_ATTEN_DB_11   -> 0 to 3.3V (most commonly used for full-range voltage)
  analogSetAttenuation(ADC_11db); // Set attenuation to handle up to 3.3V
}

void loop() {
  // Read analog values
  int adcValue1 = analogRead(VOUT1_PIN);
  int adcValue2 = analogRead(VOUT2_PIN);

  // Convert ADC value to voltage
  float voltage1 = (adcValue1 / 4095.0) * 3.3; // Convert to volts
  float voltage2 = (adcValue2 / 4095.0) * 3.3;

  // Print results to the serial monitor
  Serial.print("VOUT1 Voltage: ");
  Serial.print(voltage1, 3); // Print voltage with 3 decimal places
  Serial.println(" V");

  Serial.print("VOUT2 Voltage: ");
  Serial.print(voltage2, 3);
  Serial.println(" V");

  delay(1000); // Wait 1 second before next reading
}