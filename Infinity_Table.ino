#include <FastLED.h>

#define NUM_LEDS 50
#define BUTTON_PIN 3
#define BUTTON_LIGHT_PIN 4
#define LED_DATA_PIN 2
#define POT_PIN 0

/*-------------------Prototypes--------------*/
//Button logic
int checkButton();

//Various LED functions
void potIncrease();
void everyFourth();
void pulse();
void spectrum();
void spectrumScroll();

//Functions for setting button LED
void buttonLogic();
void setButtonLed();


unsigned long timeStamp = millis();// start keeping track of time
byte menuScroll = 1;//used for switch case/function menu
boolean doubleClick = true; // toggle function of potentiometer
boolean dimButton = false; //if true, dim buttom. dims button when pressed
boolean buttonLedOverride = false; // if true, match color of function to button LED
byte buttonEvent; //recieve return value from button logic function

struct buttonRGB {
  float r;//red
  float g;//green
  float b;//blue
} buttonColor;

/*-------------------FastLED Objects--------------*/
CRGB leds[NUM_LEDS];
CRGB buttonLed[1];


void setup() {
  Serial.begin(9600);
  
  pinMode(BUTTON_PIN, INPUT);
  digitalWrite(BUTTON_PIN, HIGH);
  
  FastLED.addLeds<WS2811, LED_DATA_PIN, RGB>(leds, NUM_LEDS);
  FastLED.addLeds<WS2812B, BUTTON_LIGHT_PIN, GRB>(buttonLed, 1);
}

void loop() {
  
  buttonEvent = checkButton();//Check button

  buttonLogic();

  /*This block of code will do different things based on different button presses (click, doubleclick, hold)
  if the button is clicked once, then increment menuScroll, which is used to determine which function to use
  if the button is double clicked, then change the function of the potentiometer
  if the button is held, turn all lights off*/
  
  if (buttonEvent == 1){
      menuScroll++;
      Serial.print("menuScroll = ");
      Serial.println(menuScroll);
  }

  else if (buttonEvent == 2){
      doubleClick = !doubleClick;
  }
  
  else if (buttonEvent == 3){  
      Serial.println("hold");
      if(FastLED.getBrightness() == 0)
        FastLED.setBrightness(255);
      else if(FastLED.getBrightness() == 255)
        FastLED.setBrightness(0);
  }
  
// as menuscroll increments, go to the next function
  switch (menuScroll) {
      case 1:
        buttonLedOverride = false; //Override button colors
        spectrumScroll();
        break;
      case 2:
        buttonLedOverride = true; //Don't override button colors
        spectrum();
        break;
      case 3:
        buttonLedOverride = false; //Dont override button colors
        everyFourth();
        break;
      case 4:
        buttonLedOverride = false; //Dont override button colors
        potIncrease();
        break;
      default:
        menuScroll = 1;
      break;
    }
  
}

/*-------------------Button LED Logic--------------*/
void buttonLogic(){
  if(!buttonLedOverride){
    buttonColor.r = 0;
    buttonColor.g = 0;
    buttonColor.b = 255;
    setButtonLed(buttonColor.r,buttonColor.g,buttonColor.b,100);
  }

  // Button light response: when button is pressed down, dim the light.
  
  // if button is pressed down
  if(dimButton)
    setButtonLed(buttonColor.r,buttonColor.g,buttonColor.b,50);//dim to 50%
  else
    setButtonLed(buttonColor.r,buttonColor.g,buttonColor.b,100);//leave at full brightness
 
  FastLED.show();
}

