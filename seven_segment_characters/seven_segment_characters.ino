const int pin_button = 12;

const int pins[] = {
  1,
  2,
  3,
  4,
  5,
  6,
  7,
  8
};

const byte pin_mask[] = {
  0b00000001,
  0b00000010,
  0b00000100,
  0b00001000,
  0b00010000,
  0b00100000,
  0b01000000,
  0b10000000
};

// pin to segment mapping
// and our pin numbering,
// the middle pins are common
// cathode on the example
//  7     | 56-78
// 6 8    |
//  5     |
// 1 3    |
//  2 .=4 | 12-34
const byte patterns[] = {
  0b00000000,
  0b10000100,
  0b11010011,
  0b11010110,
  0b10110100,
  0b01110110,
  0b01110111,
  0b11000100,
  0b11110111,
  0b11110100,
  0b11111101,
  0b11111111,
  0b01101011,
  0b11101111,
  0b01111011,
  0b01111001
};

byte pattern = patterns[0];

int btn_pushes = 0;

int status_count = LOW;
int bounce_count = LOW;
int reading_button = LOW;

long debounce_count = 0;
const long debounce_delay = 50;


int pin_state(byte pat,byte mask) {
  if ((pat & mask) == mask) {
    return HIGH;
  }
  else {
    return LOW;
  }
}

void setup() {
  pinMode(pin_button,INPUT);

  pinMode(pins[0],OUTPUT);  
  pinMode(pins[1],OUTPUT);
  pinMode(pins[2],OUTPUT);
  pinMode(pins[3],OUTPUT);
  pinMode(pins[4],OUTPUT);
  pinMode(pins[5],OUTPUT);
  pinMode(pins[6],OUTPUT);
  pinMode(pins[7],OUTPUT);
  pinMode(pins[8],OUTPUT);
}

void check_btn() {
  reading_button = digitalRead(pin_button);

  if (reading_button != bounce_count) {
    debounce_count = millis();
    bounce_count = reading_button;
  }

  if ((millis() - debounce_count) < debounce_delay) return;
  
  if (reading_button == status_count) return;

  status_count = reading_button;
  bounce_count = status_count;

  if (status_count == HIGH) {
    btn_pushes = (btn_pushes + 1) % 16;
    
    pattern = patterns[btn_pushes];
  }
}

void loop() {
  check_btn();
  
  digitalWrite(pins[0],pin_state(pattern,pin_mask[0]));
  digitalWrite(pins[1],pin_state(pattern,pin_mask[1]));
  digitalWrite(pins[2],pin_state(pattern,pin_mask[2]));
  digitalWrite(pins[3],pin_state(pattern,pin_mask[3]));
  digitalWrite(pins[4],pin_state(pattern,pin_mask[4]));
  digitalWrite(pins[5],pin_state(pattern,pin_mask[5]));
  digitalWrite(pins[6],pin_state(pattern,pin_mask[6]));
  digitalWrite(pins[7],pin_state(pattern,pin_mask[7]));
}


