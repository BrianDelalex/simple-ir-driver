// SPDX-FileCopyrightText: 2019 Limor Fried for Adafruit Industries
//
// SPDX-License-Identifier: MIT

/* Raw IR commander
 
 This sketch/program uses the Arduno and a PNA4602 to 
 decode IR received.  It then attempts to match it to a previously
 recorded IR signal.  Limor Fried, Adafruit Industries
 
 MIT License, please attribute
 check out learn.adafruit.com  for more tutorials! 
 */

// We need to use the 'raw' pin reading methods
// because timing is very important here and the digitalRead()
// procedure is slower!
//uint8_t IRpin = 2;
// Digital pin #2 is the same as Pin D2 see
// http://arduino.cc/en/Hacking/PinMapping168 for the 'raw' pin mapping
#define IRpin_PIN      PIND
#define IRpin          2

// the maximum pulse we'll listen for - 65 milliseconds is a long time
#define MAXPULSE 65000
#define NUMPULSES 50

// what our timing resolution should be, larger is better
// as its more 'precise' - but too large and you wont get
// accurate timing
#define RESOLUTION 20 

// What percent we will allow in variation to match the same code
#define FUZZINESS 20

// we will store up to 100 pulse pairs (this is -a lot-)
uint16_t pulses[NUMPULSES][2];  // pair is high and low pulse 
uint8_t currentpulse = 0; // index for pulses we're storing

#include "irsignal.h"

void setup(void) {
  Serial.begin(9600);
  Serial.println("Ready to decode IR!");
}

void loop(void) {
  int numberpulses;
  
  numberpulses = listenForIR();
  
  Serial.print("Heard ");
  Serial.print(numberpulses);
  Serial.println("-pulse long IR signal");
  if (IRcompare(numberpulses, &(VOL_MINUS_SIGNAL[0]),sizeof(VOL_MINUS_SIGNAL)/4)) {
    Serial.println("Volume - has been pressed.");
  } else if (IRcompare(numberpulses, PLAY_SIGNAL,sizeof(PLAY_SIGNAL)/4)) {
    Serial.println("Play button has been pressed.");
  } else if (IRcompare(numberpulses, VOL_PLUS_SIGNAL,sizeof(VOL_PLUS_SIGNAL)/4)) {
    Serial.println("Volume + has been pressed.");
  } else if (IRcompare(numberpulses, SETUP_SIGNAL,sizeof(SETUP_SIGNAL)/4)) {
    Serial.println("SETUP has been pressed.");
  } else if (IRcompare(numberpulses, PREVIOUS_SIGNAL,sizeof(PREVIOUS_SIGNAL)/4)) {
    Serial.println("PREVIOUS has been pressed.");
  } else if (IRcompare(numberpulses, STOP_MODE_SIGNAL,sizeof(STOP_MODE_SIGNAL)/4)) {
    Serial.println("STOP/MODE has been pressed.");
  } else if (IRcompare(numberpulses, CHNL_MINUS_SIGNAL,sizeof(CHNL_MINUS_SIGNAL)/4)) {
    Serial.println("CHANNEL - has been pressed.");
  } else if (IRcompare(numberpulses, ENTER_SIGNAL,sizeof(ENTER_SIGNAL)/4)) {
    Serial.println("ENTER has been pressed.");
  } else if (IRcompare(numberpulses, CHNL_PLUS_SIGNAL,sizeof(CHNL_PLUS_SIGNAL)/4)) {
    Serial.println("CHANNEL + has been pressed.");
  } else if (IRcompare(numberpulses, CHNL_ZERO_SIGNAL,sizeof(CHNL_ZERO_SIGNAL)/4)) {
    Serial.println("0 has been pressed.");
  } else if (IRcompare(numberpulses, NEXT_SIGNAL,sizeof(NEXT_SIGNAL)/4)) {
    Serial.println("NEXT has been pressed.");
  } else if (IRcompare(numberpulses, BACK_SIGNAL,sizeof(BACK_SIGNAL)/4)) {
    Serial.println("BACK has been pressed.");
  } else if (IRcompare(numberpulses, CHNL_ONE_SIGNAL,sizeof(CHNL_ONE_SIGNAL)/4)) {
    Serial.println("1 has been pressed.");
  } else if (IRcompare(numberpulses, CHNL_TWO_SIGNAL,sizeof(CHNL_TWO_SIGNAL)/4)) {
    Serial.println("2 has been pressed.");
  } else if (IRcompare(numberpulses, CHNL_THREE_SIGNAL,sizeof(CHNL_THREE_SIGNAL)/4)) {
    Serial.println("3 has been pressed.");
  } else if (IRcompare(numberpulses, CHNL_FOUR_SIGNAL,sizeof(CHNL_FOUR_SIGNAL)/4)) {
    Serial.println("4 has been pressed.");
  } else if (IRcompare(numberpulses, CHNL_FIVE_SIGNAL,sizeof(CHNL_FIVE_SIGNAL)/4)) {
    Serial.println("5 has been pressed.");
  } else if (IRcompare(numberpulses, CHNL_SIX_SIGNAL,sizeof(CHNL_SIX_SIGNAL)/4)) {
    Serial.println("6 has been pressed.");
  } else if (IRcompare(numberpulses, CHNL_SEVEN_SIGNAL,sizeof(CHNL_SEVEN_SIGNAL)/4)) {
    Serial.println("7 has been pressed.");
  } else if (IRcompare(numberpulses, CHNL_EIGHT_SIGNAL,sizeof(CHNL_EIGHT_SIGNAL)/4)) {
    Serial.println("8 has been pressed.");
  } else if (IRcompare(numberpulses, CHNL_NINE_SIGNAL,sizeof(CHNL_NINE_SIGNAL)/4)) {
    Serial.println("9 has been pressed.");
  } else {
    Serial.println("Unknow signal");
  }
  delay(500);
}

