float tm(float resistance, int bValue, int parallelResistor, int port) {
  // Thermistor parameters from the datasheet
  float RT0 = resistance;
  float B = (float)bValue;
  float R = (float)parallelResistor;

  // Variables for calculations
  float RT, VR, ln, TX, T0, VRT;

  // Convert T0 from Celsius to Kelvin
  T0 = 25.0 + 273.15;

  // Read the voltage across the thermistor
  VRT = (5.00 / 1023.00) * analogRead(port);

  // Calculate the voltage across the resistor
  VR = 5.00 - VRT;

  // Calculate resistance of the thermistor
  RT = VRT / (VR / R);

  // Calculate temperature from thermistor resistance
  ln = log(RT / RT0);
  TX = (1 / ((ln / B) + (1 / T0)));

  // Convert to Celsius
  TX = TX - 273.15;

  return TX;
}

void setup() {
  
  Serial.begin(9600);
  
}

void loop() {
  
  
  // Display in Celsius
  Serial.println(tm(10000, 3435, 4700, A0));                  

  
  delay(500);
}
