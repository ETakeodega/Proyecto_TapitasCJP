#include <SoftwareSerial.h>
#include <Servo.h>

#define rx 2              // RX del Arduino ← TX del EZO-RGB
#define tx 3              // TX del Arduino → RX del EZO-RGB
#define pinSensor 8      // Salida digital del sensor (queremos flanco 1->0)

Servo myservo;
Servo myservo2;

SoftwareSerial myserial(rx, tx); // Comunicación con el EZO-RGB

String inputString = "";
String sensorstring = "";

bool objetoDetectado = false;
unsigned long tempoUltimaDeteccao = 0;
const unsigned long intervaloDeteccao = 1000; // cooldown 1 s

int int_red = 0, int_grn = 0, int_blu = 0;

// --- Flanco 1->0 (HIGH->LOW) ---
int prevState = HIGH;                // estado previo del pin
const unsigned long DEBOUNCE_MS = 15;

void setup() {
  Serial.begin(9600);
  myserial.begin(9600);

  // Bias a HIGH en reposo; activo LOW (útil para disparo 1->0)
  pinMode(pinSensor, INPUT_PULLUP);

  Serial.println("Pronto. Disparo EXACTO en flanco 1->0 (HIGH->LOW).");

  myservo.attach(9);
  myservo2.attach(10);
  myservo.write(40);
  myservo2.write(25);

  // Lee estado inicial del sensor
  prevState = digitalRead(pinSensor);
  Serial.print("Estado inicial pin 7 = ");
  Serial.println(prevState == HIGH ? "HIGH" : "LOW");
}

void loop() {
  unsigned long now = millis();

  // ======== DETECCIÓN: flanco 1->0 (HIGH->LOW) ========
  int curr = digitalRead(pinSensor);

  if (curr != prevState) {
    // Debounce corto
    delay(DEBOUNCE_MS);
    curr = digitalRead(pinSensor);

    if (curr != prevState) {
      // Cambio real: ¿es flanco HIGH->LOW?
      if (prevState == HIGH && curr == LOW) {
        if (now - tempoUltimaDeteccao > intervaloDeteccao) {
          tempoUltimaDeteccao = now;
          objetoDetectado = true;
          Serial.println("Objeto DETECTADO (1->0) → Enviando R");
          myserial.print("R\r");
        } else {
          Serial.println("Deteccion ignorada por cooldown");
        }
      } else if (prevState == LOW && curr == HIGH) {
        // Salió del objeto (no mandamos nada)
        objetoDetectado = false;
        Serial.println("Objeto SALIÓ (0->1)");
      }
      prevState = curr;
    }
  }

  // ======== COMANDOS MANUALES (igual que tu original) ========
  while (Serial.available()) {
    char c = Serial.read();
    if (c == '\r' || c == '\n') {
      if (inputString.length() > 0) {
        myserial.print(inputString + "\r");   // envía con CR (requerido por EZO)
        Serial.println("Comando enviado: " + inputString);
        inputString = "";
      }
    } else {
      inputString += c;
    }
  }

  // ======== LECTURA DE RESPUESTA DEL EZO-RGB (igual) ========
  while (myserial.available()) {
    char c = myserial.read();
    Serial.write(c);
    if (c == '\r') {
      procesarRGB(sensorstring);
      sensorstring = "";
    } else if (c != '\n') {
      sensorstring += c;
    }
  }
}

void procesarRGB(String datos) {
  datos.trim();  // elimina espacios y saltos

  // Ignora líneas vacías o no numéricas (p. ej., *OK)
  if (datos.length() == 0 || !isdigit(datos[0])) return;

  char sensorstring_array[30];
  datos.toCharArray(sensorstring_array, sizeof(sensorstring_array));
  char *red = strtok(sensorstring_array, ",");
  char *grn = strtok(NULL, ",");
  char *blu = strtok(NULL, ",");

  if (red && grn && blu) {
    int_red = atoi(red);
    int_grn = atoi(grn);
    int_blu = atoi(blu);

    if (int_red == 0 && int_grn == 0 && int_blu == 0) return;

    Serial.print("R: "); Serial.print(int_red);
    Serial.print(", G: "); Serial.print(int_grn);
    Serial.print(", B: "); Serial.println(int_blu);
  }

  // Acciones por color dominante (como tenías)
  if (int_blu > int_red && int_blu > int_grn) {
    myservo.write(95); delay(2000); myservo.write(40);
  } else if (int_red > int_blu && int_red > int_grn) {
    myservo2.write(90); delay(2000); myservo2.write(25);
  } else if (int_grn > int_blu && int_grn > int_red) {
    myservo.write(40); myservo2.write(25);
  }
}
