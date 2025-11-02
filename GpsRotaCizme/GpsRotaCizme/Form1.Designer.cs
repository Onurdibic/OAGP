namespace GpsRotaCizme
{
    partial class Form1
    {
        /// <summary>
        ///Gerekli tasarımcı değişkeni.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        ///Kullanılan tüm kaynakları temizleyin.
        /// </summary>
        ///<param name="disposing">yönetilen kaynaklar dispose edilmeliyse doğru; aksi halde yanlış.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer üretilen kod

        /// <summary>
        /// Tasarımcı desteği için gerekli metot - bu metodun 
        ///içeriğini kod düzenleyici ile değiştirmeyin.
        /// </summary>
        private void InitializeComponent()
        {
            System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea7 = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
            System.Windows.Forms.DataVisualization.Charting.Legend legend7 = new System.Windows.Forms.DataVisualization.Charting.Legend();
            System.Windows.Forms.DataVisualization.Charting.Series series7 = new System.Windows.Forms.DataVisualization.Charting.Series();
            System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea8 = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
            System.Windows.Forms.DataVisualization.Charting.Legend legend8 = new System.Windows.Forms.DataVisualization.Charting.Legend();
            System.Windows.Forms.DataVisualization.Charting.Series series8 = new System.Windows.Forms.DataVisualization.Charting.Series();
            System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea9 = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
            System.Windows.Forms.DataVisualization.Charting.Legend legend9 = new System.Windows.Forms.DataVisualization.Charting.Legend();
            System.Windows.Forms.DataVisualization.Charting.Series series9 = new System.Windows.Forms.DataVisualization.Charting.Series();
            this.map = new GMap.NET.WindowsForms.GMapControl();
            this.Enlem = new System.Windows.Forms.Label();
            this.Boylam = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.sqLiteCommand1 = new System.Data.SQLite.SQLiteCommand();
            this.label3 = new System.Windows.Forms.Label();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.backgroundWorker1 = new System.ComponentModel.BackgroundWorker();
            this.label6 = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.label9 = new System.Windows.Forms.Label();
            this.label10 = new System.Windows.Forms.Label();
            this.label11 = new System.Windows.Forms.Label();
            this.label12 = new System.Windows.Forms.Label();
            this.label14 = new System.Windows.Forms.Label();
            this.label13 = new System.Windows.Forms.Label();
            this.label15 = new System.Windows.Forms.Label();
            this.label16 = new System.Windows.Forms.Label();
            this.label17 = new System.Windows.Forms.Label();
            this.label18 = new System.Windows.Forms.Label();
            this.label20 = new System.Windows.Forms.Label();
            this.chart1 = new System.Windows.Forms.DataVisualization.Charting.Chart();
            this.comboBox1 = new System.Windows.Forms.ComboBox();
            this.chart2 = new System.Windows.Forms.DataVisualization.Charting.Chart();
            this.chart3 = new System.Windows.Forms.DataVisualization.Charting.Chart();
            this.label22 = new System.Windows.Forms.Label();
            this.label19 = new System.Windows.Forms.Label();
            this.label23 = new System.Windows.Forms.Label();
            this.label24 = new System.Windows.Forms.Label();
            this.label26 = new System.Windows.Forms.Label();
            this.button1 = new System.Windows.Forms.Button();
            this.button2 = new System.Windows.Forms.Button();
            this.comboBox2 = new System.Windows.Forms.ComboBox();
            this.comboBox3 = new System.Windows.Forms.ComboBox();
            this.label25 = new System.Windows.Forms.Label();
            this.label27 = new System.Windows.Forms.Label();
            this.label28 = new System.Windows.Forms.Label();
            this.label21 = new System.Windows.Forms.Label();
            this.label29 = new System.Windows.Forms.Label();
            this.label30 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.Arayuz = new System.Windows.Forms.TabControl();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.panel5 = new System.Windows.Forms.Panel();
            this.label40 = new System.Windows.Forms.Label();
            this.button4 = new System.Windows.Forms.Button();
            this.label35 = new System.Windows.Forms.Label();
            this.button3 = new System.Windows.Forms.Button();
            this.label31 = new System.Windows.Forms.Label();
            this.panel4 = new System.Windows.Forms.Panel();
            this.label39 = new System.Windows.Forms.Label();
            this.label38 = new System.Windows.Forms.Label();
            this.label32 = new System.Windows.Forms.Label();
            this.label36 = new System.Windows.Forms.Label();
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.panel2 = new System.Windows.Forms.Panel();
            this.panel1 = new System.Windows.Forms.Panel();
            this.tabPage3 = new System.Windows.Forms.TabPage();
            this.label37 = new System.Windows.Forms.Label();
            this.label33 = new System.Windows.Forms.Label();
            this.label34 = new System.Windows.Forms.Label();
            this.Manuel = new System.Windows.Forms.TabPage();
            this.solButton = new System.Windows.Forms.Button();
            this.geriButton = new System.Windows.Forms.Button();
            this.sagButton = new System.Windows.Forms.Button();
            this.ileriButton = new System.Windows.Forms.Button();
            this.ileriDurButon = new System.Windows.Forms.Button();
            this.geriDurButon = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.chart1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.chart2)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.chart3)).BeginInit();
            this.Arayuz.SuspendLayout();
            this.tabPage1.SuspendLayout();
            this.panel5.SuspendLayout();
            this.panel4.SuspendLayout();
            this.tabPage2.SuspendLayout();
            this.panel2.SuspendLayout();
            this.panel1.SuspendLayout();
            this.tabPage3.SuspendLayout();
            this.Manuel.SuspendLayout();
            this.SuspendLayout();
            // 
            // map
            // 
            this.map.Bearing = 0F;
            this.map.CanDragMap = true;
            this.map.EmptyTileColor = System.Drawing.Color.Navy;
            this.map.GrayScaleMode = false;
            this.map.HelperLineOption = GMap.NET.WindowsForms.HelperLineOptions.DontShow;
            this.map.LevelsKeepInMemory = 5;
            this.map.Location = new System.Drawing.Point(21, 23);
            this.map.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.map.MarkersEnabled = true;
            this.map.MaxZoom = 2;
            this.map.MinZoom = 2;
            this.map.MouseWheelZoomEnabled = true;
            this.map.MouseWheelZoomType = GMap.NET.MouseWheelZoomType.MousePositionAndCenter;
            this.map.Name = "map";
            this.map.NegativeMode = false;
            this.map.PolygonsEnabled = true;
            this.map.RetryLoadTile = 0;
            this.map.RoutesEnabled = true;
            this.map.ScaleMode = GMap.NET.WindowsForms.ScaleModes.Integer;
            this.map.SelectedAreaFillColor = System.Drawing.Color.FromArgb(((int)(((byte)(33)))), ((int)(((byte)(65)))), ((int)(((byte)(105)))), ((int)(((byte)(225)))));
            this.map.ShowTileGridLines = false;
            this.map.Size = new System.Drawing.Size(856, 544);
            this.map.TabIndex = 0;
            this.map.Zoom = 0D;
            // 
            // Enlem
            // 
            this.Enlem.AutoSize = true;
            this.Enlem.BackColor = System.Drawing.SystemColors.GradientActiveCaption;
            this.Enlem.Location = new System.Drawing.Point(899, 119);
            this.Enlem.Name = "Enlem";
            this.Enlem.Size = new System.Drawing.Size(63, 16);
            this.Enlem.TabIndex = 1;
            this.Enlem.Text = "Enlem      ";
            // 
            // Boylam
            // 
            this.Boylam.AutoSize = true;
            this.Boylam.BackColor = System.Drawing.SystemColors.GradientActiveCaption;
            this.Boylam.Location = new System.Drawing.Point(899, 151);
            this.Boylam.Name = "Boylam";
            this.Boylam.Size = new System.Drawing.Size(68, 16);
            this.Boylam.TabIndex = 2;
            this.Boylam.Text = "Boylam     ";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.BackColor = System.Drawing.SystemColors.GradientActiveCaption;
            this.label1.Location = new System.Drawing.Point(953, 119);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(44, 16);
            this.label1.TabIndex = 6;
            this.label1.Text = "label1";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.BackColor = System.Drawing.SystemColors.GradientActiveCaption;
            this.label2.Location = new System.Drawing.Point(961, 151);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(44, 16);
            this.label2.TabIndex = 7;
            this.label2.Text = "label2";
            // 
            // sqLiteCommand1
            // 
            this.sqLiteCommand1.CommandText = null;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.BackColor = System.Drawing.Color.Transparent;
            this.label3.Location = new System.Drawing.Point(126, 89);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(44, 16);
            this.label3.TabIndex = 8;
            this.label3.Text = "label3";
            // 
            // pictureBox1
            // 
            this.pictureBox1.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Center;
            this.pictureBox1.Location = new System.Drawing.Point(56, 66);
            this.pictureBox1.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(170, 170);
            this.pictureBox1.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.pictureBox1.TabIndex = 10;
            this.pictureBox1.TabStop = false;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(285, 166);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(21, 16);
            this.label6.TabIndex = 12;
            this.label6.Text = "90";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(155, 284);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(28, 16);
            this.label7.TabIndex = 13;
            this.label7.Text = "180";
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(21, 171);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(28, 16);
            this.label8.TabIndex = 14;
            this.label8.Text = "270";
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(261, 82);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(21, 16);
            this.label9.TabIndex = 15;
            this.label9.Text = "45";
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(243, 251);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(28, 16);
            this.label10.TabIndex = 16;
            this.label10.Text = "135";
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.Location = new System.Drawing.Point(64, 251);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(28, 16);
            this.label11.TabIndex = 17;
            this.label11.Text = "210";
            // 
            // label12
            // 
            this.label12.AutoSize = true;
            this.label12.Location = new System.Drawing.Point(53, 86);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(28, 16);
            this.label12.TabIndex = 18;
            this.label12.Text = "315";
            // 
            // label14
            // 
            this.label14.AutoSize = true;
            this.label14.Location = new System.Drawing.Point(173, 25);
            this.label14.Name = "label14";
            this.label14.Size = new System.Drawing.Size(51, 16);
            this.label14.TabIndex = 20;
            this.label14.Text = "label14";
            // 
            // label13
            // 
            this.label13.AutoSize = true;
            this.label13.Location = new System.Drawing.Point(125, 25);
            this.label13.Name = "label13";
            this.label13.Size = new System.Drawing.Size(36, 16);
            this.label13.TabIndex = 21;
            this.label13.Text = "Rota";
            // 
            // label15
            // 
            this.label15.AutoSize = true;
            this.label15.Location = new System.Drawing.Point(363, 34);
            this.label15.Name = "label15";
            this.label15.Size = new System.Drawing.Size(36, 16);
            this.label15.TabIndex = 23;
            this.label15.Text = "Pitch";
            // 
            // label16
            // 
            this.label16.AutoSize = true;
            this.label16.Location = new System.Drawing.Point(435, 34);
            this.label16.Name = "label16";
            this.label16.Size = new System.Drawing.Size(51, 16);
            this.label16.TabIndex = 22;
            this.label16.Text = "label16";
            // 
            // label17
            // 
            this.label17.AutoSize = true;
            this.label17.Location = new System.Drawing.Point(363, 60);
            this.label17.Name = "label17";
            this.label17.Size = new System.Drawing.Size(31, 16);
            this.label17.TabIndex = 25;
            this.label17.Text = "Roll";
            // 
            // label18
            // 
            this.label18.AutoSize = true;
            this.label18.Location = new System.Drawing.Point(435, 60);
            this.label18.Name = "label18";
            this.label18.Size = new System.Drawing.Size(51, 16);
            this.label18.TabIndex = 24;
            this.label18.Text = "label18";
            // 
            // label20
            // 
            this.label20.AutoSize = true;
            this.label20.Location = new System.Drawing.Point(899, 318);
            this.label20.Name = "label20";
            this.label20.Size = new System.Drawing.Size(51, 16);
            this.label20.TabIndex = 27;
            this.label20.Text = "label20";
            // 
            // chart1
            // 
            chartArea7.Name = "ChartArea1";
            this.chart1.ChartAreas.Add(chartArea7);
            this.chart1.Cursor = System.Windows.Forms.Cursors.Default;
            legend7.Name = "Legend1";
            this.chart1.Legends.Add(legend7);
            this.chart1.Location = new System.Drawing.Point(28, 54);
            this.chart1.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.chart1.Name = "chart1";
            series7.ChartArea = "ChartArea1";
            series7.ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Spline;
            series7.Legend = "Legend1";
            series7.Name = "Pitch";
            this.chart1.Series.Add(series7);
            this.chart1.Size = new System.Drawing.Size(381, 258);
            this.chart1.TabIndex = 28;
            this.chart1.Text = "chart1";
            // 
            // comboBox1
            // 
            this.comboBox1.FormattingEnabled = true;
            this.comboBox1.Location = new System.Drawing.Point(83, 21);
            this.comboBox1.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.comboBox1.Name = "comboBox1";
            this.comboBox1.Size = new System.Drawing.Size(141, 24);
            this.comboBox1.TabIndex = 29;
            // 
            // chart2
            // 
            chartArea8.Name = "ChartArea1";
            this.chart2.ChartAreas.Add(chartArea8);
            legend8.Name = "Legend1";
            this.chart2.Legends.Add(legend8);
            this.chart2.Location = new System.Drawing.Point(17, 54);
            this.chart2.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.chart2.Name = "chart2";
            series8.ChartArea = "ChartArea1";
            series8.ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Spline;
            series8.Legend = "Legend1";
            series8.Name = "Roll";
            this.chart2.Series.Add(series8);
            this.chart2.Size = new System.Drawing.Size(381, 258);
            this.chart2.TabIndex = 30;
            this.chart2.Text = "chart2";
            // 
            // chart3
            // 
            chartArea9.Name = "ChartArea1";
            this.chart3.ChartAreas.Add(chartArea9);
            legend9.Name = "Legend1";
            this.chart3.Legends.Add(legend9);
            this.chart3.Location = new System.Drawing.Point(17, 54);
            this.chart3.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.chart3.Name = "chart3";
            series9.ChartArea = "ChartArea1";
            series9.ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Spline;
            series9.Legend = "Legend1";
            series9.Name = "Pusula";
            this.chart3.Series.Add(series9);
            this.chart3.Size = new System.Drawing.Size(392, 258);
            this.chart3.TabIndex = 31;
            this.chart3.Text = "chart3";
            // 
            // label22
            // 
            this.label22.AutoSize = true;
            this.label22.Location = new System.Drawing.Point(13, 25);
            this.label22.Name = "label22";
            this.label22.Size = new System.Drawing.Size(63, 16);
            this.label22.TabIndex = 33;
            this.label22.Text = "Grafikler :";
            // 
            // label19
            // 
            this.label19.AutoSize = true;
            this.label19.Location = new System.Drawing.Point(996, 318);
            this.label19.Name = "label19";
            this.label19.Size = new System.Drawing.Size(51, 16);
            this.label19.TabIndex = 35;
            this.label19.Text = "label19";
            // 
            // label23
            // 
            this.label23.AutoSize = true;
            this.label23.Location = new System.Drawing.Point(126, 11);
            this.label23.Name = "label23";
            this.label23.Size = new System.Drawing.Size(51, 16);
            this.label23.TabIndex = 36;
            this.label23.Text = "label23";
            // 
            // label24
            // 
            this.label24.AutoSize = true;
            this.label24.Location = new System.Drawing.Point(13, 11);
            this.label24.Name = "label24";
            this.label24.Size = new System.Drawing.Size(54, 16);
            this.label24.TabIndex = 37;
            this.label24.Text = "Sıcaklık";
            // 
            // label26
            // 
            this.label26.AutoSize = true;
            this.label26.Location = new System.Drawing.Point(669, 34);
            this.label26.Name = "label26";
            this.label26.Size = new System.Drawing.Size(120, 16);
            this.label26.TabIndex = 35;
            this.label26.Text = "Lütfen Rate Seçiniz";
            // 
            // button1
            // 
            this.button1.BackColor = System.Drawing.Color.Lime;
            this.button1.Location = new System.Drawing.Point(853, 18);
            this.button1.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(147, 85);
            this.button1.TabIndex = 36;
            this.button1.Text = "START";
            this.button1.UseVisualStyleBackColor = false;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // button2
            // 
            this.button2.BackColor = System.Drawing.Color.Red;
            this.button2.Location = new System.Drawing.Point(853, 18);
            this.button2.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(147, 85);
            this.button2.TabIndex = 31;
            this.button2.Text = "STOP";
            this.button2.UseVisualStyleBackColor = false;
            this.button2.Click += new System.EventHandler(this.button2_Click);
            // 
            // comboBox2
            // 
            this.comboBox2.FormattingEnabled = true;
            this.comboBox2.Location = new System.Drawing.Point(493, 60);
            this.comboBox2.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.comboBox2.Name = "comboBox2";
            this.comboBox2.Size = new System.Drawing.Size(121, 24);
            this.comboBox2.TabIndex = 0;
            // 
            // comboBox3
            // 
            this.comboBox3.FormattingEnabled = true;
            this.comboBox3.Location = new System.Drawing.Point(669, 60);
            this.comboBox3.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.comboBox3.Name = "comboBox3";
            this.comboBox3.Size = new System.Drawing.Size(121, 24);
            this.comboBox3.TabIndex = 1;
            // 
            // label25
            // 
            this.label25.AutoSize = true;
            this.label25.Location = new System.Drawing.Point(493, 34);
            this.label25.Name = "label25";
            this.label25.Size = new System.Drawing.Size(121, 16);
            this.label25.TabIndex = 34;
            this.label25.Text = "Lütfen COM Seçiniz";
            // 
            // label27
            // 
            this.label27.AutoSize = true;
            this.label27.BackColor = System.Drawing.Color.LightGray;
            this.label27.Location = new System.Drawing.Point(508, 23);
            this.label27.Name = "label27";
            this.label27.Size = new System.Drawing.Size(51, 16);
            this.label27.TabIndex = 39;
            this.label27.Text = "label27";
            // 
            // label28
            // 
            this.label28.AutoSize = true;
            this.label28.BackColor = System.Drawing.Color.LightGray;
            this.label28.Location = new System.Drawing.Point(379, 23);
            this.label28.Name = "label28";
            this.label28.Size = new System.Drawing.Size(154, 16);
            this.label28.TabIndex = 40;
            this.label28.Text = "Imu Paket Sayısı‎‎   ‎             ";
            // 
            // label21
            // 
            this.label21.AutoSize = true;
            this.label21.Location = new System.Drawing.Point(899, 276);
            this.label21.Name = "label21";
            this.label21.Size = new System.Drawing.Size(133, 16);
            this.label21.TabIndex = 39;
            this.label21.Text = "Gönderilen Koordinat";
            // 
            // label29
            // 
            this.label29.AutoSize = true;
            this.label29.BackColor = System.Drawing.SystemColors.Info;
            this.label29.Location = new System.Drawing.Point(1025, 53);
            this.label29.Name = "label29";
            this.label29.Size = new System.Drawing.Size(51, 16);
            this.label29.TabIndex = 39;
            this.label29.Text = "label29";
            // 
            // label30
            // 
            this.label30.AutoSize = true;
            this.label30.BackColor = System.Drawing.SystemColors.Info;
            this.label30.Location = new System.Drawing.Point(883, 53);
            this.label30.Name = "label30";
            this.label30.Size = new System.Drawing.Size(158, 16);
            this.label30.TabIndex = 40;
            this.label30.Text = "Gps Paket Sayısı‎‎   ‎             ";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(155, 50);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(30, 16);
            this.label5.TabIndex = 41;
            this.label5.Text = "0(K)";
            // 
            // Arayuz
            // 
            this.Arayuz.Controls.Add(this.tabPage1);
            this.Arayuz.Controls.Add(this.tabPage2);
            this.Arayuz.Controls.Add(this.tabPage3);
            this.Arayuz.Controls.Add(this.Manuel);
            this.Arayuz.Location = new System.Drawing.Point(29, 30);
            this.Arayuz.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.Arayuz.Name = "Arayuz";
            this.Arayuz.SelectedIndex = 0;
            this.Arayuz.Size = new System.Drawing.Size(1117, 622);
            this.Arayuz.TabIndex = 42;
            this.Arayuz.Tag = "Sistem";
            // 
            // tabPage1
            // 
            this.tabPage1.Controls.Add(this.panel5);
            this.tabPage1.Controls.Add(this.panel4);
            this.tabPage1.Controls.Add(this.button1);
            this.tabPage1.Controls.Add(this.comboBox2);
            this.tabPage1.Controls.Add(this.button2);
            this.tabPage1.Controls.Add(this.comboBox3);
            this.tabPage1.Controls.Add(this.label25);
            this.tabPage1.Controls.Add(this.label26);
            this.tabPage1.Location = new System.Drawing.Point(4, 25);
            this.tabPage1.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.tabPage1.Size = new System.Drawing.Size(1109, 593);
            this.tabPage1.TabIndex = 0;
            this.tabPage1.Text = "Sistem";
            this.tabPage1.UseVisualStyleBackColor = true;
            // 
            // panel5
            // 
            this.panel5.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(255)))), ((int)(((byte)(224)))), ((int)(((byte)(192)))));
            this.panel5.Controls.Add(this.label40);
            this.panel5.Controls.Add(this.button4);
            this.panel5.Controls.Add(this.label3);
            this.panel5.Controls.Add(this.label35);
            this.panel5.Controls.Add(this.button3);
            this.panel5.Controls.Add(this.label31);
            this.panel5.Location = new System.Drawing.Point(25, 34);
            this.panel5.Name = "panel5";
            this.panel5.Size = new System.Drawing.Size(283, 117);
            this.panel5.TabIndex = 49;
            // 
            // label40
            // 
            this.label40.AutoSize = true;
            this.label40.BackColor = System.Drawing.Color.Transparent;
            this.label40.Location = new System.Drawing.Point(17, 89);
            this.label40.Name = "label40";
            this.label40.Size = new System.Drawing.Size(82, 16);
            this.label40.TabIndex = 49;
            this.label40.Text = "Paket Sayısı‎‎";
            // 
            // button4
            // 
            this.button4.BackColor = System.Drawing.Color.Transparent;
            this.button4.Location = new System.Drawing.Point(16, 15);
            this.button4.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.button4.Name = "button4";
            this.button4.Size = new System.Drawing.Size(75, 23);
            this.button4.TabIndex = 42;
            this.button4.Text = "Yoklama";
            this.button4.UseVisualStyleBackColor = false;
            this.button4.Click += new System.EventHandler(this.button4_Click);
            // 
            // label35
            // 
            this.label35.AutoSize = true;
            this.label35.BackColor = System.Drawing.Color.Transparent;
            this.label35.Location = new System.Drawing.Point(126, 18);
            this.label35.Name = "label35";
            this.label35.Size = new System.Drawing.Size(51, 16);
            this.label35.TabIndex = 43;
            this.label35.Text = "label35";
            // 
            // button3
            // 
            this.button3.BackColor = System.Drawing.Color.Transparent;
            this.button3.Location = new System.Drawing.Point(16, 50);
            this.button3.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.button3.Name = "button3";
            this.button3.Size = new System.Drawing.Size(75, 23);
            this.button3.TabIndex = 39;
            this.button3.Text = "Versiyon";
            this.button3.UseVisualStyleBackColor = false;
            this.button3.Click += new System.EventHandler(this.button3_Click);
            // 
            // label31
            // 
            this.label31.AutoSize = true;
            this.label31.BackColor = System.Drawing.Color.Transparent;
            this.label31.Location = new System.Drawing.Point(126, 53);
            this.label31.Name = "label31";
            this.label31.Size = new System.Drawing.Size(51, 16);
            this.label31.TabIndex = 40;
            this.label31.Text = "label31";
            // 
            // panel4
            // 
            this.panel4.BackColor = System.Drawing.Color.Silver;
            this.panel4.Controls.Add(this.label24);
            this.panel4.Controls.Add(this.label23);
            this.panel4.Controls.Add(this.label39);
            this.panel4.Controls.Add(this.label38);
            this.panel4.Controls.Add(this.label32);
            this.panel4.Controls.Add(this.label36);
            this.panel4.Location = new System.Drawing.Point(25, 157);
            this.panel4.Name = "panel4";
            this.panel4.Size = new System.Drawing.Size(229, 100);
            this.panel4.TabIndex = 48;
            // 
            // label39
            // 
            this.label39.AutoSize = true;
            this.label39.Location = new System.Drawing.Point(13, 67);
            this.label39.Name = "label39";
            this.label39.Size = new System.Drawing.Size(54, 16);
            this.label39.TabIndex = 47;
            this.label39.Text = "Batarya";
            // 
            // label38
            // 
            this.label38.AutoSize = true;
            this.label38.Location = new System.Drawing.Point(126, 67);
            this.label38.Name = "label38";
            this.label38.Size = new System.Drawing.Size(51, 16);
            this.label38.TabIndex = 46;
            this.label38.Text = "label38";
            // 
            // label32
            // 
            this.label32.AutoSize = true;
            this.label32.BackColor = System.Drawing.Color.Transparent;
            this.label32.Location = new System.Drawing.Point(126, 36);
            this.label32.Name = "label32";
            this.label32.Size = new System.Drawing.Size(51, 16);
            this.label32.TabIndex = 41;
            this.label32.Text = "label32";
            // 
            // label36
            // 
            this.label36.AutoSize = true;
            this.label36.BackColor = System.Drawing.Color.Transparent;
            this.label36.Location = new System.Drawing.Point(13, 36);
            this.label36.Name = "label36";
            this.label36.Size = new System.Drawing.Size(127, 16);
            this.label36.TabIndex = 44;
            this.label36.Text = "Ortam Sıcaklığı          ";
            // 
            // tabPage2
            // 
            this.tabPage2.Controls.Add(this.label27);
            this.tabPage2.Controls.Add(this.panel2);
            this.tabPage2.Controls.Add(this.panel1);
            this.tabPage2.Controls.Add(this.label28);
            this.tabPage2.Location = new System.Drawing.Point(4, 25);
            this.tabPage2.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Padding = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.tabPage2.Size = new System.Drawing.Size(1109, 593);
            this.tabPage2.TabIndex = 1;
            this.tabPage2.Text = "Imu";
            this.tabPage2.UseVisualStyleBackColor = true;
            // 
            // panel2
            // 
            this.panel2.BackColor = System.Drawing.Color.LightGray;
            this.panel2.Controls.Add(this.chart3);
            this.panel2.Controls.Add(this.comboBox1);
            this.panel2.Controls.Add(this.chart1);
            this.panel2.Controls.Add(this.chart2);
            this.panel2.Controls.Add(this.label22);
            this.panel2.Location = new System.Drawing.Point(21, 52);
            this.panel2.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.panel2.Name = "panel2";
            this.panel2.Size = new System.Drawing.Size(445, 347);
            this.panel2.TabIndex = 43;
            // 
            // panel1
            // 
            this.panel1.BackColor = System.Drawing.Color.LightGray;
            this.panel1.Controls.Add(this.label5);
            this.panel1.Controls.Add(this.label15);
            this.panel1.Controls.Add(this.label10);
            this.panel1.Controls.Add(this.label11);
            this.panel1.Controls.Add(this.label18);
            this.panel1.Controls.Add(this.label16);
            this.panel1.Controls.Add(this.label9);
            this.panel1.Controls.Add(this.label12);
            this.panel1.Controls.Add(this.label17);
            this.panel1.Controls.Add(this.label13);
            this.panel1.Controls.Add(this.label8);
            this.panel1.Controls.Add(this.label14);
            this.panel1.Controls.Add(this.label7);
            this.panel1.Controls.Add(this.label6);
            this.panel1.Controls.Add(this.pictureBox1);
            this.panel1.Location = new System.Drawing.Point(487, 52);
            this.panel1.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(495, 347);
            this.panel1.TabIndex = 42;
            // 
            // tabPage3
            // 
            this.tabPage3.Controls.Add(this.label37);
            this.tabPage3.Controls.Add(this.label33);
            this.tabPage3.Controls.Add(this.label29);
            this.tabPage3.Controls.Add(this.label2);
            this.tabPage3.Controls.Add(this.label1);
            this.tabPage3.Controls.Add(this.label21);
            this.tabPage3.Controls.Add(this.Enlem);
            this.tabPage3.Controls.Add(this.Boylam);
            this.tabPage3.Controls.Add(this.label30);
            this.tabPage3.Controls.Add(this.label20);
            this.tabPage3.Controls.Add(this.label19);
            this.tabPage3.Controls.Add(this.label34);
            this.tabPage3.Controls.Add(this.map);
            this.tabPage3.Location = new System.Drawing.Point(4, 25);
            this.tabPage3.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.tabPage3.Name = "tabPage3";
            this.tabPage3.Padding = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.tabPage3.Size = new System.Drawing.Size(1109, 593);
            this.tabPage3.TabIndex = 2;
            this.tabPage3.Text = "Gps";
            this.tabPage3.UseVisualStyleBackColor = true;
            // 
            // label37
            // 
            this.label37.AutoSize = true;
            this.label37.Location = new System.Drawing.Point(916, 508);
            this.label37.Name = "label37";
            this.label37.Size = new System.Drawing.Size(51, 16);
            this.label37.TabIndex = 44;
            this.label37.Text = "label37";
            // 
            // label33
            // 
            this.label33.AutoSize = true;
            this.label33.BackColor = System.Drawing.SystemColors.GradientActiveCaption;
            this.label33.Location = new System.Drawing.Point(948, 182);
            this.label33.Name = "label33";
            this.label33.Size = new System.Drawing.Size(51, 16);
            this.label33.TabIndex = 42;
            this.label33.Text = "label33";
            // 
            // label34
            // 
            this.label34.AutoSize = true;
            this.label34.BackColor = System.Drawing.SystemColors.GradientActiveCaption;
            this.label34.Location = new System.Drawing.Point(899, 182);
            this.label34.Name = "label34";
            this.label34.Size = new System.Drawing.Size(82, 16);
            this.label34.TabIndex = 43;
            this.label34.Text = "Irtifa                 ";
            // 
            // Manuel
            // 
            this.Manuel.Controls.Add(this.geriDurButon);
            this.Manuel.Controls.Add(this.ileriDurButon);
            this.Manuel.Controls.Add(this.solButton);
            this.Manuel.Controls.Add(this.geriButton);
            this.Manuel.Controls.Add(this.sagButton);
            this.Manuel.Controls.Add(this.ileriButton);
            this.Manuel.Location = new System.Drawing.Point(4, 25);
            this.Manuel.Name = "Manuel";
            this.Manuel.Padding = new System.Windows.Forms.Padding(3);
            this.Manuel.Size = new System.Drawing.Size(1109, 593);
            this.Manuel.TabIndex = 3;
            this.Manuel.Text = "Manuel";
            this.Manuel.UseVisualStyleBackColor = true;
            // 
            // solButton
            // 
            this.solButton.BackColor = System.Drawing.Color.Yellow;
            this.solButton.Location = new System.Drawing.Point(147, 174);
            this.solButton.Name = "solButton";
            this.solButton.Size = new System.Drawing.Size(106, 92);
            this.solButton.TabIndex = 8;
            this.solButton.Text = "SOL";
            this.solButton.UseVisualStyleBackColor = false;
            this.solButton.Click += new System.EventHandler(this.solButton_Click);
            // 
            // geriButton
            // 
            this.geriButton.BackColor = System.Drawing.Color.Yellow;
            this.geriButton.Location = new System.Drawing.Point(259, 272);
            this.geriButton.Name = "geriButton";
            this.geriButton.Size = new System.Drawing.Size(106, 88);
            this.geriButton.TabIndex = 7;
            this.geriButton.Text = "GERI";
            this.geriButton.UseVisualStyleBackColor = false;
            this.geriButton.Click += new System.EventHandler(this.geriButton_Click);
            // 
            // sagButton
            // 
            this.sagButton.BackColor = System.Drawing.Color.Yellow;
            this.sagButton.Location = new System.Drawing.Point(372, 174);
            this.sagButton.Name = "sagButton";
            this.sagButton.Size = new System.Drawing.Size(106, 92);
            this.sagButton.TabIndex = 6;
            this.sagButton.Text = "SAĞ";
            this.sagButton.UseVisualStyleBackColor = false;
            this.sagButton.Click += new System.EventHandler(this.sagButton_Click);
            // 
            // ileriButton
            // 
            this.ileriButton.BackColor = System.Drawing.Color.Yellow;
            this.ileriButton.ForeColor = System.Drawing.Color.Black;
            this.ileriButton.Location = new System.Drawing.Point(259, 85);
            this.ileriButton.Name = "ileriButton";
            this.ileriButton.Size = new System.Drawing.Size(106, 84);
            this.ileriButton.TabIndex = 5;
            this.ileriButton.Text = "ILERI";
            this.ileriButton.UseVisualStyleBackColor = false;
            this.ileriButton.Click += new System.EventHandler(this.ileriButton_Click);
            // 
            // ileriDurButon
            // 
            this.ileriDurButon.BackColor = System.Drawing.Color.Red;
            this.ileriDurButon.Location = new System.Drawing.Point(544, 131);
            this.ileriDurButon.Name = "ileriDurButon";
            this.ileriDurButon.Size = new System.Drawing.Size(110, 77);
            this.ileriDurButon.TabIndex = 9;
            this.ileriDurButon.Text = "İLERİ DUR";
            this.ileriDurButon.UseVisualStyleBackColor = false;
            this.ileriDurButon.Click += new System.EventHandler(this.ileriDurButon_Click);
            // 
            // geriDurButon
            // 
            this.geriDurButon.BackColor = System.Drawing.Color.Red;
            this.geriDurButon.Location = new System.Drawing.Point(544, 247);
            this.geriDurButon.Name = "geriDurButon";
            this.geriDurButon.Size = new System.Drawing.Size(110, 80);
            this.geriDurButon.TabIndex = 10;
            this.geriDurButon.Text = "GERİ DUR";
            this.geriDurButon.UseVisualStyleBackColor = false;
            this.geriDurButon.Click += new System.EventHandler(this.geriDurButon_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.SystemColors.ButtonFace;
            this.ClientSize = new System.Drawing.Size(1161, 665);
            this.Controls.Add(this.Arayuz);
            this.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.Name = "Form1";
            this.Text = "Form1";
            this.Load += new System.EventHandler(this.Form1_Load);
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.chart1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.chart2)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.chart3)).EndInit();
            this.Arayuz.ResumeLayout(false);
            this.tabPage1.ResumeLayout(false);
            this.tabPage1.PerformLayout();
            this.panel5.ResumeLayout(false);
            this.panel5.PerformLayout();
            this.panel4.ResumeLayout(false);
            this.panel4.PerformLayout();
            this.tabPage2.ResumeLayout(false);
            this.tabPage2.PerformLayout();
            this.panel2.ResumeLayout(false);
            this.panel2.PerformLayout();
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.tabPage3.ResumeLayout(false);
            this.tabPage3.PerformLayout();
            this.Manuel.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private GMap.NET.WindowsForms.GMapControl map;
        private System.Windows.Forms.Label Enlem;
        private System.Windows.Forms.Label Boylam;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Data.SQLite.SQLiteCommand sqLiteCommand1;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.PictureBox pictureBox1;
        private System.ComponentModel.BackgroundWorker backgroundWorker1;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.Label label12;
        private System.Windows.Forms.Label label14;
        private System.Windows.Forms.Label label13;
        private System.Windows.Forms.Label label15;
        private System.Windows.Forms.Label label16;
        private System.Windows.Forms.Label label17;
        private System.Windows.Forms.Label label18;
        private System.Windows.Forms.Label label20;
        private System.Windows.Forms.DataVisualization.Charting.Chart chart1;
        private System.Windows.Forms.ComboBox comboBox1;
        private System.Windows.Forms.DataVisualization.Charting.Chart chart2;
        private System.Windows.Forms.DataVisualization.Charting.Chart chart3;
        private System.Windows.Forms.Label label22;
        private System.Windows.Forms.Label label19;
        private System.Windows.Forms.Label label23;
        private System.Windows.Forms.Label label24;
        private System.Windows.Forms.Label label26;
        private System.Windows.Forms.Label label25;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.ComboBox comboBox3;
        private System.Windows.Forms.ComboBox comboBox2;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Label label27;
        private System.Windows.Forms.Label label28;
        private System.Windows.Forms.Label label29;
        private System.Windows.Forms.Label label30;
        private System.Windows.Forms.Label label21;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TabControl Arayuz;
        private System.Windows.Forms.TabPage tabPage1;
        private System.Windows.Forms.TabPage tabPage2;
        private System.Windows.Forms.TabPage tabPage3;
        private System.Windows.Forms.Label label31;
        private System.Windows.Forms.Button button3;
        private System.Windows.Forms.Label label32;
        private System.Windows.Forms.Label label33;
        private System.Windows.Forms.Label label34;
        private System.Windows.Forms.Label label35;
        private System.Windows.Forms.Button button4;
        private System.Windows.Forms.Label label36;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Panel panel2;
        private System.Windows.Forms.Label label37;
        private System.Windows.Forms.Label label38;
        private System.Windows.Forms.Label label39;
        private System.Windows.Forms.Panel panel4;
        private System.Windows.Forms.Label label40;
        private System.Windows.Forms.Panel panel5;
        private System.Windows.Forms.TabPage Manuel;
        private System.Windows.Forms.Button geriButton;
        private System.Windows.Forms.Button sagButton;
        private System.Windows.Forms.Button ileriButton;
        private System.Windows.Forms.Button solButton;
        private System.Windows.Forms.Button geriDurButon;
        private System.Windows.Forms.Button ileriDurButon;
    }
}

