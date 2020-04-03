/*
 * Содержимое данного файла отвечает за
 * инициализацию различных элементов при запуске приложения
 * и
 * корректное завершение работы приложения.
 */

// Необходимые зависимости
using System;
using System.Windows.Forms;

namespace RFID19230
{
    public partial class MainForm : Form
    {
        /// <summary>
        /// Конструктор главной формы
        /// </summary>
        public MainForm()
        {
            // Инициализация формы
            InitializeComponent();
        }

        /// <summary>
        /// Действия, выполняемые после загрузки приложения
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void MainForm_Load(object sender, EventArgs e)
        {
            // Для дальнейшего соединения с базой данных
            // Инициализация параметров базы данных
            InitDb();
            // Проверка наличия подключения к базе данных
            CheckDb();

            // Для дальнейшей работы с сервером
            // Инициализация параметров сервера
            InitSrv();
            // Запуск сервера
            OnSrvStarted();
        }

        /// <summary>
        /// Действия, выполняемые перед закрытием приложения
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void MainForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            // Сохранение информации о работе системы
            SaveLogs();
        }

    }
}
