/*
 * Содержимое данного файла описывает
 * логику работы системы
 */

// Необходимые зависимости
using System;
using System.Data.OracleClient;
using System.Windows.Forms;

namespace RFID19230
{
    public partial class MainForm : Form
    {
        // Для ситуации, когда на сообщение не требуется ответ
        const string NoReply = "NoReply";

        // Индексы частей сообщения
        const int CidIndex = 0;
        const int MsgTypeIndex = 1;
        const int PidIndex = 2;
        const int ActIndex = 3;
        const int CellIndex = 4;
        const int VidIndex = 5;

        // Типы входящих сообщений
        const string MsgCheckConn = "0"; // Сообщение проверки соединения
        const string MsgCellsTake = "1"; // Запрос ячеек, доступных для взятия
        const string MsgCellsRet = "2"; // Запрос ячеек, доступных для возврата
        const string MsgPassId = "3"; // Запрос на подтверждение валидности пропуска
        const string MsgValueId = "4"; // Запрос на подтверждение валидности ценности
        const string MsgEndSession = "5"; // Сообщение об окончании работы пользователя с системой

        // Ответы на запрос о подтверждении валидности пропуска
        const string RepValidPass = "0";
        const string RepInvalidPass = "1";

        // Ответы на запрос о подтверждении валидности ценности
        const string RepValidValue = "0";
        const string RepInvalidValue = "1";
        const string RepWrongType = "2";

        // Возможные действия
        const string ActTake = "1";
        const string ActRet = "2";

        // Возможные состояния ячеек
        const string CellEnabled = "1";
        const string CellDisabled = "0";

        /// <summary>
        /// Функция, обрабатывающая входные сообщения и формирующая ответы на них
        /// </summary>
        /// <param name="Msg">Полученное сообщение</param>
        /// <returns>Ответ на сообщение</returns>
        private string OnMsgReceived(string Msg)
        {
            // Для хранения ответа
            string Reply = null;

            // Разделение сообщения на части
            string[] MsgParts = Msg.Split(';');

            // В зависимости от типа сообщения
            switch (MsgParts[MsgTypeIndex])
            {
                // Было получено сообщение проверки соединения
                case MsgCheckConn:
                    {
                        // Вывод информации о получении сообщения, ответ не требуется
                        ToLogTB("Получено сообщение проверки соединения от шкафа " + MsgParts[CidIndex] + EOL);
                        Reply = NoReply;
                        break;
                    }

                // Было получено сообщение проверки пропуска
                case MsgPassId:
                    {
                        // Вывод информации о получении сообщения, вызов функции формирования ответа для сообщения такого типа
                        ToLogTB("Получено сообщение проверки пропуска " + MsgParts[PidIndex] + " от шкафа " + MsgParts[CidIndex] + EOL);
                        Reply = CheckPass(Msg);
                        break;
                    }

                // Было получено сообщение проверки ценности
                case MsgValueId:
                    {
                        // Вывод информации о получении сообщения, вызов функции формирования ответа для сообщения такого типа
                        ToLogTB("Получено сообщение проверки ценности " + MsgParts[PidIndex] + " от шкафа " + MsgParts[CidIndex] + EOL);
                        Reply = CheckValue(Msg);
                        break;
                    }

                // Было получено сообщение об окончании работы пользователя с системой
                case MsgEndSession:
                    {
                        // Вывод информации о получении сообщения, ответ не требуется
                        ToLogTB("Получено сообщение окончания работы пользователя " + MsgParts[PidIndex] + 
                                " с системой от шкафа " + MsgParts[CidIndex] + EOL);
                        Reply = NoReply;
                        break;
                    }

                // Было получено сообщение проверки ячеек для взятия
                case MsgCellsTake:
                    {
                        // Вывод информации о получении сообщения, вызов функции формирования ответа для сообщения такого типа
                        ToLogTB("Получено сообщение проверки ячеек для взятия от шкафа " + MsgParts[CidIndex] + EOL);
                        Reply = CellsForTake(Msg);
                        break;
                    }

                // Было получено сообщение проверки ячеек для возврата
                case MsgCellsRet:
                    {
                        // Вывод информации о получении сообщения, вызов функции формирования ответа для сообщения такого типа
                        ToLogTB("Получено сообщение проверки ячеек для возврата от шкафа " + MsgParts[CidIndex] + EOL);
                        Reply = CellsForRet(Msg);
                        break;
                    }
            }

            // Вывод ответа
            ToLogTB("Отправляемый ответ: " + Reply + EOL);

            return Reply;
        }

