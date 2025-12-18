#include "button.h"

void updateButton(Button &btn, int pin) {

  btn.S = !digitalRead(pin);

  btn.HR = 0;
  btn.P = 0;
  btn.HO = 0;

  //-------------------------НАЖАТИЕ--------------------------
  // нажали (с антидребезгом)
  if (btn.S && !btn.F && millis() - btn.btn_timer > debounce) {
    btn.F = 1;
    btn.HO = 1;
    btn.btn_timer = millis();
    btn.hold_timer = millis();  // Инициализация таймера повторения
  }

  //-------------------------ОТПУСКАНИЕ--------------------------
  // если отпустили до hold, считать отпущенной
  if (!btn.S && btn.F && !btn.R && millis() - btn.btn_timer < hold) {
    btn.R = 1;
    btn.F = 0;
  }

  // если отпустили считать 1 нажатием
  if (btn.R) {
    btn.R = 0;
    btn.P = 1;
  }

  //-------------------------УДЕРЖАНИЕ--------------------------
  // Если удерживается более hold, то считать удержанием
  if (btn.F && !btn.H && millis() - btn.btn_timer > hold) {
    btn.H = 1;
    btn.hold_timer = millis();  // Сброс таймера повторения при начале удержания
  }

  // Повторение при удержании
  if (btn.H && millis() - btn.hold_timer > hold_repeat) {
    btn.HR = 1;                 // Устанавливаем флаг повторения
    btn.hold_timer = millis();  // Сбрасываем таймер для следующего повторения
  }

  // Если отпущена после hold, то считать, что была удержана
  if (!btn.S && btn.F && millis() - btn.btn_timer > hold) {
    btn.F = 0;
    btn.H = 0;
    btn.btn_timer = millis();
  }
}
