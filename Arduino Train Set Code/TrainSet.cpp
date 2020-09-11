#include <Command.h>
#include <Commander.h>
#include <Crossing.h>
#include <Detector.h>
#include <Engine.h>
#include <Entity.h>
#include <Light.h>
#include <Manager.h>
#include <Rail.h>
#include <Updater.h>

// TrainTester.ino : Defines the entry point for the console application.
// Last Editted 11/23/2012 2:15

#include "Updater.h"
#include "Engine.h"
#include "Commander.h"
#include "Manager.h"
#include "Crossing.h"
#include "Detector.h"
#include "Rail.h"
#include "Updater.h"

#define STREETCAR_STOP_DELAY 10000
#define STATION_DELAY 10000
#define NO_DELAY 0

#define NS 0
#define EW 1
#define TRAIN 0
#define DOODLEBUG 1
#define RANDOM_DELAY -1

#define DELAY 1000

int freeRam () {
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}

void _enter(int type, int id){
  // type: either BOARD or DECODER def
  // num: board or decoder id
  digitalWrite(4, bitRead(id,0));
  digitalWrite(5, bitRead(id,1));
  digitalWrite(6, bitRead(id,2));
  digitalWrite(type, HIGH);
  digitalWrite(type,LOW);
}

void initSystem(){
  Serial.begin(9600);
  
  randomSeed(0);
  pinMode(4,OUTPUT); // pins 4 - 6 are used for 
  pinMode(5,OUTPUT); // defining which decoder/board is wanted
  pinMode(6,OUTPUT);
  
  // pulseOut on pin 7 selects 
  // the board as defined by pins 4-6
  pinMode(7,OUTPUT);
  // pulseOut on pin 8 selects 
  // the decoder as defined by pins 4-6
  pinMode(8,OUTPUT);
  
  // pins 9 - 12 (a.k.a. "the bus") holds the data value 
  // either passed to or from the Arduino
  // Note: When receiving data, pinMode must be changed to INPUT
  pinMode(9,OUTPUT);
  pinMode(10,OUTPUT);
  pinMode(11,OUTPUT);
  pinMode(12,OUTPUT);
  
  delay(4000);
  
  digitalWrite(9, LOW);
  digitalWrite(10, LOW);
  digitalWrite(11, LOW);
  digitalWrite(12, LOW);
  for (int i = 7;i>=0;i--){
    _enter(7, i);
    
    for(int j=7;j>=0;j--){
      _enter(8,j);
      //Serial.print(i);
      //Serial.print(" ");
      //Serial.println(j);
    }
  }
}

NoneCommand* none_c = new NoneCommand(); // used as a null command
NoneDetector* none_d = new NoneDetector(); // used as a null detector

DetectorManager det_mgr;
  Detector* d_clear_of_crossing = &det_mgr.detectors[0];
  Detector* d_approaching_crossing = &det_mgr.detectors[1];
  Detector* d_at_station_1 = &det_mgr.detectors[2];
  Detector* d_approaching_station_1 = &det_mgr.detectors[3];
  
  Detector* spare_1 = &det_mgr.detectors[4];
  Detector* d_at_crossing = &det_mgr.detectors[5];
  Detector* d_at_station_2 = &det_mgr.detectors[6];
  Detector* d_approaching_station_2 = &det_mgr.detectors[7];
  
  Detector* t_clear_of_crossing = &det_mgr.detectors[8];
  Detector* t_approaching_crossing = &det_mgr.detectors[9];
  Detector* t_at_station_1 = &det_mgr.detectors[10];
  Detector* t_approaching_station_1 = &det_mgr.detectors[11];
  
  Detector* t_at_crossing = &det_mgr.detectors[12];
  Detector* streetcar_at_Wmid = &det_mgr.detectors[13];
  Detector* t_at_station_2 = &det_mgr.detectors[14];
  Detector* t_approaching_station_2 = &det_mgr.detectors[15];
  
  Detector* streetcar_at_C1 = &det_mgr.detectors[16];
  Detector* streetcar_at_C2 = &det_mgr.detectors[17];
  Detector* streetcar_at_A = &det_mgr.detectors[18];
  Detector* streetcar_at_Eend = &det_mgr.detectors[19];
  
  Detector* streetcar_at_B = &det_mgr.detectors[20];
  Detector* streetcar_at_Wend = &det_mgr.detectors[21];
  Detector* streetcar_at_Smid = &det_mgr.detectors[22];
  Detector* streetcar_at_Nmid = &det_mgr.detectors[23];

