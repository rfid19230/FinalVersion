// Определение состояний системы
#define STATE_SETUP               0 // Состояние инициализации
#define STATE_CONN_NET            1 // Состояние соединения с сетью
#define STATE_CONN_NET_ERR        2 // Состояние ошибки соединения с сетью
#define STATE_CONN_SRV            3 // Состояние соединения с сервером
#define STATE_CONN_SRV_ERR        4 // Состояние ошибки соединения с сервером
#define STATE_WAITING_FOR_PASS    5 // Состояние ожидания пропуска
#define STATE_CHOOSE_ACTION       6 // Состояние выбора действия
#define STATE_CHOOSE_CELL         7 // Состояние выбора ячейки
#define STATE_WAITING_FOR_VALUE   8 // Состояние ожидания ценности
#define STATE_CONFIRM_ACTION      9 // Состояние подтверждения действия

/*
 * ====================================================================================================
 */

// Для обмена данными с сервером по Ethernet
#include <Ethernet.h>

// Задержки между попытками повторного соединения с сетью/сервером при ошибке соединения
#define CONN_NET_DELAY 10000
#define CONN_SRV_DELAY 10000

// Типы отправляемых на сервер сообщений
#define MSG_CHECK_CONN  '0' // Сообщение проверки соединения
#define MSG_CELLS_TAKE  '1' // Запрос ячеек, доступных для взятия
#define MSG_CELLS_RET   '2' // Запрос ячеек, доступных для возврата
#define MSG_PASS_ID     '3' // Запрос на подтверждение валидности пропуска
#define MSG_VALUE_ID    '4' // Запрос на подтверждение валидности ценности
#define MSG_END_SESSION '5' // Сообщение об окончании работы пользователя с системой

// Ответы на запрос о подтверждении валидности пропуска
#define REP_VALID_PASS   '0'
#define REP_INVALID_PASS '1'

// Ответы на запрос о подтверждении валидности ценности
#define REP_VALID_VALUE   '0'
#define REP_INVALID_VALUE '1'
#define REP_WRONG_TYPE    '2'

EthernetClient client; // Объект библиотечного класса для соединения с сервером
const byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED}; // МАС-адрес контроллера
const byte srv_ip[] = {172, 18, 160, 26}; // IP-адрес сервера
const int srv_port = 19230; // Порт сервера

/*
 * Функция, преобразующая номер ячейки из числового формата в знаковый. 
 * Входной параметр: номер ячейки в виде числа
 * Выходной параметр: номер ячейки в виде символа
 */
char cellToChar(int c) {
  if (c == 1) return '1';
  if (c == 2) return '2';
  if (c == 3) return '3';
  if (c == 4) return '4';
  if (c == 5) return '5';
  if (c == 6) return '6';
  if (c == 7) return '7';
  if (c == 8) return '8';
}

/*
 * ====================================================================================================
 */

// Для графического дисплея
#include <UTFT.h>
#include <URTouch.h>

// Минимальные и максимальные координаты дисплея по осям
#define X_MIN 0
#define X_MAX 319

#define Y_MIN 0
#define Y_MAX 239

// Максимальные и минимальные значения АЦП по осям, а также калибровочные константы для различных частей экрана
#define TPX_MIN 0
#define TPX_MAX 4000
#define TPXD_T 4000
#define TPXD_B 4500

#define TPY_MIN 0
#define TPY_MAX 4000
#define TPYD_L 3800
#define TPYD_R 4000

// Цвет фона
#define BC_R 0
#define BC_G 0
#define BC_B 0

// Цвет большинства надписей и кнопок
#define FC_R 255
#define FC_G 255
#define FC_B 0

UTFT disp(TFT01_24SP,  30, 32, 24, 26, 28); // Объект библиотечного класса для отображения информации на дисплее
URTouch ts(38, 40, 42, 44, 46); // Объект библиотечного класса для считывания информации с дисплея

int x, y; // Для хранения координат по осям
int16_t tpx, tpy; // Для хранения уровней АЦП по осям

extern uint8_t BigFont[]; // Подключение шрифта

/*
 * Функция, преобразующая уровень АЦП по оси Y в координату по оси Х.
 * Входной параметр: уровень АЦП по оси Y.
 * Выходной параметр: координата по оси X.
 */
int tpyToX(int16_t tpy)
{
  if (tpy < TPY_MIN)
    tpy = TPY_MIN;

  if (tpy > TPY_MAX)
    tpy = TPY_MAX;

  int tpyd;
  if (int(tpy) <= int(TPY_MAX / 2))
    tpyd = TPYD_R;
  else
    tpyd = TPYD_L;
  
  return int( double(X_MAX) * ( 1.0 - double(tpy) / double(tpyd) ) );
}

/*
 * Функция, преобразующая уровень АЦП по оси X в координату по оси Y.
 * Входной параметр: уровень АЦП по оси X.
 * Выходной параметр: координата по оси Y.
 */
int tpxToY(int16_t tpx)
{
  if (tpx < TPX_MIN)
    tpx = TPX_MIN;

  if (tpx > TPX_MAX)
    tpx = TPX_MAX;

  int tpxd;
  if (int(tpx) <= int(TPX_MAX / 2))
    tpxd = TPXD_B;
  else
    tpxd = TPXD_T;
  
  return int( double(Y_MAX) * ( 1.0 - double(tpx) / double(tpxd) ) );
}