/*-------------------LED Functions--------------*/
void everyFourth(){
  int iter = 1; //Iteration.
  int readSpeed; // use POT for speed.
  static int cycleSpeedMap = 3; //Remapped speed
  int readBrightness; // use POT for brightness.
  static float brightnessMap = 70; //Remapped brightness
  
  if(doubleClick){
    readSpeed = analogRead(POT_PIN);
    cycleSpeedMap = map(readSpeed, 0, 1023, 100, 900);
  }
  else{
    readBrightness = analogRead(POT_PIN);
    brightnessMap = map(readBrightness, 1023, 0, 0, 100);
  }
  
  if (millis()-timeStamp <= cycleSpeedMap){
    //Serial.println("1000");
   
    for (int i = 0; i < NUM_LEDS; i++){
    
      if (iter == 1)
        leds[i].setRGB(0*(brightnessMap/100),0*(brightnessMap/100),255*(brightnessMap/100));
      else if (iter == 2)
        leds[i].setRGB(85*(brightnessMap/100),0*(brightnessMap/100),170*(brightnessMap/100));
      else if (iter == 3)
        leds[i].setRGB(170*(brightnessMap/100),0*(brightnessMap/100),85*(brightnessMap/100));
      else if (iter == 4)
        leds[i].setRGB(255*(brightnessMap/100),0*(brightnessMap/100),0*(brightnessMap/100));
      iter++;
      
      if (iter > 4)
        iter = 1;
    }  
  FastLED.show();
  
  }
  
  
  if (millis()-timeStamp > cycleSpeedMap && millis()-timeStamp <= (cycleSpeedMap * 2)){
    //Serial.println("2000");
    for (int i = 0; i < NUM_LEDS; i++){
      
      if (iter == 1)
        leds[i].setRGB(255*(brightnessMap/100),0*(brightnessMap/100),0*(brightnessMap/100));
      else if (iter == 2)
        leds[i].setRGB(0*(brightnessMap/100),0*(brightnessMap/100),255*(brightnessMap/100));
      else if (iter == 3)
        leds[i].setRGB(85*(brightnessMap/100),0*(brightnessMap/100),170*(brightnessMap/100));
      else if (iter == 4)
        leds[i].setRGB(170*(brightnessMap/100),0*(brightnessMap/100),85*(brightnessMap/100));
      iter++;
      
      if (iter > 4)
        iter = 1;
    }  
  FastLED.show();     
  }
  

  if (millis()-timeStamp > (cycleSpeedMap * 2) && millis()-timeStamp <= (cycleSpeedMap * 3)){
   //Serial.println("3000");
    for (int i = 0; i < NUM_LEDS; i++){
      
      if (iter == 1)
        leds[i].setRGB(170*(brightnessMap/100),0*(brightnessMap/100),85*(brightnessMap/100));
      else if (iter == 2)
        leds[i].setRGB(255*(brightnessMap/100),0*(brightnessMap/100),0*(brightnessMap/100));
      else if (iter == 3)
        leds[i].setRGB(0*(brightnessMap/100),0*(brightnessMap/100),255*(brightnessMap/100));
      else if (iter == 4)
        leds[i].setRGB(85*(brightnessMap/100),0*(brightnessMap/100),170*(brightnessMap/100));
      iter++;
      
      if (iter > 4)
        iter = 1;
        
    }    
  FastLED.show(); 
  }

    if (millis()-timeStamp > (cycleSpeedMap * 3) && millis()-timeStamp <= (cycleSpeedMap * 4)){
   //Serial.println("3000");
    for (int i = 0; i < NUM_LEDS; i++){
      
      if (iter == 1)
        leds[i].setRGB(85*(brightnessMap/100),0*(brightnessMap/100),170*(brightnessMap/100));
      else if (iter == 2)
        leds[i].setRGB(170*(brightnessMap/100),0*(brightnessMap/100),85*(brightnessMap/100));
      else if (iter == 3)
        leds[i].setRGB(255*(brightnessMap/100),0*(brightnessMap/100),0*(brightnessMap/100));
      else if (iter == 4)
        leds[i].setRGB(0*(brightnessMap/100),0*(brightnessMap/100),255*(brightnessMap/100));
      iter++;
      
      if (iter > 4)
        iter = 1;
        
    }    
  FastLED.show(); 
  }

  if (millis()-timeStamp > (cycleSpeedMap * 4)){
    //Serial.println("4000");
    timeStamp = millis();
  }

}

void potIncrease() {
  
  int readNumLeds; //Read POT (Num Leds)
  static int numLedsMap = 10; //Remapped speed
  int readBrightness; //Read POT (brightness)
  static float brightnessMapped = 100; //Remapped brightness
  
  // use POT for number of LEDs
  if(doubleClick){
    readNumLeds = analogRead(POT_PIN);
    numLedsMap = map(readNumLeds, 1023, 0, 0, NUM_LEDS);
  }
  // use POT for brightness.
  else{
    readBrightness = analogRead(POT_PIN);
    brightnessMapped = map(readBrightness, 1023, 0, 0, 100);
  }

  // First, clear the existing led values
  FastLED.clear();
  for(int led = 0; led < numLedsMap; led++) { 
    //Blue
    leds[led].setRGB(0*(brightnessMapped/100), 0*(brightnessMapped/100), 255*(brightnessMapped/100));
  }
  FastLED.show();
}

