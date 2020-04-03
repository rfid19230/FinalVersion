/*
 * Содержимое данного файла отвечает за
 * настройку и проверку состояния базы данных
 */

// Необходимые зависисмости
using System;
using System.Data.OracleClient;
using System.Drawing;
using System.IO;
using System.Windows.Forms;

namespace RFID19230
{
    public partial class MainForm : Form
    {
        // Путь к конфигурационному файлу базы данных
        const string PathToInitDbFile = "InitDb.txt";

        // Для хранения основных параметров базы данных
        string ConnStr, DbLogin, DbPassword, DbIP, DbPort;

        /// <summary>
        /// Функция, обновляющая параметры базы данных
        /// </summary>
        private void UpdateDbParams()
        {
            // Считывание новых параметров
            DbIP = DbSrvIpTB.Text;
            DbPort = DbSrvPortTB.Text;
            DbLogin = DbLoginTB.Text;
            DbPassword = DbPasswordTB.Text;

            // Формирование строки подключения
            ConnStr = "DATA SOURCE=" + DbIP + ":" + DbPort + "/XE;PASSWORD=" + DbPassword + ";PERSIST SECURITY INFO=True;USER ID=" + DbLogin;
        }

        /// <summary>
        /// Функция, инициализирующая параметры базы данных
        /// </summary>
        private void InitDb()
        {
            // Считывание параметров из конфигурационного файла
            string[] DbParams = File.ReadAllLines(PathToInitDbFile);

            // Отображение параметров
            DbSrvIpTB.Text = DbParams[0];
            DbSrvPortTB.Text = DbParams[1];
            DbLoginTB.Text = DbParams[2];
            DbPasswordTB.Text = DbParams[3];

            // Вызов функции, обновляющей параметры базы данных
            UpdateDbParams();
        }

        /// <summary>
        /// Функция, выполняющая проверку подключения к базе данных
        /// </summary>
        private void CheckDb()
        {
            // Попытка подключения с использованием строки подключения
            using (OracleConnection Conn = new OracleConnection(ConnStr))
            {
                try
                {
                    // Открытие соединения
                    Conn.Open();

                    // При успешном подключении
                    ToLogTB("Есть подключение к БД" + EOL);
                    DbStateL.Text = "БД подключена";
                    DbStateL.BackColor = Color.Lime;
                }
                catch (Exception ConnEx)
                {
                    // При ошибке открытия соединения
                    ToLogTB("Нет подключения к БД" + EOL + ConnEx.Message + EOL);
                    DbStateL.Text = "БД не подключена";
                    DbStateL.BackColor = Color.Red;
                }
            }
        }

        
    }
}
