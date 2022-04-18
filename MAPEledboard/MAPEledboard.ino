const int R_PINS[3][3] = {{2, 3, 4}, {5, 6, 7}, {8, 9, 10}};
const int B_PINS[3][3] = {{A0, A1, A2}, {A3, A4, A5}, {11, 12, 13}};
bool r_states[3][3] = {{LOW, LOW, LOW}, {LOW, LOW, LOW}, {LOW, LOW, LOW}};
bool b_states[3][3] = {{LOW, LOW, LOW}, {LOW, LOW, LOW}, {LOW, LOW, LOW}};

unsigned long startTime;

void setup() {
  startTime = millis();
  
  for (int i = 0; i <= 2; i++) {
    for (int j = 0; j <= 2; j++) {
        r_states[i][j] = LOW;
        b_states[i][j] = LOW;
      
        pinMode(R_PINS[i][j], OUTPUT);
        pinMode(B_PINS[i][j], OUTPUT);
    }
  }
  
  Serial.begin(9600);
}


// Coords of highlighted LED
int curr_x = 0, curr_y = 0;
int curr_player = 0;
char winner = ' ';

void loop() {

  // Sloppy catch for winner.
  // This could be so much better...
  if (winner != ' ') {
    if (winner == 'R') {
      for (int i = 0; i <= 2; i++) {
        for (int j = 0; j <= 2; j++) {
          digitalWrite(B_PINS[i][j], LOW);
          digitalWrite(R_PINS[i][j], HIGH);
        }
      }
    }
    if (winner == 'B') {
      for (int i = 0; i <= 2; i++) {
        for (int j = 0; j <= 2; j++) {
          digitalWrite(R_PINS[i][j], LOW);
          digitalWrite(B_PINS[i][j], HIGH);
        }
      }
    }
    delay(500);
  }

  else {
    // Read serial to update frame first!
    if (Serial.available()) {
      char incomingByte = Serial.read();
      
      // Move selection Left or Right
      // Make sure to keep coord on board using x/y mod3
      // Note* I'm using +5 for L/U because tinkercad is
      // broken and doesnt do modulo of a negative number
      if (incomingByte == 'R') {
        curr_x = (curr_x + 1) % 3;
      }
      else if (incomingByte == 'L') {
        curr_x = (curr_x + 5) % 3;  // +5 is total BS but whatever
      }
      else if (incomingByte == 'D') {
        curr_y = (curr_y + 1) % 3;
      }
      else if (incomingByte == 'U') {
        curr_y = (curr_y + 5) % 3;
      }
      
      // If byte is 'S', then select the current
      // LED for current player!
      if (incomingByte == 'S') {
  
        // Make sure current highlighted spot isn't taken!
        if ( (curr_player == 0 && b_states[curr_y][curr_x] == LOW) || (curr_player == 1 && r_states[curr_y][curr_x] == LOW) ) {
          (curr_player == 0) ? r_states[curr_y][curr_x] = HIGH : b_states[curr_y][curr_x] = HIGH;
  
          // Check winner
          if (curr_player == 0 && checkWin(r_states)) {
            Serial.println("RED WINS");
            winner = 'R';
          }
          else if (curr_player == 1 && checkWin(b_states)) {
            Serial.println("BLUE WINS");
            winner = 'B';
          }
  
          // Switch player
          curr_player = (curr_player + 1) % 2;
        
          // Reset x/y
          curr_x = 0;
          curr_y = 0; 
        }
      }
  
      if (incomingByte == '1') {
        winner = 'R';
      }
      else if (incomingByte == '2') {
        winner = 'B';
      }
      
    }
    
    // Redraw lights every "frame"
    // This way it overwrites the 
    // selection from last frame
    for (int i = 0; i <= 2; i++) {
      for (int j = 0; j <= 2; j++) {
        digitalWrite(R_PINS[i][j], r_states[i][j]);
        digitalWrite(B_PINS[i][j], b_states[i][j]);
      }
    }
  
    if ((millis() - startTime) % 1000 >= 500) {
      // Now draw selection on top of the frame!
      (curr_player == 0) ? digitalWrite(R_PINS[curr_y][curr_x], HIGH) : digitalWrite(B_PINS[curr_y][curr_x], HIGH);
    }
  }
}

bool checkWin(bool s[3][3]) {
  Serial.println("Checking win");

  // Check rows
  for (int y = 0; y < 3; y++) {
    if ((s[y][0] & s[y][1] & s[y][2]) == HIGH) {
      // Player won from row
      Serial.println("Row");
      return true;
    }
  }
  
  // Check Columns
  for (int x = 0; x < 3; x++) {
    if ((s[0][x] & s[1][x] & s[2][x]) == HIGH) {
      // Player won from c
      Serial.println("Coloumn");
      return true;
    }
  }

  // Diagonal
  // TL M BR
  if ((s[0][0] & s[1][1] & s[2][2]) == HIGH) {
    Serial.println("Diagonal");
    return true;
  }
  // TR M BL
  if ((s[0][2] & s[1][1] & s[2][0]) == HIGH) {
    Serial.println("Diagonal");
    return true;
  }
  
  return false;
}
