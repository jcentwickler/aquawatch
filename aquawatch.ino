// 1. LIBRERIAS
#include <LiquidCrystal_I2C.h>

// 2. LCD
LiquidCrystal_I2C lcd(0x20,16,2);
int ultimoEstadoMostrado = -1;

void setupLCD(){
  lcd.init();
  lcd.clear();
  lcd.backlight();
  lcd.setCursor(4,0);   
  lcd.print("AquaWatch");
  lcd.setCursor(2,1);
  lcd.print("Encendiendo...");
  delay(5000);
  lcd.setCursor(2,1);
  lcd.print("");
}

// 3. FLOAT SWITCH - SENSOR DE NIVEL DE AGUA FLOTADOR
#define PIN_FLOTADOR_SUPERIOR 13
#define PIN_FLOTADOR_INFERIOR 12
const int SALIDA_FLOTADOR_SUPERIOR = 11;
const int SALIDA_FLOTADOR_INFERIOR = 10;

void setupFloatSwitch() {
  pinMode(PIN_FLOTADOR_SUPERIOR, INPUT);
  pinMode(PIN_FLOTADOR_INFERIOR, INPUT);
  pinMode(SALIDA_FLOTADOR_SUPERIOR, OUTPUT);
  pinMode(SALIDA_FLOTADOR_INFERIOR, OUTPUT);
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
      lcd.setCursor(2, 0);
      lcd.print("Nivel de Agua");
      lcd.setCursor(4, 1);
      lcd.print("Alto");
    } else if (estadoActual == 0) {
      lcd.setCursor(2, 0);
      lcd.print("Nivel de Agua");
      lcd.setCursor(4, 1);
      lcd.print("Bajo");
    } else if (estadoActual == -1){
        lcd.setCursor(4,0);   
        lcd.print("AquaWatch");
    }
    ultimoEstadoMostrado = estadoActual;
  }
}

void loopFloatSwitch() {
  int lecturaFlotadorSuperior = digitalRead(PIN_FLOTADOR_SUPERIOR);
  int lecturaFlotadorInferior = digitalRead(PIN_FLOTADOR_INFERIOR);
  digitalWrite(SALIDA_FLOTADOR_SUPERIOR, (lecturaFlotadorSuperior == HIGH) ? HIGH : LOW);
  digitalWrite(SALIDA_FLOTADOR_INFERIOR, (lecturaFlotadorInferior == HIGH) ? HIGH : LOW);

  LCDFloatSwitch(lecturaFlotadorSuperior, lecturaFlotadorInferior);

}

//4. SENSOR DE HUMEDAD
#define PIN_POWER_SENSOR_HUMEDAD A0 
#define PIN_HUMEDAD A1
#define PIN_MOTOR_DC 9

int humedad = 0;

void setupSensorDeHumedad() {
    pinMode(PIN_POWER_SENSOR_HUMEDAD, OUTPUT);
    pinMode(PIN_HUMEDAD, INPUT);
    Serial.begin(9600);
    pinMode(PIN_MOTOR_DC, OUTPUT);
}

void loopSensorDeHumedad() {
    digitalWrite(PIN_POWER_SENSOR_HUMEDAD, HIGH);
    delay(10);
    humedad = analogRead(PIN_HUMEDAD);
    digitalWrite(PIN_POWER_SENSOR_HUMEDAD, LOW); //Apagar el sensor para evitar corrosion del metal
    Serial.println(humedad);
    digitalWrite(PIN_MOTOR_DC, LOW);

    if (humedad < 400){
        digitalWrite(PIN_MOTOR_DC, HIGH);
    }
    delay(100);
}

// 5. CODIGO PRINCIPAL
void setup() {
  setupFloatSwitch();
  setupLCD();
  setupSensorDeHumedad();
}

void loop() {
  loopFloatSwitch();
  loopSensorDeHumedad();
}