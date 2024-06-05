void setup() {
  // put your setup code here, to run once:
pinMode (4,input);
Serial.begin (9600);
}

void loop() {
  // put your main code here, to run repeatedly:
digitalRead (4);
Serial.print (digitalRead (4));
}
