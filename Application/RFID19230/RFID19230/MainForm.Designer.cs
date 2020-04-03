namespace RFID19230
{
    partial class MainForm
    {
        /// <summary>
        /// Обязательная переменная конструктора.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Освободить все используемые ресурсы.
        /// </summary>
        /// <param name="disposing">истинно, если управляемый ресурс должен быть удален; иначе ложно.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Код, автоматически созданный конструктором форм Windows

        /// <summary>
        /// Требуемый метод для поддержки конструктора — не изменяйте 
        /// содержимое этого метода с помощью редактора кода.
        /// </summary>
        private void InitializeComponent()
        {
            this.LogTP = new System.Windows.Forms.TabPage();
            this.LogB = new System.Windows.Forms.Button();
            this.LogL = new System.Windows.Forms.Label();
            this.LogTB = new System.Windows.Forms.TextBox();
            this.DatabaseTP = new System.Windows.Forms.TabPage();
            this.DbWB = new System.Windows.Forms.WebBrowser();
            this.MainTC = new System.Windows.Forms.TabControl();
            this.SettingsTP = new System.Windows.Forms.TabPage();
            this.DbStateL = new System.Windows.Forms.Label();
            this.SrvParamsB = new System.Windows.Forms.Button();
            this.DbParamsB = new System.Windows.Forms.Button();
            this.CheckDbB = new System.Windows.Forms.Button();
            this.SrvStopB = new System.Windows.Forms.Button();
            this.SrvStartB = new System.Windows.Forms.Button();
            this.SrvStateL = new System.Windows.Forms.Label();
            this.SrvPortL = new System.Windows.Forms.Label();
            this.SrvIpL = new System.Windows.Forms.Label();
            this.SrvPortTB = new System.Windows.Forms.TextBox();
            this.SrvIpTB = new System.Windows.Forms.TextBox();
            this.DbPasswordTB = new System.Windows.Forms.TextBox();
            this.DbLoginTB = new System.Windows.Forms.TextBox();
            this.DbSrvPortTB = new System.Windows.Forms.TextBox();
            this.DbSrvIpTB = new System.Windows.Forms.TextBox();
            this.DbPasswordL = new System.Windows.Forms.Label();
            this.DbLoginL = new System.Windows.Forms.Label();
            this.DbSrvPortL = new System.Windows.Forms.Label();
            this.DbSrvIpL = new System.Windows.Forms.Label();
            this.LogTP.SuspendLayout();
            this.DatabaseTP.SuspendLayout();
            this.MainTC.SuspendLayout();
            this.SettingsTP.SuspendLayout();
            this.SuspendLayout();
            // 
            // LogTP
            // 
            this.LogTP.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(215)))), ((int)(((byte)(230)))), ((int)(((byte)(237)))));
            this.LogTP.Controls.Add(this.LogB);
            this.LogTP.Controls.Add(this.LogL);
            this.LogTP.Controls.Add(this.LogTB);
            this.LogTP.Location = new System.Drawing.Point(4, 28);
            this.LogTP.Name = "LogTP";
            this.LogTP.Padding = new System.Windows.Forms.Padding(3);
            this.LogTP.Size = new System.Drawing.Size(1064, 563);
            this.LogTP.TabIndex = 0;
            this.LogTP.Text = "Логи системы";
            // 
            // LogB
            // 
            this.LogB.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(255)))), ((int)(((byte)(216)))), ((int)(((byte)(139)))));
            this.LogB.Location = new System.Drawing.Point(443, 521);
            this.LogB.Name = "LogB";
            this.LogB.Size = new System.Drawing.Size(149, 36);
            this.LogB.TabIndex = 2;
            this.LogB.Text = "Очистить";
            this.LogB.UseVisualStyleBackColor = false;
            this.LogB.Click += new System.EventHandler(this.LogB_Click);
            // 
            // LogL
            // 
            this.LogL.AutoSize = true;
            this.LogL.Location = new System.Drawing.Point(396, 6);
            this.LogL.Name = "LogL";
            this.LogL.Size = new System.Drawing.Size(225, 19);
            this.LogL.TabIndex = 1;
            this.LogL.Text = "Информация о работе системы";
            // 
            // LogTB
            // 
            this.LogTB.AcceptsReturn = true;
            this.LogTB.AcceptsTab = true;
            this.LogTB.BackColor = System.Drawing.Color.White;
            this.LogTB.Location = new System.Drawing.Point(57, 28);
            this.LogTB.Multiline = true;
            this.LogTB.Name = "LogTB";
            this.LogTB.ReadOnly = true;
            this.LogTB.Size = new System.Drawing.Size(946, 487);
            this.LogTB.TabIndex = 0;
            // 
            // DatabaseTP
            // 
            this.DatabaseTP.Controls.Add(this.DbWB);
            this.DatabaseTP.Location = new System.Drawing.Point(4, 28);
            this.DatabaseTP.Name = "DatabaseTP";
            this.DatabaseTP.Padding = new System.Windows.Forms.Padding(3);
            this.DatabaseTP.Size = new System.Drawing.Size(1064, 563);
            this.DatabaseTP.TabIndex = 5;
            this.DatabaseTP.Text = "База данных";
            this.DatabaseTP.UseVisualStyleBackColor = true;
            // 
            // DbWB
            // 
            this.DbWB.Dock = System.Windows.Forms.DockStyle.Fill;
            this.DbWB.Location = new System.Drawing.Point(3, 3);
            this.DbWB.MinimumSize = new System.Drawing.Size(20, 20);
            this.DbWB.Name = "DbWB";
            this.DbWB.Size = new System.Drawing.Size(1058, 557);
            this.DbWB.TabIndex = 0;
            this.DbWB.Url = new System.Uri("http://127.0.0.1:8080/apex/f?p=4550:1:3994552304801355", System.UriKind.Absolute);
            // 
            // MainTC
            // 
            this.MainTC.Controls.Add(this.SettingsTP);
            this.MainTC.Controls.Add(this.DatabaseTP);
            this.MainTC.Controls.Add(this.LogTP);
            this.MainTC.Location = new System.Drawing.Point(12, 12);
            this.MainTC.Name = "MainTC";
            this.MainTC.SelectedIndex = 0;
            this.MainTC.Size = new System.Drawing.Size(1072, 595);
            this.MainTC.TabIndex = 0;
            // 
            // SettingsTP
            // 
            this.SettingsTP.BackgroundImage = global::RFID19230.Properties.Resources.SettingsImg1;
            this.SettingsTP.Controls.Add(this.DbStateL);
            this.SettingsTP.Controls.Add(this.SrvParamsB);
            this.SettingsTP.Controls.Add(this.DbParamsB);
            this.SettingsTP.Controls.Add(this.CheckDbB);
            this.SettingsTP.Controls.Add(this.SrvStopB);
            this.SettingsTP.Controls.Add(this.SrvStartB);
            this.SettingsTP.Controls.Add(this.SrvStateL);
            this.SettingsTP.Controls.Add(this.SrvPortL);
            this.SettingsTP.Controls.Add(this.SrvIpL);
            this.SettingsTP.Controls.Add(this.SrvPortTB);
            this.SettingsTP.Controls.Add(this.SrvIpTB);
            this.SettingsTP.Controls.Add(this.DbPasswordTB);
            this.SettingsTP.Controls.Add(this.DbLoginTB);
            this.SettingsTP.Controls.Add(this.DbSrvPortTB);
            this.SettingsTP.Controls.Add(this.DbSrvIpTB);
            this.SettingsTP.Controls.Add(this.DbPasswordL);
            this.SettingsTP.Controls.Add(this.DbLoginL);
            this.SettingsTP.Controls.Add(this.DbSrvPortL);
            this.SettingsTP.Controls.Add(this.DbSrvIpL);
            this.SettingsTP.Location = new System.Drawing.Point(4, 28);
            this.SettingsTP.Name = "SettingsTP";
            this.SettingsTP.Padding = new System.Windows.Forms.Padding(3);
            this.SettingsTP.Size = new System.Drawing.Size(1064, 563);
            this.SettingsTP.TabIndex = 6;
            this.SettingsTP.Text = "Настройки сервера и базы данных";
            this.SettingsTP.UseVisualStyleBackColor = true;
            // 
            // DbStateL
            // 
            this.DbStateL.AutoSize = true;
            this.DbStateL.BackColor = System.Drawing.Color.Red;
            this.DbStateL.Location = new System.Drawing.Point(51, 346);
            this.DbStateL.MinimumSize = new System.Drawing.Size(175, 40);
            this.DbStateL.Name = "DbStateL";
            this.DbStateL.Size = new System.Drawing.Size(175, 40);
            this.DbStateL.TabIndex = 34;
            this.DbStateL.Text = "БД не подключена";
            this.DbStateL.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // SrvParamsB
            // 
            this.SrvParamsB.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(255)))), ((int)(((byte)(216)))), ((int)(((byte)(139)))));
            this.SrvParamsB.Location = new System.Drawing.Point(859, 418);
            this.SrvParamsB.Name = "SrvParamsB";
            this.SrvParamsB.Size = new System.Drawing.Size(118, 51);
            this.SrvParamsB.TabIndex = 33;
            this.SrvParamsB.Text = "Сохранить настройки";
            this.SrvParamsB.UseVisualStyleBackColor = false;
            this.SrvParamsB.Click += new System.EventHandler(this.SrvParamsB_Click);
            // 
            // DbParamsB
            // 
            this.DbParamsB.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(255)))), ((int)(((byte)(216)))), ((int)(((byte)(139)))));
            this.DbParamsB.Location = new System.Drawing.Point(79, 481);
            this.DbParamsB.Name = "DbParamsB";
            this.DbParamsB.Size = new System.Drawing.Size(119, 51);
            this.DbParamsB.TabIndex = 32;
            this.DbParamsB.Text = "Сохранить настройки";
            this.DbParamsB.UseVisualStyleBackColor = false;
            this.DbParamsB.Click += new System.EventHandler(this.DbParamsB_Click);
            // 
            // CheckDbB
            // 
            this.CheckDbB.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(255)))), ((int)(((byte)(216)))), ((int)(((byte)(139)))));
            this.CheckDbB.Location = new System.Drawing.Point(79, 410);
            this.CheckDbB.Name = "CheckDbB";
            this.CheckDbB.Size = new System.Drawing.Size(119, 51);
            this.CheckDbB.TabIndex = 31;
            this.CheckDbB.Text = "Проверка подключения";
            this.CheckDbB.UseVisualStyleBackColor = false;
            this.CheckDbB.Click += new System.EventHandler(this.CheckDbB_Click);
            // 
            // SrvStopB
            // 
            this.SrvStopB.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(255)))), ((int)(((byte)(216)))), ((int)(((byte)(139)))));
            this.SrvStopB.Location = new System.Drawing.Point(859, 367);
            this.SrvStopB.Name = "SrvStopB";
            this.SrvStopB.Size = new System.Drawing.Size(119, 32);
            this.SrvStopB.TabIndex = 30;
            this.SrvStopB.Text = "Остановка";
            this.SrvStopB.UseVisualStyleBackColor = false;
            this.SrvStopB.Click += new System.EventHandler(this.SrvStopB_Click);
            // 
            // SrvStartB
            // 
            this.SrvStartB.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(255)))), ((int)(((byte)(216)))), ((int)(((byte)(139)))));
            this.SrvStartB.Location = new System.Drawing.Point(859, 314);
            this.SrvStartB.Name = "SrvStartB";
            this.SrvStartB.Size = new System.Drawing.Size(119, 32);
            this.SrvStartB.TabIndex = 29;
            this.SrvStartB.Text = "Запуск";
            this.SrvStartB.UseVisualStyleBackColor = false;
            this.SrvStartB.Click += new System.EventHandler(this.SrvStartB_Click);
            // 
            // SrvStateL
            // 
            this.SrvStateL.AutoSize = true;
            this.SrvStateL.BackColor = System.Drawing.Color.Red;
            this.SrvStateL.Location = new System.Drawing.Point(831, 241);
            this.SrvStateL.MinimumSize = new System.Drawing.Size(175, 40);
            this.SrvStateL.Name = "SrvStateL";
            this.SrvStateL.Size = new System.Drawing.Size(175, 40);
            this.SrvStateL.TabIndex = 28;
            this.SrvStateL.Text = "Сервер остановлен";
            this.SrvStateL.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // SrvPortL
            // 
            this.SrvPortL.AutoSize = true;
            this.SrvPortL.Location = new System.Drawing.Point(868, 168);
            this.SrvPortL.Name = "SrvPortL";
            this.SrvPortL.Size = new System.Drawing.Size(100, 19);
            this.SrvPortL.TabIndex = 27;
            this.SrvPortL.Text = "Порт сервера";
            // 
            // SrvIpL
            // 
            this.SrvIpL.AutoSize = true;
            this.SrvIpL.Location = new System.Drawing.Point(856, 98);
            this.SrvIpL.Name = "SrvIpL";
            this.SrvIpL.Size = new System.Drawing.Size(125, 19);
            this.SrvIpL.TabIndex = 26;
            this.SrvIpL.Text = "IP-адрес сервера";
            // 
            // SrvPortTB
            // 
            this.SrvPortTB.Location = new System.Drawing.Point(844, 190);
            this.SrvPortTB.Name = "SrvPortTB";
            this.SrvPortTB.Size = new System.Drawing.Size(148, 27);
            this.SrvPortTB.TabIndex = 25;
            this.SrvPortTB.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // SrvIpTB
            // 
            this.SrvIpTB.Location = new System.Drawing.Point(844, 120);
            this.SrvIpTB.Name = "SrvIpTB";
            this.SrvIpTB.Size = new System.Drawing.Size(148, 27);
            this.SrvIpTB.TabIndex = 24;
            this.SrvIpTB.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // DbPasswordTB
            // 
            this.DbPasswordTB.Location = new System.Drawing.Point(64, 282);
            this.DbPasswordTB.Name = "DbPasswordTB";
            this.DbPasswordTB.PasswordChar = '*';
            this.DbPasswordTB.Size = new System.Drawing.Size(148, 27);
            this.DbPasswordTB.TabIndex = 3;
            this.DbPasswordTB.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // DbLoginTB
            // 
            this.DbLoginTB.Location = new System.Drawing.Point(64, 207);
            this.DbLoginTB.Name = "DbLoginTB";
            this.DbLoginTB.Size = new System.Drawing.Size(148, 27);
            this.DbLoginTB.TabIndex = 2;
            this.DbLoginTB.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // DbSrvPortTB
            // 
            this.DbSrvPortTB.Location = new System.Drawing.Point(64, 134);
            this.DbSrvPortTB.Name = "DbSrvPortTB";
            this.DbSrvPortTB.Size = new System.Drawing.Size(148, 27);
            this.DbSrvPortTB.TabIndex = 1;
            this.DbSrvPortTB.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // DbSrvIpTB
            // 
            this.DbSrvIpTB.Location = new System.Drawing.Point(64, 64);
            this.DbSrvIpTB.Name = "DbSrvIpTB";
            this.DbSrvIpTB.Size = new System.Drawing.Size(148, 27);
            this.DbSrvIpTB.TabIndex = 0;
            this.DbSrvIpTB.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // DbPasswordL
            // 
            this.DbPasswordL.AutoSize = true;
            this.DbPasswordL.Location = new System.Drawing.Point(109, 260);
            this.DbPasswordL.Name = "DbPasswordL";
            this.DbPasswordL.Size = new System.Drawing.Size(59, 19);
            this.DbPasswordL.TabIndex = 7;
            this.DbPasswordL.Text = "Пароль";
            // 
            // DbLoginL
            // 
            this.DbLoginL.AutoSize = true;
            this.DbLoginL.Location = new System.Drawing.Point(113, 185);
            this.DbLoginL.Name = "DbLoginL";
            this.DbLoginL.Size = new System.Drawing.Size(51, 19);
            this.DbLoginL.TabIndex = 6;
            this.DbLoginL.Text = "Логин";
            // 
            // DbSrvPortL
            // 
            this.DbSrvPortL.AutoSize = true;
            this.DbSrvPortL.Location = new System.Drawing.Point(77, 112);
            this.DbSrvPortL.Name = "DbSrvPortL";
            this.DbSrvPortL.Size = new System.Drawing.Size(123, 19);
            this.DbSrvPortL.TabIndex = 5;
            this.DbSrvPortL.Text = "Порт сервера БД";
            // 
            // DbSrvIpL
            // 
            this.DbSrvIpL.AutoSize = true;
            this.DbSrvIpL.Location = new System.Drawing.Point(64, 42);
            this.DbSrvIpL.Name = "DbSrvIpL";
            this.DbSrvIpL.Size = new System.Drawing.Size(148, 19);
            this.DbSrvIpL.TabIndex = 4;
            this.DbSrvIpL.Text = "IP-адрес сервера БД";
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 19F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(255)))), ((int)(((byte)(216)))), ((int)(((byte)(139)))));
            this.ClientSize = new System.Drawing.Size(1096, 619);
            this.Controls.Add(this.MainTC);
            this.Font = new System.Drawing.Font("Calibri", 12F);
            this.Margin = new System.Windows.Forms.Padding(4);
            this.MaximumSize = new System.Drawing.Size(1200, 700);
            this.Name = "MainForm";
            this.Text = "Система автоматизированного учета материальных ценностей";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.MainForm_FormClosing);
            this.Load += new System.EventHandler(this.MainForm_Load);
            this.LogTP.ResumeLayout(false);
            this.LogTP.PerformLayout();
            this.DatabaseTP.ResumeLayout(false);
            this.MainTC.ResumeLayout(false);
            this.SettingsTP.ResumeLayout(false);
            this.SettingsTP.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion
        private System.Windows.Forms.TabPage LogTP;
        private System.Windows.Forms.Button LogB;
        private System.Windows.Forms.Label LogL;
        private System.Windows.Forms.TextBox LogTB;
        private System.Windows.Forms.TabPage DatabaseTP;
        private System.Windows.Forms.TabPage SettingsTP;
        private System.Windows.Forms.TextBox DbPasswordTB;
        private System.Windows.Forms.TextBox DbLoginTB;
        private System.Windows.Forms.TextBox DbSrvPortTB;
        private System.Windows.Forms.TextBox DbSrvIpTB;
        private System.Windows.Forms.Label DbPasswordL;
        private System.Windows.Forms.Label DbLoginL;
        private System.Windows.Forms.Label DbSrvPortL;
        private System.Windows.Forms.Label DbSrvIpL;
        private System.Windows.Forms.TabControl MainTC;
        private System.Windows.Forms.Button SrvStopB;
        private System.Windows.Forms.Button SrvStartB;
        private System.Windows.Forms.Label SrvStateL;
        private System.Windows.Forms.Label SrvPortL;
        private System.Windows.Forms.Label SrvIpL;
        private System.Windows.Forms.TextBox SrvPortTB;
        private System.Windows.Forms.TextBox SrvIpTB;
        private System.Windows.Forms.Label DbStateL;
        private System.Windows.Forms.Button SrvParamsB;
        private System.Windows.Forms.Button DbParamsB;
        private System.Windows.Forms.Button CheckDbB;
        private System.Windows.Forms.WebBrowser DbWB;
    }
}

