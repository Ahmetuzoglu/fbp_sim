#ifndef COMMON_H
#define COMMON_H

#include <vector>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

struct ImageMatrix {
    int width, height;
    std::vector<double> data;
    ImageMatrix(int w, int h) : width(w), height(h), data(w * h, 0.0) {}

    void setPixel(int x, int y, double value) {
        if (x >= 0 && x < width && y >= 0 && y < height) data[y * width + x] = value;
    }

    double getPixel(int x, int y) const {
        if (x >= 0 && x < width && y >= 0 && y < height) return data[y * width + x];
        return 0.0;
    }

    double getBilinearPixel(double x, double y) const {
        int x1 = static_cast<int>(std::floor(x)), y1 = static_cast<int>(std::floor(y));
        if (x1 < 0 || x1 + 1 >= width || y1 < 0 || y1 + 1 >= height) return 0.0;
        double dx = x - x1, dy = y - y1;
        return (getPixel(x1, y1) * (1 - dx) + getPixel(x1 + 1, y1) * dx) * (1 - dy) + 
               (getPixel(x1, y1 + 1) * (1 - dx) + getPixel(x1 + 1, y1 + 1) * dx) * dy;
    }
};

struct SinogramData {
    int numAngles, numRays;
    std::vector<std::vector<double>> data;
    SinogramData(int angles, int rays) : numAngles(angles), numRays(rays) {
        data.resize(numAngles, std::vector<double>(numRays, 0.0));
    }
};

#endif