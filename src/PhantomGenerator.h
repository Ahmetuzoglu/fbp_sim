#ifndef PHANTOM_GENERATOR_H
#define PHANTOM_GENERATOR_H

#include "Common.h"
#include <ctime>
#include <cmath>
#include <cstdlib>
#include <algorithm>

class PhantomGenerator {
private:
    // --- YARDIMCI ÇİZİM MOTORLARI ---

    // 1. Organik Doku Çizici (İçi boş/dolu destekli)
    static void drawOrganicTissue(ImageMatrix& img, int cx, int cy, int rad, double intensity, bool hollow = false) {
        for (int y = 0; y < img.height; ++y) {
            for (int x = 0; x < img.width; ++x) {
                double dist = std::sqrt(std::pow(x - cx, 2) + std::pow(y - cy, 2));
                if (dist <= rad) {
                    if (hollow && dist < rad * 0.7) continue; // İçi boşaltılmış organlar için
                    double grad = intensity * (1.0 - (dist / rad) * 0.4);
                    if (grad > img.getPixel(x, y)) img.setPixel(x, y, grad);
                }
            }
        }
    }

    // 2. Kemik ve Çizgi Çizici (Kalınlık destekli DDA/Bresenham melezi)
    static void drawBoneLine(ImageMatrix& img, int x1, int y1, int x2, int y2, double intensity, int thickness = 1) {
        int dx = std::abs(x2 - x1), dy = std::abs(y2 - y1);
        int sx = (x1 < x2) ? 1 : -1, sy = (y1 < y2) ? 1 : -1;
        int err = dx - dy;
        while (true) {
            // Kalınlık katmanları
            for(int tx = -thickness/2; tx <= thickness/2; ++tx) {
                for(int ty = -thickness/2; ty <= thickness/2; ++ty) {
                    img.setPixel(x1+tx, y1+ty, intensity);
                }
            }
            if (x1 == x2 && y1 == y2) break;
            int e2 = 2 * err;
            if (e2 > -dy) { err -= dy; x1 += sx; }
            if (e2 < dx) { err += dx; y1 += sy; }
        }
    }

    // 3. Fraktal Damar/Bronş Ağacı Çizici
    static void drawFractalTree(ImageMatrix& img, double x, double y, double angle, double length, double intensity, int depth) {
        if (depth == 0 || length < 2.0) return;
        double x2 = x + length * std::cos(angle);
        double y2 = y + length * std::sin(angle);
        
        int thickness = (depth > 4) ? 2 : 1;
        drawBoneLine(img, (int)x, (int)y, (int)x2, (int)y2, intensity, thickness);

        double angleVariation = 0.3 + ((rand() % 100) / 500.0 - 0.1); 
        double lengthDecay = 0.7 + ((rand() % 100) / 1000.0);

        drawFractalTree(img, x2, y2, angle - angleVariation, length * lengthDecay, intensity * 0.95, depth - 1);
        drawFractalTree(img, x2, y2, angle + angleVariation, length * lengthDecay, intensity * 0.95, depth - 1);
    }

public:
    // --- KLASİK VE BİLİM KURGU VARLIKLARI ---

    // 1. Core Entity
    static ImageMatrix createCoreEntity(int size) {
        ImageMatrix img(size, size);
        int cx = size / 2, cy = size / 2;
        drawOrganicTissue(img, cx, cy, size * 0.12, 1.0); 
        drawOrganicTissue(img, cx + size * 0.15, cy - size * 0.15, size * 0.06, 0.8);
        return img;
    }

    // 2. Void Larva
    static ImageMatrix createVoidLarva(int size) {
        ImageMatrix img(size, size);
        int cx = size / 2, cy = size / 2;
        for (int i = 0; i < 5; ++i) {
            int ox = cx + (i * size * 0.08) - (size * 0.15);
            int oy = cy + std::sin(i * 0.8) * (size * 0.1);
            drawOrganicTissue(img, ox, oy, size * 0.07, 0.9 - (i * 0.1));
        }
        return img;
    }

