using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO.Ports;
using System.Runtime.InteropServices;
using static System.Windows.Forms.VisualStyles.VisualStyleElement;
using GMap.NET;
using GMap.NET.MapProviders;
using GMap.NET.WindowsForms;
using GMap.NET.WindowsForms.Markers;
using System.Drawing.Drawing2D;
using System.Windows.Forms.DataVisualization.Charting;
using Windows.Storage.Streams;
using System.IO;
using Newtonsoft.Json.Linq;
using System.Drawing.Imaging;
using GMap.NET.WindowsPresentation;

namespace GpsRotaCizme
{
    public partial class Form1 : Form
    {
        private SerialPort serialPort;
        private Timer VeriTimer;
        private Timer IslemTimer;
        
        private GrafikYonetici grafikyonetici;
        private PusulaYonetici pusulayonetici;
        private MapYonetici mapyonetici;

      
        private byte[] bytes =new byte[4];

        int intBits = 0;
        byte[] floatBuffer = new byte[4];

        private const int BufferSize = 100;
        private byte[] ringBuffer = new byte[BufferSize];
        private int writeIndex = 0;
        private int readIndex = 0;
        private object bufferLock = new object();

        private byte[] kullanilanBuffer = new byte[BufferSize];
        private int kullanilanBufferIndex_s8 = 0;
        private int startIndex = 0;

        byte[] RotaPaket = new byte[14];
        byte[] VersiyonPaket = new byte[9];
        byte[] YoklamaPaket = new byte[9];
        byte[] DurPaket= new byte[9];
        byte[] YonPaket = new byte[9];

        private Int32 maks = 70, min = 0;

        // --- Sabitler ve bayraklar ---
        private byte Baslik1 = 0x12;
        private byte Baslik2 = 0x34;
        private byte CRC8 = 0;
        private bool veriAlBayrak = true;
        private bool yoklamaFlag = false;
        private bool rotaGonderFlag = true;

        // --- Sayaçlar ---
        static int counter = 0;
        static int gpsCounter = 0;
        static int imuCounter = 0;
        static int sistemCounter = 0;


        private enum Durumlar
        {
            Baslik1Coz,
            Baslik2Coz,
            PaketTuruSec,
            DataBoyutuAl,
            DataOku
        }
        private enum GelenPaketler
        {
            GPS=0x01,
            IMU=0x02,
            VERSIYON=0x03,
            YOKLAMA=0x04,
            ROTA =0x05,
            SISTEM=0x06
        }
        private enum GidenPaketler
        {
            ROTA = 0x01,
            VERSIYON = 0x02,
            YOKLAMA = 0x03,
            DUR = 0x04,
            YON = 0x05
        }

        // --- Form başlangıcı ---
        public Form1()
        {
            InitializeComponent();
            ComboBoxYapilandir();
            PusulaYoneticisiYapilandir();

            grafikyonetici = new GrafikYonetici(new Chart[] { chart1, chart2, chart3 });
            mapyonetici = new MapYonetici(map);

            label19.Visible = false;
            label20.Visible = false;
            label37.Visible = false;
            button2.Visible = false;
            button1.Visible = true;

            comboBox1.SelectedIndexChanged += new EventHandler(ComboBox1SecilenIndexDegisimi);
            map.MouseClick += new MouseEventHandler(Map_MouseClick);
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            map.DragButton = MouseButtons.Right;
        }
        // --- Bağlan Butonu ---
        private void button1_Click(object sender, EventArgs e)
        {
            try
            {
                string secilenPort = comboBox2.SelectedItem.ToString();
                int secilenBaudRate = int.Parse(comboBox3.SelectedItem.ToString());

                button2.Visible = true;
                button1.Visible = false;

                SerialPortYapilandir(secilenPort, secilenBaudRate);
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Seri port açma hatası: {ex.Message}");
            }
        }
        // --- Seri port yapılandırma ---
        // --- Seri port ayarları ---
        private void SerialPortYapilandir(string portIsmi, int baudRate)
        {
            serialPort = new SerialPort
            {
                PortName = portIsmi,
                BaudRate = baudRate,
                ReadTimeout = 500,
                WriteTimeout = 500
            };

            serialPort.DataReceived += SerialPort_DataReceived;
            serialPort.Open();

            comboBox2.Visible = false;
            comboBox3.Visible = false;
            label25.Visible = false;
            label26.Visible = false;
        }

