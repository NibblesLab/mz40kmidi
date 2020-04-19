//-----------------------------------------------
// MIDI interface for MZ-40K by Arduino
//    by Oh!Ishi / Nibbles lab.
//-----------------------------------------------

#include <MIDI.h>

#define MAX_NOTE_CNT  8

byte pC[32] = {0x04, 0x08, 0x10, 0x20, 0x04, 0x08, 0x10, 0x20, 0x04, 0x08, 0x10, 0x20, 0x04, 0x08, 0x10, 0x20, 0x04, 0x08, 0x10, 0x20, 0x04, 0x08, 0x10, 0x20, 0x06, 0x0a, 0x12, 0x22, 0x05, 0x09, 0x11, 0x21};
byte pD[32] = {0x48, 0x48, 0x48, 0x48, 0x44, 0x44, 0x44, 0x44, 0x60, 0x60, 0x60, 0x60, 0x50, 0x50, 0x50, 0x50, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40};
byte pB[32] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x02, 0x02, 0x02, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

byte Rch = 1;
unsigned char NoteCnt = 0;
unsigned char NoteBuf[MAX_NOTE_CNT];

MIDI_CREATE_DEFAULT_INSTANCE();

// キーを離す
void keyOff() {
  // マトリクスをリセット
  PORTC = 0;
  PORTD = 0xc0;
  PORTB = 0;
}

// キーを押す
void keyOn(byte note) {
  if(note < 40) {
    note = 0;
  } else {
    note -= 40;
  }
  if(note > 31) {
    note = 31;
  }

  // マトリクスをセット
  PORTC = pC[note];
  PORTD = pD[note];
  PORTB = pB[note];
}

// ノートオフ
void NoteOff(byte note) {
  unsigned char i;
  bool flg = false;

  // バッファに最後に登録されている音(今鳴っている)か
  if(NoteBuf[NoteCnt-1] == note) {
    // リストを詰める
    NoteCnt--;
  } else {
    // リストから探して削除
    for(i = 0; i < NoteCnt; i++) {
      if(flg) {
        NoteBuf[i-1] = NoteBuf[i];
      }
      if(NoteBuf[i] == note) {
        flg = true;
      }
    }
    if(flg) NoteCnt--;
  }

  if(NoteCnt == 0) {
    // バッファがカラの場合はGATEオフ
    keyOff();
  } else {
    // 二番目に新しかった音を鳴らす
    keyOn(NoteBuf[NoteCnt-1]);
  }
}

// ノートオン
void NoteOn(byte note) {
  unsigned char i;

  // バッファに登録済のノートは無視
  for(i = 0; i < NoteCnt; i++) {
    if(NoteBuf[i] == note) {
      return;
    }
  }

  // バッファがいっぱい？
  if(NoteCnt == MAX_NOTE_CNT) {
    // 玉突き処理
    for(i = 0; i < (MAX_NOTE_CNT - 1); i++) {
      NoteBuf[i] = NoteBuf[i+1];
    }
    NoteBuf[MAX_NOTE_CNT - 1] = note;
  }
  else {
    NoteBuf[NoteCnt] = note;
    NoteCnt++;
  }

  keyOn(note);
}

// ノートオフハンドラ
void handleNoteOff(byte channel, byte note, byte velocity) {
  if (Rch != channel) {
    return;
  }

  NoteOff(note);
}

// ノートオンハンドラ
void handleNoteOn(byte channel, byte note, byte velocity) {
  if (Rch != channel) {
    return;
  }

  if (velocity == 0) {
    NoteOff(note);
  } else {
    NoteOn(note);
  }
}

// システムリセット
void handleSystemReset(void) {
  keyOff();
  NoteCnt = 0;
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

  keyOff();

  MIDI.setHandleNoteOff(handleNoteOff);
  MIDI.setHandleNoteOn(handleNoteOn);
  MIDI.setHandleSystemReset(handleSystemReset);

  MIDI.begin(MIDI_CHANNEL_OMNI);
  MIDI.turnThruOff();
}

void loop() {
  if (NoteCnt == 0) {
    Rch = 0x10 - ((PINB >> 2) & 0x0f);
  }

  MIDI.read();
}
