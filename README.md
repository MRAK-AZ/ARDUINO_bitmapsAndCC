
![PROJECT_PHOTO](https://github.com/MRAK-AZ/ARDUINO_bitmapsAndCC/blob/main/ProjectPhoto.png)

# Arduino Bitmaper AND ControllCenter

## Описание
Прошивка для Arduino Nano ATmega328, предназначенная для чтения bitmap-изображений с возможностью управления периферией.

**Примечание:** Вероятно, этот проект разделится на два разных, но пока у меня не хватает времени:
1. Читалка битмапов и текста + дополнительные функции
2. Конструктор для создания панели управления на основе Arduino и OLED дисплея

## Функционал
- **Меню чтения битмапов**
- **Меню изменения яркости дисплея**
- **Меню управления пином** - контроль состояния GPIO-пина
- **Меню Controll Center** - управление дополнительными параметрами (в разработке)

## Управление

### Основное меню:
- **UP** - перемещение вверх/вперёд
- **DOWN** - перемещение вниз/назад
- **OK** - выбор пункта меню
- **Удержание OK** - выход из подменю

### В подменю:
- **UP** - увеличить значение параметра / движение вперёд
- **DOWN** - уменьшить значение параметра / движение назад
- **OK** - инвертировать состояние
- **Удержание OK** - возврат в предыдущее меню

## Настройки в коде

```cpp
#define OLED_SOFT_BUFFER_64
#include <GyverOLED.h> // Библиотека дисплея
GyverOLED<SSD1306_128x64, OLED_BUFFER> oled; // Определение типа матрицы

// Настройка кнопок
#include "button.h"
#define PIN_OK 2   // Пин кнопки OK
#define PIN_UP 3   // Пин кнопки UP
#define PIN_DOWN 4 // Пин кнопки DOWN
Button btnOK, btnUP, btnDOWN;

// -------------------- BITMAP's -------------------- //
#include "bitmap.h" // Файл с битмапами
const uint8_t* const bitmaps[] PROGMEM = { bmp1, bmp2 }; // Инициализация битмапов
#define BITMAPS_COUNT (sizeof(bitmaps) / sizeof(bitmaps[0])) // Автоматический расчет количества битмапов

// ------------------- GENERAL MENU -----------------------//
#define ITEMS 4 // Количество пунктов в главном меню | УКАЗЫВАТЬ ОБЯЗАТЕЛНО !!

const char i0[] PROGMEM = "BITMAP GALLERY"; // Пункт 1
const char i1[] PROGMEM = "BRIGHTNESS";     // Пункт 2
const char i2[] PROGMEM = "PIN STATE";      // Пункт 3
const char i3[] PROGMEM = "CC MENU";        // Пункт 4

const char* const gNames[] PROGMEM = {
  i0, i1, i2, i3
};

// ------------------- Controll Center MENU -----------------------//
#define CC_ITEMS 4 // Количество пунктов в Controll Center | УКАЗЫВАТЬ ОБЯЗАТЕЛНО !!

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

STdata display = { 0, 255, 0, 0 }; // Инициализация структуры дисплея
STdata CC = { 0, 0, 0, 1 };        // Инициализация структуры Controll Center
```

## Добавление BITMAP'ов

Для добавления собственных Bitmap'ов необходимо:

1. **Конвертировать изображение в массив значений:**
   ```cpp
   const uint8_t bmpX[] PROGMEM = {...}; // Где X - номер битмапа
   ```

2. **Использовать утилиту "image-processor":**
   - Утилита находится в директории проекта (заимствована из [microReader](https://github.com/Nich1con/microReader/tree/main))
   - Запустите программу, выберите изображение
   - Настройте параметры для хорошей читаемости
   - Скопируйте массив данных (левая панель, нижний правый угол)

3. **Интеграция в код:**
   - Вставьте скопированный массив в файл `bitmap.h`
   - Измените название массива на `bmpX`
   - Добавьте новый битмап в массив инициализации:
   ```cpp
   const uint8_t* const bitmaps[] PROGMEM = { bmp1, bmp2, bmpX };
   ```

## Используемые библиотеки

- **[GyverOLED](https://github.com/GyverLibs/GyverOLED/)**

**Примечание:** В данной прошивке используется модифицированная версия библиотеки GyverOLED.
