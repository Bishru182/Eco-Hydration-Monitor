#include "DHT.h"
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

#define DHTPIN 2// Digital pin connected to the DHT sensor
#define DHTTYPE DHT11  


Servo myservo;  // create servo object to control a servo
LiquidCrystal_I2C lcd(0x27, 16, 2);

int pos = 0; 
float thresh = 46;

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  lcd.init();
  lcd.clear();
  lcd.backlight();

  Serial.begin(9600);
  Serial.println(F("DHTxx test!"));
  dht.begin();

  myservo.attach(9);
}

void loop() {
  // Wait a few seconds between measurements.
  delay(2000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahrenheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print(F("Humidity: "));
  Serial.print(h);

  char buffer1[16];
  char buffer2[16];


  lcd.setCursor(0, 0);   //Set cursor to character 2 on line 0
  dtostrf(h, 5, 2, buffer1);  // Convert float to string
  lcd.print("Humidity: ");
  lcd.print(buffer1);

  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(f);
  Serial.print(F("°F  Heat index: "));
  Serial.print(hic);
  Serial.print(F("°C "));

  lcd.setCursor(0, 1);   //Move cursor to character 2 on line 1
  dtostrf(hic, 5, 2, buffer2);  // Convert float to string
  lcd.print("Temp: ");
  lcd.print(buffer2);
  lcd.print("C");

  Serial.print(hif);
  Serial.println(F("°F"));

 


  if(hic >= thresh){
    myservo.write(180);  // tell servo to go to position in variable '180 degrees'
    delay(1000); 
    myservo.write(0); // tell servo to go to position in variable 'initial position'
    delay(1000);

  }
  myservo.write(0); 
 
}
