#include <Servo.h>

Servo myservo1;  
Servo myservo2;

int led_R = 8;
int led_G = 9;
int led_B = 10;

#include <SoftwareSerial.h>    
                                                     
#define rx 2                                          //define what pin rx is going to be
#define tx 3                                          //define what pin tx is going to be

SoftwareSerial myserial(rx, tx);                      //define how the soft serial port is going to work


String inputstring = "";                              //a string to hold incoming data from the PC
String sensorstring = "";                             //a string to hold the data from the Atlas Scientific product
boolean input_string_complete = false;                //have we received all the data from the PC
boolean sensor_string_complete = false;               //have we received all the data from the Atlas Scientific product




void setup() {    
  
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
  Serial.println(blu);   
                                //this is the blue value
  int_red= atoi(red);                                 //uncomment this line to convert the char to an int
 int_grn= atoi(grn);                                 //uncomment this line to convert the char to an int
 int_blu= atoi(blu);
  if (int_red > 130) {
    digitalWrite(led_R, HIGH);
    digitalWrite(led_G, LOW);
    digitalWrite(led_B, LOW);
    myservo1.write(90);  
    delay(10);
    myservo1.write(0);              
}

  if (int_grn > 130) {
    digitalWrite(led_R, LOW);
    digitalWrite(led_G, HIGH);
    digitalWrite(led_B, LOW);
    myservo2.write(90);
    delay(10);
    myservo2.write(0);
}

  if (int_blu > 130) {
    digitalWrite(led_R, LOW);
    digitalWrite(led_G, LOW);
    digitalWrite(led_B, HIGH);
}


                                  //uncomment this line to convert the char to an int
}

