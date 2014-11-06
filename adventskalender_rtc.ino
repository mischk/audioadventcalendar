// Audio Advent Calendar 
// (c) by Mischka 2011 (ebert.micha@web.de)
// Licensed under Creative Commons-Attribution-Share Alike
// feel free to optimize and modify the code, please share your
// mods on the internet and send me a mail with the link

// rtc hardware setup
// scl --> A5
// sda --> A4
// vcc --> A3
// gnd --> A2




#include <EEPROM.h>

#include <FatReader.h>
#include <SdReader.h>
#include <avr/pgmspace.h>
#include "WaveUtil.h"
#include "WaveHC.h"
#include <Wire.h>
#include "RTClib.h"


SdReader card;    // This object holds the information for the card
FatVolume vol;    // This holds the information for the partition on the card
FatReader root;   // This holds the information for the filesystem on the card
FatReader f;      // This holds the information for the file we're play

WaveHC wave;      // This is the only wave (audio) object, since we will only play one at a time

RTC_DS1307 rtc;   // rtc object

#define DEBOUNCE 100  // button debouncer

// int day = EEPROM.read(0)+1;              //reads the day from EEPROM, counts +1
char *fileName[]= {   //filenames
         "dummy",     //dummy for "day 0"
         "1.wav",     //filename for day 1 ...
         "2.wav",     //
         "3.wav",     //24 audio tracks must be stored on the sd-card in this way
         "4.wav",     //you can change filenames if you prefer
         "5.wav",
         "6.wav",
         "7.wav",
         "8.wav",
         "9.wav",
         "10.wav",
         "11.wav",
         "12.wav",
         "13.wav",
         "14.wav",
         "15.wav",
         "16.wav",
         "17.wav",
         "18.wav",
         "19.wav",
         "20.wav",
         "21.wav",
         "22.wav",
         "23.wav",
         "24.wav"
       };
int button = 8 ;        // button at pin 8


// this handy function will return the number of bytes currently free in RAM, great for debugging!   
int freeRam(void)
{
  extern int  __bss_end; 
  extern int  *__brkval; 
  int free_memory; 
  if((int)__brkval == 0) {
    free_memory = ((int)&free_memory) - ((int)&__bss_end); 
  }
  else {
    free_memory = ((int)&free_memory) - ((int)__brkval); 
  }
  return free_memory; 
} 

void sdErrorCheck(void)
{
  if (!card.errorCode()) return;
  putstring("\n\rSD I/O error: ");
  Serial.print(card.errorCode(), HEX);
  putstring(", ");
  Serial.println(card.errorData(), HEX);
  while(1);
}

void setup() {
  
  
  digitalWrite(button, HIGH);        // set pull-up resistor for button pin
  
  
  
  
  // set up serial port
  Serial.begin(9600);
  putstring_nl("WAV Advent Calendar");
  
   putstring("Free RAM: ");       // This can help with debugging, running out of RAM is bad
  Serial.println(freeRam());      // if this is under 150 bytes it may spell trouble!
  
  // Set the output pins for the DAC control. This pins are defined in the library
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
 
  // pin 9 LED
  pinMode(9, OUTPUT);
 
  // power for the rtc
  
  pinMode(A3, OUTPUT);
  pinMode(A2, OUTPUT);
  digitalWrite(A3, HIGH);
  digitalWrite(A2, LOW);
  
  // start rtc
  Wire.begin();
  rtc.begin();
 
  //  if (!card.init(true)) { //play with 4 MHz spi if 8MHz isn't working for you
  if (!card.init()) {         //play with 8 MHz spi (default faster!)  
    putstring_nl("Card init. failed!");  // Something went wrong, lets print out why
    sdErrorCheck();
    while(1);                            // then 'halt' - do nothing!
  }
  
  // enable optimize read - some cards may timeout. Disable if you're having problems
  card.partialBlockRead(true);
 
// Now we will look for a FAT partition!
  uint8_t part;
  for (part = 0; part < 5; part++) {     // we have up to 5 slots to look in
    if (vol.init(card, part)) 
      break;                             // we found one, lets bail
  }
  if (part == 5) {                       // if we ended up not finding one  :(
    putstring_nl("No valid FAT partition!");
    sdErrorCheck();      // Something went wrong, lets print out why
    while(1);                            // then 'halt' - do nothing!
  }
  
  // Lets tell the user about what we found
  putstring("Using partition ");
  Serial.print(part, DEC);
  putstring(", type is FAT");
  Serial.println(vol.fatType(),DEC);     // FAT16 or FAT32?
  
  // Try to open the root directory
  if (!root.openRoot(vol)) {
    putstring_nl("Can't open root dir!"); // Something went wrong,
    while(1);                             // then 'halt' - do nothing!
  }
  
  // Whew! We got past the tough parts.
  putstring_nl("Ready!");
}

void loop() {
  
      DateTime now = rtc.now();
      if (now.day() < 25){
        if (now.month() == 12){
          
        Serial.println(now.day());                  //tells the day via serial out
        Serial.print("playing track ");
        Serial.println(fileName[now.day()]);
        
        digitalWrite(9, HIGH);              //turn on LEDs

        playcomplete(fileName[now.day()]);        //play wav file
      
        digitalWrite(9, LOW);               //turn off LEDs
        }
      }
      
      else{
        Serial.println("not an advent day");
 
      }
      
      while (digitalRead(button)== HIGH){ //if you like to play it again, push the button!
        
      }
      
   
  }




// Plays a full file from beginning to end with no pause.
void playcomplete(char *name) {
  // call our helper to find and play this name
  playfile(name);
  while (wave.isplaying) {
  // do nothing while its playing
  }
  // now its done playing
}

void playfile(char *name) {
  // see if the wave object is currently doing something
  if (wave.isplaying) {// already playing something, so stop it!
    wave.stop(); // stop it
  }
  // look in the root directory and open the file
  if (!f.open(root, name)) {
    putstring("Couldn't open file "); Serial.print(name); return;
  }
  // OK read the file and turn it into a wave object
  if (!wave.create(f)) {
    putstring_nl("Not a valid WAV"); return;
  }
  
  // ok time to play! start playback
  wave.play();
}