/*
 * Далее идет серия функций, предназначенная для отображения различных надписей и кнопок, 
 * а также считывания нажатий на дисплей.
 */

/*
 * Функция, выводящая на экран дисплея надпись "В ожидании пропуска".
 * Входные параметры: составляющие цвета, которым необходимо отобразить надпись, в модели RGB.
 * Выходных параметров нет.
 */
void printWFP(int r, int g, int b)
{
  disp.setColor(r, g, b);
  disp.print("WAITING", 103, 24);
  disp.print("FOR", 135, 84);
  disp.print("PASS", 127, 144);
}

/*
 * Функция, выводящая на экран дисплея надпись "Доступ разрешен".
 * Входные параметры: составляющие цвета, которым необходимо отобразить надпись, в модели RGB.
 * Выходных параметров нет.
 */
void printAG(int r, int g, int b)
{
  disp.setColor(r, g, b);
  disp.print("ACCESS GRANTED", 47, 204);
}

/*
 * Функция, выводящая на экран дисплея надпись "Доступ не разрешен".
 * Входные параметры: составляющие цвета, которым необходимо отобразить надпись, в модели RGB.
 * Выходных параметров нет.
 */
void printAD(int r, int g, int b)
{
  disp.setColor(r, g, b);
  disp.print("ACCESS DENIED", 55, 204);
}

/*
 * Функция, выводящая на экран дисплея кнопки "Взятие", "Возврат", "Выход".
 * Входные параметры: составляющие цвета, которым необходимо отобразить элементы, в модели RGB.
 * Выходных параметров нет.
 */
void printTRQ(int r, int g, int b)
{
  disp.setColor(r, g, b);
  
  disp.print("CHOOSE ACTION", 55, 7);
  
  disp.drawRoundRect(50, 40, 270, 90);
  disp.print("TAKE", 127, 57);

  disp.drawRoundRect(50, 110, 270, 160);
  disp.print("RETURN", 111, 127);

  disp.drawRoundRect(50, 180, 270, 230);
  disp.print("QUIT", 127, 197);
}

/*
 * Функция, по координатам нажатия на дисплей определяющая нажатую кнопку.
 * Входные параметры: координаты нажатия.
 * Выходной параметр: номер нажатой кнопки.
 */
int touchTRQ(int x, int y)
{
  int res = 0;

  if (x >= 50 && x <= 270 && y >= 40 && y <= 90)
    res = 1; // TAKE

  if (x >= 50 && x <= 270 && y >= 110 && y <= 160)
    res = 2; // RETURN

  if (x >= 50 && x <= 270 && y >= 180 && y <= 230)
    res = 3; // QUIT

  return res;
}

/*
 * Функция, выводящая на экран дисплея кнопки выбора ячеек, а также кнопки "Назад" и "Выход".
 * Входные параметры: составляющие цвета, которым необходимо отобразить элементы, в модели RGB, а также указание, какие ячейки доступны.
 * Выходных параметров нет.
 */
void printCBQ(int r, int g, int b, int c_en[8])
{
  disp.setColor(r, g, b);

  disp.print("CHOOSE CELL", 71, 7);

  if (c_en[0] == 1) {
    disp.drawRoundRect(10, 40, 70, 100);
    disp.print("1", 32, 62);
  }

  if (c_en[1] == 1) {
    disp.drawRoundRect(90, 40, 150, 100);
    disp.print("2", 112, 62);
  }

  if (c_en[2] == 1) {
    disp.drawRoundRect(170, 40, 230, 100);
    disp.print("3", 192, 62);
  }

  if (c_en[3] == 1) {
    disp.drawRoundRect(250, 40, 310, 100);
    disp.print("4", 272, 62);
  }

  if (c_en[4] == 1) {
    disp.drawRoundRect(10, 120, 70, 180);
    disp.print("5", 32, 142);
  }

  if (c_en[5] == 1) {
    disp.drawRoundRect(90, 120, 150, 180);
    disp.print("6", 112, 142);
  }

  if (c_en[6] == 1) {
    disp.drawRoundRect(170, 120, 230, 180);
    disp.print("7", 192, 142);
  }

  if (c_en[7] == 1) {
    disp.drawRoundRect(250, 120, 310, 180);
    disp.print("8", 272, 142);
  }

  disp.drawRoundRect(40, 200, 120, 230);
  disp.print("BACK", 48, 207);
  
  disp.drawRoundRect(200, 200, 280, 230);
  disp.print("QUIT", 208, 207);
}

/*
 * Функция, по координатам нажатия на дисплей определяющая, какая кнопка была нажата.
 * Входные параметры: составляющие цвета, которым необходимо отобразить надпись, в модели RGB, а также указание, какие ячейки доступны.
 * Выходной параметр: номер нажатой кнопки.
 */