//KGO: added size of compare sample. Only compare the minimum of the two
boolean IRcompare(int numpulses, const int Signal[], int refsize) {
  Serial.print("Refsize: ");
  Serial.println(refsize);
  int count = min(numpulses,refsize);
  Serial.print("count set to: ");
  Serial.println(count);
  for (int i=0; i< count-1; i++) {
    int oncode = pulses[i][1] * RESOLUTION / 10;
    int offcode = pulses[i+1][0] * RESOLUTION / 10;
    int signal_on = pgm_read_word_near(Signal + (i*2));
    int signal_off = pgm_read_word_near(Signal + (i*2 + 1));
#ifdef DEBUG    
    Serial.print(i);
    Serial.print(": ");
    Serial.print(oncode); // the ON signal we heard
    Serial.print(" - ");
    Serial.print(signal_on); // the ON signal we want 
#endif   
    
    // check to make sure the error is less than FUZZINESS percent
    if ( abs(oncode - signal_on) <= (signal_on * FUZZINESS / 100)) {
#ifdef DEBUG
      Serial.print(" (ok)");
#endif
    } else {
#ifdef DEBUG
      Serial.print(" (x)");
#endif
      // we didn't match perfectly, return a false match
      return false;
    }
    
    
#ifdef DEBUG
    Serial.print("  \t"); // tab
    Serial.print(offcode); // the OFF signal we heard
    Serial.print(" - ");
    Serial.print(signal_off); // the OFF signal we want 
#endif    
    
    if ( abs(offcode - signal_off) <= (signal_off * FUZZINESS / 100)) {
#ifdef DEBUG
      Serial.print(" (ok)");
#endif
    } else {
#ifdef DEBUG
      Serial.print(" (x)");
#endif
      // we didn't match perfectly, return a false match
      return false;
    }
    
#ifdef DEBUG
    Serial.println();
#endif
  }
  // Everything matched!
  return true;
}

int listenForIR(void) {
  currentpulse = 0;
  
  while (1) {
    uint16_t highpulse, lowpulse;  // temporary storage timing
    highpulse = lowpulse = 0; // start out with no pulse length
  
//  while (digitalRead(IRpin)) { // this is too slow!
    while (IRpin_PIN & (1 << IRpin)) {
       // pin is still HIGH

       // count off another few microseconds
       highpulse++;
       delayMicroseconds(RESOLUTION);

       // If the pulse is too long, we 'timed out' - either nothing
       // was received or the code is finished, so print what
       // we've grabbed so far, and then reset
       
       // KGO: Added check for end of receive buffer
       if (((highpulse >= MAXPULSE) && (currentpulse != 0))|| currentpulse == NUMPULSES) {
         return currentpulse;
       }
    }
    // we didn't time out so lets stash the reading
    pulses[currentpulse][0] = highpulse;
  
    // same as above
    while (! (IRpin_PIN & _BV(IRpin))) {
       // pin is still LOW
       lowpulse++;
       delayMicroseconds(RESOLUTION);
        // KGO: Added check for end of receive buffer
        if (((lowpulse >= MAXPULSE)  && (currentpulse != 0))|| currentpulse == NUMPULSES) {
         return currentpulse;
       }
    }
    pulses[currentpulse][1] = lowpulse;

    // we read one high-low pulse successfully, continue!
    currentpulse++;
  }
}
void printpulses(void) {
  Serial.println("\n\r\n\rReceived: \n\rOFF \tON");
  for (uint8_t i = 0; i < currentpulse; i++) {
    Serial.print(pulses[i][0] * RESOLUTION, DEC);
    Serial.print(" usec, ");
    Serial.print(pulses[i][1] * RESOLUTION, DEC);
    Serial.println(" usec");
  }
  
  // print it in a 'array' format
  Serial.println("int IRsignal[] = {");
  Serial.println("// ON, OFF (in 10's of microseconds)");
  for (uint8_t i = 0; i < currentpulse-1; i++) {
    Serial.print("\t"); // tab
    Serial.print(pulses[i][1] * RESOLUTION / 10, DEC);
    Serial.print(", ");
    Serial.print(pulses[i+1][0] * RESOLUTION / 10, DEC);
    Serial.println(",");
  }
  Serial.print("\t"); // tab
  Serial.print(pulses[currentpulse-1][1] * RESOLUTION / 10, DEC);
  Serial.print(", 0};");
}
