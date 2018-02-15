/*
  Arduino RFID Keyboard 

  This little code shows you how easy you can use a RFID Modul in combination with a Keyboard function (from Arduino Pro Micro (ATmega32U4)).
  Freely usable for all with the exception of military and other anti-social activities.
  Created 22.8.2015 by Marc-André Tragé
  
  Pinout for Arduino Pro Micro
  ! VCC  to RFID RC522 Modul needs 3.3v, more informations about this at line 22.
    GND  GND
    IRG  not connected
    RST  pin 6
    SDA  pin 10  
    MISO pin 14
    SCK  pin 15  
    MOSI pin 16

  What you need for a little RFID fun:
    +  Arduino Pro Micro and Leonardo or equal Boards with a ATmega32U4
       it can emulate via USB a keyboard or a mouse) order types like this => https://www.google.de/search?q=arduino+pro+micro
    +  RFID RC522 Modul like this => https://www.google.de/search?q=rfid+rc522
    +  if you have only 5.0v VCC on Board, use a AMS1117-3.3 like this => http://www.advanced-monolithic.com/pdf/ds1117.pdf
    +  LED Button like this => https://www.google.de/search?q=LED+Light+Tactile+Push+Button
    
  If you have any useful changes or improvements please let me know.
  (Not a wish (but a command)) have fun (NOW (and for the rest of your life))!
*/


// ATmega32U4 Boards needs this include for USB output (keyboard / mouse)
   #include <Keyboard.h>

// BEG RFID module Part
   #include <SPI.h> 
   #include <MFRC522.h> 
   #define SS_PIN  10 
   #define RST_PIN 6 
   MFRC522 mfrc522(SS_PIN, RST_PIN);
// END


// BEG | everything important for the RFID module
   String PW1 = "00 00 00 00"; // RFID access ID User 1
   String PW2 = "00 00 00 00"; // RFID access ID User 2
   String RFID_Key;            // Real RFID

// With Fake_RFID_PW1/PW2 can you set a alternativ part of the ID/Password for posting via Keyboard
// exclude the sending of the original RFID via Keyboard out.

   // Please don't use " in your Passwords
   String RFID_Fake_PW1 = "User 1 Password"; // Password for User 1.
   String RFID_Fake_PW2 = "User 2 Password"; // Password for User 2.

   // Sending the Password via Keyboard with 0 = "UpperCase off" or with 1 = "UpperCase on".
   int UpperCase = 0;
// END


// BEG | everything important for the LED Button 
   int LED                =  8; // To control the button led.
   int LED_Button         =  9; // To interrogate the button.
   int LED_On_Off         =  0; // 0 = LED off | 1 = LED on.
   int LED_Interval       =  0; // The starting value should always be 0.
   int LED_Interval_Time  = 10; // LED interval time.
   int LED_Flasher_a      =  1; // LED_Flasher time on in ms.
   int LED_Flasher_b      = 25; // LED_Flasher time off in ms.
// END


// Speed- or Tuning- values 
   int Global_wait_for_input =    0; // The starting value should always be 0.
   int Global_wait_Time      =   30; // Wait time to send password after the key verification.
   int LED_Global_off_wait   =   25; // Should not be less than 25.
   int Read_Interval         = 1000; // Time interval between RFID read operations.
   int Wait_Time_PW          = 2500; // Wait (ms) until the password is sent again (the button should still be pressed).


void setup(){
  
  // RFID Modul
  SPI.begin();
  mfrc522.PCD_Init();
  
  // LED Button
  pinMode(LED, OUTPUT);
  pinMode(LED_Button, INPUT_PULLUP);
  
  //LED Button Startup blinker
  for (int i = 20; i > 0; i--){
    digitalWrite(LED, HIGH); delay(i);
    digitalWrite(LED, LOW);  delay(i*3);
    }
  }

// LED_Flasher(a,b) a = LED on Time | b = LED off Time
int LED_Flasher(int a, int b){       
  digitalWrite(LED, HIGH); delay(a);
  digitalWrite(LED, LOW);  delay(b);
  }

// LED a wait and global wait actions
int LED_Interval_and_Global_wait(){
  LED_Interval ++;
  Global_wait_for_input ++;
  
  if(LED_On_Off==1){
    LED_Flasher(LED_Flasher_a,LED_Flasher_b);                 
    }

  if(LED_On_Off==1){
    if(Global_wait_for_input > Global_wait_Time){
      Global_wait_for_input = 0;
      LED_Interval = 0;
      LED_On_Off = 0;
      }
      
    if(LED_Interval > LED_Interval_Time){
      digitalWrite(LED, HIGH);
      delay(1);
      digitalWrite(LED, LOW);
      delay(1);
      LED_Interval=0;
      }
      
    }
    
  else{
    digitalWrite(LED, LOW);
    delay(LED_Global_off_wait);
    }
    
  }


void loop(){

  LED_Interval_and_Global_wait(); // So please, does not the function name say everything? :)

// Is there to find an RFID card?
  if ( ! mfrc522.PICC_IsNewCardPresent()){
    return;
    }
    
  if ( ! mfrc522.PICC_ReadCardSerial()){
    return;
    }
  
// If an RFID card has been scanned, then the id will be assembled
  String RFID = "";

  for (byte i = 0; i < mfrc522.uid.size; i++) {
    RFID.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    RFID.concat(String(mfrc522.uid.uidByte[i], HEX));
    }

  RFID.toUpperCase();
  RFID_Key=RFID.substring(1);

// LED_Button actions
  if (digitalRead(LED_Button) == LOW){
    // if RFID_Fake_PW1 free post the real RFID via Keyboard
    if(RFID_Fake_PW1==""){
      Keyboard.print("RFID "+RFID_Key);
      Keyboard.print("\n");
      }

// The LED_On_Off is at 1 send via keyboard PW1 or PW2
    if(LED_On_Off==1){
      LED_On_Off=0;

      if(UpperCase!=0){
        RFID_Fake_PW1.toUpperCase();
        RFID_Fake_PW2.toUpperCase();
        }
        
      if(RFID.substring(1) == PW1){
        Keyboard.print(RFID_Fake_PW1);
        }
        
      else{
        Keyboard.print(RFID_Fake_PW2);
        }
        
      Keyboard.print("\n");
      delay(Wait_Time_PW);
      
    }
    
  }

// Check PW1 and PW2, switch LED from off(0) to on(1) or from 1 to 0.
// If LED_On_Off is set to 1, PW1 or PW2 can be sent via keyboard when the LED_Button is pressed.
  if (RFID.substring(1) == PW1 || RFID.substring(1) == PW2){
    LED_On_Off=1;
    delay(Read_Interval);
    }
    
  else{
    LED_On_Off=0;
    delay(Read_Interval);
    }
    
  }