int touchCBQ(int x, int y, int c_en[8])
{
  int res = 0;

  if (x >= 10 && x <= 70 && y >= 40 && y <= 100 && c_en[0] == 1)
    res = 1; // cell 1

  if (x >= 90 && x <= 150 && y >= 40 && y <= 100 && c_en[1] == 1)
    res = 2; // cell 2

  if (x >= 170 && x <= 230 && y >= 40 && y <= 100 && c_en[2] == 1)
    res = 3; // cell 3

  if (x >= 250 && x <= 310 && y >= 40 && y <= 100 && c_en[3] == 1)
    res = 4; // cell 4

  if (x >= 10 && x <= 70 && y >= 120 && y <= 180 && c_en[4] == 1)
    res = 5; // cell 5

  if (x >= 90 && x <= 150 && y >= 120 && y <= 180 && c_en[5] == 1)
    res = 6; // cell 6

  if (x >= 170 && x <= 230 && y >= 120 && y <= 180 && c_en[6] == 1)
    res = 7; // cell 7

  if (x >= 250 && x <= 310 && y >= 120 && y <= 180 && c_en[7] == 1)
    res = 8; // cell 8

  if (x >= 40 && x <= 120 && y >= 200 && y <= 230)
    res = 9; // BACK

  if (x >= 200 && x <= 280 && y >= 200 && y <= 230)
    res = 10; // QUIT

  return res;
} 

/*
 * Функция, выводящая на экран дисплея надпись "Инициализация компонентов".
 * Входные параметры: составляющие цвета, которым необходимо отобразить надпись, в модели RGB.
 * Выходных параметров нет.
 */
void printIC(int r, int g, int b)
{
  disp.setColor(r, g, b);

  disp.print("INITIALIZING", 63, 52);
  disp.print("COMPONENTS", 79, 172);
}

/*
 * Функция, выводящая на экран дисплея надпись "Ошибка соединения с сетью", а также кнопку "Перезагрузка".
 * Входные параметры: составляющие цвета, которым необходимо отобразить элементы, в модели RGB.
 * Выходных параметров нет.
 */
void printNCE(int r, int g, int b)
{
  disp.setColor(r, g, b);

  disp.print("NET", 135, 22);
  disp.print("CONNECTION", 79, 82);
  disp.print("ERROR", 119, 142);

  disp.drawRoundRect(10, 190, 310, 230);
  disp.print("RESET", 119, 202);
}

/*
 * Функция, по координатам нажатия на дисплей определяющая, какая кнопка была нажата.
 * Входные параметры: координаты нажатия.
 * Выходной параметр: номер нажатой кнопки.
 */
int touchNCE(int x, int y)
{
  int res = 0;

  if (x >= 10 && x <= 310 && y >= 190 && y <= 230)
    res = 1; // RESET

  return res;
}

/*
 * Функция, выводящая на экран дисплея надпись "Ошибка соединения с сервером", а также кнопку "Перезагрузка".
 * Входные параметры: составляющие цвета, которым необходимо отобразить элементы, в модели RGB.
 * Выходных параметров нет.
 */
void printSCE(int r, int g, int b)
{
  disp.setColor(r, g, b);

  disp.print("SERVER", 111, 22);
  disp.print("CONNECTION", 79, 82);
  disp.print("ERROR", 119, 142);

  disp.drawRoundRect(10, 190, 310, 230);
  disp.print("RESET", 119, 202);
}

/*
 * Функция, по координатам нажатия на дисплей определяющая, какая кнопка была нажата.
 * Входные параметры: координаты нажатия.
 * Выходной параметр: номер нажатой кнопки.
 */
int touchSCE(int x, int y)
{
  int res = 0;

  if (x >= 10 && x <= 310 && y >= 190 && y <= 230)
    res = 1; // RESET

  return res;
}

/*
 * Функция, выводящая на экран дисплея надпись "Закройте ячейку и нажмите ОК", а также кнопку "ОК".
 * Входные параметры: составляющие цвета, которым необходимо отобразить элементы, в модели RGB.
 * Выходных параметров нет.
 */
void printCOK(int r, int g, int b)
{
  disp.setColor(r, g, b);

  disp.print("CLOSE CELL", 79, 22);
  disp.print("AND", 135, 82);
  disp.print("PRESS OK", 95, 142);

  disp.drawRoundRect(100, 190, 220, 230);
  disp.print("OK", 143, 202);
}

/*
 * Функция, по координатам нажатия на дисплей определяющая, какая кнопка была нажата.
 * Входные параметры: координаты нажатия.
 * Выходной параметр: номер нажатой кнопки.
 */
int touchCOK(int x, int y)
{
  int res = 0;

  if (x >= 100 && x <= 220 && y >= 190 && y <= 230)
    res = 1; // OK

  return res;
}

/*
 * Функция, выводящая на экран дисплея надпись "В ожидании ценности", а также кнопки "Назад" и "Выход".
 * Входные параметры: составляющие цвета, которым необходимо отобразить элементы, в модели RGB.
 * Выходных параметров нет.
 */
void printWFV(int r, int g, int b)
{
  disp.setColor(r, g, b);
  disp.print("WAITING", 103, 24);
  disp.print("FOR VALUE", 87, 84);
  
  disp.drawRoundRect(50, 130, 150, 170);
  disp.print("BACK", 68, 144);

  disp.drawRoundRect(170, 130, 270, 170);
  disp.print("QUIT", 188, 144);
}

/*
 * Функция, по координатам нажатия на дисплей определяющая, какая кнопка была нажата.
 * Входные параметры: координаты нажатия.
 * Выходной параметр: номер нажатой кнопки.
 */