        /// <summary>
        /// Функция для проверки валидности пропуска
        /// </summary>
        /// <param name="Msg">Полученное сообщение</param>
        /// <returns>Валиден ли пропуск</returns>
        private string CheckPass(string Msg)
        {
            // Разделение сообщения на части
            string[] MsgParts = Msg.Split(';');

            // Ответ по умолчанию
            string Reply = RepInvalidPass;

            // Создание подключения к БД с использованием строки подключения
            using (OracleConnection Conn = new OracleConnection(ConnStr))
            {
                try
                {
                    // Открытие соединения
                    Conn.Open();

                    // Формирование запроса в БД
                    string QueryStr = "SELECT * FROM users WHERE u_id = " + "'" + MsgParts[PidIndex] + "'";

                    OracleCommand Cmd = new OracleCommand(QueryStr, Conn);

                    try
                    {
                        // Выполнение запроса
                        OracleDataReader Reader = Cmd.ExecuteReader();
                        if (Reader.HasRows)
                        {
                            // Права доступа есть
                            Reply = RepValidPass;
                        }
                    }
                    catch (Exception ReadEx)
                    {
                        // При ошибке выполнения запроса
                        ToLogTB("Ошибка при выполнении запроса" + EOL + QueryStr + EOL + ReadEx.Message + EOL);
                    }
                }
                catch (Exception ConnEx)
                {
                    // При ошибке подключения к БД
                    ToLogTB("Ошибка при подключении к БД" + EOL + ConnEx.Message + EOL);
                }
            }

            return Reply;
        }

