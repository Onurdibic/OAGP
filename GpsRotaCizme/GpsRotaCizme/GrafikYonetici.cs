using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms.DataVisualization.Charting;

namespace GpsRotaCizme
{
    public class GrafikYonetici
    {
        private Chart[] grafikler;

        public GrafikYonetici(Chart[] grafikler)
        {
            this.grafikler = grafikler;
            TabloYapilandir();
        }
        private void TabloYapilandir()
        {
            for (int i = 0; i < grafikler.Length; i++)
            {
                grafikler[i].Visible = false;
            }

            grafikler[1].Visible = true;
        }
        public void GrafikGuncelle(Chart grafik, int deger, int minY, int maxY,int min,int max)
        {
            int maxVeriNoktasi = 40;
            grafik.Series[0].Points.AddXY((min + max) / 2, deger);

            while (grafik.Series[0].Points.Count > maxVeriNoktasi)
            {
                grafik.Series[0].Points.RemoveAt(0);
            }

            grafik.ChartAreas[0].AxisX.Minimum = min;
            grafik.ChartAreas[0].AxisX.Maximum = max;
            grafik.ChartAreas[0].AxisY.Minimum = minY;
            grafik.ChartAreas[0].AxisY.Maximum = maxY;
            grafik.ChartAreas[0].AxisX.ScaleView.Zoom(min, max);
        }

        public void GrafikGorunurlugunuAyarla(int secilenIndex)
        {
            for (int i = 0; i < grafikler.Length; i++)
            {
                grafikler[i].Visible = (i == secilenIndex);
            }
        }
    }
}
