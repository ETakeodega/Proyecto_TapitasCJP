int sensor_pin = 4;
int sensor_prox = 0;
int contador_tapas = 0;
int contador_r = 0;
int contador_g = 0;
int contador_r = 0;
void setup() {
  // put your setup code here, to run once:
pinMode (sensor_pin,INPUT);
Serial.begin (9600);
}

void loop() {
  // put your main code here, to run repeatedly:
sensor_prox = digitalRead (sensor_pin);
//Serial.println (digitalRead (sensor_pin));

if (sensor_prox){
  digitalRead (sensor_pin);
  if (sensor_prox != digitalRead (sensor_pin)){
    Serial.println('R');
    contador_tapas ++;
Serial.println(contador_tapas);
  }
  



}

}



