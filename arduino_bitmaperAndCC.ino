#define OLED_SOFT_BUFFER_64
#include <GyverOLED.h>                        // Библиотека дисплея
GyverOLED<SSD1306_128x64, OLED_BUFFER> oled;  // Определение типа матрицы

// Настройка кнопок
#include "button.h"
#define PIN_OK 2    // Пин кнопки OK
#define PIN_UP 3    // Пин кнопки UP
#define PIN_DOWN 4  // Пин кнопки DOWN
Button btnOK, btnUP, btnDOWN;

#define PIN_CONTROL 5  // пин для управления

// -------------------- BITMAP's -------------------- //
#include "bitmap.h"                                             // Файл с битмапами
const uint8_t* const bitmaps[] PROGMEM = { bmp1, bmp2, bmp3 };  // Инициализация битмапов
#define BITMAPS_COUNT (sizeof(bitmaps) / sizeof(bitmaps[0]))    // Автоматический расчет количества битмапов

// ------------------- GENERAL MENU -----------------------//
#define ITEMS 4  // Количество пунктов в главном меню | УКАЗЫВАТЬ ОБЯЗАТЕЛНО !!

const char i0[] PROGMEM = "BITMAP GALLERY";  // Пункт 1
const char i1[] PROGMEM = "BRIGHTNESS";      // Пункт 2
const char i2[] PROGMEM = "PIN STATE";       // Пункт 3
const char i3[] PROGMEM = "CC MENU";         // Пункт 4

const char* const gNames[] PROGMEM = {
  i0, i1, i2, i3
};

// ------------------- Controll Center MENU -----------------------//
#define CC_ITEMS 4  // Количество пунктов в Controll Center | УКАЗЫВАТЬ ОБЯЗАТЕЛНО !!

const char icc0[] PROGMEM = "Parameter 1";
const char icc1[] PROGMEM = "Parameter 2";
const char icc2[] PROGMEM = "Parameter 3";
const char icc3[] PROGMEM = "Parameter 4";

const char* const ccNames[] PROGMEM = {
  icc0, icc1, icc2, icc3
};

// ------------------- Структуры данных -----------------------//
struct STdata {
  uint8_t pointer : 4;
  uint8_t brightness : 8;
  uint8_t i : 4;
  bool invertF : 1;
};

STdata display = { 0, 255, 0, 0 };  // Инициализация структуры дисплея
STdata CC = { 0, 0, 0, 1 };         // Инициализация структуры Controll Center

void setup() {
  pinMode(PIN_OK, INPUT_PULLUP);
  pinMode(PIN_UP, INPUT_PULLUP);
  pinMode(PIN_DOWN, INPUT_PULLUP);

  pinMode(PIN_CONTROL, OUTPUT);

  oled.init();
  Wire.setClock(1000000L);
  oled.setContrast(display.brightness);
  oled.setScale(1);
}

void loop() {
  updateAllButtoin();

  if ((btnUP.P) || (btnUP.HR)) {
    display.pointer = constrain(display.pointer - 1, 0, ITEMS - 1);
  }

  if ((btnDOWN.P) || (btnDOWN.HR)) {
    display.pointer = constrain(display.pointer + 1, 0, ITEMS - 1);
  }

  if (btnOK.P) {
    switch (display.pointer) {
      case 0: showBitmaps(); break;
      case 1: adjustBrightness(); break;
      case 2: addPIN(); break;
      case 3: ControllCenter(); break;
    }
  }

  oled.clear();
  oled.home();

  for (display.i = 0; display.i < ITEMS; display.i++) {
    printMenuItem(display.i, display.pointer, gNames);
  }

  oled.update();
}

