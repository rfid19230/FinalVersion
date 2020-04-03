/*
 * Содержимое данного файла отвечает за
 * обеспечение взаимодейсвтия с графическим интерфейсом на вкладке "Логи системы"
 * и
 * корректное отображение и сохранение информации о работе системы
 */

// Необходимые зависимости
using System;
using System.IO;
using System.Windows.Forms;

namespace RFID19230
{
    public partial class MainForm : Form
    {       
        // Комбинация символов, означающая переход на новую строку
        const string EOL = "\r\n";

        // Путь к файлу с логами работы системы
        const string PathToLogFile = "Logs.txt";

        /// <summary>
        /// Функция, выполняющая сохранение информации о работе системы
        /// </summary>
        private void SaveLogs()
        {
            // Запись несохраненных логов в файл
            File.AppendAllText(PathToLogFile, LogTB.Text);
        }

        /// <summary>
        /// Функция, выполняющая вывод информации о работе программы в специальное окно
        /// </summary>
        /// <param name="info">Строка с информацией, которую необходимо вывести</param>
        void ToLogTB(string Info)
        {
            // Проверка, из какого потока вызывается функция
            if (InvokeRequired)
            {
                // Если не из главного
                Invoke(new Action(() => LogTB.Text += DateTime.Now + ": " + Info));

                // Во избежание переполнения окна вывода информации о работе программы
                if (LogTB.Text.Length >= 0.95 * LogTB.MaxLength)
                {
                    // Сохранение логов и очистка специального окна
                    SaveLogs();
                    Invoke(new Action(() => LogTB.Clear()));                 
                }
            }
            else
            {
                // Если из главного
                LogTB.Text += DateTime.Now + ": " + Info;

                // Во избежание переполнения окна вывода информации о работе программы
                if (LogTB.Text.Length >= 0.95 * LogTB.MaxLength)
                {
                    // Сохранение логов и очистка специального окна
                    SaveLogs();
                    LogTB.Clear();
                }
            }
        }

        /// <summary>
        /// Действия, выполняемые при нажатии на кнопку "Очистить" на вкладке "Логи системы"
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void LogB_Click(object sender, EventArgs e)
        {
            // Сохранение логов и очистка специального окна
            SaveLogs();
            Invoke(new Action(() => LogTB.Clear())); 
        }

    }
}
