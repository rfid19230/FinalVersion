/*
 * Содержимое данного файла отвечает за
 * настройку и обеспечение работы сервера,
 * принимающего сообщения от микроконтроллера
 */

// Необходимые зависимости
using System;
using System.Drawing;
using System.IO;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System.Windows.Forms;


namespace RFID19230
{
    public partial class MainForm : Form
    {     
        // Путь к конфигурационному файлу сервера
        const string PathToInitSrvFile = "InitSrv.txt";
            
        // Порт и IP-адрес сервера
        int SrvPort;
        string SrvIp;

        // Для создания объекта сервера
        public TcpListener Srv = null;

        // Флаг остановки работы сервера
        public static bool SrvStop;


        /// <summary>
        /// Инициализация параметров сервера при помощи конфигурационного файла
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void InitSrv()
        {
            // Считывание параметров сервера из конфигурационного файла
            string[] SrvParams = File.ReadAllLines(PathToInitSrvFile);

            // Отображение считанных параметров
            SrvIpTB.Text = SrvParams[0];
            SrvPortTB.Text = SrvParams[1];
        }

        /// <summary>
        /// Функция, выполняющая обновление параметров сервера
        /// </summary>
        private void UpdateSrvParams()
        {
            // Считывание новых параметров
            SrvIp = SrvIpTB.Text;
            SrvPort = Int32.Parse(SrvPortTB.Text);
        }

        /// <summary>
        /// Описание действий потока сервера
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void OnSrvStarted()
        {
            // Обновление параметров сервера
            UpdateSrvParams();

            // Создание нового потока, играющего роль сервера
            Thread SrvThread = new Thread(() =>
            {
                // Описание действий, выполняемых в серверном потоке
                // Настройка некоторых элементов интерфейса
                Invoke(new Action(() =>
                {
                    SrvStartB.Enabled = false;
                    SrvStopB.Enabled = true;
                    SrvParamsB.Enabled = false;

                    SrvIpTB.Enabled = false;
                    SrvPortTB.Enabled = false;

                    SrvStateL.BackColor = Color.Lime;
                    SrvStateL.Text = "Сервер запущен";
                }));

                // Установка флага состояния сервера
                SrvStop = false;

                try
                {
                    // Запуск сервера
                    Srv = new TcpListener(IPAddress.Parse(SrvIp), SrvPort);
                    Srv.Start();
                    ToLogTB("Сервер запущен" + ", IP: " + SrvIp + ", порт: " + SrvPort.ToString() + EOL);

                    // Цикл ожидания подключений с проверкой флага состояния сервера
                    while (!SrvStop)
                    {
                        // Непосредственно ожидание подключения
                        while (!SrvStop && !Srv.Pending())
                        {
                            Thread.Sleep(100);
                        }
                        if (!SrvStop)
                        {
                            // При входящем подключении
                            NetworkStream Stream = null;
                            TcpClient Client = null;
                            try
                            {
                                // Установка соединения с клиентом
                                Client = Srv.AcceptTcpClient();
                                Thread.Sleep(100);

                                ToLogTB("Зафиксировано входящее подключение" + EOL);

                                // Считывание полученного сообщения
                                Stream = Client.GetStream();
                                byte[] Data = new byte[64]; // Буфер для получаемых данных

                                // Получаем сообщение
                                StringBuilder Builder = new StringBuilder();
                                int Bytes = 0;
                                do
                                {
                                    Bytes = Stream.Read(Data, 0, Data.Length);
                                    Builder.Append(Encoding.ASCII.GetString(Data, 0, Bytes));
                                }
                                while (Stream.DataAvailable);

                                // Обработка полученного сообщения
                                string Msg = Builder.ToString();
                                ToLogTB("Получено сообщение: " + Msg + EOL);

                                string Reply = OnMsgReceived(Msg);

                                // Отправка ответа
                                if (Reply != NoReply)
                                {
                                    Data = Encoding.ASCII.GetBytes(Reply);
                                    Stream.Write(Data, 0, Data.Length);
                                }

                            }
                            catch (Exception ex)
                            {
                                // При ошибке
                                ToLogTB(ex.Message + EOL + "Ошибка в клиентском потоке" + EOL);
                            }
                            finally
                            {
                                // При завершении работы
                                if (Stream != null)
                                    Stream.Close();
                                if (Client != null)
                                {
                                    Client.Close();
                                    ToLogTB("Завершено обслуживание входящего подключения" + EOL);
                                }
                            }
 
                        }
                    }
                }
                catch (Exception ex)
                {
                    // При ошибке
                    ToLogTB(ex.Message + EOL + "Ошибка сервера" + EOL);
                }
                finally
                {
                    // При ошибке завершения работы
                    if (Srv != null)
                    {
                        Srv.Stop();
                    }
                    Invoke(new Action(() =>
                    {
                        SrvStartB.Enabled = true;
                        SrvStopB.Enabled = false;
                        SrvParamsB.Enabled = true;

                        SrvIpTB.Enabled = true;
                        SrvPortTB.Enabled = true;

                        SrvStateL.BackColor = Color.Red;
                        SrvStateL.Text = "Сервер остановлен";
                    }));
                    ToLogTB("Сервер остановлен" + EOL);
                }
            });
            SrvThread.IsBackground = true;
            SrvThread.Start();
        }

    }
}