        /// <summary>
        /// Функция для проверки валидности ценности
        /// </summary>
        /// <param name="Msg">Полученное сообщение</param>
        /// <returns>Валидна ли вещь</returns>
        private string CheckValue(string Msg)
        {
            string[] MsgParts = Msg.Split(';');
            string Reply = RepInvalidValue;

            // Создание подключения к БД с использованием строки подключения
            using (OracleConnection Conn = new OracleConnection(ConnStr))
            {
                try
                {
                    // Открытие соединения
                    Conn.Open();

                    // Формирование запроса в БД
                    string QueryStr = "SELECT * FROM mvalues WHERE mv_id = " + "'" + MsgParts[VidIndex] + "'";

                    OracleCommand Cmd = new OracleCommand(QueryStr, Conn);

                    try
                    {
                        // Выполнение запроса
                        OracleDataReader Reader = Cmd.ExecuteReader();
                        if (Reader.HasRows)
                        {
                            // Если производится взятие
                            if (MsgParts[ActIndex] == ActTake)
                            {
                                // Права доступа есть
                                Reply = RepValidValue;

                                // Получение текущего количества вещей в ячейке
                                QueryStr = "SELECT cs_currval FROM cells WHERE cs_closet = " + "'" + MsgParts[CidIndex] + "'" +
                                           " AND cs_num = " + "'" + MsgParts[CellIndex] + "'";
                                Cmd.CommandText = QueryStr;
                                int CurrVal = Int32.Parse(Cmd.ExecuteScalar().ToString());

                                // Обновление текущего количества вещей в ячейке
                                CurrVal--;

                                // Запись обновленного количества вещей в ячейке
                                QueryStr = "UPDATE cells SET cs_currval = " + "'" + CurrVal.ToString() + "'" +
                                           " WHERE cs_closet = " + "'" + MsgParts[CidIndex] + "'" + 
                                           " AND cs_num = " + "'" + MsgParts[CellIndex] + "'";
                                Cmd.CommandText = QueryStr;
                                Cmd.ExecuteNonQuery();

                                // Формирование записи о взятии вещи
                                QueryStr = "INSERT INTO tvalues VALUES (" +
                                           "'" + MsgParts[PidIndex] + "'," +
                                           "'" + MsgParts[VidIndex] + "', sysdate, NULL)";
                                Cmd.CommandText = QueryStr;
                                Cmd.ExecuteNonQuery();
                            }
                            // Если производится возврат
                            else
                            {
                                // Получение типа ценности
                                QueryStr = "SELECT mv_type FROM mvalues WHERE mv_id = " + "'" + MsgParts[VidIndex] + "'";
                                Cmd.CommandText = QueryStr;
                                string ValueType = Cmd.ExecuteScalar().ToString();

                                // Получение типа ячейки
                                QueryStr = "SELECT cs_type FROM cells" +
                                           " WHERE cs_num = " + "'" + MsgParts[CellIndex] + "'" +
                                           " AND cs_closet = " + "'" + MsgParts[CidIndex] + "'";
                                Cmd.CommandText = QueryStr;
                                string CellType = Cmd.ExecuteScalar().ToString();

                                // При совпадении типов
                                if (CellType == ValueType)
                                {
                                    Reply = RepValidValue;

                                    // Получение текущего количества вещей в ячейке
                                    QueryStr = "SELECT cs_currval FROM cells WHERE cs_closet = " + "'" + MsgParts[CidIndex] + "'" +
                                               " AND cs_num = " + "'" + MsgParts[CellIndex] + "'";
                                    Cmd.CommandText = QueryStr;
                                    int CurrVal = Int32.Parse(Cmd.ExecuteScalar().ToString());

                                    // Обновление текущего количества вещей в ячейке
                                    CurrVal++;

                                    // Запись обновленного количества вещей в ячейке
                                    QueryStr = "UPDATE cells SET cs_currval = " + "'" + CurrVal.ToString() + "'" +
                                               " WHERE cs_closet = " + "'" + MsgParts[CidIndex] + "'" +
                                               " AND cs_num = " + "'" + MsgParts[CellIndex] + "'";
                                    Cmd.CommandText = QueryStr;
                                    Cmd.ExecuteNonQuery();

                                    // Формирование записи о возврате вещи
                                    QueryStr = "UPDATE tvalues SET tv_ret = sysdate WHERE tv_ret IS NULL" +
                                               " AND tv_user = " + "'" + MsgParts[PidIndex] + "'" +
                                               " AND tv_value = " + "'" + MsgParts[VidIndex] + "'";
                                    Cmd.CommandText = QueryStr;
                                    Cmd.ExecuteNonQuery();

                                    // Обновление информации о текущем местоположении вещи
                                    QueryStr = "UPDATE mvalues SET mv_closet = " + "'" + MsgParts[CidIndex] + "'" +
                                               " AND tv_cell = " + "'" + MsgParts[CellIndex] + "'" +
                                               " WHERE mv_id = " + "'" + MsgParts[VidIndex] + "'";
                                }
                                // При несовпадении типов
                                else
                                {
                                    Reply = RepWrongType;
                                }
                            }

                        }

                    }
                    catch (Exception ReadEx)
                    {
                        // При ошибке выполнения запроса
                        ToLogTB("Ошибка при выполнении запроса" + EOL + QueryStr + EOL + ReadEx.Message + EOL);
                    }
                }
                catch (Exception ConnEx)
                {
                    // При ошибке подключения к БД
                    ToLogTB("Ошибка при подключении к БД" + EOL + ConnEx.Message + EOL);
                }
            }

            return Reply;
        }

