
/* ******************************************
 *  
 * Script to set the logger clock
 *
 * Adaptation of the "DS3231_alarms.ino" example of the "Rtc by Makuna" library
 * 
 * Wouter Buytaert, Riverlabs
 * 
 * Important notes: 
 *  - Only run the script once, and do not restart the logger or the clock will be set again
 *  - Open the serial terminal before uploading, because starting the terminal in Arduino resets the CPU
 *  - If you want the clock to run in GMT/UTC, adjust TZ (required for telemetry)
 *  
 ********************************************/
 
#define TZ 1                                // time zone offset from UTC in hours


#include <Wire.h>
#include <RtcDS3231.h> 
#include <avr/io.h>
#include <avr/interrupt.h>
RtcDS3231<TwoWire> MyRtc(Wire);

#define RtcSquareWavePin 2 
//#define RtcSquareWaveInterrupt 0


#if defined(_SAMD21_)                       // SAMD21 boards (sparkfun mini at least) use SerialUSB for the Serial Monitor
    #define Serial SerialUSB
#endif


volatile uint16_t interruptCount = 0;
volatile bool interruptFlag = false;

void InterruptServiceRoutine()
{
    interruptCount++;
    interruptFlag = true;
}

void setup () 
{
    Serial.begin(115200);

    // set the interupt pin to input mode
    pinMode(RtcSquareWavePin, INPUT);
    
    MyRtc.Begin();

    RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__) - TZ * 3600 - -11;
    RtcDateTime now = MyRtc.GetDateTime();
    MyRtc.SetDateTime(compiled);
    
    MyRtc.Enable32kHzPin(false);
    MyRtc.SetSquareWavePin(DS3231SquareWavePin_ModeAlarmBoth); 

    // Alarm 2 set to trigger at the top of the minute
    DS3231AlarmTwo alarm2(
            0,
            0,
            0, 
            DS3231AlarmTwoControl_OncePerMinute);
    MyRtc.SetAlarmTwo(alarm2);

    // throw away any old alarm state before we ran
    MyRtc.LatchAlarmsTriggeredFlags();

    // setup external interupt 
    attachInterrupt(digitalPinToInterrupt(RtcSquareWavePin), InterruptServiceRoutine, FALLING);
}

void loop () 
{

    RtcDateTime now = MyRtc.GetDateTime();

    printDateTime(now);
    Serial.println();

    // we only want to show time every 10 seconds
    // but we want to show response to the interupt firing
    for (int timeCount = 0; timeCount < 20; timeCount++)
    {
        if (Alarmed())
        {
            Serial.print(">>Interrupt Count: ");
            Serial.print(interruptCount);
            Serial.println("<<");
        }
        delay(500);
    }
}

bool Alarmed()
{
    bool wasAlarmed = false;
    if (interruptFlag)  // check our flag that gets sets in the interupt
    {
        wasAlarmed = true;
        cli();                     // see https://www.pjrc.com/teensy/interrupts.html
        interruptFlag = false;     // reset the flag
        sei();
        
        // this gives us which alarms triggered and
        // then allows for others to trigger again
        DS3231AlarmFlag flag = MyRtc.LatchAlarmsTriggeredFlags();

        if (flag & DS3231AlarmFlag_Alarm1)
        {
            Serial.println("alarm one triggered");
        }
        if (flag & DS3231AlarmFlag_Alarm2)
        {
            Serial.println("alarm two triggered");
        }
    }
    return wasAlarmed;
}

#define countof(a) (sizeof(a) / sizeof(a[0]))

void printDateTime(const RtcDateTime& dt)
{
  char datestring[20];

  snprintf_P(datestring, 
      countof(datestring),
      PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
      dt.Month(),
      dt.Day(),
      dt.Year(),
      dt.Hour(),
      dt.Minute(),
      dt.Second() );
    Serial.print(datestring);
}