        // --- Veri geldiğinde otomatik tetiklenen olay ---
        private void SerialPort_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            try
            {
                if (!serialPort.IsOpen || !veriAlBayrak)
                    return;

                int alinanDataBytes = serialPort.BytesToRead;
                byte[] buffer = new byte[alinanDataBytes];
                serialPort.Read(buffer, 0, alinanDataBytes);

                lock (bufferLock)
                {
                    for (int i = 0; i < alinanDataBytes; i++)
                    {
                        kullanilanBuffer[kullanilanBufferIndex_s8] = buffer[i];
                        kullanilanBufferIndex_s8 = (kullanilanBufferIndex_s8 + 1) % BufferSize;
                    }
                }

                // Timer yerine burada doğrudan işleme geç
                this.BeginInvoke(new Action(() =>
                {
                    PaketCoz(kullanilanBuffer);
                }));
            }
            catch (Exception ex)
            {
                Console.WriteLine($"DataReceived hatası: {ex.Message}");
            }
        }


        // --- Kalıcı durum değişkenleri ---
        private Durumlar mevcutDurum = Durumlar.Baslik1Coz;
        private GelenPaketler mevcutPaket;
        private Int16 mevcutDataUzunlugu = 0;

        // --- Yardımcı: Mevcut veri miktarı ---
        private int KullanilabilirVeri()
        {
            if (kullanilanBufferIndex_s8 >= startIndex)
                return kullanilanBufferIndex_s8 - startIndex;
            else
                return BufferSize - startIndex + kullanilanBufferIndex_s8;
        }

        // --- Optimize edilmiş PaketCoz ---
        private void PaketCoz(byte[] dataBuffer)
        {
            while (KullanilabilirVeri() > 0)
            {
                switch (mevcutDurum)
                {
                    case Durumlar.Baslik1Coz:
                        if (dataBuffer[startIndex] == Baslik1)
                        {
                            mevcutDurum = Durumlar.Baslik2Coz;
                        }
                        startIndex = (startIndex + 1) % BufferSize;
                        break;

                    case Durumlar.Baslik2Coz:
                        if (KullanilabilirVeri() < 1)
                            return;

                        if (dataBuffer[startIndex] == Baslik2)
                        {
                            mevcutDurum = Durumlar.PaketTuruSec;
                        }
                        else
                        {
                            mevcutDurum = Durumlar.Baslik1Coz; // yanlış başlıksa sıfırla
                        }
                        startIndex = (startIndex + 1) % BufferSize;
                        break;

                    case Durumlar.PaketTuruSec:
                        if (KullanilabilirVeri() < 1)
                            return;

                        mevcutPaket = (GelenPaketler)dataBuffer[startIndex];
                        mevcutDurum = Durumlar.DataBoyutuAl;
                        startIndex = (startIndex + 1) % BufferSize;
                        break;

                    case Durumlar.DataBoyutuAl:
                        if (KullanilabilirVeri() < 1)
                            return;

                        mevcutDataUzunlugu = dataBuffer[startIndex];
                        mevcutDurum = Durumlar.DataOku;
                        startIndex = (startIndex + 1) % BufferSize;
                        break;

                    case Durumlar.DataOku:
                        if (KullanilabilirVeri() < mevcutDataUzunlugu)
                            return; // veri tamamlanmadı, bekle

                        try
                        {
                            switch (mevcutPaket)
                            {
                                case GelenPaketler.GPS:
                                    GpsPaketIsle(dataBuffer, mevcutDataUzunlugu);
                                    gpsCounter++;
                                    break;
                                case GelenPaketler.IMU:
                                    ImuPaketIsle(dataBuffer, mevcutDataUzunlugu);
                                    imuCounter++;
                                    break;
                                case GelenPaketler.SISTEM:
                                    SistemPaketIsle(dataBuffer, mevcutDataUzunlugu);
                                    sistemCounter++;
                                    break;
                                case GelenPaketler.VERSIYON:
                                    VersiyonPaketIsle(dataBuffer, mevcutDataUzunlugu);
                                    break;
                                case GelenPaketler.YOKLAMA:
                                    YoklamaPaketIsle(dataBuffer, mevcutDataUzunlugu);
                                    break;
                                case GelenPaketler.ROTA:
                                    RotaPaketIsle(dataBuffer, mevcutDataUzunlugu);
                                    break;
                            }
                        }
                        catch (Exception ex)
                        {
                            Console.WriteLine($"Paket çözüm hatası: {ex.Message}");
                        }

                        startIndex = (startIndex + mevcutDataUzunlugu) % BufferSize;
                        mevcutDurum = Durumlar.Baslik1Coz;
                        break;
                }
            }
        }


