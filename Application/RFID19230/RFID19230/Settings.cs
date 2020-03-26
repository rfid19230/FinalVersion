/*
 * Содержимое данного файла отвечает за
 * обеспечение взаимодействия с графическим интерфейсом
 * на вкладке "Настройки сервера и базы данных"
 */

// Необходимые зависисмости
using System;
using System.IO;
using System.Windows.Forms;

namespace RFID19230
{
    public partial class MainForm : Form
    {
        /// <summary>
        /// Действия, выполняемые при нажатии на кнопку "Проверка подключения"
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void CheckDbB_Click(object sender, EventArgs e)
        {
            // Вызов функции проверки подключения
            CheckDb();
        }

        /// <summary>
        /// Действия, выполняемые при нажатии на кнопку "Сохранить настройки" базы данных
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void DbParamsB_Click(object sender, EventArgs e)
        {
            // Вызов функции обновления настроек базы данных
            UpdateDbParams();

            // Запись новых настроек в конфигурационный файл базы данных
            string[] NewParams = { DbIP, DbPort, DbLogin, DbPassword };
            File.WriteAllLines(PathToInitDbFile, NewParams);

            // Сохранение информации о внесенных изменениях
            ToLogTB("Изменены настройки базы данных" + EOL);
        }

        /// <summary>
        /// Действия, выполняемые при нажатии на кнопку "Остановка" сервера
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void SrvStopB_Click(object sender, EventArgs e)
        {
            // Установка флага остановки сервера
            SrvStop = true;
        }

        /// <summary>
        /// Действия, выполняемые при нажатии на кнопку "Запуск" сервера
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void SrvStartB_Click(object sender, EventArgs e)
        {
            // Вызов функции, создающей и запускающей поток сервера
            OnSrvStarted();
        }

        /// <summary>
        /// Действия, выполняемые при нажатии на кнопку "Сохранить настройки" сервера
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void SrvParamsB_Click(object sender, EventArgs e)
        {
            // Вызов функции обновления настроек сервера
            UpdateSrvParams();

            // Запись новых настроек в конфигурационный файл сервера
            string[] NewParams = { SrvIp, SrvPort.ToString() };
            File.WriteAllLines(PathToInitSrvFile, NewParams);

            // Сохранение информации о внесенных изменениях
            ToLogTB("Изменены настройки сервера" + EOL);
        }

    }
}
