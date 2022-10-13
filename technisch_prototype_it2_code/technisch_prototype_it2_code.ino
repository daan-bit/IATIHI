#include <Wire.h>
#include <LiquidCrystal_I2C.h>

//SCL A5
//SDA A4

const int onOffButton = 2;
const int okButton = 3;
const int startButton = 4;
const int returnButton = 5;
const int klokButton = 6;
const int speaker = 8;

const int modusSelecter = A2;
const int multiSelecter = A7;

int ingesteldeGraden = 0;
int ingesteldeTijd = 0;
boolean terug = false;
boolean terugNaarGraden = false;

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);


void setup() {
  Serial.begin(9600);
  
  //Led display
  lcd.begin(16,2);
  lcd.clear();
  lcd.print("OFF");

  //Buttons
  pinMode(onOffButton, INPUT_PULLUP);
  pinMode(okButton, INPUT_PULLUP);
  pinMode(startButton, INPUT_PULLUP);
  pinMode(returnButton, INPUT_PULLUP);
  pinMode(klokButton, INPUT_PULLUP);
  pinMode(speaker, OUTPUT);
}

void loop() {
  if(!digitalRead(onOffButton)){
    selecteerModus();   
  }
}

void selecteerModus(){
  while(true){
    int modusValue = analogRead(modusSelecter);
    if(modusValue <= 20 && modusValue > 0){
      lcd.clear();
      lcd.print("selecteer een");
      lcd.setCursor(0,1);
      lcd.print("modus en klik OK");
      lcd.setCursor(0,0);
    }
    else if(modusValue <= 250 && modusValue > 20){ 
      lcd.clear();
      lcd.print("Onderverwarming");
      lcd.setCursor(0,1);
      lcd.print("klik OK");
      lcd.setCursor(0,0);
    }else if(modusValue <= 500  && modusValue > 250){
      lcd.clear();
      lcd.print("Bovenverwarming");
      lcd.setCursor(0,1);
      lcd.print("klik OK");
      lcd.setCursor(0,0);
    }else if(modusValue <= 750 && modusValue > 500){
      lcd.clear();
      lcd.print("B + O verwarming");
      lcd.setCursor(0,1);
      lcd.print("klik OK");
      lcd.setCursor(0,0);
    }else if(modusValue <= 1023 && modusValue > 750){
      lcd.clear();
      lcd.print("Hetelucht");
      lcd.setCursor(0,1);
      lcd.print("klik OK");
      lcd.setCursor(0,0);
    }
    if(!digitalRead(okButton)){
      break;  
    }     
  }
  selecteerGraden(); 
}

void selecteerGraden(){
        
      while(true){
        int multiValue = analogRead(multiSelecter);
        lcd.clear();
        lcd.setCursor(0,0);
        ingesteldeGraden = map(multiValue, 0, 1023, 250, 0);
        lcd.print(ingesteldeGraden);
        lcd.print(" graden");
        lcd.setCursor(0,1);
        lcd.print("klik START");
        
              
        if(!digitalRead(startButton)){
          break;
        }

        if(!digitalRead(returnButton)){
          terug = true;
          break;  
        }       
      }
      if(terug){
        terug = false;
        selecteerModus();
      }else{
        for(int i = 0; i <= ingesteldeGraden; i++){
          lcd.clear();
          lcd.print("verwarm");
          lcd.setCursor(8,0);
          lcd.print(i);
          lcd.setCursor(0,0);
          delay(10);
         }
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Oven");
        lcd.setCursor(5,0);
        lcd.print(ingesteldeGraden);
        lcd.setCursor(0,0);
      }
      selecteerTijd();
      
}

void selecteerTijd(){
   while(true){
        if(!digitalRead(klokButton)){
          break;
        }
        
        if(!digitalRead(returnButton)){
          terugNaarGraden = true;
          delay(100);
          break;  
        } 
      }
      
   if(terugNaarGraden){
      terugNaarGraden = false;
      selecteerGraden();
   }else{
      lcd.clear();
      lcd.print("Zet multiknop");
      lcd.setCursor(0,1);
      lcd.print("op 0");
      while(true){
        int multiValue = analogRead(multiSelecter);
        if(multiValue >= 1015){
          while(true){
            lcd.clear();
            int multiValue = analogRead(multiSelecter);
            ingesteldeTijd = map(multiValue, 0, 1015, 20, 0);
            lcd.print(ingesteldeTijd);
            lcd.setCursor(3,0);
            lcd.print("min");        
            lcd.setCursor(0,1);
            lcd.print("Klik OK");

            if(!digitalRead(okButton)){
              break;
            }
          }
          break;          
        }
      }
      for(int i = ingesteldeTijd; i >= 0; i--){
        lcd.clear();
        lcd.print("Timer:");
        lcd.setCursor(6,0);
        lcd.print(i);
        delay(1000);
        lcd.setCursor(0,0);
      }

      for(int i = 2; i >= 0; i--){
        digitalWrite(speaker, HIGH);
        delay(500);
        digitalWrite(speaker, LOW);
        delay(500);
      }
        
      while(true){
        if(!digitalRead(onOffButton)){
          lcd.clear();
          lcd.print("OFF");
          delay(200);
          loop();
        }
      }
      
  }
}
 