//void pulse(){
//  float in, out;
//  
//  for (in = 0; in < 6.283; in = in + 0.001){
//    out = sin(in) * 127.5 + 127.5;
//
//    if(millis()-timeStamp >= 1){
//      timeStamp = millis();
//      
//      for (int i = 0; i < NUM_LEDS; i++){
//      leds[i].setRGB(out, out, out);
//      }
//      FastLED.show();
//     
//    }
//  }
//}

void spectrum(){
  
  static float rgbColor[3] = {255,0,0}; //RGB Array, start at red
  unsigned long interval = 50; //interval between color incrementations

  int val = analogRead(POT_PIN); //potentiometer input
  float brightness = map(val, 1023, 0, 0, 100 );//mapped value for brightness
  
  //Incrementing Variables
  static int i = 0; //0-255 value for color incrementation
  static int decColor = 0; // color to decrement
  static int incColor; // color to increment
  
  if(millis()-timeStamp >= interval){
    timeStamp = millis();
    
    // Choose the colors to increment and decrement.
    //start with rgbColor[0],rgbColor[1]
    if(decColor == 2)
      incColor = 0;
    else 
      incColor = decColor + 1;
      
    // cross-fade the two colours.
    rgbColor[decColor] -= 1;
    rgbColor[incColor] += 1;
  
  
    // Set all LEDs
    for (int i = 0; i < NUM_LEDS; i++){
      leds[i].setRGB(rgbColor[0]*(brightness/100), rgbColor[1]*(brightness/100), rgbColor[2]*(brightness/100));
    }
    
    //Show all LEDs
    FastLED.show();

    
    // set button RGB colors
    buttonColor.r = rgbColor[0];
    buttonColor.g = rgbColor[1];
    buttonColor.b = rgbColor[2];

  
    //Increment Variables
    i++;
    if(i >= 255){
      decColor++;
      i = 0; //reset i
    }
  
    //reset decColor
    if (decColor >= 3)
      decColor = 0;
  }
}


void spectrumScroll(){
  static int rgbColor[3] = {255,0,0};
  
  int val = analogRead(POT_PIN);
  int cycleSpeed = map(val, 0, 1000, 0, 255);
  static int i = 0; // keep track of everytime array elements switch (RG to GB to BR)
  
//  for (int decColor = 0; decColor < 3; decColor += 1) {
//    int incColor = decColor == 2 ? 0 : decColor + 1;
//
//    // cross-fade the two colours.
//    int k = 0; //loop counter
//
//    while(k <= 255) {
//      
//      if(k % 15 == 0 && i == 0){
//      // Set LED
//        leds[k/15].setRGB(rgbColor[0], rgbColor[1], rgbColor[2]);
//      }
//      else if(k % 15 == 0 && i == 1){
//      // Set LED
//        leds[k/15+17].setRGB(rgbColor[0], rgbColor[1], rgbColor[2]);
//      }
//      else if(k % 15 == 0 && i == 2){
//      // Set LED
//      if (k/15+34 == 50)
//        k = 225;
//        leds[k/15+34].setRGB(rgbColor[0], rgbColor[1], rgbColor[2]);
//      }
//      
//      rgbColor[decColor] -= 1;
//      rgbColor[incColor] += 1;
//
//      k++;
// 
////      if(k > 240 && i == 2) //break loop before led 51
////        break;
//        
//      if(k >= 255) //Incerement i when loop is done
//        i++;
//    }
//  }
//  Serial.println("FastLED.show()");
//  FastLED.show();

  static int decColor;
  static int incColor;
  for(int i = 765; i <= 765; i++){
    
    // Red and Green Channel
    if(i >= 0 && i <= 255){
      decColor = 0;
      incColor = 1;

      for(int i = 0; i < 255; i++){
        if(i % 15 == 0){
          // Set LED
          leds[i/15].setRGB(rgbColor[0], rgbColor[1], rgbColor[2]);
        }
        
        rgbColor[decColor] -= 1;
        rgbColor[incColor] += 1;
      }
    }
    
    // Green and Blue Channel
    else if(i > 255 && i <= 510){
      decColor = 1;
      incColor = 2;

      for(int i = 0; i < 255; i++){
        if(i % 15 == 0){
          // Set LED
          leds[i/15+17].setRGB(rgbColor[0], rgbColor[1], rgbColor[2]);
        }
        
        rgbColor[decColor] -= 1;
        rgbColor[incColor] += 1;
      }
    }
    // Blue and Red Channel
    else if(i > 510 && i <= 765){
      decColor = 2;
      incColor = 0;

      for(int i = 0; i < 255; i++){
        if(i % 15 == 0){
          // Set LED
          leds[i/15+34].setRGB(rgbColor[0], rgbColor[1], rgbColor[2]);
        }
        
        rgbColor[decColor] -= 1;
        rgbColor[incColor] += 1;
      }
    }
    
  }
  FastLED.show();
}




