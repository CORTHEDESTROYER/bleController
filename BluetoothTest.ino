#include <ArduinoBLE.h>

const int ledPin = 12;  // PWM-capable pin

// Custom UUIDs
// 504B4853-2043-444A-5354-208520252026
BLEService ledService("26738915-1234-4321-1234-1a2b3c4d5e6f");
BLEUnsignedCharCharacteristic brightnessChar(
  "4d3c2b1a-4321-1234-4321-280980280980",
  BLEWrite | BLEWriteWithoutResponse | BLERead
);

void setup() {
  pinMode(ledPin, OUTPUT);

  if (!BLE.begin()) {
    while (1); // halt if BLE fails
  }

  BLE.setLocalName("Nano33BLE_LED");
  BLE.setAdvertisedService(ledService);

  ledService.addCharacteristic(brightnessChar);
  BLE.addService(ledService);

  brightnessChar.writeValue(128); // initial brightness

  BLE.advertise();
}

void loop() {
  BLEDevice central = BLE.central();

  if (central) {
    while (central.connected()) {
      if (brightnessChar.written()) {
        // Always read the *current* value directly
        int brightness = brightnessChar.value();
        analogWrite(ledPin, brightness);
      }
      delay(10); // keep connection stable
    }
  }
}