        private void GpsPaketIsle(byte[] dataBuffer, Int16 dataLength_s16)
        {
            if (dataLength_s16 == 13 && yoklamaFlag==true)
            {
                CRC8 = CRC8Hesaplama(dataBuffer, startIndex, startIndex + 12);
                if (dataBuffer[(startIndex + 12) % BufferSize] == CRC8)
                {
                    float Enlem = FloataDonustur(dataBuffer, startIndex);
                    float Boylam = FloataDonustur(dataBuffer, (startIndex + 4) % BufferSize);
                    float Irtifa = FloataDonustur(dataBuffer, (startIndex + 8) % BufferSize);
                    

                    if (Enlem != 0 && Boylam != 0)
                    {
                        label1.Text = Enlem.ToString();
                        label2.Text = Boylam.ToString();
                        mapyonetici.MapGuncelle(Enlem, Boylam);
                    }
                    label33.Text = Irtifa.ToString();
                    //label32.Text = Derece.ToString() + " C";
                    counter++;
                    label29.Text = gpsCounter.ToString();
                }
            }
        }
        private void ImuPaketIsle(byte[] dataBuffer, Int16 dataLength_s16)
        {
            if (dataLength_s16 == 17 && yoklamaFlag==true)
            {
                CRC8 = CRC8Hesaplama(dataBuffer, startIndex, startIndex + 16);
                if (dataBuffer[(startIndex + 16) % BufferSize] == CRC8)
                {
                    float Pitch = FloataDonustur(dataBuffer, startIndex);
                    float Roll = FloataDonustur(dataBuffer, (startIndex + 4) % BufferSize);
                    float Yaw = FloataDonustur(dataBuffer, (startIndex + 8) % BufferSize);
                    float Sicaklik = FloataDonustur(dataBuffer, (startIndex + 12) % BufferSize);

                    if (Pitch != 0 && Roll != 0 && Yaw != 0)
                    {
                        label16.Text = Pitch.ToString("F2");
                        label18.Text = Roll.ToString("F2");
                        label14.Text = Yaw.ToString("F2");
                        label23.Text = Sicaklik.ToString("F2");
                        PusulaFotografiniDondur((int)Yaw);
                        grafikyonetici.GrafikGuncelle(chart1, (int)Pitch, -90, +90, min, maks);
                        grafikyonetici.GrafikGuncelle(chart2, (int)Roll, -90, +90, min, maks);
                        grafikyonetici.GrafikGuncelle(chart3, (int)Yaw, 0, +360, min, maks);
                        min++;
                        maks++;
                    }
                    counter++;
                    label27.Text = imuCounter.ToString();
                }
            }
        }
        private void SistemPaketIsle(byte[] dataBuffer, Int16 dataLength_s16)
        {
            if (dataLength_s16 == 9 && yoklamaFlag == true)
            {
                CRC8 = CRC8Hesaplama(dataBuffer, startIndex, startIndex + 8);
                if (dataBuffer[(startIndex + 8) % BufferSize] == CRC8)
                {
                    float Derece = FloataDonustur(dataBuffer, startIndex);
                    float Batarya = FloataDonustur(dataBuffer, (startIndex + 4) % BufferSize);

                    label32.Text = Derece.ToString() + " C";
                    label38.Text = "% "+ Batarya.ToString();
                    counter++;
                }
            }
        }
        private void VersiyonPaketIsle(byte[] dataBuffer, Int16 dataLength_s16)
        {
            if (dataLength_s16 == 4)
            {
                CRC8 = CRC8Hesaplama(dataBuffer, startIndex, startIndex + 3);
                if (dataBuffer[(startIndex + 3) % BufferSize] == CRC8)
                {
                    float VersiyonB = dataBuffer[startIndex];
                    float VersiyonO = dataBuffer[startIndex + 1];
                    float VersiyonS = dataBuffer[startIndex + 2];

                    label31.Text = VersiyonB.ToString() + "." + VersiyonO.ToString() + "." + VersiyonS.ToString();
                    counter++;
                }
            }
        }
        private void YoklamaPaketIsle(byte[] dataBuffer, Int16 dataLength_s16)
        {
            if (dataLength_s16 == 4)
            {
                CRC8 = CRC8Hesaplama(dataBuffer, startIndex, startIndex + 3);
                if (dataBuffer[(startIndex + 3) % BufferSize] == CRC8)
                {
                    yoklamaFlag = true;
                    
                    label35.Text = "Veri Alınıyor ve Işleniyor..";
                    counter++;
                }
            }
        }
        private void RotaPaketIsle(byte[] dataBuffer, Int16 dataLength_s16)
        {
            if (dataLength_s16 == 4)
            {
                CRC8 = CRC8Hesaplama(dataBuffer, startIndex, startIndex + 3);
                if (dataBuffer[(startIndex + 3) % BufferSize] == CRC8)
                {
                    rotaGonderFlag= true;
                    label37.Text = "Araç Duruyor..";
                    counter++;
                }
            }
        }
        private byte CRC8Hesaplama(byte[]data, int baslangic,int bitis)
        {
            byte crc = 0x00;

            for (int i = baslangic; i < bitis; i++)
            {
                crc ^= data[i%BufferSize]; // CRC değerini, dizinin bir sonraki byte ile XOR
                for (int j = 0; j < 8; j++) //Her bir byte için döngü
                {
                    if ((crc & 0x80) != 0)//CRC değerinin en soldaki biti 1 mi
                    {
                        crc = (byte)((crc << 1) ^ 0X07); //En yüksek bit birse CRC değerini bir bit sola kaydır ve XOR işlemi yap
                    }
                    else
                    {
                        crc <<= 1; //En yüksek bit sıfırsa CRC değerini bir bit sola kaydır
                    }
                }
            }

            return crc;
        }

