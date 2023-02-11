#include <TVout.h>
#include <font8x8.h>
#include <font6x8.h>

#include <Arduino.h>

#include <PS2Keyboard.h>


const int DataPin = 4;
const int IRQpin =  3;

PS2Keyboard keyboard;

float delVal = 14;
TVout tv;

char screen[] = {'M', 'M', 'o', 'O', 'S'};

char cursOvr[3] = {' ', ' ', ' '};

const int countRMax = 31;
int countR = -1;
double prevMillis = 0;
boolean cursorOn = false;
int cursorPos = 0;

void setup(){
  delay(1000);

  keyboard.begin(DataPin, IRQpin);
  Serial.begin(115200);
  Serial.println("Keyboard test:");  

  // if(screen == char('screen')){
  //   Serial.println("it's equal");
  // }


  delay(1000);

  tv.begin(NTSC,120,96);
  tv.select_font(font6x8);
}


void loop(){
  tv.println(millis()/1000);

 if (keyboard.available()) {
   
    // read the next key
    char c = keyboard.read();
    
    // check for some of the special keys
    if (c == PS2_ENTER) {
      Serial.println();
      for (size_t i = 0; i < countRMax+5; i++)
      {
        screen[i] = ' ';
      }
      countR = -1;
    
    } else if (c == PS2_TAB) {

      Serial.print("[Tab]");
      
    } else if (c == PS2_ESC) {
      Serial.print("[ESC]");

    } else if (c == PS2_PAGEDOWN) {
      Serial.print("[PgDn]");
    } else if (c == PS2_PAGEUP) {
      Serial.print("[PgUp]");
    } else if (c == PS2_LEFTARROW) {
      Serial.print("[Left]");

      if(countR != 0){
        cursOvr[0] = screen[countR];
        for (size_t i = 0; i < 3; i++)
        {
          cursOvr[i] = i-1;
        }
            countR--;
      }

    } else if (c == PS2_RIGHTARROW) {
      Serial.print("[Right]");

      cursOvr[3] = screen[countR];
      if(countR < countRMax){
        for (size_t i = 0; i < 3; i++)
        {
          cursOvr[i] = i-1;
        }
        
        countR++;
      }

    } else if (c == PS2_UPARROW) {
      Serial.print("[Up]");
    } else if (c == PS2_DOWNARROW) {
      Serial.print("[Down]");
    } else if (c == PS2_DELETE) {

      if(countR > -1){
        screen[countR] = ' ';
        screen[countR+1] = ' ';
        countR--;
      }
      
      Serial.print("[Del]");

    } else {

      if(countR == countRMax){
        countR = 0;
      }
      else {
        countR++;
      }

      // otherwise, just print all normal characters
      screen[countR] = c;
      Serial.print(c); Serial.print(" ");
      
    }
    Serial.println(countR);
  }

  if(millis() - prevMillis > 350){
    cursorOn = !cursorOn;
    prevMillis = millis();
  }
  if(cursorOn == true){
    screen[countR+1] = '<';
  } else if(cursorOn == false){
    screen[countR+1] = '~';
  }
  
  

  tv.println(screen);

  tv.delay_frame(2);
  tv.clear_screen();
  
}
