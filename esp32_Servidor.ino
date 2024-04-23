#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#define SERVICE_UUID                "3feb1e8a-3981-4045-ad39-b225135013a0"
#define CONTROL_CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define LED_PIN_1                   18
#define LED_PIN_2                   19
#define LED_PIN_3                   21

char ledStatus = 49; // Inicialmente, establece el estado en '1' (encendido)

BLECharacteristic controlCharacteristic(
  CONTROL_CHARACTERISTIC_UUID,
  BLECharacteristic::PROPERTY_READ |
  BLECharacteristic::PROPERTY_WRITE
);

void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE!");
  Serial.println("Initializing device");
  BLEDevice::init("VELOMOVIL"); // Inicializa el dispositivo con su nombre
  Serial.println("Creating server");
  BLEServer *pServer = BLEDevice::createServer(); // Crea el servidor
  Serial.println("Adding service UUID");
  BLEService *pService = pServer->createService(SERVICE_UUID); // Crea un nuevo servicio en el servidor
  
  Serial.println("Adding name characteristic");
  BLECharacteristic *nameCharacteristic = pService->createCharacteristic(
                                         BLEUUID((uint16_t)0x2A00),
                                         BLECharacteristic::PROPERTY_READ
                                       );
  nameCharacteristic->setValue("Led");
  
  Serial.println("Adding control characteristic");
  pService->addCharacteristic(&controlCharacteristic);
  controlCharacteristic.setValue(&ledStatus); // Valor uint8_t con longitud 1
  Serial.println("Starting...");
  pService->start();
  
  Serial.println("Creating advertising");
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  
  Serial.println("Characteristic defined! Now you can read it in your phone!");

  pinMode(LED_PIN_1, OUTPUT); // Configura el pin del LED 1 como salida
  pinMode(LED_PIN_2, OUTPUT); // Configura el pin del LED 2 como salida
  pinMode(LED_PIN_3, OUTPUT); // Configura el pin del LED 3 como salida

  // Enciende los LEDs al inicio
  digitalWrite(LED_PIN_1, HIGH);
  digitalWrite(LED_PIN_2, HIGH);
  digitalWrite(LED_PIN_3, HIGH);
}

void loop() {
  std::string controlValue = controlCharacteristic.getValue();
  if (controlValue[0] != ledStatus) {
    Serial.print("Value changed... new value: ");
    Serial.println(controlValue[0]);
    ledStatus = controlValue[0];
    if (ledStatus == 48) {
      digitalWrite(LED_PIN_1, LOW); // LED 1 Apagado
      digitalWrite(LED_PIN_2, LOW); // LED 2 Apagado
      digitalWrite(LED_PIN_3, LOW); // LED 3 Apagado
    }
    else if (ledStatus == 49) {
      digitalWrite(LED_PIN_1, HIGH); // LED 1 Encendido
      digitalWrite(LED_PIN_2, HIGH); // LED 2 Encendido
      digitalWrite(LED_PIN_3, HIGH); // LED 3 Encendido
    }
  }
}