    // 3. Ancient Spore
    static ImageMatrix createAncientSpore(int size) {
        ImageMatrix img(size, size);
        int cx = size / 2, cy = size / 2;
        drawOrganicTissue(img, cx, cy, size * 0.1, 1.0); 
        for (int a = 0; a < 360; a += 45) {
            double rad = a * M_PI / 180.0;
            for (int d = 1; d <= 3; ++d) {
                int px = cx + std::cos(rad) * (d * size * 0.08);
                int py = cy + std::sin(rad) * (d * size * 0.08);
                drawOrganicTissue(img, px, py, size * 0.03, 0.6 / d);
            }
        }
        return img;
    }

    // 4. Xeno Lungs
    static ImageMatrix createXenoLungs(int size) {
        ImageMatrix img(size, size);
        int cx = size / 2, cy = size / 2;
        drawOrganicTissue(img, cx - size*0.18, cy, size * 0.15, 0.5); 
        drawOrganicTissue(img, cx + size*0.18, cy, size * 0.15, 0.5);
        for(int i=0; i<8; ++i) {
            int rx = (rand() % (int)(size * 0.4)) + size * 0.3;
            int ry = (rand() % (int)(size * 0.4)) + size * 0.3;
            drawOrganicTissue(img, rx, ry, 3, 0.9);
        }
        return img;
    }

    // 5. Neural Cortex
    static ImageMatrix createNeuralCortex(int size) {
        ImageMatrix img(size, size);
        int cx = size / 2, cy = size / 2;
        for (int i = 0; i < 20; ++i) {
            double angle = (i / 20.0) * 2.0 * M_PI;
            int ox = cx + std::cos(angle * 2) * (size * 0.2);
            int oy = cy + std::sin(angle * 3) * (size * 0.2);
            drawOrganicTissue(img, ox, oy, size * 0.06, 0.7);
        }
        return img;
    }

    // 6. Grid Calibrator
    static ImageMatrix createGridCalibrator(int size) {
        ImageMatrix img(size, size);
        for (int y = 0; y < size; y += 15) {
            for (int x = 0; x < size; x += 15) {
                img.setPixel(x, y, 1.0);
            }
        }
        return img;
    }

    // 7. Orijinal Kanserli Akciğer (Restore edildi!)
    static ImageMatrix createCancerousLung(int size) {
        ImageMatrix img(size, size);
        int cx = size / 2, cy = size / 2;

        for (int y = 0; y < size; ++y) {
            for (int x = 0; x < size; ++x) {
                double d = std::sqrt(std::pow(x - cx, 2) + std::pow(y - cy, 2));
                if (d < size * 0.45 && d > size * 0.42) img.setPixel(x, y, 1.0);
            }
        }
        drawOrganicTissue(img, cx - size * 0.18, cy, size * 0.15, 0.2); 
        drawOrganicTissue(img, cx + size * 0.18, cy, size * 0.15, 0.2);

        int tumorX = cx + size * 0.12, tumorY = cy - size * 0.05;
        drawOrganicTissue(img, tumorX, tumorY, size * 0.04, 0.9); 
        
        for(int i=0; i<5; ++i) {
            double ang = i * 72 * M_PI / 180.0;
            int sx = tumorX + std::cos(ang) * 10;
            int sy = tumorY + std::sin(ang) * 10;
            drawOrganicTissue(img, sx, sy, 3, 0.6);
        }
        drawOrganicTissue(img, cx, cy + size * 0.05, size * 0.08, 0.6); 
        drawOrganicTissue(img, cx, cy + size * 0.38, size * 0.05, 1.0); 
        return img;
    }

    // 8. Çöp Adam
    static ImageMatrix createStickHuman(int size) {
        ImageMatrix img(size, size);
        int cx = size / 2, cy = size / 2;

        drawOrganicTissue(img, cx, size * 0.2, size * 0.08, 1.0);
        drawBoneLine(img, cx, size * 0.28, cx, size * 0.6, 0.9, 2);
        drawBoneLine(img, cx, size * 0.35, cx - size * 0.15, size * 0.45, 0.8, 2); 
        drawBoneLine(img, cx, size * 0.35, cx + size * 0.15, size * 0.45, 0.8, 2); 
        drawBoneLine(img, cx, size * 0.6, cx - size * 0.12, size * 0.85, 0.9, 2); 
        drawBoneLine(img, cx, size * 0.6, cx + size * 0.12, size * 0.85, 0.9, 2); 
        return img;
    } 

