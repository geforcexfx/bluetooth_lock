#include <LiquidCrystal.h>
#include <EEPROM.h>
#include <Servo.h>;

char buffer[10];
int address = 0;
static unsigned long SaveTimer;                
static unsigned long SaveDelay = (30 * 1000);  
char ch;
char CODE[10] = "123142E";                  
char Str[10]; 
char CodeLength = 6;
int Pos = 0;
bool Unlocked;
static unsigned long DisplayTimer; 
static unsigned long DisplayDelay = 200;
int count = 0;

LiquidCrystal lcd(12, 11, 5, 4, 3, 2); 
char data[8]; 

int buttonPin1 = 6;   
int buttonPin2 = 7;
int buttonPin3 = 8;
int buttonPin4 = 9;

int enterbutton = 10; 
int clearlockbutton = 13;

Servo myServo;                    

void setup() {
  for(int i=0;i<=7;i++){
    data[i]=0;
  }
   myServo.attach(A1);  

  Lock();
  
  int EEPROMCodeOK = true;
  for (Pos = 0; Pos <= (CodeLength); Pos++) {
    Str[Pos] =  EEPROM.read(Pos);
    if (!(strrchr("1123456789", Str[Pos]))) { 
      EEPROMCodeOK = false;
    }
  }
  Pos++;
  Str[Pos] =  EEPROM.read(Pos); 
  if (Str[CodeLength + 1] != 'E') EEPROMCodeOK = false; 
  if (EEPROMCodeOK) {
    Str[CodeLength + 2] = '\0';
    strncpy(CODE, Str, CodeLength + 1);
  }
  ClearCode();

  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);
  pinMode(buttonPin3, INPUT_PULLUP);
  pinMode(buttonPin4, INPUT_PULLUP);

  pinMode(enterbutton, INPUT_PULLUP);
  pinMode(clearlockbutton, INPUT_PULLUP);

  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Hello.");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Password:");
  Serial.begin(9600);
  DisplayTimer = millis() + 200;
}

void loop() {

  Lock();
  
  Pos = constrain(Pos, 0, CodeLength);

  int buttonState1 = digitalRead(buttonPin1);  
  int buttonState2 = digitalRead(buttonPin2);
  int buttonState3 = digitalRead(buttonPin3);
  int buttonState4 = digitalRead(buttonPin4);

  int clButtonState = digitalRead(clearlockbutton);
  int enterButtonState = digitalRead(enterbutton);

  lcd.setCursor(9, 0);
 lcd.print(data);
    check();
    
 if( count == 6){
     data[7] = 0;
 }
     
 bluetooth();
    
  if (buttonState1 == LOW) {
    Str[Pos] = '1';
    Pos++;
    Str[Pos] = '\0';
    delay(250); 
    while (digitalRead(buttonPin1) == LOW); 

  }

  else if (buttonState2 == LOW) {
    Str[Pos] = '2';
    Pos++;
    Str[Pos] = '\0';
    delay(250); 
    while (digitalRead(buttonPin2) == LOW);

  }

  else if (buttonState3 == LOW) {
    Str[Pos] = '3';
    Pos++;
    Str[Pos] = '\0';
    delay(250); 
    while (digitalRead(buttonPin3) == LOW);
  }

  else if (buttonState4 == LOW) {
    Str[Pos] = '4';
    Pos++;
    Str[Pos] = '\0';
    delay(250); 
    while (digitalRead(buttonPin4) == LOW); 

  }
  else if (enterButtonState == LOW) {
    Str[Pos] = 'E';
    Pos++;
    Str[Pos] = '\0';
    delay(250);

    lcd.setCursor(15, 0);
    lcd.print("E");
    delay(400);
    lcd.setCursor(15, 0);
    lcd.print(" ");
    
    while (digitalRead(buttonPin1) == LOW); 
     if (strcmp (Str,CODE) == 0 || strcmp (data,CODE) == 0) {
      Unlocked = true;
      Serial.print("0");
        //}
      Serial.print("1");
      lcd.setCursor(0, 0);
      lcd.print(" Access Granted");
      delay(2000);
      lcd.clear();
      lcd.print("    Unlocked");
      
    } 

    else { 

      lcd.clear();
      lcd.print(" Access Denied.");
      delay(2000);
      lcd.clear();
      lcd.print("Password:");

    }

    while (Unlocked) {
      Unlock();
      if (digitalRead(clearlockbutton) == LOW) {
        delay(200);
        lcd.clear();
        lcd.print("     Locked");
        delay(2000);
        lcd.clear();
        Unlocked = false;
         for(int i=0;i<=7;i++){
         data[i]=0;
        }
        SaveTimer = millis() + 30000;
      }
    }
    
    ClearCode();

    
  }

  else if (clButtonState == LOW) {
    delay(500);

    while (clearlockbutton == LOW);
    if ((millis() - SaveTimer) > 4500) {
      
    }
    
    ClearCode();
    
  }

  if ( (long)( millis() - DisplayTimer ) >= 0) {
    DisplayTimer += DisplayDelay;
    lcd.setCursor(9, 0); 
    lcd.print(Str);
    lcd.setCursor(15, 0);
    lcd.print(" ");

    if (clButtonState == LOW) {

      lcd.clear();
      lcd.print("Password:");
      
    }


  }

}

void bluetooth(){
  if (strcmp (data,CODE) == 0) {
      Unlocked = true;
      lcd.setCursor(0, 0);
      lcd.print(" Access Granted");
      delay(2000);
      lcd.clear();
      lcd.print("    Unlocked");
      
    } 

    while (Unlocked) {
      Unlock();
      
      if (digitalRead(clearlockbutton) == LOW) {
        //for(int i=0;i<=1;i++){
        Serial.print("0");
        //}
        Serial.print("1");
        delay(200);
        lcd.clear();
        lcd.print("     Locked");
        delay(2000);
        lcd.clear();
        Unlocked = false;
        for(int i=0;i<=7;i++){
       data[i]=0;
         }
         count = 0;
         
        SaveTimer = millis() + 30000;
        ClearCode();
      }
      
     
    }
     
    
  // ClearCode();

    
}
  
void ClearCode() {
  
  Pos = 0;
  Str[Pos] = '\0';
   /*for(int i=0;i<=7;i++){
    data[i]=0;
  }*/
  //data[1]=0;
  lcd.setCursor(0, 0);
  lcd.print("Password:");
  lcd.setCursor(0, 1);
  lcd.print("          ");
  
}

void check(){
  int u;
  if(Serial.available() > 0){
    ch = Serial.read();
    
    data[count]=ch;

       //Serial.print(data[count]);
          //lcd.print(data[count]);
  count = count + 1;
  
  }
  else
  {
    return;
  }
}

void Unlock() {
  
  myServo.write(117);
  
}

void Lock() {
 
  myServo.write(26);
  
}
