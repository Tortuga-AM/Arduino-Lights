#define NUM_COLORS 4
#define NUM_PINS 2
#define FLASH_DURATION 500

int redPins[2] = {2, 3};
int bluePins[2] = {4, 5};
int yellowPins[2] = {6, 7};
int greenPins[2] = {8, 9};
bool busy = false;
bool endgame = false;


int ledPins[8][2] = {{redPins}, {bluePins}, {yellowPins}, {greenPins}};

enum LEDState {
  OFF,
  ON,
  FLASH,
  FLASH_ALL
};

enum LEDColors {
  RED = 0,
  BLUE = 1,
  YELLOW = 2,
  GREEN = 3
};

void flashAllLights(int flashDuration = 500) {
  // Loop through all colors and set them to flash
  for (int color = 0; color < 4; color++) {
    for (int i = 0; i < NUM_PINS; i++) {
      digitalWrite(ledPins[color][i], HIGH); // Turn on all LEDs
    }
  }
  delay(flashDuration); // Keep them on for the flash duration

  for (int color = 0; color < 4; color++) {
    for (int i = 0; i < NUM_PINS; i++) {
      digitalWrite(ledPins[color][i], LOW); // Turn off all LEDs
    }
  }
  delay(flashDuration); // Keep them off for the flash duration
}

void setColor(LEDColors color, LEDState state) {
  switch(state) {
    case OFF:
      for (int i = 0; i < NUM_PINS; i++) {
        digitalWrite(ledPins[color][i], LOW);
      }
      break;

    case ON:
      for (int i = 0; i < NUM_PINS; i++) {
        digitalWrite(ledPins[color][i], HIGH);
      }
      break; 
    
    case FLASH:
      for (int i = 0; i < NUM_PINS; i++) {
        digitalWrite(ledPins[color][i], HIGH);
      }
      delay(FLASH_DURATION);
      for (int i = 0; i < NUM_PINS; i++) {
        digitalWrite(ledPins[color][i], LOW);
      }
      delay(FLASH_DURATION);
  }
}

void reset() {
  for (int color = 0; color < 4; color++) {
    for (int i = 0; i < NUM_PINS; i++) {
      digitalWrite(ledPins[color][i], LOW); // Turn off all LEDs
    }
  }
}

void setup() {
  // put your setup code here, to run once:
  for (int i = 0; i < NUM_COLORS; i++) {
    for (int j = 0; j < NUM_PINS; j++) {
      pinMode(ledPins[i][j], OUTPUT);
    }
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() > 0) {
    char command = Serial.read();

    switch (command) {
      case 'Auto':  // Autonomous
        if (!busy) {
          setColor(LEDColors::RED, LEDState::OFF);
        }
        setColor(LEDColors::BLUE, LEDState::ON);
        setColor(LEDColors::YELLOW, LEDState::OFF);
        setColor(LEDColors::GREEN, LEDState::OFF);
        break;
      case 'Tele':  // Teleop
        if (!busy) {
          setColor(LEDColors::RED, LEDState::OFF);
        }

        setColor(LEDColors::BLUE, LEDState::OFF);
        if (!endgame) {
          setColor(LEDColors::YELLOW, LEDState::OFF);
        }
        setColor(LEDColors::GREEN, LEDState::ON);
        break;
      case 'LP':  // Low battery
        busy = true;
        setColor(LEDColors::RED, LEDState::FLASH);
        break;
      case 'D':  // Disabled
        busy = true;
        setColor(LEDColors::RED, LEDState::ON);
        break;
      case 'PR': // Power restored
        busy = false;
      case 'E': // Enabled
        busy = false;
      case 'End':
        endgame = true;
        setColor(LEDColors::YELLOW, LEDState::ON);
      default:  // Unknown state
        flashAllLights();
    }
  }
}