int touchWFV(int x, int y)
{
  int res = 0;

  if (x >= 50 && x <= 150 && y >= 130 && y <= 170)
    res = 1; // BACK

  if (x >= 170 && x <= 270 && y >= 130 && y <= 170)
    res = 2; // QUIT

  return res;
}

/*
 * Функция, выводящая на экран дисплея надпись "Валидная ценнось".
 * Входные параметры: составляющие цвета, которым необходимо отобразить надпись, в модели RGB.
 * Выходных параметров нет.
 */
void printVV(int r, int g, int b)
{
  disp.setColor(r, g, b);
  disp.print("VALID VALUE", 71, 204);
}

/*
 * Функция, выводящая на экран дисплея надпись "Не валидная ценность".
 * Входные параметры: составляющие цвета, которым необходимо отобразить надпись, в модели RGB.
 * Выходных параметров нет.
 */
void printIV(int r, int g, int b)
{
  disp.setColor(r, g, b);
  disp.print("INVALID VALUE", 55, 204);
}

/*
 * Функция, выводящая на экран дисплея надпись "Неправильный тип (ценности)".
 * Входные параметры: составляющие цвета, которым необходимо отобразить надпись, в модели RGB.
 * Выходных параметров нет.
 */
void printWT(int r, int g, int b)
{
  disp.setColor(r, g, b);
  disp.print("WRONG TYPE", 79, 204);
}

/*
 * ====================================================================================================
 */

// Для считывателя меток
/* 
 *  Содержимое UID используемых меток
 * 0 символ - знак начала передачи (числовой код 2) 
 * 1-2 символы - служебные
 * 3-10 символы - собственно UID
 * 11-12 символы - проверочные
 * 13 символ - знак конца передачи (числовой код 3)
 */
#define UID_LEN 14
#define UID_FROM 3
#define UID_TO 10

char pid[UID_LEN]; // Для хранения ID пропуска
char vid[UID_LEN]; // Для хранения ID ценности

/*
 * ====================================================================================================
 */

// Пины, к которым подключены замки
#define L1 23
#define L2 25
#define L3 27
#define L4 29
#define L5 31
#define L6 33
#define L7 35
#define L8 37

int locks [] = {L1, L2, L3, L4, L5, L6, L7, L8}; // Для удобства работы с замками
const int locks_amount = 8; // Количество замков, оно же используется как количество ячеек

/*
 * Функция, выполняющая закрытие всех замков.
 * Входных и выходных параметров нет.
 */
void lockAll () {
  for (int i = 0; i < locks_amount; ++i)
  {
    digitalWrite(locks[i], LOW);
  }
}

/*
 * Функция, выполняющая открытие всех замков.
 * Входных и выходных параметров нет.
 */
void unlockAll () {
  for (int i = 0; i < locks_amount; ++i)
  {
    digitalWrite(locks[i], HIGH);
  }
}

/*
 * Функция, выполняющая закрытие определенного замка.
 * Входной параметр: номер ячейки.
 * Выходных параметров нет.
 */
void lock(int cell_number) {
  digitalWrite(locks[cell_number], LOW);
}

/*
 * Функция, выполняющая открытие определенного замка.
 * Входной параметр: номер ячейки.
 * Выходных параметров нет.
 */
void unlock(int cell_number) {
  digitalWrite(locks[cell_number], HIGH);
}

/*
 * ====================================================================================================
 */

// Прочее для умного шкафа
#include <SPI.h>

#define TIMEOUT 30000 // Максимальное время бездействия в течение сеанса работы с системой
#define CONN_NET_ERR_DELAY 60000 // Задержка до перехода в состояние инициализации из состояния ошибки соединения с сетью
#define CONN_SRV_ERR_DELAY 60000 // Задержка до перехода в состояние инициализации из состояния ошибки соединения с сервером

// ID шкафа и его длина
const char cid[] = {'M', 'A', '0', '7', '0', '1'};
const int cid_length = 6;

// Для хранения текущего и предыдущего состояния
int state;
int prev_state;

// Возможные действия и переменная для хранения текущего действия
#define ACT_TAKE '1'
#define ACT_RET  '2'
char action; // 2 == RETURN, 1 == TAKE

// Для хранения информации о текущей ячейке и доступных ячейках
int cell;
int cells_enable[locks_amount];
#define CELL_ENABLED  '1'
#define CELL_DISABLED '0'

// Для вычисления задержек
unsigned long m, curr_m;

// Две функции для очистки последовательных портов №0 и №1

void clearSerial() {
  while (Serial.available()) {
    Serial.read();
  }
}

void clearSerial1() {
  while (Serial1.available()) {
    Serial1.read();
  }
}

/*
 * ====================================================================================================
 */

// Инициализация компонентов