    // 9. Bio-Mechanical Sentinel
    static ImageMatrix createSentinel(int size) {
        ImageMatrix img(size, size);
        int cx = size / 2, cy = size / 2;

        for (int y = 0; y < size; ++y) {
            for (int x = 0; x < size; ++x) {
                double d = std::sqrt(std::pow(x - cx, 2) + std::pow(y - cy, 2));
                if (d < size * 0.42 && d > size * 0.40) img.setPixel(x, y, 0.9);
            }
        }
        for (int i = 0; i < 6; ++i) {
            int ry = size * 0.3 + (i * size * 0.08);
            drawOrganicTissue(img, cx, ry, size * 0.025, 1.0);
            drawBoneLine(img, cx, ry, cx - size * 0.25, ry - size * 0.05, 0.6, 2);
            drawBoneLine(img, cx, ry, cx + size * 0.25, ry - size * 0.05, 0.6, 2);
        }
        drawOrganicTissue(img, cx, cy, size * 0.12, 0.4); 
        drawOrganicTissue(img, cx, cy, size * 0.06, 1.0); 

        int hx = cx, hy = size * 0.18;
        drawOrganicTissue(img, hx, hy, size * 0.09, 0.8); 
        drawOrganicTissue(img, hx - size * 0.03, hy - size * 0.02, size * 0.015, 1.0);
        drawOrganicTissue(img, hx + size * 0.03, hy - size * 0.02, size * 0.015, 1.0);

        for (int i = 0; i < 12; ++i) {
            double angle = i * 30 * M_PI / 180.0;
            int x_end = cx + std::cos(angle) * size * 0.35;
            int y_end = cy + std::sin(angle) * size * 0.35;
            drawBoneLine(img, cx, cy, x_end, y_end, 0.3);
            drawOrganicTissue(img, (cx + x_end) / 2, (cy + y_end) / 2, 2, 0.8);
        }
        drawBoneLine(img, cx - size * 0.15, size * 0.85, cx + size * 0.15, size * 0.85, 0.7, 2);
        drawBoneLine(img, cx, size * 0.6, cx - size * 0.1, size * 0.85, 0.8, 2);
        drawBoneLine(img, cx, size * 0.6, cx + size * 0.1, size * 0.85, 0.8, 2);
        return img;
    }

