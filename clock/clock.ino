#include <IRremote.h>

/* Hex IR Codes:
 *
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
 */


enum {
  CODE_NO_CHANGE = 0xFFFFFFFF,
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
  CODE_PLUS = 0xFFA857,
  CODE_PREV = 0xFF22DD,
  CODE_NEXT = 0xFF02FD
};

const int pin_latch_4 =  2;
const int pin_data__4 =  3;
const int pin_clock_4 =  4;

const int pin_digit[] = {
  8,
  5,
  6,
  7
};

const int pin_latch_1 =  9;
const int pin_data__1 = 10;
const int pin_clock_1 = 11;



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

byte output_old[]  = {
  codes[0],
  codes[0],
  codes[0],
  codes[0],
  
  codes[0]
};
byte output[] = {
  code_dot,
  code_dot,
  code_dot,
  code_dot,
  
  code_dot
};
int digits[] = { -1, -1, -1, -1, -1 };

const int digit_count = 5;
int digit_active = 0;


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
    if (*cur > -1) *cur = (*cur + 9) % 10;
    break;
  case CODE_PLUS:
    if (*cur > -1) *cur = (*cur + 1) % 10;
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

const int pin_input = 12; //set D12 as input signal pin
IRrecv irrecv(pin_input);
decode_results signals;
void setup() {
  pinMode(pin_latch_4, OUTPUT);
  pinMode(pin_clock_4, OUTPUT);
  pinMode(pin_data__4, OUTPUT);
  
  pinMode(pin_digit[0], OUTPUT);
  pinMode(pin_digit[1], OUTPUT);
  pinMode(pin_digit[2], OUTPUT);
  pinMode(pin_digit[3], OUTPUT);
  
  pinMode(pin_latch_1, OUTPUT);
  pinMode(pin_clock_1, OUTPUT);
  pinMode(pin_data__1, OUTPUT);
  
  pinMode(pin_input, INPUT);
  Serial.begin(9600);
  irrecv.enableIRIn(); // enable input from IR receiver
}

void loop() {
  int i = 0;
  
  if (irrecv.decode(&signals)) {
    Serial.println(signals.value, HEX);
    irrecv.resume(); // get the next signal
    
    if (signals.value == CODE_PREV) {
      digit_active = (digit_active + 4) % 5;
      Serial.print("prev active digit");
      Serial.println(digit_active, DEC);
    }
    else if (signals.value == CODE_NEXT) {
      digit_active = (digit_active + 1) % 5;
      Serial.print("next active digit");
      Serial.println(digit_active, DEC);
    }
    else if (signals.value != CODE_NO_CHANGE) {
      output[digit_active] = sig_to_output(&digits[digit_active],signals.value);
    }
  }
  
    digitalWrite(pin_digit[0], HIGH);
    digitalWrite(pin_digit[1], HIGH);
    digitalWrite(pin_digit[2], HIGH);
    digitalWrite(pin_digit[3], HIGH);
  
  if (output[digit_active] != output_old[digit_active]) {
    output_old[digit_active] = output[digit_active];
  }
  
  for(i = 0; i < (digit_count - 1 ); i++) {
    digitalWrite(pin_digit[i], LOW);
    
    digitalWrite(pin_latch_4, LOW);
    shiftOut(pin_data__4, pin_clock_4, MSBFIRST, output[i]);
    digitalWrite(pin_latch_4, HIGH);
    
    digitalWrite(pin_digit[i], HIGH);
    
    delay(4);
  }
  
  
  digitalWrite(pin_latch_1, 0);
  shiftOut(pin_data__1, pin_clock_1, MSBFIRST, output[4]);
  digitalWrite(pin_latch_1, 1);
  
  delay(1);
}