void setup() {
  // Запуск последовательных портов
  Serial.begin(9600);
  while (!Serial) delay(10);
  Serial1.begin(9600);
  while (!Serial1) delay(10);

  Serial.println("In STATE_SETUP");

  // Инициализация дисплея
  disp.InitLCD(LANDSCAPE);
  disp.setFont(BigFont);
  disp.setBackColor(BC_R, BC_G, BC_B);
  disp.setColor(BC_R, BC_G, BC_B);
  disp.fillRect(X_MIN, Y_MIN, X_MAX, Y_MAX);
  
  ts.InitTouch(LANDSCAPE);
  ts.setPrecision(PREC_HI); 

  printIC(FC_R, FC_G, FC_B);

  // Установка режима для пинов замков и закрытие замков
  for (int i = 0; i < locks_amount; ++i) {
    pinMode(locks[i], OUTPUT);
  }

  lockAll();

  // Инициализация состояния системы
  prev_state = STATE_SETUP;
  state = STATE_CONN_NET;

}

/*
 * ====================================================================================================
 */

// Логика работы системы

void loop() {
  Serial.println("In LOOP");

  switch (state) {
    
    case STATE_CONN_NET:
      Serial.println("In switch, STATE_CONN_NET");
      connNet();
    break;

    case STATE_CONN_NET_ERR:
      Serial.println("In switch, STATE_CONN_NET_ERR");
      connNetErr();
    break;

    case STATE_CONN_SRV:
      Serial.println("In switch, STATE_CONN_SRV");
      connSrv();
    break;

    case STATE_CONN_SRV_ERR:
      Serial.println("In switch, STATE_CONN_SRV_ERR");
      connSrvErr();
    break;

    case STATE_WAITING_FOR_PASS:
      Serial.println("In switch, STATE_WAITING_FOR_PASS");
      waitingForPass();
    break;

    case STATE_CHOOSE_ACTION:
      Serial.println("In switch, STATE_CHOOSE_ACTION");
      chooseAction();
    break;

    case STATE_CHOOSE_CELL:
      Serial.println("In switch, STATE_CHOOSE_CELL");
      chooseCell();
    break;

     case STATE_WAITING_FOR_VALUE:
      Serial.println("In switch, STATE_WAITING_FOR_VALUE");
      waitingForValue();
    break;

    case STATE_CONFIRM_ACTION:
      Serial.println("In switch, STATE_CONFIRM_ACTION");
      confirmAction();
    break;

    default:
      Serial.println("In switch, case DEFAULT. Something went wrong, going to SETUP");
      setup();

  }
}

/*
 * ====================================================================================================
 */
// Далее следуют функции, описывающие действия в различгых состояниях. Входных и выходных параметров у них нет.

/*
 * Функция, описывающая действия в состоянии соединения с сетью
 */
void connNet() {
    // Попытка соединения с сетью
    if (Ethernet.begin(mac) == 0) {
      // При ошибке подключения
      Serial.println("Failed to start Eth");

      printIC(BC_R, BC_G, BC_B);
      
      prev_state = STATE_CONN_NET;
      state = STATE_CONN_NET_ERR;
    }
    else {
      // При успешном подключении
      Serial.print("Eth started successfully, my IP: ");
      Serial.println(Ethernet.localIP());
      
      prev_state = STATE_CONN_NET;
      state = STATE_CONN_SRV;
    }
}

/*
 * ====================================================================================================
 */

/*
 * Функция, описывающая действия в состоянии ошибки соединения с сетью
 */
void connNetErr() {
  printNCE(FC_R, FC_G, FC_B); // Отображение соответствующего экрана

  // Для расчета задержки
  m = millis();
  curr_m = m;

  int button = 0; // Для определения, какая кнопка нажата

  // Цикл до нажатия на кнопку или истечения времени
  while (curr_m - m < TIMEOUT && button == 0) {
    while (curr_m - m < TIMEOUT && !ts.dataAvailable()) {
      delay(250);
      curr_m = millis();
    }

    if (ts.dataAvailable()) {
      ts.read();
      button = touchNCE(tpyToX(ts.TP_Y), tpxToY(ts.TP_X));
    }
  }

  printNCE(BC_R, BC_G, BC_B); // Стирание соответствующего экрана
  Serial.println("Going to STATE_SETUP");
  setup();
}

/*
 * ====================================================================================================
 */

/*
 * Функция, описывающая действия в состоянии соединения с сервером
 */
void connSrv() {
  // Попытка соединения
  if (client.connect(srv_ip, srv_port)) {
    // При успешном соединении
    for (int i = 0; i < cid_length; ++i) {
      client.print(cid[i]);
    }
    client.print(';');
    client.print(MSG_CHECK_CONN);
    client.stop();

    Serial.println("Connection to srv: successful");

    prev_state = STATE_CONN_SRV;
    state = STATE_WAITING_FOR_PASS;
  }
  else {
    // При ошибке соединения
    Serial.println("Connection to srv: failed");
      
    prev_state = STATE_CONN_SRV;
    state = STATE_CONN_SRV_ERR;
  }

  printIC(BC_R, BC_G, BC_B);
}

/*
 * ====================================================================================================
 */

/*
 * Функция, описывающая действия в состоянии ошибки соединения с сервером
 */
void connSrvErr() {
  printSCE(FC_R, FC_G, FC_B); // Отображение соответствующего экрана

  // Для расчета задержки
  m = millis();
  curr_m = m;

  int button = 0;

  // Цикл до нажатия кнопки или истечения времени
  while (curr_m - m < TIMEOUT && button == 0) {
    while (curr_m - m < TIMEOUT && !ts.dataAvailable()) {
      delay(250);
      curr_m = millis();
    }

    if (ts.dataAvailable()) {
      ts.read();
      button = touchSCE(tpyToX(ts.TP_Y), tpxToY(ts.TP_X));
    }
  }

  printSCE(BC_R, BC_G, BC_B); // Стирание соответствующего экрана
  Serial.println("Going to STATE_SETUP");
  setup();
}