Command* street_NS_green = new Command(1,3,1);
Command* street_NS_yellow = new Command(1,3,2);
Command* street_NS_red = new Command(1,3,4);
Command* street_EW_green = new Command(1,4,1);
Command* street_EW_yellow = new Command(1,4,2);
Command* street_EW_red = new Command(1,4,4);

DecoderID xing_list (1,1);
  Command* train_crossing_green = new Command(&xing_list, 3);
  Command* train_crossing_red = new Command(&xing_list, 2);
  Command* doodlebug_crossing_green = new Command(&xing_list, 1);
  Command* doodlebug_crossing_red = new Command(&xing_list, 0);

DecoderID station_list (1,2);
  Command* train_station_green = new Command(&station_list, 3);
  Command* train_station_red = new Command(&station_list, 2);
  Command* doodlebug_station_green = new Command(&station_list, 1);
  Command* doodlebug_station_red = new Command(&station_list, 0);

Updater* updtr = new Updater();
Manager* man = new Manager(&det_mgr);
Commander* cmdr = new Commander(man);

// Create Rails
Rail* doodlebug_line = new Rail();
Rail* train_line = new Rail();
Rail* NS_line = new Rail();
Rail* EW_line = new Rail();

//Create Crossing Lights
CrossingLight* street_light = new CrossingLight(man, street_NS_green, street_NS_yellow, street_NS_red, 
  street_EW_green, street_EW_yellow, street_EW_red);

CrossingLight* train_station_light = new CrossingLight(man, train_station_green, train_station_red, 
  none_c, none_c);
CrossingLight* doodlebug_station_light = new CrossingLight(man, doodlebug_station_green, 
  doodlebug_station_red, none_c, none_c);
CrossingLight* train_doodlebug_light = new CrossingLight(man, train_crossing_green, train_crossing_red, 
  doodlebug_crossing_green, doodlebug_crossing_red);

// Create Engines
Engine* streetcar_EW = new Engine(cmdr, 2, 8, 8, 12); // 1000
Engine* streetcar_NS = new Engine(cmdr, 3, 8, 8, 12); // 1009

Engine* train = new Engine(cmdr, 0, 12, 16, 16); // 3030
Engine* doodlebug = new Engine(cmdr, 1, 14, 22, 26); // 3232

// street_car_NS crossings
Crossing streetcar_light_NS2 (man, NS_line, street_light, streetcar_NS, none_d, streetcar_at_C2, none_d);
Crossing streetcar_north_mid (man, NS_line, NULL, streetcar_NS, none_d, streetcar_at_Nmid, none_d);
Crossing streetcar_light_NS (man, NS_line, street_light, streetcar_NS, none_d, streetcar_at_C1, none_d);
Crossing streetcar_south_mid (man, NS_line, NULL, streetcar_NS, none_d, streetcar_at_Smid, none_d);

// street_car_EW crossings
Crossing streetcar_west_end (man, EW_line, NULL, streetcar_EW, none_d, streetcar_at_Wend, none_d);
Crossing streetcar_west_mid (man, EW_line, NULL, streetcar_EW, none_d, streetcar_at_Wmid, none_d);
Crossing streetcar_light_EW (man, EW_line, street_light, streetcar_EW, none_d, streetcar_at_A, streetcar_at_B);
Crossing streetcar_east_end (man, EW_line, NULL, streetcar_EW, none_d, streetcar_at_Eend, none_d);


//train crossings
Crossing train_station_1 (man, train_line, train_station_light, train, t_approaching_station_1, t_at_station_1, none_d);
Crossing train_station_2 (man, train_line, train_station_light, train, t_approaching_station_2, t_at_station_2, none_d);
Crossing train_crossing (man, train_line, train_doodlebug_light, train, t_approaching_crossing, t_at_crossing, t_clear_of_crossing);

//doodlebug crossings
Crossing doodlebug_station_1 (man, doodlebug_line, doodlebug_station_light, doodlebug, d_approaching_station_1, d_at_station_1, none_d);
Crossing doodlebug_station_2 (man, doodlebug_line, doodlebug_station_light, doodlebug, d_approaching_station_2, d_at_station_2, none_d);
Crossing doodlebug_crossing (man, doodlebug_line, train_doodlebug_light, doodlebug, d_approaching_crossing, d_at_crossing, d_clear_of_crossing);

