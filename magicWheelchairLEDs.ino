#include <WS2812FX.h>
#include <EEPROM.h>

#define LED_COUNT 16
#define LED_PIN 4

#define TIMER_MS 5000
#define wheelColor

//rotary encoder stuff
#define outputA 6
#define outputB 7
int counter = 0; 
int aState;
int aLastState;
int new_mode = 0; 
int NUM_MODES = 5;
int address = 0;

// constants won't change. They're used here to set pin numbers:
const int buttonPin = 2;     // the number of the pushbutton pin
const int ledPin =  13;      // the number of the LED pin

// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status
int led_mode;
//Modes: 2-Breath, 3-Colorwipe, 14-Dual Scan, 19-Twinkle, 43-Larson Scanner, 52-Halloween)
//int led_mode = [FX_MODE_BREATH                   2,
                  //FX_MODE_COLOR_WIPE               3
                 // ];
                 
int modeCounter = 0;
//int max;


// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
WS2812FX ws2812fx = WS2812FX(LED_COUNT, LED_PIN, NEO_RGB + NEO_KHZ800);

unsigned long last_change = 0;
unsigned long now = 0;

void Update(int lmode){
  byte Wheelpos = EEPROM.read(lmode);
  Serial.println(Wheelpos);
  //pass param to colorwheel and retrieve COLOR
  //Wheel(Wheelpos);  
  ws2812fx.setColor(Wheel(Wheelpos));
      switch(lmode) {
        case 1:
        //do something Breath mode
        //ws2812fx.setMode((ws2812fx.getMode(FX_MODE_BREATH)));
        //ws2812fx.setColor(wheelColor);        
        ws2812fx.setMode(2);
        
        break;
    
        case 2:
        //do something twinkle mode
        //ws2812fx.setMode((ws2812fx.getMode(FX_MODE_TWINKLE)));
        //ws2812fx.setMode(FX_MODE_TWINKLE);
        ws2812fx.setColor(YELLOW);
        ws2812fx.setMode(19);
        break;
        
        case 3:
        //do something  Theater chase
        //ws2812fx.setMode((ws2812fx.getMode(FX_MODE_THEATER_CHASE)));
        //ws2812fx.setMode(FX_MODE_THEATER_CHASE);
        ws2812fx.setColor(MAGENTA);
        ws2812fx.setMode(16);
        break;
        
        case 4:
        //do something  larson scanner mode
        //ws2812fx.setMode((ws2812fx.getMode(FX_MODE_LARSON_SCANNER)));
        //ws2812fx.setMode(FX_MODE_LARSON_SCANNER);
        ws2812fx.setColor(GREEN);
        ws2812fx.setMode(43);
        break;
        
        case 5:
        //do something halloween mode
        //ws2812fx.setMode((ws2812fx.getMode(FX_MODE_HALLOWEEN)));
        //ws2812fx.setMode(FX_MODE_HALLOWEEN);
        ws2812fx.setColor(ORANGE);
        ws2812fx.setMode(52);
        break;
        
        default:
        //ws2812fx.setMode((ws2812fx.getMode(FX_MODE_RUNNING_LIGHTS)));
        //ws2812fx.setMode(FX_MODE_RUNNING_LIGHTS);
        ws2812fx.setMode(7);
      }
        
      }

void setup() {
  Serial.begin(115200);
  ws2812fx.init();
  ws2812fx.setBrightness(255);
  ws2812fx.setSpeed(1000);
  ws2812fx.setColor(0x007BFF);
  ws2812fx.setMode(FX_MODE_STATIC);
  ws2812fx.start();
    // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT_PULLUP);
     pinMode (outputA,INPUT_PULLUP);
   pinMode (outputB,INPUT_PULLUP);
   
   //Serial.begin (9600);
   // Reads the initial state of the outputA
   aLastState = digitalRead(outputA);
   new_mode == 0;
}

void loop() {
  now = millis();

  ws2812fx.service();

    // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);

    // check if the pushbutton is pressed. If it is, the buttonState is HIGH:(increment count by one)
  if (buttonState == LOW) {
    // turn LED on:
    //digitalWrite(ledPin, HIGH);
    if (new_mode == 0){
//      modeCounter ++;
      led_mode ++;
      if (led_mode > NUM_MODES) led_mode = 0;
      //last_change = now;
      new_mode = 1;
      Serial.println(led_mode);
      Update(led_mode);  
    }

  }else{
   new_mode = 0;  
  }





    
  

    //ENCODER STUFF follows

   aState = digitalRead(outputA); // Reads the "current" state of the outputA
   // If the previous and the current state of the outputA are different, that means a Pulse has occured
   if (aState != aLastState){     
     // If the outputB state is different to the outputA state, that means the encoder is rotating clockwise
     if (digitalRead(outputB) != aState) { 
       counter ++;
       EEPROM.update(address, counter);
       address = address + 1;
       if (address == EEPROM.length()) {
           address = 0;
       }
     } else {
       counter --;
       EEPROM.update(address, counter);
       address = address + 1;
       if (address == EEPROM.length()) {
            address = 0;
       }
     }
     Serial.print("Position: ");
     Serial.println(counter);
   } 
   aLastState = aState; // Updates the previous state of the outputA with the current state
   //else {
    // turn LED off:
   // digitalWrite(ledPin, LOW);
  //}

  //if(now - last_change > TIMER_MS) {
   // ws2812fx.setMode((ws2812fx.getMode() + 1) % ws2812fx.getModeCount());
    //last_change = now;
  //}

  //COLORWHEEL STUFF FOLLOWS

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    wheelcolor =(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    wheelColor =(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  wheelColor = (WheelPos * 3, 255 - WheelPos * 3, 0);
}
}


//EEPROM STUFF FOLLOWS



  /***
    Update the particular EEPROM cell.
    these values will remain there when the board is
    turned off.
  ***/
 // EEPROM.update(address, val);

  /***
    The function EEPROM.update(address, val) is equivalent to the following:

    if( EEPROM.read(address) != val ){
      EEPROM.write(address, val);
    }
  ***/


  /***
    Advance to the next address, when at the end restart at the beginning.

    Larger AVR processors have larger EEPROM sizes, E.g:
    - Arduno Duemilanove: 512b EEPROM storage.
    - Arduino Uno:        1kb EEPROM storage.
    - Arduino Mega:       4kb EEPROM storage.

    Rather than hard-coding the length, you should use the pre-provided length function.
    This will make your code portable to all AVR processors.
  ***/
  //address = address + 1;
  //if (address == EEPROM.length()) {
    //address = 0;
  //}

  /***
    As the EEPROM sizes are powers of two, wrapping (preventing overflow) of an
    EEPROM address is also doable by a bitwise and of the length - 1.

    ++address &= EEPROM.length() - 1;
  ***/

  //delay(100);
//}


 

*/ 
