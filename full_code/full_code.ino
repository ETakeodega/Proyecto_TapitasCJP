#include <Servo.h>


Servo myservo1;  
Servo myservo2;


int led_R = 8;
int led_G = 9;
int led_B = 10;
int sensor_pin = 4;
int sensor_prox = 0;
int contador_tapas = 0;
int contador_r = 0;
int contador_g = 0;
int contador_b = 0;
#include <SoftwareSerial.h>    
                                                     
#define rx 2                                          //define what pin rx is going to be
#define tx 3                                          //define what pin tx is going to be

SoftwareSerial myserial(rx, tx);                      //define how the soft serial port is going to work


String inputstring = "";                              //a string to hold incoming data from the PC
String sensorstring = "";                             //a string to hold the data from the Atlas Scientific product
boolean input_string_complete = false;                //have we received all the data from the PC
boolean sensor_string_complete = false;               //have we received all the data from the Atlas Scientific product

int int_red= 0;                                 //uncomment this line to convert the char to an int
int int_grn= 0;                              //uncomment this line to convert the char to an int
int int_blu= 0; 

#include <LiquidCrystal.h>

const int rs = 13, en = 12, d4 = 19, d5 = 18, d6 = 17, d7 = 16;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {  

  lcd.begin (16,2);
  lcd.setCursor(0, 0);
  lcd.print("R:  G:  B:  T:");
  
    
  pinMode (sensor_pin,INPUT);
  myservo1.attach(5);
  myservo2.attach(6);  

  pinMode(led_R, OUTPUT);
  pinMode(led_G, OUTPUT);
  pinMode(led_B, OUTPUT);

  Serial.begin(9600);                                 
  myserial.begin(9600);                              
  inputstring.reserve(10);                            
  sensorstring.reserve(30);                           
}


void serialEvent() {                                  
  inputstring = Serial.readStringUntil(13);          
  input_string_complete = true;                       
}


void loop() {                                         //here we go...

  sensor_prox = digitalRead (sensor_pin);
  //Serial.println (digitalRead (sensor_pin));

  if (sensor_prox){
    if (sensor_prox != digitalRead (sensor_pin)){
          myserial.println('\R'); 
          sensor_string_complete = true ;                           //add a <CR> to the end of the string

      contador_tapas ++;
  Serial.println(contador_tapas);

    }}
  if (input_string_complete == true) {                //if a string from the PC has been received in its entirety
    myserial.print(inputstring);                      //send that string to the Atlas Scientific product
    myserial.print('\r');                             //add a <CR> to the end of the string
    inputstring = "";                                 //clear the string
    input_string_complete = false;                    //reset the flag used to tell if we have received a completed string from the PC
  }

  if (myserial.available() > 0) {                     //if we see that the Atlas Scientific product has sent a character
    char inchar = (char)myserial.read();              //get the char we just received
    sensorstring += inchar;                           //add the char to the var called sensorstring
    if (inchar == '\r') {                             //if the incoming character is a <CR>
      sensor_string_complete = true;                  //set the flag
    }
  }


  if (sensor_string_complete == true) {               //if a string from the Atlas Scientific product has been received in its entirety
    if (isdigit(sensorstring[0]) == false) {          //if the first character in the string is a digit
      Serial.println(sensorstring);                   //send that string to the PC's serial monitor
    }
    else                                              //if the first character in the string is NOT a digit
    {
      print_RGB_data();                               //then call this function 
    }
    sensorstring = "";                                //clear the string
    sensor_string_complete = false;                   //reset the flag used to tell if we have received a completed string from the Atlas Scientific product
  }
}



void print_RGB_data(void) {                           //this function will pars the string  

  char sensorstring_array[30];                        //we make a char array
  char *red;                                          //char pointer used in string parsing
  char *grn;                                          //char pointer used in string parsing
  char *blu;                                          //char pointer used in string parsing
  int int_red;                                        //used to hold an int that is the color red 
  int int_grn;                                        //used to hold an int that is the color green 
  int int_blu;                                        //used to hold an int that is the color blue
  
  sensorstring.toCharArray(sensorstring_array, 30);   //convert the string to a char array 
  red = strtok(sensorstring_array, ",");              //let's pars the array at each comma
  grn = strtok(NULL, ",");                            //let's pars the array at each comma
  blu = strtok(NULL, ",");                            //let's pars the array at each comma
  

  Serial.print("RED:");                                //we now print each value we parsed separately
  Serial.println(red);                                 //this is the red value

  Serial.print("GREEN:");                              //we now print each value we parsed separately
  Serial.println(grn);                                 //this is the green value

  Serial.print("BLUE:");                               //we now print each value we parsed separately
  Serial.println(blu);                                 //this is the blue value
  
int_red= atoi(red);                                 //uncomment this line to convert the char to an int
int_grn= atoi(grn);                                 //uncomment this line to convert the char to an int
int_blu= atoi(blu); 

  if (int_red > 175) {
    digitalWrite(led_R, HIGH);
    digitalWrite(led_G, LOW);
    digitalWrite(led_B, LOW);
    myservo1.write(90);  
    delay(10);
    myservo1.write(0);
    Serial.print("red:");
    contador_r ++; 
  }
  if (int_grn > 175) {
    digitalWrite(led_R, LOW);
    digitalWrite(led_G, HIGH);
    digitalWrite(led_B, LOW);
    myservo2.write(90);
    delay(10);
    myservo2.write(0);
    Serial.print("grn:");
    contador_g ++; 
}

  if (int_blu > 175) {
    digitalWrite(led_R, LOW);
    digitalWrite(led_G, LOW);
    digitalWrite(led_B, HIGH);
    Serial.print("blu:"); 
    contador_b ++;    
}

  lcd.setCursor(0, 1);
  lcd.print(contador_r);

   lcd.setCursor(4, 1);
  lcd.print(contador_g);

   lcd.setCursor(8, 1);
  lcd.print(contador_b);

   lcd.setCursor(12, 1);
  lcd.print(contador_tapas);

                                //uncomment this line to convert the char to an int
}