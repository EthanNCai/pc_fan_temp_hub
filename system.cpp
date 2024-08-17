#include<LiquidCrystal.h> 
#define MANUAL 1
#define AUTO 0
const int switch_pin = 12;
const int rs=7,en=6,d4=5,d5=4,d6=3,d7=2;
const int knob=A3;
const int pin_tmp_psu = A0;
const int pin_tmp_water = A1;
const int pwm = 11;
int mode = AUTO;

LiquidCrystal lcd(rs,en,d4,d5,d6,d7);
float psu_temp = 25.53;
float water_temp = 65.31;
int manual_percentage = 66.3;

int auto_fan = 0;

String float2str(float number);
float tm(float resistance, int bValue, int parallelResistor, int port);

void setup()
{
  pinMode(switch_pin, INPUT); 
  pinMode(knob, INPUT); 
  pinMode(pwm, OUTPUT);
  lcd.clear();
  lcd.begin(20,2);
  // Serial.begin(9600); 
}
 
void loop() {

  mode =  digitalRead(switch_pin); 
  
  water_temp = tm(10000, 3435, 4700, pin_tmp_water);
  psu_temp = tm(10000, 3435, 4700, pin_tmp_psu);


  

  lcd.setCursor(0, 0); 
  lcd.print("PSU"); 
  lcd.setCursor(7, 0); 
  lcd.print("WATER"); 
  lcd.setCursor(0, 1); 
  lcd.print(float2str(psu_temp) + String((char)223) + "C"); 
  lcd.setCursor(7, 1); 
  lcd.print(float2str(water_temp) + String((char)223)  + "C");
  lcd.setCursor(14, 0); 
  lcd.print("FAN"); 
  lcd.setCursor(19, 1);
  if(mode == AUTO){
      lcd.print("A");
      lcd.setCursor(14, 1); 
      lcd.print(String("    "));
      
      lcd.setCursor(18, 0);
      lcd.print("  ");
      
      int knob_position = map(analogRead(knob), 0, 1023, 0, 100);
      if(knob_position > 65){
        knob_position = 65;
      }
      if(knob_position < 30){
        knob_position = 30;
      }
      lcd.setCursor(18, 0);
      lcd.print(knob_position);
      lcd.setCursor(14, 1); 


      // (20,20) (k,100)
      float k =  80.0/(knob_position - 20);
      float b =  20 - k * 20.0;
      // Serial.print(k);
      // Serial.print(" ");
      // Serial.print(b);
      // Serial.print(" ");
      // Serial.print(water_temp);
      // Serial.println(" ");
      auto_fan = int(k * water_temp + b);
      if(auto_fan > 100){
        auto_fan = 100;
      }
      if(auto_fan < 0){
        auto_fan = 0;
      }
      
      lcd.print(auto_fan + String("%"));
      analogWrite(pwm,map(auto_fan, 0, 100, 0, 255));
      
  }else{
      int knob_position = map(analogRead(knob), 0, 1023, 0, 100);
      lcd.print("M");
      lcd.setCursor(18, 0);
      lcd.print("  ");1
      lcd.setCursor(14, 1); 
      lcd.print(String("    "));
      lcd.setCursor(14, 1); 
      lcd.print(knob_position + String("%"));
      analogWrite(pwm,map(knob_position, 0, 100, 0, 255));
  }
  
  delay(500);
}


String float2str(float number) {
  float roundedNumber = roundf(number * 10.0) / 10.0;
  char str[5];
  dtostrf(roundedNumber, 4, 1, str); // 格式化为长度为 4 的字符串，保留 1 位小数
  return String(str);
}

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

