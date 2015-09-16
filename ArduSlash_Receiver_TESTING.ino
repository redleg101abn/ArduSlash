/* ArduSlash Receiver
 *  
 *  By Bill Graver
 *  
 *  Inspired by the idea and code of Dr. Peter Dalmaris of Tech Explorations
 *  https://github.com/futureshocked/arduauto/blob/master/bare_receiver/bare_receiver.ino
 *  
 *  Code uses the RF24 libary from maniacbug
 *  https://github.com/maniacbug/RF24
 *  
 */

#include <SPI.h>   //SPI library, which enables us to use the Nunchuk
#include "nRF24L01.h"   //
#include "RF24.h"

class RF24Test: public RF24
{
public: RF24Test(int a, int b): RF24(a,b) {}
};

RF24Test radio(9,10);

// Radio pipe addresses for the 2 nodes to communicate.
const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };

void setup()
{
  Serial.begin(57600);
  
  radio.begin();
  
  radio.openWritingPipe(pipes[1]);
  radio.openReadingPipe(1,pipes[0]);
  
  radio.startListening();
  Serial.println("Listening...");
}

void loop()
{
    // if there is data ready
    if ( radio.available() )
     {
      byte transmission[7];
      bool done = false;
      while (!done)
      {
        // Fetch the payload, and see if this was the last one.
        done = radio.read( &transmission, 7 );

        // Spew it
        Serial.print("Analog X axis is: "); Serial.println(transmission[0]);
        Serial.print("Analog Y axis is: "); Serial.println(transmission[1]);
        Serial.print("Accelerometer X is: "); Serial.println(transmission[2]);
        Serial.print("Accelerometer Y is: "); Serial.println(transmission[3]);
        Serial.print("Accelerometer Z is: "); Serial.println(transmission[4]);
        Serial.print("Z Button is: "); Serial.println(transmission[5]);
        Serial.print("C Button is: "); Serial.println(transmission[6]);
        //Serial.print(" ");
        //Serial.print(transmission[1]);
        //Serial.print(" ");
        //Serial.print(transmission[2]);
        //Serial.print(" ");
        //Serial.print(transmission[3]);
        //Serial.print(" ");
        //Serial.print(transmission[4]);
        //Serial.print(" ");
        //Serial.print(transmission[5]);
        //Serial.print(" ");
        //Serial.print(transmission[6]);
        //Serial.print("   ");

   // Delay just a little bit to let the other unit
   // make the transition to receiver
   delay(20);
      }

      // First, stop listening so we can talk
      radio.stopListening();

      // Send the final one back.
      byte response = B0;
      radio.write( &response, sizeof(response) );
      //Serial.println("Sent response.\n\r");

      // Now, resume listening so we catch the next packets.
      radio.startListening();

    }  
}