// Test suites
void test_detectors(DetectorManager* det_mgr){
  // manual test
  for(int i=0;i<=23;i=i+4){
    Serial.print(det_mgr->detectors[i].cur);
    Serial.print(det_mgr->detectors[i+1].cur);
    Serial.print(det_mgr->detectors[i+2].cur);
    Serial.print(det_mgr->detectors[i+3].cur);
    Serial.print(" ");
  }
  Serial.println();
}

void test_lights(){
  // Test crossing and station lights
  train_crossing_green->sendCommand(1);
  delay(DELAY);
  train_crossing_red->sendCommand(1);
  delay(DELAY);
  doodlebug_crossing_green->sendCommand(1);
  delay(DELAY);
  doodlebug_crossing_red->sendCommand(1);
  delay(DELAY);
  
  
  train_station_green->sendCommand(1);
  delay(DELAY);
  train_station_red->sendCommand(1);
  delay(DELAY);
  doodlebug_station_green->sendCommand(1);
  delay(DELAY);
  doodlebug_station_red->sendCommand(1);
  delay(DELAY);
  
  train_crossing_green->sendCommand(0);
  delay(DELAY);
  train_crossing_red->sendCommand(0);
  delay(DELAY);
  doodlebug_crossing_green->sendCommand(0);
  delay(DELAY);
  doodlebug_crossing_red->sendCommand(0);
  delay(DELAY);
  
  train_station_green->sendCommand(0);
  delay(DELAY);
  train_station_red->sendCommand(0);
  delay(DELAY);
  doodlebug_station_green->sendCommand(0);
  delay(DELAY);
  doodlebug_station_red->sendCommand(0);
  delay(DELAY);
  
  // Test streetcar_lights: no test is needed they naturally cycle
}

void test_DCC(){
  train->setSpeed(0, FWD);
  delay(DELAY);
  train->setSpeed(6, FWD);
  delay(DELAY);
  train->setSpeed(8, FWD);
  delay(DELAY);
  train->setSpeed(10, FWD);
  delay(DELAY);
  train->setSpeed(12, FWD);
  delay(DELAY);
  train->setSpeed(14, FWD);
  delay(DELAY);
  train->setSpeed(16, FWD);
  delay(DELAY);
  train->setSpeed(18, FWD);
  delay(DELAY);
  train->setSpeed(20, FWD);
  delay(DELAY);
  train->setSpeed(22, FWD);
  delay(DELAY);
  train->setSpeed(24, FWD);
  delay(DELAY);
  train->setSpeed(26, FWD);
  delay(DELAY);
  
  train->setSpeed(0, REV);
  delay(DELAY);
  train->setSpeed(6, REV);
  delay(DELAY);
  train->setSpeed(8, REV);
  delay(DELAY);
  train->setSpeed(10, REV);
  delay(DELAY);
  train->setSpeed(12, REV);
  delay(DELAY);
  train->setSpeed(14, REV);
  delay(DELAY);
  train->setSpeed(16, REV);
  delay(DELAY);
  train->setSpeed(18, REV);
  delay(DELAY);
  train->setSpeed(20, REV);
  delay(DELAY);
  train->setSpeed(22, REV);
  delay(DELAY);
  train->setSpeed(24, REV);
  delay(DELAY);
  train->setSpeed(26, REV);
  delay(DELAY);
}

