
const int color_pin =  7;
const int level_pin =  6;
const int led_pin   = 13;

const int color_pins[] = {
   9,
  11,
  10,
  12
};

const int levels[] = {
   15,
   95,
  175,
  255
};

int color_levels[] = { 0, 0, 0, 0 };

int btn_pushes   = 0;
int status_color = LOW;
int status_level = LOW;

int bounce_color = LOW;
int bounce_level = LOW;
int btn_reading  = LOW;

long debounce_color  = 0;
long debounce_level  = 0;
long debounce_delay = 50;

void setup() {
  pinMode(led_pin, OUTPUT);
  
  pinMode(color_pin, INPUT);
  pinMode(level_pin, INPUT);
  
  pinMode(color_pins[0], OUTPUT);
  pinMode(color_pins[1], OUTPUT);
  pinMode(color_pins[2], OUTPUT);
  pinMode(color_pins[3], OUTPUT);

  Serial.begin(9600);
}

void check_btn_color() {
  btn_reading = digitalRead(color_pin);

  if (btn_reading != bounce_color) {
    debounce_color = millis();
    bounce_color = btn_reading;
  }

  if ((millis() - debounce_color) < debounce_delay) return;
  
  if (btn_reading == status_color) return;

  status_color = btn_reading;
  bounce_color = status_color;

  if (status_color == HIGH) {
    btn_pushes = (btn_pushes + 1) % 4;
    
    Serial.print("setting counter ");
    Serial.println(btn_pushes);
  }

  if (btn_pushes == 0) {
    digitalWrite(led_pin, LOW);

    analogWrite(color_pins[1], LOW);
    analogWrite(color_pins[2], LOW);
    analogWrite(color_pins[3], LOW);
  }
  else {
    digitalWrite(led_pin, HIGH);

    analogWrite(color_pins[btn_pushes - 1], LOW);
    analogWrite(color_pins[btn_pushes], levels[color_levels[btn_pushes]]);
  }
}

void check_btn_level() {
  btn_reading = digitalRead(level_pin);

  if (btn_reading != bounce_level) {
    debounce_level = millis();
    bounce_level = btn_reading;
  }

  if ((millis() - debounce_level) < debounce_delay) return;
  
  if (btn_reading == status_level) return;

  status_level = btn_reading;
  bounce_level = status_level;

  if (status_level == HIGH && btn_pushes != 0) {
    color_levels[btn_pushes] = (color_levels[btn_pushes] + 1) % 4;
    
    Serial.print("setting level ");
    Serial.println(color_levels[btn_pushes]);

    analogWrite(color_pins[btn_pushes], levels[color_levels[btn_pushes]]);
  }
}

void loop() {
  check_btn_color();
  check_btn_level();
}