        private float FloataDonustur(byte[] buffer, int startIndex)
        {

            floatBuffer[0] = buffer[(startIndex + 3) % BufferSize];
            floatBuffer[1] = buffer[(startIndex + 2) % BufferSize];
            floatBuffer[2] = buffer[(startIndex + 1) % BufferSize];
            floatBuffer[3] = buffer[(startIndex + 0) % BufferSize];

            intBits = (floatBuffer[0] << 24) |
                      (floatBuffer[1] << 16) |
                      (floatBuffer[2] << 8)  |
                      (floatBuffer[3] << 0)   ;

            bytes = BitConverter.GetBytes(intBits);
            
            return BitConverter.ToSingle(bytes, 0);
        }
   
        private void ComboBox1SecilenIndexDegisimi(object sender, EventArgs e)
        {
            int secilenIndex = comboBox1.SelectedIndex;

            grafikyonetici.GrafikGorunurlugunuAyarla(secilenIndex);
        }
        private void PusulaFotografiniDondur(float baslik)
        {
            Image dondurulmusResim = pusulayonetici.DonmusResimAl(baslik);
            pictureBox1.Image = dondurulmusResim;
        }

        private void Map_MouseClick(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left && rotaGonderFlag == true)
            {
                // Harita tıklaması ile koordinat alımı
                PointLatLng nokta = map.FromLocalToLatLng(e.X, e.Y);
                float lat = (float)Math.Round(nokta.Lat, 6);
                float lng = (float)Math.Round(nokta.Lng, 6);
                label19.Text = lat.ToString();
                label20.Text = lng.ToString();

                byte[] latBytes = BitConverter.GetBytes(lat);
                byte[] lngBytes = BitConverter.GetBytes(lng);

                RotaPaket[0] = Baslik1;
                RotaPaket[1] = Baslik2;
                RotaPaket[2] = (byte)GidenPaketler.ROTA; // Rota Paket tanımı
                RotaPaket[3] = 0x09; // Data uzunluğu

                for (int i = 0; i < 4; i++)
                {
                    RotaPaket[4 + i] = latBytes[i];
                    RotaPaket[8 + i] = lngBytes[i];
                }

                CRC8 = CRC8Hesaplama(RotaPaket, 4, 12);
                RotaPaket[12] = CRC8;
                RotaPaket[13] = 0xFF;

                foreach (byte b in RotaPaket)
                {
                    serialPort.Write(new byte[] { b }, 0, 1);
                }

                mapyonetici.IsaretciEkle(nokta);
                label19.Visible = true;
                label20.Visible = true;
                label37.Visible = true;
                label37.Text = "Yeni Noktaya İlerleniyor..";
                rotaGonderFlag = false;
            }

