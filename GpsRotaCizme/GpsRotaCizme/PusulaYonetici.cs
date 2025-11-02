using System;
using System.Drawing;

namespace GpsRotaCizme
{
    public class PusulaYonetici
    {
        private Bitmap orijinalResim;
        private Bitmap donmusResim;

        public PusulaYonetici(string resimYolu)
        {
            ResmiYukle(resimYolu);
        }

        private void ResmiYukle(string resimYolu)
        {
            if (System.IO.File.Exists(resimYolu))
            {
                orijinalResim = new Bitmap(resimYolu);
                donmusResim = new Bitmap(orijinalResim.Width, orijinalResim.Height);
            }
        }

        public Image DonmusResimAl(float derece)
        {
            if (orijinalResim == null)
                return null;

            using (Graphics g = Graphics.FromImage(donmusResim))
            {
                g.Clear(Color.Transparent);
                g.TranslateTransform(orijinalResim.Width / 2, orijinalResim.Height / 2);
                g.RotateTransform(derece);
                g.TranslateTransform(-orijinalResim.Width / 2, -orijinalResim.Height / 2);
                g.DrawImage(orijinalResim, 0, 0);
            }

            return donmusResim;
        }
    }
}

