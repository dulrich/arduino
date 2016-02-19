#include <IRremote.h>

/*  Infrared Remote Controller & IR Receiver demo program
  copy right John Yu
  connect VS1838B to  D10 see http://osoyoo.com/?p=144
*/

/*
 * CH- 0xFFA25D
 * CH  0xFF629D
 * CH+ 0xFFE21D
 * |<< 0xFF22DD
 * >>| 0xFF02FD
 * >|| 0xFFC23D
 *  -  0xFFE01F
 *  +  0xFFA857
 * eq  0xFF906F
 *  0  0xFF6897
 * 100 0xFF9867
 * 200 0xFFB04F
 *  1  0xFF30CF
 *  2  0xFF18E7
 *  3  0xFF7A85
 *  4  0xFF10EF
 *  5  0xFF38C7
 *  6  0xFF5AA5
 *  7  0xFF42BD
 *  8  0xFF4AB5
 *  9  0xFF52AD
 *
 * 1010001001011101
 * 0110001010011101
 * 1110001000011101
 * 0010001011011101
 * 0000001011111101
 * 1100001000111101
 * 1110000000011111
 * 1010100001010111
 * 1001000001101111
 * 0110100010010111
 * 1001100001100111
 * 1011000001001111
 * 0011000011001111
 * 0001100011100111
 * 0111101010000101
 * 0001000011101111
 * 0011100011000111
 * 0101101010100101
 * 0100001010111101
 * 0100101010110101
 * 0101001010101101
 *
 * 1010001001011101 | 0110001010011101 | 1110001000011101
 * 0010001011011101 | 0000001011111101 | 1100001000111101
 * 1110000000011111 | 1010100001010111 | 1001000001101111
 * 0110100010010111 | 1001100001100111 | 1011000001001111
 * 0011000011001111 | 0001100011100111 | 0111101010000101
 * 0001000011101111 | 0011100011000111 | 0101101010100101
 * 0100001010111101 | 0100101010110101 | 0101001010101101
 */


enum {
 CODE_0 = 0xFF6897,
 CODE_1 = 0xFF30CF,
 CODE_2 = 0xFF18E7,
 CODE_3 = 0xFF7A85,
 CODE_4 = 0xFF10EF,
 CODE_5 = 0xFF38C7,
 CODE_6 = 0xFF5AA5,
 CODE_7 = 0xFF42BD,
 CODE_8 = 0xFF4AB5,
 CODE_9 = 0xFF52AD,
 CODE_MINUS = 0xFFE01F,
 CODE_PLUS = 0xFFA857
};

const int pin_latch =  8;
const int pin_clock = 12;
const int pin_data  = 11;

const byte code_dot = 0b00001000;
const byte codes[] = {
  0b11100111,
  0b10000100,
  0b11010011,
  0b11010110,
  0b10110100,
  0b01110110,
  0b01110111,
  0b11000100,
  0b11110111,
  0b11110100
};

byte output_old  = codes[0];
byte output = code_dot;
int digit = -1;

byte sig_to_output(int *cur, int sig) {
  switch(sig) {
  case CODE_0:
    *cur = 0;
    break;
  case CODE_1:
    *cur = 1;
    break;
  case CODE_2:
    *cur = 2;
    break;
  case CODE_3:
    *cur = 3;
    break;
  case CODE_4:
    *cur = 4;
    break;
  case CODE_5:
    *cur = 5;
    break;
  case CODE_6:
    *cur = 6;
    break;
  case CODE_7:
    *cur = 7;
    break;
  case CODE_8:
    *cur = 8;
    break;
  case CODE_9:
    *cur = 9;
    break;
  case CODE_MINUS:
    *cur = (*cur + 9) % 10;
    break;
  case CODE_PLUS:
    *cur = (*cur + 1) % 10;
    break;
  default:
    *cur = -1;
  }
  
  if (*cur == -1) {
    return code_dot;
  }
  else {
    return codes[*cur];
  }
}

int input_pin = 10; //set D10 as input signal pin
IRrecv irrecv(input_pin);
decode_results signals;
void setup() {
  pinMode(pin_latch, OUTPUT);
  pinMode(pin_clock, OUTPUT);
  pinMode(pin_data, OUTPUT);
  
  Serial.begin(9600);
  irrecv.enableIRIn(); // enable input from IR receiver
}

void loop() {
  if (irrecv.decode(&signals)) {
    Serial.println(signals.value, HEX);
    irrecv.resume(); // get the next signal
    
    if (signals.value != 0xFFFFFFFF) {
      output = sig_to_output(&digit,signals.value);
    }
  }

  if (output != output_old) {
    digitalWrite(pin_latch, 0);
    shiftOut(pin_data, pin_clock, MSBFIRST, output);
    digitalWrite(pin_latch, 1);
    
    output_old = output;
  }
}