void setButtonLed(float r, float g, float b, float brightness){
    
    buttonLed[0].setRGB(r*(brightness/100),g*(brightness/100),b*(brightness/100));
}

// THE FOLLOWING CODE IS BORROWED FOR FUNCTIONALITY, I DID NOT WRITE THIS CODE

/*-------------------Button Event Handler--------------*/
// Button timing variables
int debounce = 20; // ms debounce period to prevent flickering when pressing or releasing the button
int DCgap = 250; // max ms between clicks for a double click event
int holdTime = 2000; // ms hold period: how long to wait for press+hold event
int longHoldTime = 5000; // ms long hold period: how long to wait for press+hold event

// Other button variables
boolean buttonVal = HIGH; // value read from button
boolean buttonLast = HIGH; // buffered value of the button's previous state
boolean DCwaiting = false; // whether we're waiting for a double click (down)
boolean DConUp = false; // whether to register a double click on next release, or whether to wait and click
boolean singleOK = true; // whether it's OK to do a single click
long downTime = -1; // time the button was pressed down
long upTime = -1; // time the button was released
boolean ignoreUp = false; // whether to ignore the button release because the click+hold was triggered
boolean waitForUp = false; // when held, whether to wait for the up event
boolean holdEventPast = false; // whether or not the hold event happened already
boolean longHoldEventPast = false;// whether or not the long hold event happened already

int checkButton(){ 
  int event = 0;
  // Read the state of the button
  buttonVal = digitalRead(BUTTON_PIN);
  // Button pressed down
  if (buttonVal == LOW && buttonLast == HIGH && (millis() - upTime) > debounce) {
    downTime = millis();
    ignoreUp = false;
    waitForUp = false;
    singleOK = true;
    holdEventPast = false;
    longHoldEventPast = false;
    dimButton = true; // dim button led when pressed down
    if ((millis()-upTime) < DCgap && DConUp == false && DCwaiting == true) 
      DConUp = true;
    else 
      DConUp = false;
    DCwaiting = false;
  }
  // Button released
  else if (buttonVal == HIGH && buttonLast == LOW && (millis() - downTime) > debounce) {
    dimButton = false; // undim button led on release
    if (not ignoreUp) {
    upTime = millis();
      if (DConUp == false) 
        DCwaiting = true;
      else {
        event = 2;
        DConUp = false;
        DCwaiting = false;
        singleOK = false;
      }
    }
  }
  // Test for normal click event: DCgap expired
  if ( buttonVal == HIGH && (millis()-upTime) >= DCgap && DCwaiting == true && DConUp == false && singleOK == true) {
    event = 1;
    DCwaiting = false;
  }
  // Test for hold
  if (buttonVal == LOW && (millis() - downTime) >= holdTime) {
  // Trigger "normal" hold
    if (not holdEventPast) {
      event = 3;
      waitForUp = true;
      ignoreUp = true;
      DConUp = false;
      DCwaiting = false;
      //downTime = millis();
      holdEventPast = true;
    }
    // Trigger "long" hold
    if ((millis() - downTime) >= longHoldTime) {
      if (not longHoldEventPast) {
        event = 4;
        longHoldEventPast = true;
      }
    }
  }
  buttonLast = buttonVal;
  return event;
}
