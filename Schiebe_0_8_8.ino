//**************************LIBARIES************************
#include <Keypad.h>

#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>

#include <SPI.h>
#include "SdFat.h"

//**************************SETTINGS************************

#define debug         //enable debug
#define serial_speed 115200         //set com baudrate

#define audio           //enable audio

#define volt            //enable voltage checks

//**************************PINS****************************

#define clock_pin 3
#define clock_pin_low_level PINE5
#define data_pin 4
#define data_pin_low_level PING5
#define interrupt_pin 1

#define sd_slct_pin 53
#define caliper_button_pin 5

#define button_1_pin 11
#define button_2_pin 10
#define button_3_pin 8
#define button_row_pin 9

#define sda_pin 20
#define scl_pin 21

#ifdef audio
 #define buzz_pin 12
#else 
 #define buzz_pin 199 
#endif 

#define vin_pin 0

//**************************DEBUG***************************

#ifdef debug
 #define bug_print(x)  Serial.print(x)
 #define bug_println(x) Serial.println(x)
#else
 #define bug_print(x)
 #define bug_println(x)
#endif

//*************************INIT*****************************

//lcd
LiquidCrystal_I2C  lcd(0x3F, 2, 1, 0, 4, 5, 6, 7);

uint16_t display_delay = 30;
//uint32_t display_counter = 0;

//SD
SdFat SD;

File file1;
char file1_name[] = "data.txt";

//keypad
const byte ROWS = 1;
const byte COLS = 3;
char hexKeys[ROWS][COLS] =
{
  {'1', '2', '3'}
};
byte rowPins[ROWS] = {button_row_pin};
byte colPins[COLS] = {button_1_pin, button_2_pin, button_3_pin};
Keypad cKeypad = Keypad( makeKeymap(hexKeys), rowPins, colPins, ROWS, COLS);

//*************************GLOBAL***************************
//measurement
volatile bool ready_flag = false;
volatile float value = 0.0;
volatile uint32_t raw_value = 0;
volatile int8_t sign_calip = 1;
volatile uint8_t bit_pos_counter = 0;
volatile uint32_t last_data = 0;

//caliper

float cal_offset = 0.0;

//*************************GENERAL**************************

// voltage 
static float voltage_threshold = 3.5;
#ifdef volt 
 bool ignore_voltage = false; 
#else 
 bool ignore_voltage = true;
#endif

//menu
int8_t menu_slct_x = 0;
int8_t menu_slct_y = 0;

//sample
bool init_take_sample = true; 
uint32_t sample_counter = 0;
float current_sample = 0.0;
float prev_sample = 0.0;
bool sample_taken = false;

//sample data

char tree[4][20] = {{"poplar"},{"palm tree"},{"tanne"},{"birch"}}; 
const uint8_t number_of_trees = 4 - 1;
uint8_t tree_slct = 0;

char area[4][20] = {{"schlumpfhausen"},{"aldi"},{"viessmann"},{"pappelberg"}};
const uint8_t number_of_areas = 4 - 1;
uint8_t area_slct = 0;


void setup()
{
  init1();
  welcome();

  bug_println("________________leaving__setup____________________");
}

void loop()
{
  if(1)
  {
    if (menu_slct_x == 0)
    {
      mainMenu();
    }
    else if (menu_slct_x >= 1)
    {
      if (menu_slct_y == 0)
      {
        takeSamples();
      }
      else if (menu_slct_y == 1)
      {
        editStorage();
      }
      else if (menu_slct_y == 2)
      {
        settings();
      }
    }
    limitMenu();
    background();
  }
  else 
  {
    background();
  }

}



