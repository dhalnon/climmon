#include <Wire.h>
#include <ArduinoBLE.h>
#include <Adafruit_MCP9808.h>

BLEService tempSensor("180K");

BLEIntCharacteristic currentTemperature("CCC1", BLERead );

// Create the MCP9808 temperature sensor object
Adafruit_MCP9808 tempsensor = Adafruit_MCP9808();

void setup() {
  Serial.begin(9600);
  // while (!Serial); //waits for serial terminal to be open, necessary in newer arduino boards.
  pinMode(LED_BUILTIN, OUTPUT); // initialize the built-in LED pin to indicate when a central is connected

  Serial.println("MCP9808 demo");
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");

    while (1);
  }
  BLE.setDeviceName("Arduino Nano 33 BLE - MCP9808");
  BLE.setLocalName("calroom.MCP9808");

  currentTemperature.setEventHandler(BLERead, readTemperature);
  tempSensor.addCharacteristic(currentTemperature);

  BLE.addService(tempSensor);
  BLE.setAdvertisedService(tempSensor);

  BLE.setEventHandler(BLEConnected, blePeripheralConnectHandler);
  BLE.setEventHandler(BLEDisconnected, blePeripheralDisconnectHandler);
  BLE.advertise();
  Serial.println("Bluetooth device active, waiting for connections...");
  
  // Make sure the sensor is found, you can also pass in a different i2c
  // address with tempsensor.begin(0x19) for example, also can be left in blank for default address use
  // Also there is a table with all addres possible for this sensor, you can connect multiple sensors
  // to the same i2c bus, just configure each sensor with a different address and define multiple objects for that
  //  A2 A1 A0 address
  //  0  0  0   0x18  this is the default address
  //  0  0  1   0x19
  //  0  1  0   0x1A
  //  0  1  1   0x1B
  //  1  0  0   0x1C
  //  1  0  1   0x1D
  //  1  1  0   0x1E
  //  1  1  1   0x1F
  if (!tempsensor.begin(0x18)) {
    Serial.println("Couldn't find MCP9808! Check your connections and verify the address is correct.");
    while (1);
  }
    
   Serial.println("Found MCP9808!");

  tempsensor.setResolution(1); // sets the resolution mode of reading, the modes are defined in the table bellow:
  // Mode Resolution SampleTime
  //  0    0.5°C       30 ms
  //  1    0.25°C      65 ms
  //  2    0.125°C     130 ms
  //  3    0.0625°C    250 ms
}

void loop() {
  BLE.poll();
}

void blePeripheralConnectHandler(BLEDevice central) {
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.print("Connected event, central: "); Serial.println(central.address());
}

void blePeripheralDisconnectHandler(BLEDevice central) {
  digitalWrite(LED_BUILTIN, LOW);
  Serial.print("Disconnected event, central: "); Serial.println(central.address());
}

void readTemperature(BLEDevice central, BLECharacteristic characteristic) {
  // float c = tempsensor.readTempC();
  float f = tempsensor.readTempF();
  float c = tempsensor.readTempC();
  int tF = convertFloatToInt(f);
  int tC = convertFloatToInt(tC);
  currentTemperature.writeValue(tC);
  Serial.print("Temperature = "); Serial.println(c);
}

int convertFloatToInt(float x) {
  x = x * 100;
  if (x < -32768) {
    return -32768;
  }
  if (x > 32767) {
    return 32767;
  }
  return (int)round(x);
}