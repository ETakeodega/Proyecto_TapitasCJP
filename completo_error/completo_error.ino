#include <Servo.h>
#include <SoftwareSerial.h>

#define rx 2
#define tx 3
#define pinSensor 7

Servo myservo;
Servo myservo2;
SoftwareSerial myserial(rx, tx);

String inputstring = "";
String sensorstring = "";  // <-- declarada no topo, visível em todo o código
boolean sensor_string_complete = false;

bool objetoDetectado = false;
bool leituraProcessada = false;
int estadoAnteriorSensor = 1;
unsigned long tempoUltimaLeitura = 0;
const unsigned long tempoEsperaEntreObjetos = 2000;

void setup() {
  myservo.attach(9);
  myservo2.attach(10);
  myservo.write(0);
  myservo2.write(0);

  Serial.begin(9600);
  myserial.begin(9600);

  inputstring.reserve(10);
  sensorstring.reserve(100);

  pinMode(pinSensor, INPUT);

  Serial.println("Sistema iniciado.");
}

void loop() {
  if (Serial.available()) {
    inputstring = Serial.readStringUntil('\r');
    myserial.print(inputstring);
    myserial.print('\r');
    inputstring = "";
  }

  int leituraAtual = digitalRead(pinSensor);
  unsigned long agora = millis();

  if (leituraAtual == 0 && estadoAnteriorSensor == 1 && !objetoDetectado && (agora - tempoUltimaLeitura > tempoEsperaEntreObjetos)) {
    objetoDetectado = true;
    tempoUltimaLeitura = agora;
    leituraProcessada = false;

    Serial.println("Objeto detectado → enviando comando R");
    sensor();
  }

  if (leituraAtual == 1 && objetoDetectado) {
    objetoDetectado = false;
    Serial.println("Objeto removido → pronto para nova leitura");
  }

  estadoAnteriorSensor = leituraAtual;

  while (myserial.available() > 0) {
    char inchar = (char)myserial.read();
    if (inchar != '\r' && inchar != '\n') {
      sensorstring += inchar;
    } else if (sensorstring.length() > 0) {
      sensor_string_complete = true;
    }
  }

  if (sensor_string_complete && !leituraProcessada) {
    processarRGB();  // <- sensorstring visível aqui
    leituraProcessada = true;
    sensorstring = "";
    sensor_string_complete = false;
  }
}

void sensor() {
  myserial.print("R\r");
}

// ✅ FUNÇÃO processarRGB corrigida
void processarRGB() {
  int int_red = 0, int_grn = 0, int_blu = 0;

  Serial.print("Recebido do sensor (raw): ");
  Serial.println(sensorstring);

  // Verifica se a string contém exatamente duas vírgulas e começa com número
  if (sensorstring.indexOf(',') > 0 && isDigit(sensorstring.charAt(0))) {
    int virg1 = sensorstring.indexOf(',');
    int virg2 = sensorstring.indexOf(',', virg1 + 1);

    if (virg1 != -1 && virg2 != -1) {
      // Conversão correta e limpa
      String rStr = sensorstring.substring(0, virg1);
      String gStr = sensorstring.substring(virg1 + 1, virg2);
      String bStr = sensorstring.substring(virg2 + 1);

      rStr.trim();
      gStr.trim();
      bStr.trim();

      int_red = rStr.toInt();
      int_grn = gStr.toInt();
      int_blu = bStr.toInt();

      Serial.println("========== RESULTADO RGB ==========");
      Serial.print("RED   = "); Serial.println(int_red);
      Serial.print("GREEN = "); Serial.println(int_grn);
      Serial.print("BLUE  = "); Serial.println(int_blu);

      if (int_blu > int_red && int_blu > int_grn) {
        Serial.println("→ Azul dominante → Servo 1 acionado");
        myservo.write(90);
        delay(3000);
        myservo.write(0);
      } else if (int_red > int_blu && int_red > int_grn) {
        Serial.println("→ Vermelho dominante → Servo 2 acionado");
        myservo2.write(90);
        delay(3000);
        myservo2.write(0);
      } else if (int_grn > int_red && int_grn > int_blu) {
        Serial.println("→ Verde dominante → Nenhum servo acionado");
      } else {
        Serial.println("→ Sem cor dominante clara (empate ou erro).");
      }

      Serial.println("===================================");

    } else {
      Serial.println("→ Erro: formato CSV incompleto.");
    }
  } else {
    Serial.println("→ Ignorado: linha inválida ou não numérica.");
  }
}