        /// <summary>
        /// Функция для проверки возможности взятия из ячеек шкафа
        /// </summary>
        /// <param name="Msg">Полученное сообщение</param>
        /// <returns>Ячейки, доступные для взятия</returns>
        private string CellsForTake(string Msg)
        {
            // Разделение сообщения на чатсти
            string[] MsgParts = Msg.Split(';');

            // Для дальнейшего формирования ответа
            string Reply = "";

            // Создание подключения к БД с использованием строки подключения
            using (OracleConnection Conn = new OracleConnection(ConnStr))
            {
                try
                {
                    // Открытие соединения
                    Conn.Open();

                    // Формирование запроса в БД
                    string QueryStr = "SELECT cs_num, cs_currval, cs_maxval FROM cells WHERE cs_closet = " + "'" + MsgParts[CidIndex] + "'" +
                                      " ORDER BY cs_num ASC";

                    OracleCommand Cmd = new OracleCommand(QueryStr, Conn);

                    try
                    {
                        // Выполнение запроса
                        OracleDataReader Reader = Cmd.ExecuteReader();
                        if (Reader.HasRows)
                        {
                            while (Reader.Read())
                            {
                                // Для каждой ячейки шкафа
                                if (Reader.GetValue(1).ToString() == "0")
                                {
                                    // Если не доступна для взятия
                                    Reply += CellDisabled;
                                }
                                else
                                {
                                    // В противном случае
                                    Reply += CellEnabled;
                                }
                            }
                        }
                    }
                    catch (Exception ReadEx)
                    {
                        // При ошибке выполнения запроса
                        ToLogTB("Ошибка при выполнении запроса" + EOL + QueryStr + EOL + ReadEx.Message + EOL);
                    }
                }
                catch (Exception ConnEx)
                {
                    // При ошибке подключения к БД
                    ToLogTB("Ошибка при подключении к БД" + EOL + ConnEx.Message + EOL);
                }
            }

            return Reply;
        }

        /// <summary>
        /// Функция для проверки возможности возврата в ячейки шкафа
        /// </summary>
        /// <param name="Msg">Полученное сообщение</param>
        /// <returns>Ячейки, доступные для возврата</returns>
        private string CellsForRet(string Msg)
        {
            // Разделение сообщения на части
            string[] MsgParts = Msg.Split(';');

            // Для формирования ответа
            string Reply = "";

            // Создание подключения к БД с использованием строки подключения
            using (OracleConnection Conn = new OracleConnection(ConnStr))
            {
                try
                {
                    // Открытие соединения
                    Conn.Open();

                    // Формирование запроса в БД
                    string QueryStr = "SELECT cs_num, cs_currval, cs_maxval FROM cells WHERE cs_closet = " + "'" + MsgParts[CidIndex] + "'" +
                                      " ORDER BY cs_num ASC";

                    OracleCommand Cmd = new OracleCommand(QueryStr, Conn);

                    try
                    {
                        // Выполнение запроса
                        OracleDataReader Reader = Cmd.ExecuteReader();
                        if (Reader.HasRows)
                        {
                            while (Reader.Read())
                            {
                                // Для каждой из ячеек шкафа
                                if (Reader.GetValue(1).ToString() == Reader.GetValue(2).ToString())
                                {
                                    // При невозможности возврата
                                    Reply += CellDisabled;
                                }
                                else
                                {
                                    // В противном случае
                                    Reply += CellEnabled;
                                }
                            }
                        }
                    }
                    catch (Exception ReadEx)
                    {
                        // При ошибке выполнения запроса
                        ToLogTB("Ошибка при выполнении запроса" + EOL + QueryStr + EOL + ReadEx.Message + EOL);
                    }
                }
                catch (Exception ConnEx)
                {
                    // При ошибке подключения к БД
                    ToLogTB("Ошибка при подключении к БД" + EOL + ConnEx.Message + EOL);
                }
            }

            return Reply;
        }

    }
}
