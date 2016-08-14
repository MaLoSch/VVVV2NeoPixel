//based on a sketch and patch by colorsound

#include <FastLED.h> //implement FastLED lib


//////////////////////////////////////////////////////////////////////////////////Set these values
#define NUM_LEDS 12 //number of LEDs
#define DATA_PIN 9 //Pin the RGB LED strip is attached to
const int ValuesPerLED = 3;
const int packetBufferSize = 38;
//////////////////////////////////////////////////////////////////////////////////Set these values


CRGB leds[NUM_LEDS]; //Array for RGB LEDs
int LEDValues[NUM_LEDS][ValuesPerLED]; //Two dimensional array to store RGB values for every LED
char packetBuffer[packetBufferSize]; //Buffer to store incoming string from vvvv - (Num_LED * 3) + 2

void setup()   
{
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS); //initiate RGB LED strip
  Serial.begin(9600); //start Serial Communication
}
void loop() {
  if (Serial.available() > 0) //Check if data is coming in through serial
  {
    Serial.readBytes(packetBuffer,packetBufferSize);// 182 is the packetsize = 180 values + '<' and '>'
    if( packetBuffer[0]=='<' && packetBuffer[packetBufferSize-1]=='>') //if first and last chars of string are '<' and '>' we know the whole package has arrived
    {
      unsigned char CleanBuffer [packetBufferSize-2]; //new Array to store string without starting and closing tag
      memcpy(CleanBuffer,packetBuffer+1,packetBufferSize-2); //copies the content from packetBuffer to CleanBuffer excluding the first and last element
      
      int index=0; //helper to point to a specific data point
      
      for(int i=0; i<NUM_LEDS; i++) //nested for loop to run through two dimensional array
      {
        for(int j=0; j<3; j++)
        {
          LEDValues[i][j] = CleanBuffer[index]; //store values of CleanBuffer in 2D Array
          index++; //increase index to get next data point in next run through the loop
        }
      }
    }
  }
  
  for(int i=0; i<NUM_LEDS; i++) //for loop to push values to LEDs
  {
    leds[i] = CRGB(LEDValues[i][0],LEDValues[i][1],LEDValues[i][2]);
  }
  
  FastLED.show(); //updated RGB LED strip
  delay(5);
}
