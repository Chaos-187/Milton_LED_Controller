#include "FastLED.h"

FASTLED_USING_NAMESPACE

// Author : Kyle Harris
// Date Created : 24/02/2018
// Description : Test App for LED's in the milton
// Version : 1.00
// Enviroment : Dev


#define DATA_PIN    3
#define BTN1_PIN  6
#define BTN2_PIN  7
int BTN1_val = 0;
int BTN1_lastButtonState = HIGH;   // the previous reading from the input pin


int BTN2_val = 0;
int BTN2_lastButtonState = HIGH;   // the previous reading from the input pin





//#define CLK_PIN   4
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS    150
CRGB leds[NUM_LEDS];

#define BRIGHTNESS          255
#define FRAMES_PER_SECOND  120

void setup() {
  //Start serial comms for debug
  Serial.begin(57600);
  Serial.println("Controller Version V1.00 Started ");
  pinMode(BTN1_PIN, INPUT_PULLUP);
  pinMode(BTN2_PIN, INPUT_PULLUP);
  delay(3000); // 3 second delay for recovery
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  //FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);

}

// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = { rainbow, rainbowWithGlitter, confetti, sinelon, juggle, bpm };

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns

void loop() {
 //READ BUTTON1
 
  BTN1_val = digitalRead(BTN1_PIN);     // read the input pin
 // Serial.print("Value 1 = ");
 // Serial.println(BTN1_val);
  
  
  //READ BUTTON2
  BTN2_val = digitalRead(BTN2_PIN);     // read the input pin
  //Serial.print("Value 2 = ");
 // Serial.println(BTN2_val);
 // delay(200);
  
  
  // put your main code here, to run repeatedly:
 // Call the current pattern function once, updating the 'leds' array
  gPatterns[gCurrentPatternNumber]();

  // send the 'leds' array out to the actual LED strip
  FastLED.show();  
  // insert a delay to keep the framerate modest
  FastLED.delay(1000/FRAMES_PER_SECOND); 

  // do some periodic updates
  EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow
  EVERY_N_SECONDS( 10 ) { nextPattern(); } // change patterns periodically


//######################SPECIAL CODE
  
if (BTN1_lastButtonState == BTN1_val){
  //DO NOTHING
}else
{
  //Serial.print("Value 2 = ");
  
  nextPattern();
  
}
BTN1_lastButtonState = BTN1_val;

if (BTN2_lastButtonState == BTN2_val){
  //DO NOTHING
}else
{
  //Serial.print("Value 2 = ");
  
  FastLED.clear();
  
}
BTN2_lastButtonState = BTN2_val;


//######################SPECIAL CODE

 
}

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void serialEvent() {

  if (Serial.available() > 0) {
    // read the incoming byte:
    //incomingByte = Serial.read();
    char myChar = Serial.read();
    int myString(myChar);
    // say what you got:
    //lcd.setCursor(0, 0);
    //lcd.print("receiveing data: ");
    //lcd.setCursor(0, 1);
    //lcd.print(myString);
    
    switch (myString) {
      case 48:
      Serial.println("You Pressed 0 ");
       
      
      break;
      
      case 49:
      Serial.println("You Pressed 1");
       
      break;
      
      case 50:
      Serial.println("You Pressed 2");
      ;
      break;
      
      case 51:
      Serial.println("You Pressed 3");
      break;
      
      case 52:
      Serial.println("You Pressed 4");
      break;
      
      //DEFAULT IF NONE MATCHED
      default:
      Serial.println("FUCK OFF");
      break;
    }
Serial.flush();
  }
  }

void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
}


//DIFFERENT FUNCTIONS FROM FASTLED EXAMPLE REAL

void rainbow() 
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, gHue, 7);
}

void rainbowWithGlitter() 
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(80);
}

void addGlitter( fract8 chanceOfGlitter) 
{
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}

void confetti() 
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);
}

void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = beatsin16( 13, 0, NUM_LEDS-1 );
  leds[pos] += CHSV( gHue, 255, 192);
}

void bpm()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
}

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[beatsin16( i+7, 0, NUM_LEDS-1 )] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
  }

 
