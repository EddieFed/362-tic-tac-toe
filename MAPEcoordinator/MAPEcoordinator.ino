/*
 * Tic-Tac-Toe Coordinator
 * 
 * Takes the input from two joysticks, coordinates whos turn it is
 * and then relays that to the LED array arduino!
 */
 
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

SoftwareSerial s1(6, 7);  // Controller 1
SoftwareSerial s2(8, 9);  // Controller 2

void setup() {
  lcd.begin(16, 2);
  lcd.print("Tic-Tac-Toe!");

  // Create serial connections
  Serial.begin(9600); // Output
  s1.begin(9600);     // Player 1 input
  s2.begin(9600);     // Player 2 input
}

int curr_player = 0;  // 0 = p1, 1 = p2
void loop() {
  lcd.setCursor(0, 1);
  char inByte;
  
  if (curr_player == 0) {
    s1.listen();
    inByte = s1.read();
    lcd.print("p1");
  }
  else {
    s2.listen();
    inByte = s1.read();
    lcd.print("p2");
  }

  if (inByte == 'S') {
    curr_player = (curr_player + 1) % 2;
  }
  
  // Send and display!
  lcd.print(inByte);
  Serial.write(inByte);
  
}