/*
 * ====================================================================================================
 */

/*
 * Функция, описывающая действия в состоянии ожидания пропуска
 */
void waitingForPass() {
  printWFP(FC_R, FC_G, FC_B); // Отображение соответствующего экрана

  clearSerial1(); // Очистка последовательного порта, связанного со считывателем

  // Ожидание метки пропуска
  while (Serial1.available() < UID_LEN) {
    delay(250);
  }

  // Считывание метки пропуска
  for (int i = 0; i < UID_LEN; ++i) {
    pid[i] = Serial1.read();
  }
    
  Serial.println("Pass ID was read: ");
  for (int i = UID_FROM; i <= UID_TO; ++i) {
    Serial.print(pid[i]);
  }
  Serial.println();

  // Отправка считанного ID на сервер
  if (client.connect(srv_ip, srv_port)) {
    for (int i = 0; i < cid_length; ++i) {
      client.print(cid[i]);
    }
    
    client.print(';');
    
    client.print(MSG_PASS_ID);

    client.print(';');
    
    for (int i = UID_FROM; i <= UID_TO; ++i) {
      client.print(pid[i]);
    }

    Serial.println("Pass ID was sent to server");

    while (!client.available()) {
      delay(100);
    }

    // Получение ответа
    char reply = client.read();
    client.stop();

    if (reply == REP_VALID_PASS) {
      // Если пропуск валидный
      Serial.println("Pass is valid");
      
      prev_state = STATE_WAITING_FOR_PASS;
      state = STATE_CHOOSE_ACTION;

      printAG(0, 255, 0);
      delay(2000);
      printAG(BC_R, BC_G, BC_B);
      clearSerial1();

      printWFP(BC_R, BC_G, BC_B);
    }
    
    if (reply == REP_INVALID_PASS) {
      // Если пропуск не валидный
      Serial.println("Pass is invalid");
      
      prev_state = STATE_WAITING_FOR_PASS;
      state = STATE_WAITING_FOR_PASS;

      printAD(255, 0, 0);
      delay(2000);
      printAD(BC_R, BC_G, BC_B);
      clearSerial1();
    }
  }
  else {
    // При ошибке соединения с сервером
    Serial.println("Server connection error occured");

    prev_state = STATE_WAITING_FOR_PASS;
    state = STATE_CONN_SRV_ERR;

    printWFP(BC_R, BC_G, BC_B);
  }
}

/*
 * ====================================================================================================
 */

/*
 * Функция, описывающая действия в состоянии выбора действия
 */
void chooseAction() {
  printTRQ(FC_R, FC_G, FC_B); // Отображение соответствующего экрана

  // Для задержек и нажатой кнопки
  m = millis();
  curr_m = m;
  int button = 0;

  // Цикл до нажатия на кнопку или истечения времени
  while (curr_m - m < TIMEOUT && button == 0) {
    while (curr_m - m < TIMEOUT && !ts.dataAvailable()) {
      delay(250);
      curr_m = millis();
    }

    if (ts.dataAvailable()) {
      ts.read();
      button = touchTRQ(tpyToX(ts.TP_Y), tpxToY(ts.TP_X));
    }
  }

  if (button == 0) {
    // Потому что действия при истечении времени и нажатии на кнопку ВЫХОД одинаковые
    button = 3;
  }

  if (button == 1) {
    // При нажатии на кнопку ВЗЯТИЕ
    Serial.println("Button TAKE was pressed");
    
    action = ACT_TAKE;

    prev_state = STATE_CHOOSE_ACTION;
    state = STATE_CHOOSE_CELL;
  }

  if (button == 2) {
    // При нажатии на кнопку ВОЗВРАТ
    Serial.println("Button RETURN was pressed");
    
    action = ACT_RET;

    prev_state = STATE_CHOOSE_ACTION;
    state = STATE_CHOOSE_CELL;
  }

  if (button == 3) {
    // При нажатии на кнопку ВЫХОД или истечении времени
    Serial.println("TIMEOUT or button QUIT was pressed");

    if (client.connect(srv_ip, srv_port)) {
      // Отправка сообщения об окончании работы пользователя с системой
      for (int i = 0; i < cid_length; ++i) {
        client.print(cid[i]);
      }

      client.print(';');

      client.print(MSG_END_SESSION);

      client.stop();

      prev_state = STATE_CHOOSE_ACTION;
      state = STATE_WAITING_FOR_PASS;
    }
    else {
      // При ошибке соединения с сервером
      Serial.println("Server connection error occured");

      prev_state = STATE_CHOOSE_ACTION;
      state = STATE_CONN_SRV_ERR;
    }
  }

  printTRQ(BC_R, BC_G, BC_B); // Стирание соответствующего экрана
}

/*
 * ====================================================================================================
 */

/*
 * Функция, описывающая действия в состоянии выбора ячейки
 */
