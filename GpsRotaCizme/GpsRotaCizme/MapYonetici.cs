using System;
using System.Collections.Generic;
using System.Drawing;
using System.Windows.Forms;
using GMap.NET;
using GMap.NET.MapProviders;
using GMap.NET.WindowsForms;
using GMap.NET.WindowsForms.Markers;

namespace GpsRotaCizme
{
    internal class MapYonetici
    {
        private GMapControl map;
        private GMapOverlay isaretciYeri;
        private List<PointLatLng> points;
        private GMapOverlay rotaYeri;

        private GMapOverlay kalmanYeri;   // 🔵 Kalman için yeni overlay

        Bitmap bmp = new Bitmap(5, 5);
        Bitmap bmpKalman = new Bitmap(10, 10);   // ⚫ Kalman için siyah nokta

        public MapYonetici(GMapControl mapControl)
        {
            map = mapControl;
            map.MapProvider = GMapProviders.GoogleMap;
            map.Position = new PointLatLng(41.2174316, 36.4566603);
            map.MinZoom = 13;
            map.MaxZoom = 24;
            map.Zoom = 20;

            points = new List<PointLatLng>();

            isaretciYeri = new GMapOverlay("isaretciYeri");
            rotaYeri = new GMapOverlay("rotaYeri");
            kalmanYeri = new GMapOverlay("kalmanYeri"); // ⚫ Kalman için overlay

            map.Overlays.Add(isaretciYeri);
            map.Overlays.Add(rotaYeri);
            map.Overlays.Add(kalmanYeri);

            // GPS mavi nokta
            using (Graphics gfx = Graphics.FromImage(bmp))
            {
                gfx.FillEllipse(Brushes.Blue, 0, 0, 3, 3);
            }

            // Kalman siyah nokta
            using (Graphics gfx = Graphics.FromImage(bmpKalman))
            {
                gfx.FillEllipse(Brushes.Black, 0, 0, 6, 6);
            }
        }

        // 🔵 GPS güncellemesi
        public void MapGuncelle(double lat, double lon)
        {
            PointLatLng nokta = new PointLatLng(lat, lon);
            points.Add(nokta);
            map.Position = nokta;

            GMarkerGoogle marker = new GMarkerGoogle(nokta, bmp);
            isaretciYeri.Markers.Add(marker);

            if (points.Count > 1)
            {
                GMapRoute rota = new GMapRoute(points, "Rota");
                rota.Stroke = new Pen(Color.Red, 3);
                rotaYeri.Routes.Clear();
                rotaYeri.Routes.Add(rota);
            }
        }

        // 🔴 Manuel işaretçi
        public void IsaretciEkle(PointLatLng nokta)
        {
            GMarkerGoogle marker = new GMarkerGoogle(nokta, GMarkerGoogleType.red_dot);
            isaretciYeri.Markers.Add(marker);
            map.Refresh();
        }

        // ⚫ KALMAN NOKTA EKLEME
        /*
        public void KalmanNoktaEkle(double lat, double lon)
        {
            PointLatLng nokta = new PointLatLng(lat, lon);
            GMarkerGoogle marker = new GMarkerGoogle(nokta, bmpKalman);
            kalmanYeri.Markers.Add(marker);

            map.Refresh();
        }*/
        // ⚫ KALMAN NOKTA EKLEME (önceki silinir)

        public void KalmanNoktaEkle(double lat, double lon)
        {
            PointLatLng nokta = new PointLatLng(lat, lon);

            // Önceki Kalman noktasını temizle
            kalmanYeri.Markers.Clear();

            // Yeni Kalman noktasını ekle
            GMarkerGoogle marker = new GMarkerGoogle(nokta, bmpKalman);
            kalmanYeri.Markers.Add(marker);

            map.Refresh();
        }

    }
}
