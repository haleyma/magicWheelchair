//code from https://gist.github.com/andijcr/90ee99f938298bec15ed

#include <Adafruit_NeoPixel.h>

const int NUM_LEDS = 16;           // number of leds in strip
const int LED_PIN = 4;             // pin for led strip
#define MAX_P 20

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

int li=0;
int direction=+1;

uint32_t Red=strip.Color(255, 0, 0);
uint32_t Blank=strip.Color(0, 0, 0);
void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  //decay
  for(uint16_t i=0; i<MAX_P; i++) {
    uint8_t r= (strip.getPixelColor(i)>>16) & 0xff;
    r=(r*3)/5;
      strip.setPixelColor(i, r, 0, 0);
  }

  strip.setPixelColor(li, Red);
  if(li>(MAX_P-1)){
    li=MAX_P;
    direction=-1;
  }else if(li<0){
    li=-1;
    direction=1;
  }
  li+=direction;
  strip.show();
  delay(10);
}