void chooseCell() {
  if (client.connect(srv_ip, srv_port)) {
    // Отправка запроса на получение доступных для выбранного действия ячеек
    for (int i = 0; i < cid_length; ++i) {
      client.print(cid[i]);
    }
    
    client.print(';');
    
    if (action = ACT_TAKE) {
      client.print(MSG_CELLS_TAKE);
      Serial.println("MSG_CELLS_TAKE was sent to server");
    }
    if (action = ACT_RET) {
      client.print(MSG_CELLS_RET);
      Serial.println("MSG_CELLS_RET was sent to server");
    }

    while (!client.available()) {
      delay(10);
    }

    // Получение ответа
    for (int i = 0; i < locks_amount; ++i) {
      char reply = client.read();
      if (reply == CELL_DISABLED) {
        cells_enable[i] = 0;
      }
      if (reply == CELL_ENABLED) {
        cells_enable[i] = 1;
      }
    }

    client.stop();

    Serial.print("cells_enable received: ");
    for (int i = 0; i < locks_amount; ++i) {
      Serial.print(cells_enable[i]);
    }
    Serial.println();

    printCBQ(FC_R, FC_G, FC_B, cells_enable); // Отображение соответствующего экрана

    // Для расчета задержки и определения нажатой кнопки
    m = millis();
    curr_m = m;
    int button = 0;

    // Цикл до нажатия на кнопку или истечения времени
    while (curr_m - m < TIMEOUT && button == 0) {
      while (curr_m - m < TIMEOUT && !ts.dataAvailable()) {
        delay(250);
        curr_m = millis();
      }

      if (ts.dataAvailable()) {
        ts.read();
        button = touchCBQ(tpyToX(ts.TP_Y), tpxToY(ts.TP_X), cells_enable);
      }
    }

    if (button == 0) {
      // Потому что при истечении времени и нажатии на кнопку ВЫХОД действия одинаковые
      button = 10;
    }

    if (button >= 1 && button <= 8) {
      // Если выбрана одна из ячеек
      Serial.print("Number of chosen cell: ");
      Serial.println(button);
      
      cell = button;

      prev_state = STATE_CHOOSE_CELL;
      state = STATE_WAITING_FOR_VALUE;
    }

    if (button == 9) {
      // Если нажата кнопка НАЗАД
      Serial.println("Button BACK was pressed");

      prev_state = STATE_CHOOSE_CELL;
      state = STATE_CHOOSE_ACTION;
    }

    if (button == 10) {
      // Если истекло время или нажата кнопка ВЫХОД
      Serial.println("TIMEOUT or button QUIT was pressed");

      if (client.connect(srv_ip, srv_port)) {
        // Отправка сообщения о завершении работы пользователя с системой
        for (int i = 0; i < cid_length; ++i) {
          client.print(cid[i]);
        }

        client.print(';');

        client.print(MSG_END_SESSION);

        client.stop();
        
        prev_state = STATE_CHOOSE_CELL;
        state = STATE_WAITING_FOR_PASS;
      }
      else {
        // При ошибке соединения с сервером
        Serial.println("Server connection error occured");

        prev_state = STATE_CHOOSE_CELL;
        state = STATE_CONN_SRV_ERR;
      }
    }

    printCBQ(BC_R, BC_G, BC_B, cells_enable);
  }
  else {
    // При ошибке соединения с сервером
    Serial.println("Server connection error occured");

    prev_state = STATE_CHOOSE_CELL;
    state = STATE_CONN_SRV_ERR; 
  }
}

/*
 * ====================================================================================================
 */

/*
 * Функция, описывающая действия в состоянии ожидания ценности
 */
