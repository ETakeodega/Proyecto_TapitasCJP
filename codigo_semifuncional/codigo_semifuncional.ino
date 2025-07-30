#include <SoftwareSerial.h>
#include <Servo.h>

#define rx 2              // RX do Arduino ← TX do sensor EZO-RGB
#define tx 3              // TX do Arduino → RX do sensor EZO-RGB
#define pinSensor 7       // Pino digital para o sensor de proximidade

Servo myservo;
Servo myservo2;

SoftwareSerial myserial(rx, tx); // Comunicação com o sensor EZO-RGB

String inputString = "";
String sensorstring = "";

bool objetoDetectado = false;
unsigned long tempoUltimaDeteccao = 0;
const unsigned long intervaloDeteccao = 1000; // 1 segundo
int int_red = 0;
int int_grn = 0;
int int_blu = 0;

void setup() {
  Serial.begin(9600);
  myserial.begin(9600);
  pinMode(pinSensor, INPUT);

  Serial.println("Digite comandos ou aproxime um objeto para enviar 'R'");
  Serial.println("Comandos: R, i, Baud,9600, etc.");

  myservo.attach(9); 
  myservo2.attach(10);

  myservo.write(0);
  myservo2.write(0);

}

void loop() {
  unsigned long tempoAtual = millis();

  // Envia 'R' automaticamente quando o sensor detecta objeto (LOW)
  if (digitalRead(pinSensor) == LOW && !objetoDetectado && (tempoAtual - tempoUltimaDeteccao > intervaloDeteccao)) {
    objetoDetectado = true;
    tempoUltimaDeteccao = tempoAtual;
    Serial.println("Objeto detectado → Enviando comando: R");
    myserial.print("R\r");
  }

  // Libera nova detecção quando objeto sai
  if (digitalRead(pinSensor) == HIGH && objetoDetectado) {
    objetoDetectado = false;
  }

  // Comandos manuais via Monitor Serial
  if (Serial.available()) {
    inputString = Serial.readStringUntil('\n');
    inputString.trim();
    myserial.print(inputString + "\r");
    Serial.println("Comando enviado: " + inputString);
  }
int xx =0;
  // Resposta do sensor EZO-RGB → exibe no Monitor Serial
  while (myserial.available()) {
    char c = myserial.read();
    Serial.write(c);
 if (c == '\r') {
      // Una línea completa llegó → procesar
      procesarRGB(sensorstring);
      sensorstring = "";
    } else {
      sensorstring += c;
    }
    }
 
}


void procesarRGB(String datos) {
  datos.trim();  // elimina espacios y saltos de línea

  // Ignora respuestas vacías o no numéricas (como *OK)
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

    // Opcional: ignora lectura nula (0,0,0)
    if (int_red == 0 && int_grn == 0 && int_blu == 0) return;

    // Mostrar en formato limpio
    Serial.print("R: ");
    Serial.print(int_red);
    Serial.print(", G: ");
    Serial.print(int_grn);
    Serial.print(", B: ");
    Serial.println(int_blu);
  }

if (int_blu > int_red and int_blu > int_grn) {
  myservo.write(90);
  delay(3000);
  myservo.write(0);
}

else if (int_red > int_blu and int_red > int_grn) {
  myservo2.write(90);
  delay(3000);
  myservo2.write(0);
}

else if (int_grn > int_blu and int_grn > int_red){
  myservo.write(0);
  myservo2.write(0);
}




}




    