void ControllCenter() {

  while (1) {
    updateAllButtoin();

    if ((btnUP.P) || (btnUP.HR)) {
      CC.pointer = constrain(CC.pointer - 1, 0, CC_ITEMS - 1);
    }

    if ((btnDOWN.P) || (btnDOWN.HR)) {
      CC.pointer = constrain(CC.pointer + 1, 0, CC_ITEMS - 1);
    }

    if (btnOK.P) {
      switch (CC.pointer) {
        case 0: break;
        case 1: break;
        case 2: break;
      }
    }

    if (btnOK.H) return;
    oled.clear();
    oled.home();

    for (CC.i = 0; CC.i < CC_ITEMS; CC.i++) {
      printMenuItem(CC.i, CC.pointer, ccNames);
    }

    oled.update();
  }
}

void adjustBrightness() {
  while (1) {
    updateAllButtoin();

    if ((btnUP.P) || (btnUP.HR)) {
      display.brightness = constrain(display.brightness + 25, 25, 255);
      oled.setContrast(display.brightness);
    }

    if ((btnDOWN.P) || (btnDOWN.HR)) {
      display.brightness = constrain(display.brightness - 25, 25, 255);
      oled.setContrast(display.brightness);
    }

    oled.clear();
    oled.setCursor(0, 2);
    oled.print(F("BRIGHTNESS"));
    oled.setCursor(0, 4);
    oled.print(F("VALUE - "));
    oled.print(display.brightness);
    oled.print(F("/255"));
    oled.update();

    if ((btnOK.HR)) return;
  }
}

void addPIN() {
  while (1) {
    updateAllButtoin();

    if (CC.invertF) {
      if ((btnUP.P) || (btnUP.HR)) {
        CC.brightness = constrain(CC.brightness + 15, 0, 255);
        analogWrite(PIN_CONTROL, CC.brightness);
      }

      if ((btnDOWN.P) || (btnDOWN.HR)) {
        CC.brightness = constrain(CC.brightness - 15, 0, 255);
        analogWrite(PIN_CONTROL, CC.brightness);
      }
    }

    if (btnOK.P) {
      if (CC.invertF) {
        analogWrite(PIN_CONTROL, 0);
      } else {
        analogWrite(PIN_CONTROL, CC.brightness);
      }
      CC.invertF = !CC.invertF;
    }

    oled.clear();
    oled.setCursor(0, 1);
    oled.print(F("PIN 1"));
    oled.setCursor(0, 3);
    oled.print(F("STATE - "));
    if (CC.invertF) oled.print(F("ON"));
    else oled.print(F("OFF"));
    oled.setCursor(0, 5);
    oled.print(F("VALUE - "));
    oled.print(CC.brightness);
    oled.print(F("/255"));

    oled.update();

    if ((btnOK.HR)) return;
  }
}

// --------- ПОДМЕНЮ ЛИСТАНИЯ БИТМАПОВ 128x64 ----------
void showBitmaps() {
  int8_t index = 0;

  while (1) {
    updateAllButtoin();

    if ((btnUP.P) || (btnUP.HR)) {
      index = constrain(index + 1, 0, BITMAPS_COUNT - 1);
    }

    if ((btnDOWN.P) || (btnDOWN.HR)) {
      index = constrain(index - 1, 0, BITMAPS_COUNT - 1);
    }

    if ((btnOK.P)) display.invertF = !display.invertF;
    if ((btnOK.HR)) {
      btnOK.HR = 0;
      return;
    }

    oled.clear();
    const uint8_t* bmp = (const uint8_t*)pgm_read_word(&(bitmaps[index]));
    oled.drawBitmap(0, 0, bmp, 128, 64, display.invertF);
    oled.update();
  }
}

void printMenuItem(uint8_t num, uint8_t pointer, const char* const* names) {
  char buffer[21];
  uint16_t ptr = pgm_read_word(&(names[num]));
  uint8_t i = 0;

  do {
    buffer[i] = (char)(pgm_read_byte(ptr++));
  } while (buffer[i++] != NULL);

  if (num == pointer) oled.print(F("> "));

  oled.println(buffer);
}

void updateAllButtoin() {
  updateButton(btnOK, PIN_OK);
  updateButton(btnUP, PIN_UP);
  updateButton(btnDOWN, PIN_DOWN);
}