//Runtime code
void setup(){
  initSystem();
  
  //Define RailList
  man->rail_list->addRail(NS_line);
  man->rail_list->addRail(EW_line);
  man->rail_list->addRail(train_line);
  man->rail_list->addRail(doodlebug_line);
  
  // Define streetcar NS crossing activity
  streetcar_light_NS2.atInt(RISING_EDGE, NO_DELAY, OBEY_LIGHT);
  
  streetcar_light_NS2.addIntersection(&streetcar_light_NS);
  
  streetcar_north_mid.atInt(RISING_EDGE, NO_DELAY, RANDOM_STOP);
  
  streetcar_light_NS.atInt(RISING_EDGE, NO_DELAY, OBEY_LIGHT);
  
  streetcar_south_mid.atInt(RISING_EDGE, 2500, RANDOM_STOP);
  
  streetcar_light_NS.addLight(NS);
  streetcar_light_NS2.addLight(EW);
  
  // Define streetcar EW crossing activity
  streetcar_west_end.atInt(RISING_EDGE, NO_DELAY, STOP);
  streetcar_west_end.atInt(RISING_EDGE, NO_DELAY, CHANGE_DIRECTION_FWD);
  streetcar_west_end.atInt(RISING_EDGE, STREETCAR_STOP_DELAY, GO);
  streetcar_west_end.addIntersection(&streetcar_light_EW);
  
  streetcar_west_mid.atInt(RISING_EDGE, NO_DELAY, RANDOM_STOP);
  
  streetcar_light_EW.atInt(RISING_EDGE, NO_DELAY, OBEY_LIGHT);
  streetcar_light_EW.addLight(EW);
  streetcar_light_EW.clearInt(RISING_EDGE, NO_DELAY, GO); // when clear of intersection go back to normal speed
  
  streetcar_east_end.atInt(RISING_EDGE, NO_DELAY, STOP);
  streetcar_east_end.atInt(RISING_EDGE, NO_DELAY, CHANGE_DIRECTION_REV);
  streetcar_east_end.atInt(RISING_EDGE, STREETCAR_STOP_DELAY, GO);
  streetcar_east_end.addIntersection(&streetcar_light_EW);
  
  // train station activity
  train_station_1.approachInt(RISING_EDGE, NO_DELAY, LIGHT_RED);
  train_station_1.approachInt(RISING_EDGE, NO_DELAY, SLOW_DOWN);
  train_station_1.atInt(RISING_EDGE, NO_DELAY, STOP);
  train_station_1.atInt(RISING_EDGE, STATION_DELAY, LIGHT_GREEN);
  train_station_1.atInt(RISING_EDGE, STATION_DELAY, SPEED_UP);
  
  train_station_2.approachInt(RISING_EDGE, NO_DELAY, LIGHT_RED);
  train_station_2.approachInt(RISING_EDGE, NO_DELAY, SLOW_DOWN);
  train_station_2.atInt(RISING_EDGE, NO_DELAY, STOP);
  train_station_2.atInt(RISING_EDGE, STATION_DELAY, LIGHT_GREEN);
  train_station_2.atInt(RISING_EDGE, STATION_DELAY, SPEED_UP);
  
  train_station_1.addLight(0);
  train_station_2.addLight(0);
  
  // doodlebug station activity
  doodlebug_station_1.approachInt(RISING_EDGE, NO_DELAY, LIGHT_RED);
  doodlebug_station_1.approachInt(RISING_EDGE, NO_DELAY, SLOW_DOWN);
  doodlebug_station_1.atInt(RISING_EDGE, NO_DELAY, STOP);
  doodlebug_station_1.atInt(RISING_EDGE, STATION_DELAY, LIGHT_GREEN);
  doodlebug_station_1.atInt(RISING_EDGE, STATION_DELAY, SPEED_UP);

  doodlebug_station_2.approachInt(RISING_EDGE, NO_DELAY, LIGHT_RED);
  doodlebug_station_2.approachInt(RISING_EDGE, NO_DELAY, SLOW_DOWN);
  doodlebug_station_2.atInt(RISING_EDGE, NO_DELAY, STOP);
  doodlebug_station_2.atInt(RISING_EDGE, STATION_DELAY, LIGHT_GREEN);
  doodlebug_station_2.atInt(RISING_EDGE, STATION_DELAY, SPEED_UP);

  doodlebug_station_1.addLight(0);
  doodlebug_station_2.addLight(0);
  
  // crossing activity
  train_crossing.approachInt(RISING_EDGE, NO_DELAY, FIRST);
  train_crossing.atInt(RISING_EDGE, NO_DELAY, OBEY_LIGHT);
  train_crossing.clearInt(RISING_EDGE, NO_DELAY, FIRST_CLEAR);

  doodlebug_crossing.approachInt(RISING_EDGE, NO_DELAY, FIRST);
  doodlebug_crossing.atInt(RISING_EDGE, NO_DELAY, OBEY_LIGHT);
  doodlebug_crossing.clearInt(RISING_EDGE, NO_DELAY, FIRST_CLEAR);

  train_crossing.addLight(TRAIN);
  doodlebug_crossing.addLight(DOODLEBUG);

  // Start Trains
  doodlebug->go();
  train->go();
  streetcar_NS->go();
  streetcar_EW->go();
}

void loop(){
  man->run();
  
  //test_detectors(&det_mgr);
  
  //test_DCC();
  
  //test_lights();
  
  //Serial.println(freeRam());
  /*
  Serial.print("1000: ");
  streetcar_EW->printInfo();
  Serial.print("1009: ");
  streetcar_NS->printInfo();
  Serial.print("3030: ");
  train->printInfo();
  Serial.print("3232: ");
  doodlebug->printInfo();
  Serial.println();
  */
}




