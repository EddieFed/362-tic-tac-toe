/*
 * Tic-Tac-Toe Joystick Controller
 * Code by Eddie Federmeyer
 * 
 * This code simply sends the joystick direction (left/right) over I2C Serial to
 * the "Game Coordinator" by Alexis Lara. The buttons is used to select the location.
 */

#define VRX_PIN A0  // JoyStick X direction pin 0-1023
#define VRY_PIN A1  // JoyStick Y direction pin 0-1023
#define BTN_PIN 3   // Select button

#define TX_PIN 1
#define RX_PIN 0

// Button stuff
const int UP_THRESHOLD    = 100;
const int DOWN_THRESHOLD  = 900;
const int RIGHT_THRESHOLD = 900;
const int LEFT_THRESHOLD  = 100;
enum JoystickDirection {  NONE = 0x0, UP = 0x1, DOWN = 0x2, LEFT = 0x4, RIGHT = 0x8 };
enum HeldState { NA, HELD };  // NA is neutral state
HeldState currJoystickState = NA;
HeldState currButtonState   = NA;


void setup() {
  pinMode(VRX_PIN, INPUT);
  pinMode(VRY_PIN, INPUT);
  pinMode(BTN_PIN, INPUT);

  Serial.begin(9600);
}


void loop() {

  // First check if the select button is pressed!
  int buttonRead = digitalRead(BTN_PIN);
  if (buttonRead == HIGH && currButtonState == NA) {
    // Send over Serial to the "Game Coordinator"
//    Serial.println("Select button pressed!");
    Serial.write('S');
    currButtonState = HELD;
  }
  else if (buttonRead == LOW) {
    currButtonState = NA;
  }

  // Now determine information for joystick
  
  int joystickX = analogRead(VRX_PIN);
  int joystickY = analogRead(VRY_PIN);
  int jDirection = NONE;

  // Determine LEFT/RIGHT
  if (joystickX >= RIGHT_THRESHOLD) {
    jDirection = jDirection | RIGHT;
  }
  else if (joystickX <= LEFT_THRESHOLD) {
    jDirection = jDirection | LEFT;
  }

  // Determine DOWN/UP
  if (joystickY >= DOWN_THRESHOLD) {
    jDirection = jDirection | DOWN;
  }
  else if (joystickY <= UP_THRESHOLD) {
    jDirection = jDirection | UP;
  }

  // If button is HELD, ignore direction. if HELD and "NONE" direction, remove HELD
  if (currJoystickState == NA && jDirection != NONE) {
    // Print/Send Data!
    printJDirection(jDirection);
    currJoystickState = HELD;
    delay(100);
  }
  else if (currJoystickState == HELD && jDirection == NONE) {
    currJoystickState = NA;
  }

  delay(10);
}

// This will send the direction over Serial to the "Game Coordinator"
void printJDirection(int jDirection) {
  if (jDirection & LEFT) {
    Serial.write('L');
  }

  if (jDirection & RIGHT) {
    Serial.write('R');
  }

  if (jDirection & UP) {
    Serial.write('U');
  }

  if (jDirection & DOWN) {
    Serial.write('D');
  }

  
}
