/*
  Section Four + Section Five in one sketch
  Set SECTION below to 4 or 5, then upload.

  Section Four, Q2 -- Button example (File > Examples > 02.Digital > Button), modified
    Change from the original: ledPin changed from 13 to 3 (LED is on D3)
    Hold the button -> LED on, release -> LED off

  Section Five -- button-controlled LED states
    Q1  Starts with the LED steady on; each press switches to the next state
    Q2  Order: steady -> slow blink -> medium blink -> fast blink
    Q3  The 4th press goes back to steady

  Circuit (Section Four, Q1 -- the same for both sections)
    Button module: IN -> D2, VCC -> 5V, GND -> GND
    LED          : D3 -> 220 ohm resistor -> LED long leg (+)
                   LED short leg (-) -> GND
*/

const int SECTION = 5;       // <-- change to 5 for Section Five, then upload again

const int buttonPin = 2;     // the number of the pushbutton pin
const int ledPin = 3;        // CHANGED: LED is on D3 (was 13)

// ---------- Section Four ----------
int buttonState = 0;         // variable for reading the pushbutton status

// ---------- Section Five ----------
const unsigned long DEBOUNCE_MS = 30;

// How long the LED stays on (then off) in each state, in ms. 0 = steady.
const unsigned long BLINK_MS[] = {0, 1000, 500, 150};
const char* STATE_NAMES[] = {"Steady", "Slow blink", "Medium blink", "Fast blink"};

int state = 0;               // Q1: start in steady
unsigned long stateStart = 0;

bool idleLevel;              // button reading when NOT pressed, detected at power-on
bool lastReading;
bool isPressed = false;
unsigned long lastBounce = 0;

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);

  // Section Five: works whether the button module outputs HIGH or LOW when pressed.
  // Just don't hold the button while plugging in the USB cable.
  idleLevel = digitalRead(buttonPin);
  lastReading = idleLevel;

  Serial.print("Running Section ");
  Serial.println(SECTION);
}

void loop() {
  if (SECTION == 4) {
    // ===== Section Four: the Button example =====
    // read the state of the pushbutton value:
    buttonState = digitalRead(buttonPin);

    // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
    if (buttonState == HIGH) {
      digitalWrite(ledPin, HIGH);   // turn LED on
    } else {
      digitalWrite(ledPin, LOW);    // turn LED off
    }

  } else {
    // ===== Section Five: steady -> slow -> medium -> fast -> steady =====
    unsigned long now = millis();

    // detect each new press (with debounce)
    bool reading = digitalRead(buttonPin);
    if (reading != lastReading) {
      lastReading = reading;
      lastBounce = now;
    }

    if (now - lastBounce >= DEBOUNCE_MS) {
      bool down = reading != idleLevel;
      if (down && !isPressed) {
        // Q2 + Q3: 0 -> 1 -> 2 -> 3, and the 4th press wraps back to 0 (steady)
        state = (state + 1) % (sizeof(BLINK_MS) / sizeof(BLINK_MS[0]));
        stateStart = now;
        Serial.print("State: ");
        Serial.println(STATE_NAMES[state]);
      }
      isPressed = down;
    }

    // LED output (millis-based, so presses are never missed)
    if (BLINK_MS[state] == 0) {
      digitalWrite(ledPin, HIGH);
    } else {
      digitalWrite(ledPin, ((now - stateStart) / BLINK_MS[state]) % 2 == 0 ? HIGH : LOW);
    }
  }
}
