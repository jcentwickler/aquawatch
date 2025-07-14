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
  lcd.setCursor(1,1);
  lcd.print("Encendiendo...");
  delay(3000);
  lcd.setCursor(2,1);
  lcd.clear();
  lcd.setCursor(4,0);   
  lcd.print("AquaWatch");
  lcd.setCursor(1,1);
  lcd.print("Sistema Activo");
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
      lcd.setCursor(1, 0);
      lcd.print("Nivel de Agua:");
      lcd.setCursor(5, 1);
      lcd.print("Maximo");
    } else if (estadoActual == 0) {
      lcd.setCursor(1, 0);
      lcd.print("Nivel de Agua:");
      lcd.setCursor(6, 1);
      lcd.print("Bajo");
    } else if (estadoActual == -1){
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
  digitalWrite(SALIDA_FLOTADOR_SUPERIOR, (lecturaFlotadorSuperior == HIGH) ? HIGH : LOW);
  digitalWrite(SALIDA_FLOTADOR_INFERIOR, (lecturaFlotadorInferior == HIGH) ? HIGH : LOW);

  LCDFloatSwitch(lecturaFlotadorSuperior, lecturaFlotadorInferior);

}

//4. SENSOR DE HUMEDAD
#define PIN_POWER_SENSOR_HUMEDAD A0 
#define PIN_HUMEDAD A1
#define PIN_MOTOR_DC 9

int humedad = 0;
bool yaRiego = false;

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
    Serial.println(humedad);

    if (humedad < 400 && !yaRiego){
        digitalWrite(PIN_MOTOR_DC, HIGH);  // Encender motor
        delay(5000); // Mantenerlo encendido por 5 segundos
        digitalWrite(PIN_MOTOR_DC, LOW); // Apagar motor después de 5 segundos
        yaRiego = true;  // Marcar que ya regó

    } 

    if (humedad >= 400) {
        digitalWrite(PIN_MOTOR_DC, LOW);
        delay(500);
        yaRiego = false;
    }
    
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