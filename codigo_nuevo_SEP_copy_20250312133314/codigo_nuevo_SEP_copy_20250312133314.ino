
#include <Servo.h>
#include <SoftwareSerial.h>                           //we have to include the SoftwareSerial library, or else we can't use it
#define rx 2                                          //define what pin rx is going to be
#define tx 3                                         //define what pin tx is going to be

Servo myservo;
Servo myservo2;

int pos = 0;

SoftwareSerial myserial(rx, tx);                      //define how the soft serial port is going to work


String inputstring = "";                              //a string to hold incoming data from the PC
String sensorstring = "";                             //a string to hold the data from the Atlas Scientific product
boolean input_string_complete = false;                //have we received all the data from the PC
boolean sensor_string_complete = false;               //have we received all the data from the Atlas Scientific product

#define pinSensor 7
int obstaculo = HIGH;

bool estado=true;
int c=0;

void setup() {      
  
  myservo.attach(9); 
  myservo2.attach(10); // attaches the servo on pin 9 to the servo object

  myservo.write(0);
  myservo2.write(0);

  //                                   //set up the hardware
  Serial.begin(9600);                                 //set baud rate for the hardware serial port_0 to 9600
  myserial.begin(9600);                               //set baud rate for the software serial port to 9600
  inputstring.reserve(10);                            //set aside some bytes for receiving data from the PC
  sensorstring.reserve(30);    
  
 
  pinMode(pinSensor, INPUT);
                         //set aside some bytes for receiving data from Atlas Scientific product
}



void loop() { 

   if (Serial.available()) {                //if a string from the PC has been received in its entirety
    inputstring = Serial.readStringUntil(13);           //read the string until we see a <CR>
    myserial.print(inputstring);                      //send that string to the Atlas Scientific product
    myserial.print('\r');                             //add a <CR> to the end of the string
    inputstring = "";                                 //clear the string
  }

    if (digitalRead(pinSensor) == 0 and estado) {
    // Serial.print("R");
    // myserial.print('\r');                             //add a <CR> to the end of the string
    estado=false;
    c++;
    sensor ();
    Serial.print(c);
    

    }      
    if (digitalRead(pinSensor) == 1 ){
      estado=true;
    }                                 //here we go...
  if (input_string_complete == true) {                //if a string from the PC has been received in its entirety
    Serial.print('\r');                             //add a <CR> to the end of the string
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
int_blu= atoi(blu);                                 //uncomment this line to convert the char to an int


if (int_red>int_grn and int_red>int_blu);
  myservo.write(90);
  
}


  


void sensor (){
  myserial.print("R\r");

      // print_RGB_data();                               //then call this function 
                 //set the flag
}