// 1. LIBRERIAS
#include <LiquidCrystal_I2C.h>

// 2. LCD
LiquidCrystal_I2C lcd(0x27,16,2);
int ultimoEstadoMostrado = -1;

void setupLCD(){
  lcd.init();
  lcd.clear();
  lcd.backlight();
  lcd.setCursor(4,0);   
  lcd.print("AquaSmart");
  lcd.setCursor(1,1);
  lcd.print("Encendiendo...");
  delay(3000);
  lcd.setCursor(2,1);
  lcd.clear();
  lcd.setCursor(4,0);   
  lcd.print("AquaSmart");
  lcd.setCursor(1,1);
  lcd.print("Sistema Activo");
}

void LCDHome(){
  lcd.clear();
  lcd.setCursor(4,0);   
  lcd.print("AquaSmart");
  lcd.setCursor(1,1);
  lcd.print("Sistema Activo");
}

void LCDRegando(){
  lcd.clear();
  lcd.setCursor(4,0);   
  lcd.print("AquaSmart");
  lcd.setCursor(3,1);
  lcd.print("Regando...");
}

// 3. FLOAT SWITCH - SENSOR DE NIVEL DE AGUA FLOTADOR
#define PIN_FLOTADOR_SUPERIOR 13
#define PIN_FLOTADOR_INFERIOR 10

void setupFloatSwitch() {
  pinMode(PIN_FLOTADOR_SUPERIOR, INPUT);
  pinMode(PIN_FLOTADOR_INFERIOR, INPUT_PULLUP);
}

void LCDFloatSwitch(int lecturaSuperior, int lecturaInferior) {
  int estadoActual;

  if (lecturaSuperior == HIGH) {
    estadoActual = 1; 
  } else if (lecturaInferior == LOW) {
    estadoActual = 0; 
  } else {
    estadoActual = -1; 
  }

  if (estadoActual != ultimoEstadoMostrado) {
    lcd.clear();
    if (estadoActual == 1) {
      lcd.setCursor(1, 0);
      lcd.print("Nivel de Agua:");
      lcd.setCursor(5, 1);
      lcd.print("Maximo");
    }   
    else if (estadoActual == 0) {
      lcd.setCursor(1, 0);
      lcd.print("Nivel de Agua:");
      lcd.setCursor(6, 1);
      lcd.print("Bajo"); 
    }  else if (estadoActual == -1){
        lcd.setCursor(4,0);   
        lcd.print("AquaWatch");
        lcd.setCursor(1,1);
        lcd.print("Sistema Activo");
    }
    ultimoEstadoMostrado = estadoActual;
  }
}

void loopFloatSwitch() {
  int lecturaFlotadorSuperior = digitalRead(PIN_FLOTADOR_SUPERIOR);
  int lecturaFlotadorInferior = digitalRead(PIN_FLOTADOR_INFERIOR);
  LCDFloatSwitch(lecturaFlotadorSuperior, lecturaFlotadorInferior);
}

//4. SENSOR DE HUMEDAD
#define PIN_HUMEDAD A0
#define PIN_MOTOR_DC 3

int humedad = 0;

void setupSensorDeHumedad() {
    pinMode(PIN_HUMEDAD, INPUT);
    Serial.begin(9600);
    pinMode(PIN_MOTOR_DC, OUTPUT);
}

void loopSensorDeHumedad() {
    humedad = analogRead(PIN_HUMEDAD);
    Serial.print("Humedad: ");
    Serial.println(humedad);

    // Leer el flotador inferior
    int estadoFlotadorInferior = digitalRead(PIN_FLOTADOR_INFERIOR);

    // Condición: humedad baja Y hay agua (flotador inferior en HIGH)
    if (humedad > 400 && estadoFlotadorInferior == HIGH) {
        digitalWrite(PIN_MOTOR_DC, HIGH);  // Encender motor
        LCDRegando();
        delay(10000);                        // Mantenerlo encendido por 10 segundos
        LCDHome();
        digitalWrite(PIN_MOTOR_DC, LOW);    // Apagar motor
        delay(15000);                        // Esperar 15 segundos antes de la siguiente lectura
    }
}

// 5. CODIGO PRINCIPAL
void setup() {
  Serial.begin(9600);
  setupFloatSwitch();
  setupLCD();
  setupSensorDeHumedad();
}

void loop() {
  loopFloatSwitch();
  loopSensorDeHumedad();
}