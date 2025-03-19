#include <Servo.h>
#include <SoftwareSerial.h>

#define rx 2      // Pin RX para comunicación serial
#define tx 3      // Pin TX para comunicación serial
#define pinSensor 7  // Pin del sensor de proximidad

Servo myservo;
Servo myservo2;
SoftwareSerial myserial(rx, tx);

String inputstring = "";
String sensorstring = "";
boolean input_string_complete = false;
boolean sensor_string_complete = false;

bool objetoDetectado = false; // Variable para controlar la detección
int c = 0;
unsigned long tiempoUltimaDeteccion = 0; // Guarda el tiempo de la última detección
const unsigned long intervaloDeteccion = 1000; // Tiempo mínimo entre detecciones (1 segundo)

void setup() {      
  myservo.attach(9);   // Servo principal en el pin 9
  myservo2.attach(10); // Segundo servo en el pin 10

  myservo.write(0);
  myservo2.write(0);

  Serial.begin(9600);   // Comunicación serial con la PC
  myserial.begin(9600); // Comunicación con el sensor de color
  inputstring.reserve(10);
  sensorstring.reserve(30);

  pinMode(pinSensor, INPUT); // Configuración del sensor de proximidad
}

void loop() { 
  if (Serial.available()) {
    inputstring = Serial.readStringUntil(13); // Leer datos de la PC
    myserial.print(inputstring);
    myserial.print('\r');
    inputstring = "";
  }

  unsigned long tiempoActual = millis(); // Obtener el tiempo actual

  if (digitalRead(pinSensor) == 0 && !objetoDetectado && (tiempoActual - tiempoUltimaDeteccion > intervaloDeteccion)) {
    objetoDetectado = true; // Marca que el objeto fue detectado
    tiempoUltimaDeteccion = tiempoActual; // Guarda el tiempo de la detección
    c++;
    sensor();
    Serial.print("Objeto detectado. Contador: ");
    Serial.println(c);
  }      

  if (digitalRead(pinSensor) == 1 && objetoDetectado) {
    objetoDetectado = false; // Permite nueva detección solo cuando el objeto desaparezca
  }

  if (input_string_complete) {
    Serial.print('\r');
    inputstring = "";
    input_string_complete = false;
  }

  if (myserial.available() > 0) {
    char inchar = (char)myserial.read();
    sensorstring += inchar;
    if (inchar == '\r') {
      sensor_string_complete = true;
    }
  }

  if (sensor_string_complete) {
    if (!isdigit(sensorstring[0])) {
      Serial.println(sensorstring);
    } else {
      print_RGB_data();
    }
    sensorstring = "";
    sensor_string_complete = false;
  }
}

void print_RGB_data() {
  char sensorstring_array[30];
  char *red, *grn, *blu;
  int int_red, int_grn, int_blu;

  sensorstring.toCharArray(sensorstring_array, 30);
  red = strtok(sensorstring_array, ",");
  grn = strtok(NULL, ",");
  blu = strtok(NULL, ",");

  Serial.print("RED: "); Serial.println(red);
  Serial.print("GREEN: "); Serial.println(grn);
  Serial.print("BLUE: "); Serial.println(blu);

  int_red = atoi(red);
  int_grn = atoi(grn);
  int_blu = atoi(blu);

  // Movimiento del servo motor según la condición B > R y B > G
  if (int_blu > int_red and int_blu > int_grn) {
    myservo.write(90);
    delay(3000);
    myservo.write(0);
  } else {
    myservo.write(0);
  }

  if (int_red > int_blu and int_red > int_grn) {
    myservo2.write(90);
    delay(3000);
    myservo2.write(0);
  } else {
    myservo2.write(0);
  }

  if (int_grn > int_blu and int_grn > int_red) {
    myservo.write(90);
    myservo2.write(90);
    delay(3000);
    myservo.write(0);
    myservo2.write(0);
  } else {
    myservo.write(0);
    myservo2.write(0);
  }

}

void sensor() {
  myserial.print("R\r");
}
