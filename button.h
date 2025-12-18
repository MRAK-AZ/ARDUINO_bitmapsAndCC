#pragma once
#include <Arduino.h>

struct Button {
  bool S;                    // храним состояния кнопок (S - State)
  bool F;                    // флажки кнопок (F - Flag)
  bool R;                    // флажки кнопок на отпускание (R - Release)
  bool P;                    // флажки кнопок на нажатие (P - Press)
  bool H;                    // флажки кнопок на удержание (многократный вызов) (H - Hold)
  bool HO;                   // флажки кнопок на удержание (один вызов при нажатии) (HO - Hold Once)
  bool HR;                   // флажок повторения удержания (HR - Hold Repeat)
  unsigned long btn_timer;   // таймер последнего нажатия кнопки
  unsigned long hold_timer;  // таймер для повторения удержания
};

#define hold 200         // время (мс), после которого кнопка считается зажатой
#define debounce 20      // (мс), антидребезг
#define hold_repeat 200  // период (мс) между повторениями при удержании

// --- функции ---
void updateButton(Button &btn, int pin);
