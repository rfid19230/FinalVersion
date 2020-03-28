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
