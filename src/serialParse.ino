#include <XSAA1064.h>
//#include <EncButton.h>
#include <GParser.h>
#include "AsyncStream.h"
//#include <PCA9554.h>  // Load the PCA9554 Library
AsyncStream<10> serial(&Serial, ';');  //Executer and terminator

#define SRL_SPD 9600  //Serial Speed

XSAA1064 LeftColumn = XSAA1064(0x38);  //Constructor XSAA1064
XSAA1064 RightColumn = XSAA1064(0x3B);  //Constructor XSAA1064

//PCA9554 ioCon1(0x24);

/***************Button**************/

//#include <EncButton.h>

//#define PWR_BTN 3  // Power button is connected to D3
//#define RBT_BTN 4  // Reboot button is connected to D4
//#define RBT_BTN 5  // Open trigger is connected to D5

//EncButton<EB_TICK, PWR_BTN> PWR;  
//EncButton<EB_TICK, RBT_BTN> RBT;
//EncButton<EB_TICK, OPN_BTN> OPN;

/***************Flags***************/

//bool isPressedPWR = 0;  //Flag for PWR_BTN
//bool isPressedRBT = 0;  //Flag for RBT_BTN
//bool isPressedOPN = 0;  //Flag for OPN_BTN

void setup() {
    LeftColumn.begin();
    RightColumn.begin();
    //ioCon1.portMode(ALLOUTPUT);
    Serial.begin(SRL_SPD);
    //pinMode(PWR_BTN, INPUT);
    //pinMode(RBT_BTN, INPUT);
    //pinMode(RBT_BTN, INPUT);
    LeftColumn.value(0, 1);  // Reset LLightShow
    RightColumn.value(0, 1);  // Reset RLightShow
}

void loop() {
    parsing();
}

void parsing() {
if (serial.available()) {
    GParser data(serial.buf, ',');  // Parser take buffer
    int ints[10];           // Array for ints
    data.parseInts(ints);   // Move ints to array

    switch (ints[0]) {
        case 0:
            switch (ints[1]) {
                case 0:
                    LeftColumn.value(ints[2], ints[3]);
                break;
                case 1:
                    RightColumn.value(ints[2], ints[3]);
                break;
            }
        break;
        case 1:
            
        break;
    }   
  }
}