            if (e.Button == MouseButtons.Middle)
            {
                rotaGonderFlag = true;
                DurPaket[0] = Baslik1;
                DurPaket[1] = Baslik2;
                DurPaket[2] = (byte)GidenPaketler.DUR; // Dur Paket tanımı
                DurPaket[3] = 0x04; // Data uzunluğu
                DurPaket[4] = 0x12;
                DurPaket[5] = 0x24;
                DurPaket[6] = 0x48;

                CRC8 = CRC8Hesaplama(DurPaket, 4, 7);
                DurPaket[7] = CRC8;
                DurPaket[8] = 0xFF;

                foreach (byte b in DurPaket)
                {
                    serialPort.Write(new byte[] { b }, 0, 1);
                }
            }
        }
        private void button3_Click(object sender, EventArgs e)
        {

            VersiyonPaket[0] = Baslik1;
            VersiyonPaket[1] = Baslik2;
            VersiyonPaket[2] = (byte)GidenPaketler.VERSIYON;//Versiyon Sorgu Paket tanımı
            VersiyonPaket[3] = 0x04; //Data Uzunluğu
            VersiyonPaket[4] = 0x15;
            VersiyonPaket[5] = 0x24;
            VersiyonPaket[6] = 0xA8;
            CRC8 = CRC8Hesaplama(VersiyonPaket, 4, 7);
            VersiyonPaket[7] = CRC8;
            VersiyonPaket[8] = 0xFF;
            if(yoklamaFlag==true)
            {
                foreach (byte b in VersiyonPaket)
                {
                    serialPort.Write(new byte[] { b }, 0, 1);
                }
            }
        }
        private void button4_Click(object sender, EventArgs e)
        {   
            yoklamaFlag= false;
            veriAlBayrak = true;
            YoklamaPaket[0] = Baslik1;
            YoklamaPaket[1] = Baslik2;
            YoklamaPaket[2] = (byte)GidenPaketler.YOKLAMA;//Yoklama Paket tanımı
            YoklamaPaket[3] = 0x04; //Data Uzunluğu
            YoklamaPaket[4] = 0x27;
            YoklamaPaket[5] = 0x24;
            YoklamaPaket[6] = 0xA1;
            CRC8 = CRC8Hesaplama(YoklamaPaket, 4, 7);
            YoklamaPaket[7] = CRC8;
            YoklamaPaket[8] = 0xFF;
            foreach (byte b in YoklamaPaket)
            {
                serialPort.Write(new byte[] { b }, 0, 1);
            }
            label35.Text = "Yoklama Bekleniyor..";
        }

        private void button2_Click(object sender, EventArgs e)
        {
            serialPort.Close();
            comboBox2.Visible = true;
            comboBox3.Visible = true;
            label25.Visible=true;
            label26.Visible=true;   
            button2.Visible=false;
            button1.Visible=true;
        }

        private void YonPaketGonder(GidenPaketler paketTipi, byte yon)
        {
            // Paket hazırla
            byte[] paket = new byte[8];
            paket[0] = Baslik1;
            paket[1] = Baslik2;
            paket[2] = (byte)paketTipi;
            paket[3] = 0x04;  // Data uzunluğu
            paket[4] = yon;
            paket[5] = 0x00;
            paket[6] = 0x00;
            paket[7] = CRC8Hesaplama(paket, 4, 7);

            if (yoklamaFlag && serialPort != null && serialPort.IsOpen)
            {
                Task.Run(() =>
                {
                    try
                    {
                        serialPort.Write(paket, 0, paket.Length);
                    }
                    catch (Exception ex)
                    {
                        Console.WriteLine("Serial gönderim hatası: " + ex.Message);
                    }
                });
            }
        }
        // Durum bayrakları
        private bool ileriAktif = false;
        private bool geriAktif = false;
        private bool sagAktif = false;
        private bool solAktif = false;
        private void ileriButton_Click(object sender, EventArgs e)
        {
            YonPaketGonder(GidenPaketler.YON, 0x01); // İleri 
        }

        private void solButton_Click(object sender, EventArgs e)
        {
            YonPaketGonder(GidenPaketler.YON, 0x04); // Sol
        }

        private void sagButton_Click(object sender, EventArgs e)
        {
            YonPaketGonder(GidenPaketler.YON, 0x03); // Sağ
        }

        private void geriButton_Click(object sender, EventArgs e)
        {
            YonPaketGonder(GidenPaketler.YON, 0x02); // Geri
        }
        private void ileriDurButon_Click(object sender, EventArgs e)
        {
            YonPaketGonder(GidenPaketler.YON, 0x05);
        }

        private void geriDurButon_Click(object sender, EventArgs e)
        {
            YonPaketGonder(GidenPaketler.YON, 0x06);
        }

        private void PusulaYoneticisiYapilandir()
        {
            string imagePath = @"C:\Users\T_rab\OneDrive\Desktop\compassPlateWhite-.png";
            pusulayonetici = new PusulaYonetici(imagePath);
            pictureBox1.Image = pusulayonetici.DonmusResimAl(0); // Başlangıçta 0 derece
            pictureBox1.SizeMode = PictureBoxSizeMode.Zoom;
        }

    

        private void ComboBoxYapilandir()
        {
            comboBox1.Items.AddRange(new string[] { "Pitch", "Roll", "Pusula" });
            comboBox1.SelectedIndex = 0;
            comboBox2.Items.AddRange(SerialPort.GetPortNames());
            if (comboBox2.Items.Count > 0)
                comboBox2.SelectedIndex = 0;
            comboBox3.Items.AddRange(new string[] { "9600", "19200", "39200", "115200" });
            comboBox3.SelectedIndex = 3;
        }


    }
}
