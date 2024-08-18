#include <Wire.h>  

// Definición de pines
#define BP_START_PIN (2)  // Emulación del botón de inicio del dispositivo de medición de presión arterial
#define VALVE_PIN (3)     // Verifica si la medición ha terminado

volatile byte i2c_data_rx;  // Indica que hay datos disponibles en el bus I2C
volatile uint16_t count;    // Indica el número total de datos recolectados
volatile uint8_t sys, dia, hr;  // Almacena los valores medidos: sistólica, diastólica y frecuencia cardíaca

void setup() {
  Serial.begin(115200);
  pinMode(BP_START_PIN, OUTPUT);
  pinMode(VALVE_PIN, INPUT);
  Wire.begin(0x50);  
  Wire.onReceive(receiveEvent);  

  Serial.println("Esperando datos I2C...");
}

void loop() {
  // Procesa los datos si han sido recibidos
  if (count == 35) {
    Serial.print("Datos de Presión Arterial: ");
    Serial.print(sys);
    Serial.print("/");
    Serial.println(dia);
    Serial.print("Frecuencia Cardiaca: ");
    Serial.println(hr);
    Serial.println("");
    
   
    count = 0;
  } else if (count > 0 && count != 35) {
    Serial.println("Error en la recepción de datos.");
    count = 0;  
  }

  delay(500);  
}

void receiveEvent(int iData) { 
  if (iData > 0) {
    while (iData--) {
      i2c_data_rx = Wire.read(); 
      count++;  

      
      if (count == 28) {
        sys = i2c_data_rx;
      }
      if (count == 29) {
        dia = i2c_data_rx;
      }
      if (count == 30) {
        hr = i2c_data_rx;
      }
    }
  }
}
