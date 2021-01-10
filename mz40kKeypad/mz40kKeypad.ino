//-----------------------------------------------
// MIDI interface for MZ-40K by Arduino
//    by Oh!Ishi / Nibbles lab.
//-----------------------------------------------

byte pC[32] = {0x04, 0x08, 0x10, 0x20, 0x04, 0x08, 0x10, 0x20, 0x04, 0x08, 0x10, 0x20, 0x04, 0x08, 0x10, 0x20, 0x04, 0x08, 0x10, 0x20, 0x04, 0x08, 0x10, 0x20, 0x06, 0x0a, 0x12, 0x22, 0x05, 0x09, 0x11, 0x21};
byte pD[32] = {0x48, 0x48, 0x48, 0x48, 0x44, 0x44, 0x44, 0x44, 0x60, 0x60, 0x60, 0x60, 0x50, 0x50, 0x50, 0x50, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40};
byte pB[32] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x02, 0x02, 0x02, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

// キーを離す
void keyOff() {
  // マトリクスをリセット
  PORTC = 0;
  PORTD = 0xc0;
  PORTB = 0;
}

// キーを押す
void keyOn(byte key) {
  // マトリクスをセット
  PORTC = pC[key];
  PORTD = pD[key];
  PORTB = pB[key];
}

void setup() {
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(A4, OUTPUT);
  pinMode(A5, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, INPUT_PULLUP);
  pinMode(11, INPUT_PULLUP);
  pinMode(12, INPUT_PULLUP);
  pinMode(13, INPUT_PULLUP);

  Serial.begin(9600);

  keyOff();
}

void loop() {
  int dat = Serial.read();

  if (dat != -1) {
    switch (dat) {
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
        keyOn(dat - '0');
        break;
      case 'A':
      case 'B':
      case 'C':
      case 'D':
      case 'E':
      case 'F':
        keyOn(dat - 'A' + 10);
        break;
      case 'a':
      case 'b':
      case 'c':
      case 'd':
      case 'e':
      case 'f':
        keyOn(dat - 'a' + 10);
        break;
      case '@':
        keyOn(16);
        break;
      case 'R':
      case 'r':
        keyOn(17);
        break;
      case 'W':
      case 'w':
        keyOn(18);
        break;
      case 'L':
      case 'l':
        keyOn(19);
        break;
      default:
        return;
    }
    delay(100);
    keyOff();
    delay(100);
  }
}
