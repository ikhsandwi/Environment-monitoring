
const int sampleWindow = 50;                              // Sample window width in mS (50 mS = 20Hz)
unsigned int sample;

 //--------------------------------------------------------------------------------------------
 //                                            SETUP
 //--------------------------------------------------------------------------------------------
 
void setup() 
{
   Serial.begin(9600);                                    //Serial comms for debugging
//   analogReadResolution(10);
}
 
//--------------------------------------------------------------------------------------------
 //                                         MAIN LOOP
 //--------------------------------------------------------------------------------------------

void loop() 
{
   unsigned long startMillis= millis();                   // Start of sample window
   float peakToPeak = 0;                                  // peak-to-peak level
 
   unsigned int signalMax = 0;                            //minimum value
   unsigned int signalMin = 1024;                         //maximum value
 
                                                          // collect data for 50 mS
   while (millis() - startMillis < sampleWindow)
   {
      sample = analogRead(A0);                             //get reading from microphone
      if (sample < 1024)                                  // toss out spurious readings
      {
         if (sample > signalMax)
         {
            signalMax = sample;                           // save just the max levels
         }
         else if (sample < signalMin)
         {
            signalMin = sample;                           // save just the min levels
         }
      }
   }
   sample = analogRead(34);  
   peakToPeak = signalMax - signalMin;                    // max - min = peak-peak amplitude
   float db1 = map(peakToPeak,20,900,44.5,90);             //calibrate for deciBels
   
//   Serial.print(sample);
//   Serial.print("\t");
   Serial.print(db1);                                     //write calibrated deciBels
   Serial.println(" dB");                                  //write units
   delay(150);
}
