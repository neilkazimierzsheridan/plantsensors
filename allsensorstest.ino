

// OneWire Temp Sensor includes

#include <OneWire.h>
#include <DallasTemperature.h>
 
// Data wire is plugged into port ? on the Arduino
#define ONE_WIRE_BUS 3
 
 
// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
 
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);
 

// DHT humidity/temp sensor includes

#include "DHT.h"

#define DHTPIN 2     // what pin we're connected to


#define DHTTYPE DHT11   // DHT 11


DHT dht(DHTPIN, DHTTYPE);

#if defined(ARDUINO_ARCH_AVR)
    #define debug  Serial

#elif defined(ARDUINO_ARCH_SAMD) ||  defined(ARDUINO_ARCH_SAM)
    #define debug  SerialUSB
#else
    #define debug  Serial
#endif

// ends DHT includes

// Grove Sunlight Sensor includes

#include <Wire.h>

#include "Arduino.h"
#include "SI114X.h"

SI114X SI1145 = SI114X();

// ends Grove Sunlight Sensor includes

void setup() {

// OneWire sensor setup

  // start serial port
    Serial.begin(115200);
    Serial.println("Dallas Temperature IC Control Library Demo");
 
    // Start up the library
    sensors.begin();

// DHT sensor setup

    debug.begin(115200);
    debug.println("DHTxx test!");
    Wire.begin();

    /*if using WIO link,must pull up the power pin.*/
    // pinMode(PIN_GROVE_POWER, OUTPUT);
    // digitalWrite(PIN_GROVE_POWER, 1);

    dht.begin();
    
// Sunlight sensor setup
    Serial.begin(115200);
    Serial.println("Beginning Si1145!");

    while (!SI1145.Begin()) {
        Serial.println("Si1145 is not ready!");
        delay(1000);
    }
    Serial.println("Si1145 is ready!");
}

// ends sunlight sensor setup

void loop() {
    Serial.println("");
    Serial.print("//--------------------------------------//\r\n");
    Serial.print("........ Visible, IR, and UV Radiation ........\r\n");
    Serial.print("Vis: "); Serial.println(SI1145.ReadVisible());
    Serial.print("IR: "); Serial.println(SI1145.ReadIR());
    //the real UV value must be div 100 from the reg value , datasheet for more information.
    Serial.print("UV: ");  Serial.println((float)SI1145.ReadUV() / 100);
    delay(1000);
    Serial.println("");
  // Sunlight sensor above
  



// DHT loop  
    float temp_hum_val[2] = {0};
    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)


    if (!dht.readTempAndHumidity(temp_hum_val)) {
        Serial.println("");
        Serial.print("//--------------------------------------//\r\n");
        Serial.print("........ Temperature and Humidity ........\r\n");
        Serial.print("Humidity: "); 
        Serial.print(temp_hum_val[0]); 
        Serial.print(" %\t");
        Serial.print("Temperature: "); 
        Serial.print(temp_hum_val[1]);
        Serial.print(" *C");
        Serial.println("");
    } else {
        debug.println("Failed to get temprature and humidity value.");
    }

    delay(1500);

// OneWire Temp Sensor Loop

    // call sensors.requestTemperatures() to issue a global temperature 
    // request to all devices on the bus
    // Serial.print("Requesting temperatures...");
    sensors.requestTemperatures(); // Send the command to get temperatures
    //Serial.println("DONE");
    // After we got the temperatures, we can print them here.
    // We use the function ByIndex, and as an example get the temperature from the first sensor only.
    float tempC = sensors.getTempCByIndex(0);
 
    // Check if reading was successful
    if(tempC != DEVICE_DISCONNECTED_C) 
    {
      Serial.println("");
      Serial.print("//--------------------------------------//\r\n");
      Serial.print("........ Soil Temperature ........\r\n");
      Serial.print("Soil Temperature is: ");
      Serial.println(tempC);
    } 
    else
    {
      Serial.println("Error: Could not read temperature data");
    }
    
}