void waitingForValue() {
  printWFV(FC_R, FC_G, FC_B); // Отображение соответствующего экрана

  clearSerial1(); // Очистка последовательного порта

  if (action == ACT_TAKE) {
    unlock(cell - 1);
  }

  // Для расчета задержки и фиксации нажатия на кнопки
  m = millis();
  curr_m = m;
  int button = 0;

  // Цикл до истечения времени или нажатия на кнопку или считывания ценности
  while (curr_m - m < TIMEOUT && button == 0 && Serial1.available() < UID_LEN) {
    while (curr_m - m < TIMEOUT && !ts.dataAvailable() && Serial1.available() < UID_LEN) {
      delay(250);
      curr_m = millis();
    }

    if (ts.dataAvailable()) {
      ts.read();
      button = touchWFV(tpyToX(ts.TP_Y), tpxToY(ts.TP_X));
    }
  }

  if (curr_m - m >= TIMEOUT || button == 2) {
    // При нажатии на кнопку ВЫХОД или истечении времени
    Serial.println("TIMEOUT or button QUIT was pressed");

    if (client.connect(srv_ip, srv_port)) {
      // Отправка сообщения на сервер о завершении работы пользователя с системой
      for (int i = 0; i < cid_length; ++i) {
        client.print(cid[i]);
      }

      client.print(';');

      client.print(MSG_END_SESSION);

      client.stop();
        
      prev_state = STATE_WAITING_FOR_VALUE;
      state = STATE_WAITING_FOR_PASS;
    }
    else {
      // При ошибке соединения с сервером
      Serial.println("Server connection error occured");

      prev_state = STATE_WAITING_FOR_VALUE;
      state = STATE_CONN_SRV_ERR;
    }

    if (action == ACT_TAKE) {
      lock(cell - 1);
    }

    printWFV(BC_R, BC_G, BC_B);
  }
  else if (button == 1) {
    // При нажатии на кнопку НАЗАД
    Serial.println("Button BACK was pressed");

    prev_state = STATE_WAITING_FOR_VALUE;
    state = STATE_CHOOSE_CELL;

    if (action == ACT_TAKE) {
      lock(cell - 1);
    }

    printWFV(BC_R, BC_G, BC_B);
  }
  else if (Serial1.available() >= UID_LEN) {
    // Если была считана метка ценности
    for (int i = 0; i < UID_LEN; ++i) {
      vid[i] = Serial1.read();
    }
    
    Serial.println("Value ID was read: ");
    for (int i = UID_FROM; i <= UID_TO; ++i) {
      Serial.print(vid[i]);
    }
    Serial.println();

    if (client.connect(srv_ip, srv_port)) {
      // Отправка ID считанной ценности на сервер
      for (int i = 0; i < cid_length; ++i) {
        client.print(cid[i]);
      }
    
      client.print(';');
  
      client.print(MSG_VALUE_ID);

      client.print(';');
    
      for (int i = UID_FROM; i <= UID_TO; ++i) {
        client.print(pid[i]);
      }

      client.print(';');

      client.print(action);

      client.print(';');

      client.print(cellToChar(cell));

      client.print(';');

      for (int i = 0; i < cid_length; ++i) {
        client.print(vid[i]);
      }

      Serial.println("Value ID was sent to server");

      while (!client.available()) {
        delay(100);
      }

      // Получение ответа
      char reply = client.read();
      client.stop();

      if (reply == REP_VALID_VALUE) {
        // Если ценность валидная
        Serial.println("Value is valid");

        if (action == ACT_RET) {
          unlock(cell - 1);
        }
      
        prev_state = STATE_WAITING_FOR_VALUE;
        state = STATE_CONFIRM_ACTION;

        printVV(0, 255, 0);
        delay(2000);
        printVV(BC_R, BC_G, BC_B);
        clearSerial1();

        printWFV(BC_R, BC_G, BC_B);
      }
    
      if (reply == REP_INVALID_VALUE) {
        // Если ценность не валидная
        Serial.println("Value is invalid");
     
        prev_state = STATE_WAITING_FOR_VALUE;
        state = STATE_WAITING_FOR_VALUE;

        printIV(255, 0, 0);
        delay(2000);
        printIV(BC_R, BC_G, BC_B);
        clearSerial1();
      }

      if (reply == REP_WRONG_TYPE) {
        // Если ценность неправильного типа (такое возможно только при возврате)
        Serial.println("Value of wrong type");
     
        prev_state = STATE_WAITING_FOR_VALUE;
        state = STATE_WAITING_FOR_VALUE;

        printWT(255, 0, 0);
        delay(2000);
        printWT(BC_R, BC_G, BC_B);
        clearSerial1();
      }
    }
    else {
      // При ошибке соединения с сервером
      Serial.println("Server connection error occured");

      if (action == ACT_TAKE) {
        lock(cell - 1);
      }

      prev_state = STATE_WAITING_FOR_VALUE;
      state = STATE_CONN_SRV_ERR;

      clearSerial1();

      printWFP(BC_R, BC_G, BC_B);
    }
  }
}

/*
 * ====================================================================================================
 */

/*
 * Функция, описывающая действия в состоянии подтверждения действия
 */
void confirmAction() {
  printCOK(FC_R, FC_G, FC_B); // Отображение соответствующего экрана

  //Для расчета задержек и фиксации нажатия на кнопку
  m = millis();
  curr_m = m;
  int button = 0;

  // Цикл до нажатия на кнопку или истечения времени
  while (curr_m - m < TIMEOUT && button == 0) {
    while (curr_m - m < TIMEOUT && !ts.dataAvailable()) {
      delay(250);
      curr_m = m;
    }

    if (ts.dataAvailable()) {
      ts.read();
      button = touchCOK(tpyToX(ts.TP_Y), tpxToY(ts.TP_X));
    }
  }

  lock(cell - 1); // Закрытие замка

  if (button == 0) {
    // При истечении времени
    Serial.println("TIMEOUT, going to WAITING_FOR_PASS");
    
    if (client.connect(srv_ip, srv_port)) {
      //Отправка на сервер сообщения о завершении работы пользователя с системой
      for (int i = 0; i < cid_length; ++i) {
        client.print(cid[i]);
      }

      client.print(';');

      client.print(MSG_END_SESSION);

      client.stop();
        
      prev_state = STATE_CONFIRM_ACTION;
      state = STATE_WAITING_FOR_PASS;
    }
    else {
      // При ошибке соединения с сервером
      Serial.println("Server connection error occured");

      prev_state = STATE_CONFIRM_ACTION;
      state = STATE_CONN_SRV_ERR;
    }
  }
  else {
    // При нажатии кнопки ОК
    Serial.println("Button OK pressed, going to CHOOSE_ACTION");
    
    prev_state = STATE_CONFIRM_ACTION;
    state = STATE_CHOOSE_ACTION;
  }

  printCOK(BC_R, BC_G, BC_B); // Стирание соответствующего экрана
}