    // 10. THE ANATOMICAL TITAN
    static ImageMatrix createHyperComplexTorso(int size) {
        ImageMatrix img(size, size);
        int cx = size / 2, cy = size / 2;
        srand(time(0)); 

        for(int y=0; y<size; ++y) for(int x=0; x<size; ++x) img.setPixel(x,y, 0.1);
        drawOrganicTissue(img, cx, cy, size*0.48, 0.25); 

        drawOrganicTissue(img, cx, cy + size*0.25, size*0.08, 0.95); 
        drawOrganicTissue(img, cx, cy + size*0.25, size*0.03, 0.5, true); 
        drawBoneLine(img, cx - size*0.08, cy + size*0.28, cx + size*0.08, cy + size*0.28, 0.9, 3);

        for(int i=0; i<24; ++i) {
            double angle = i * 15 * M_PI / 180.0;
            if (angle > M_PI * 0.8 && angle < M_PI * 2.2) continue; 
            double rx = size * 0.4, ry = size * 0.3;
            int bx = cx + rx * std::cos(angle);
            int by = cy + ry * std::sin(angle) * 0.8;
            drawOrganicTissue(img, bx, by, size*0.02, 0.9);
        }

        drawOrganicTissue(img, cx - size*0.2, cy - size*0.05, size*0.18, 0.15); 
        drawOrganicTissue(img, cx + size*0.2, cy - size*0.05, size*0.18, 0.15);

        drawFractalTree(img, cx - size*0.05, cy - size*0.1, M_PI, size*0.08, 0.7, 8); 
        drawFractalTree(img, cx + size*0.05, cy - size*0.1, 0, size*0.08, 0.7, 8);

        drawOrganicTissue(img, cx - size*0.05, cy + size*0.05, size*0.12, 0.6);
        drawOrganicTissue(img, cx - size*0.08, cy + size*0.08, size*0.05, 0.7, true); 
        drawOrganicTissue(img, cx - size*0.02, cy + size*0.08, size*0.04, 0.7, true); 
        drawOrganicTissue(img, cx, cy - size*0.05, size*0.035, 0.85);
        drawOrganicTissue(img, cx + size*0.01, cy - size*0.05, size*0.005, 1.0);

        for(int y=0; y<size; ++y) {
            for(int x=0; x<size; ++x) {
                if(x > cx && y > cy + size*0.1 && x < cx + size*0.35 && y < cy + size*0.4) {
                     double d = std::sqrt(std::pow(x - (cx+size*0.15), 2) + std::pow(y - (cy+size*0.25), 2));
                     if(d < size*0.15) img.setPixel(x, y, 0.55);
                }
            }
        }
        drawOrganicTissue(img, cx + size*0.2, cy + size*0.25, size*0.02, 0.3);
        drawOrganicTissue(img, cx + size*0.25, cy + size*0.3, size*0.015, 0.8);

        drawOrganicTissue(img, cx - size*0.2, cy + size*0.25, size*0.08, 0.4, true);
        drawOrganicTissue(img, cx - size*0.22, cy + size*0.22, size*0.03, 0.05);

        return img;
    }
    // --- LİTERATÜR STANDARDI: SHEPP-LOGAN PHANTOM ---
    // Tıbbi görüntüleme algoritmalarının dünya çapındaki test standartıdır.
    // İnsan kafatasını ve beyin dokusunu 10 adet elips ile modeller.
    static ImageMatrix createSheppLogan(int size) {
        ImageMatrix img(size, size);
        double cx = size / 2.0;
        double cy = size / 2.0;

        // Elips Parametreleri: { Yoğunluk, a (x-yarıçap), b (y-yarıçap), merkez_x, merkez_y, açı(derece) }
        struct EllipseDef { double intensity, a, b, cx, cy, theta; };
        EllipseDef ellipses[] = {
            { 1.0,   0.6900, 0.9200,  0.0,     0.0,     0.0 },   // Dış kafatası
            {-0.8,   0.6624, 0.8740,  0.0,    -0.0184,  0.0 },   // Beyin maddesi (İçi boşaltır)
            {-0.2,   0.1100, 0.3100,  0.2200,  0.0,    -18.0 },  // Sağ ventrikül
            {-0.2,   0.1600, 0.4100, -0.2200,  0.0,     18.0 },  // Sol ventrikül
            { 0.1,   0.2100, 0.2500,  0.0,     0.3500,  0.0 },   // Tümör 1
            { 0.1,   0.0460, 0.0460,  0.0,     0.1000,  0.0 },   // Tümör 2
            { 0.1,   0.0460, 0.0460,  0.0,    -0.1000,  0.0 },   // Tümör 3
            { 0.1,   0.0460, 0.0230, -0.0800, -0.6050,  0.0 },   // Küçük lezyon 1
            { 0.1,   0.0230, 0.0230,  0.0,    -0.6060,  0.0 },   // Küçük lezyon 2
            { 0.1,   0.0230, 0.0460,  0.0600, -0.6050,  0.0 }    // Küçük lezyon 3
        };

        for (const auto& e : ellipses) {
            // Normalize koordinatları matris boyutuna çevir
            double a = e.a * (size / 2.0);
            double b = e.b * (size / 2.0);
            double ecx = cx + e.cx * (size / 2.0);
            double ecy = cy - e.cy * (size / 2.0); // Y ekseni aşağı doğru artar
            double theta = e.theta * M_PI / 180.0;

            double cosTh = std::cos(theta);
            double sinTh = std::sin(theta);

            // Elips denklemi ile tarama
            for (int y = 0; y < size; ++y) {
                for (int x = 0; x < size; ++x) {
                    double dx = x - ecx;
                    double dy = y - ecy;
                    
                    // Ekseni döndür
                    double rx = dx * cosTh + dy * sinTh;
                    double ry = -dx * sinTh + dy * cosTh;
                    
                    // İçinde mi kontrol et: (x^2 / a^2) + (y^2 / b^2) <= 1
                    if ((rx * rx) / (a * a) + (ry * ry) / (b * b) <= 1.0) {
                        double current = img.getPixel(x, y);
                        // Shepp-Logan değerleri üst üste toplanarak (additive) oluşturulur
                        img.setPixel(x, y, current + e.intensity);
                    }
                }
            }
        }
        
        // Fiziksel sınırları koru (Garantör)
        for(auto& val : img.data) {
            if (val < 0.0) val = 0.0;
            if (val > 1.0) val = 1.0;
        }

        return img;
    }
};

#endif