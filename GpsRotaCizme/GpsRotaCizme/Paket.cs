using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms.DataVisualization.Charting;

namespace GpsRotaCizme
{
    
    public class PaketIsleyici
    {/*
        private enum Durumlar
        {
            Baslik1Coz,
            Baslik2Coz,
            PaketTuruSec,
            DataBoyutuAl,
            DataOku

        }
        private enum Paketler
        {
            GPS = 0x01,
            IMU = 0x02,
            VERSIYON = 0x03,
            YOKLAMA = 0x04,
            ROTA = 0x05

        }
        private const byte Baslik1 = 0x12;
        private const byte Baslik2 = 0x34;
        private int baslangicIndeksi = 0;
        private const int BufferSize = 700;
        bool yoklamaFlag = false;
        byte CRC8 = 0;
        int intBits = 0;
        byte[] bytes = new byte[4];
        byte[] floatBuffer = new byte[4];

        public void VeriIsle(byte[] veriBuffer, int bufferIndeks)
        {
            Int16 dataLength_s16 = 0;

            Durumlar Durum = Durumlar.Baslik1Coz;
            Paketler Paket = Paketler.IMU;
            label3.Text = counter.ToString();
            while (baslangicIndeksi != bufferIndeks)
            {
                switch (Durum)
                {
                    case Durumlar.Baslik1Coz:
                        if (veriBuffer[baslangicIndeksi] == Baslik1 && veriBuffer[baslangicIndeksi] != 0)
                        {
                            Durum = Durumlar.Baslik2Coz;
                        }
                        baslangicIndeksi = (baslangicIndeksi + 1) % BufferSize;
                        break;

                    case Durumlar.Baslik2Coz:
                        if (veriBuffer[baslangicIndeksi] == Baslik2 && veriBuffer[baslangicIndeksi] != 0)
                        {
                            Durum = Durumlar.PaketTuruSec;
                            baslangicIndeksi = (baslangicIndeksi + 1) % BufferSize;
                        }
                        else
                        {
                            Durum = Durumlar.Baslik1Coz;
                            baslangicIndeksi = (baslangicIndeksi + 1) % BufferSize;
                        }
                        break;

                    case Durumlar.PaketTuruSec:
                        Paket = (Paketler)veriBuffer[baslangicIndeksi];
                        Durum = Durumlar.DataBoyutuAl;
                        baslangicIndeksi = (baslangicIndeksi + 1) % BufferSize;
                        break;

                    case Durumlar.DataBoyutuAl:
                        dataLength_s16 = veriBuffer[baslangicIndeksi];
                        Durum = Durumlar.DataOku;
                        baslangicIndeksi = (baslangicIndeksi + 1) % BufferSize;
                        break;

                    case Durumlar.DataOku:
                        if (baslangicIndeksi == bufferIndeks)
                        {
                            return;
                        }

                        switch (Paket)
                        {
                            case Paketler.GPS:
                                GpsPaketIsle(veriBuffer, baslangicIndeksi, dataLength_s16);
                                break;
                            case Paketler.IMU:
                                ImuPaketIsle(veriBuffer, baslangicIndeksi, dataLength_s16);
                                break;
                            case Paketler.VERSIYON:
                                VersiyonPaketIsle(veriBuffer, baslangicIndeksi, dataLength_s16);
                                break;
                            case Paketler.YOKLAMA:
                                YoklamaPaketIsle(veriBuffer, baslangicIndeksi, dataLength_s16);
                                break;
                            case Paketler.ROTA:
                                RotaPaketIsle(veriBuffer, baslangicIndeksi, dataLength_s16);
                                break;
                        }

                        baslangicIndeksi = (baslangicIndeksi + dataLength_s16) % BufferSize;
                        Durum = Durumlar.Baslik1Coz;
                        break;
                }
            }
        }

        private void GpsPaketIsle(byte[] veriBuffer, int baslangicIndeksi, int veriUzunlugu)
        {
            if (veriUzunlugu == 17 && yoklamaFlag == true)
            {
                CRC8 = CRC8Hesaplama(veriBuffer, baslangicIndeksi, baslangicIndeksi + 16);
                if (veriBuffer[(baslangicIndeksi + 16) % BufferSize] == CRC8)
                {
                    float Enlem = FloataDonustur(veriBuffer, baslangicIndeksi);
                    float Boylam = FloataDonustur(veriBuffer, (baslangicIndeksi + 4) % BufferSize);
                    float Irtifa = FloataDonustur(veriBuffer, (baslangicIndeksi + 8) % BufferSize);
                    float Derece = FloataDonustur(veriBuffer, (baslangicIndeksi + 12) % BufferSize);

                    if (Enlem != 0 && Boylam != 0)
                    {
                        label1.Text = Enlem.ToString();
                        label2.Text = Boylam.ToString();
                        MapGuncelle(Enlem, Boylam);
                    }
                    label33.Text = Irtifa.ToString();
                    label32.Text = Derece.ToString() + " C";
                    gpsCounter++;
                    counter++;
                    label29.Text = gpsCounter.ToString();
                }
            }
        }

        private void ImuPaketIsle(byte[] veriBuffer, int baslangicIndeksi, int veriUzunlugu)
        {
            if (veriUzunlugu == 17 && yoklamaFlag == true)
            {
                CRC8 = CRC8Hesaplama(veriBuffer, baslangicIndeksi, baslangicIndeksi + 16);
                if (veriBuffer[(baslangicIndeksi + 16) % BufferSize] == CRC8)
                {
                    float Pitch = FloataDonustur(veriBuffer, baslangicIndeksi);
                    float Roll = FloataDonustur(veriBuffer, (baslangicIndeksi + 4) % BufferSize);
                    float Yaw = FloataDonustur(veriBuffer, (baslangicIndeksi + 8) % BufferSize);
                    float Sicaklik = FloataDonustur(veriBuffer, (baslangicIndeksi + 12) % BufferSize);

                    if (Pitch != 0 && Roll != 0 && Yaw != 0)
                    {
                        label16.Text = Pitch.ToString("F2");
                        label18.Text = Roll.ToString("F2");
                        label14.Text = Yaw.ToString("F2");
                        label23.Text = Sicaklik.ToString("F2");
                        PusulaFotografiniDondur((int)Yaw);
                        GrafikGuncelle(chart1, (int)Pitch, -90, +90);
                        GrafikGuncelle(chart2, (int)Roll, -90, +90);
                        GrafikGuncelle(chart3, (int)Yaw, 0, +360);
                        min++;
                        maks++;
                    }
                    imuCounter++;
                    counter++;
                    label27.Text = imuCounter.ToString();
                }
            }
        }

        private void VersiyonPaketIsle(byte[] veriBuffer, int baslangicIndeksi, int veriUzunlugu)
        {
            if (veriUzunlugu == 4)
            {
                CRC8 = CRC8Hesaplama(veriBuffer, baslangicIndeksi, baslangicIndeksi + 3);
                if (veriBuffer[(baslangicIndeksi + 3) % BufferSize] == CRC8)
                {
                    float VersiyonB = veriBuffer[baslangicIndeksi];
                    float VersiyonO = veriBuffer[baslangicIndeksi + 1];
                    float VersiyonS = veriBuffer[baslangicIndeksi + 2];

                    label31.Text = VersiyonB.ToString() + "." + VersiyonO.ToString() + "." + VersiyonS.ToString();
                    counter++;
                }
            }
        }

        private void YoklamaPaketIsle(byte[] veriBuffer, int baslangicIndeksi, int veriUzunlugu)
        {
            if (veriUzunlugu == 4)
            {
                CRC8 = CRC8Hesaplama(veriBuffer, baslangicIndeksi, baslangicIndeksi + 3);
                if (veriBuffer[(baslangicIndeksi + 3) % BufferSize] == CRC8)
                {
                    yoklamaFlag = true;

                    label35.Text = "Veri Alınıyor ve Işleniyor..";
                    counter++;
                }
            }
        }
        private void RotaPaketIsle(byte[] veriBuffer, int baslangicIndeksi, int veriUzunlugu)
        {
            if (veriUzunlugu == 4)
            {
                CRC8 = CRC8Hesaplama(veriBuffer, baslangicIndeksi, baslangicIndeksi + 3);
                if (veriBuffer[(baslangicIndeksi + 3) % BufferSize] == CRC8)
                {
                    rotaGonderFlag = true;
                    label37.Text = "Araç Duruyor..";
                    counter++;
                }
            }
        }
        private byte CRC8Hesaplama(byte[] data, int baslangic, int bitis)
        {
            byte crc = 0x00;

            for (int i = baslangic; i < bitis; i++)
            {
                crc ^= data[i % BufferSize]; // CRC değerini, dizinin bir sonraki byte ile XOR
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
                      (floatBuffer[2] << 8) |
                      (floatBuffer[3] << 0);

            bytes = BitConverter.GetBytes(intBits);

            return BitConverter.ToSingle(bytes, 0);
        }*/
    }

}
