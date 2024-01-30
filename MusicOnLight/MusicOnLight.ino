// It plays random music from sd card in format track000.mp3 if it's light and stoped if it's dark.

// AI Description:
// The selected code is part of a program that plays random music from an SD card when it is light and stops the music when it is dark. The code includes the following functions:

// setup(): This function initializes the SD card, sets up the MP3 player, and sets the volume.
// loop(): This function checks if the current song has finished playing and calls the nextSong() function if it has. It also checks the photocell input and calls the nextSong() function if the photocell reading indicates that it is light.
// nextSong(): This function selects a new random song from the SD card and plays it.
// photocellReading: This variable stores the current photocell reading.
// isTurnedOn: This variable indicates whether the lights are on or off.


#include <SFEMP3Shield.h>
#include <SFEMP3ShieldConfig.h>
#include <SFEMP3Shieldmainpage.h>

#include <SPI.h>
#include <SdFat.h>
#include <SFEMP3Shield.h>
#include <TrueRandom.h>


int totalTracks = 99;
int current_track = 0;
char current_track_string[32];
int prev_track = 0;

uint8_t result;
int32_t offset_ms = 30000;

const uint16_t monoMode = 1;
int photocellPin = 0; // sensor and 10 kOhm pull-down resistor connected to a0
int photocellReading;
bool isTurnedOn = true;

SdFat sd;
SFEMP3Shield MP3player;

void setup() {
  Serial.begin(9600);


  if (!sd.begin(9, SPI_HALF_SPEED)) sd.initErrorHalt();
  if (!sd.chdir("/")) sd.errorHalt("sd.chdir");

  MP3player.begin();
  MP3player.setMonoMode(monoMode);
  MP3player.setVolume(10, 10);
  Serial.println(F("Start"));
  nextSong();
}

void nextSong() {


  while (current_track == prev_track) {
    current_track = TrueRandom.random(totalTracks);
  }
  prev_track = current_track;


  sprintf(current_track_string, "%02d", current_track);
  char other_string[64] = "track0";
  char other_string_f[64] = ".mp3";


  strcat(other_string, current_track_string);
  strcat(other_string, other_string_f);


  MP3player.stopTrack();
  result = MP3player.playMP3((char *)other_string);
}


void loop() {

  // Below is only needed if not interrupt driven. Safe to remove if not using.
#if defined(USE_MP3_REFILL_MEANS) \
    && ( (USE_MP3_REFILL_MEANS == USE_MP3_SimpleTimer) \
    ||   (USE_MP3_REFILL_MEANS == USE_MP3_Polled)      )

  MP3player.available();
#endif



  if (!MP3player.isPlaying() && isTurnedOn == true) {
    Serial.println(F("Song Finished"));
    nextSong();
    delay(1000);
  }

  photocellReading = analogRead(photocellPin);


  if (photocellReading > 1000 && isTurnedOn == true) { // dark
    MP3player.stopTrack();
    isTurnedOn = false;
    Serial.println(F("Song Stoped"));
    delay(1000);
  } else if (photocellReading < 1000 && isTurnedOn == false) {
    isTurnedOn = true;
    Serial.println(F("Song Play"));
    nextSong();
    delay(1000); 
  }
}
