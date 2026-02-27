#ifndef CT_PROCESSOR_H
#define CT_PROCESSOR_H

#include "Common.h"
#include <iostream>

class CTProcessor {
public:
    // 1. Scanning
    static SinogramData forwardProject(const ImageMatrix& phantom, double fanAngle, double sourceRadius) {
        int numAngles = 360, numRays = 1000;
        double stepSize = 0.1, centerX = phantom.width / 2.0, centerY = phantom.height / 2.0;
        SinogramData sinogram(numAngles, numRays);

        for (int a = 0; a < numAngles; ++a) {
            double rad = a * M_PI / 180.0;
            double srcX = centerX + sourceRadius * std::cos(rad), srcY = centerY + sourceRadius * std::sin(rad);
            for (int r = 0; r < numRays; ++r) {
                double rayAng = rad + M_PI - (fanAngle / 2.0) + (r * fanAngle / numRays);
                double dx = std::cos(rayAng) * stepSize, dy = std::sin(rayAng) * stepSize;
                double curX = srcX, curY = srcY, sum = 0.0;
                for (int s = 0; s < (sourceRadius * 2.5) / stepSize; ++s) {
                    curX += dx; curY += dy;
                    if (curX >= 0 && curX < phantom.width && curY >= 0 && curY < phantom.height)
                        sum += phantom.getBilinearPixel(curX, curY);
                }
                sinogram.data[a][r] = sum;
            }
        }
        return sinogram;
    }

    // 2. Shepp-Logan Filtered Ramp
    static SinogramData applyFilter(const SinogramData& rawSino) {
        SinogramData filtSino(rawSino.numAngles, rawSino.numRays);
        int kSize = 101, mid = kSize / 2;
        std::vector<double> kernel(kSize, 0.0);
        
        for (int i = 0; i < kSize; ++i) {
            int n = i - mid;
            if (n == 0) kernel[i] = 0.25;
            else if (n % 2 != 0) {
                double ramp = -1.0 / (M_PI * M_PI * n * n);
                // Shepp-Logan Window (Sinc function)
                double window = std::sin(M_PI * n / kSize) / (M_PI * n / kSize);
                kernel[i] = ramp * window;
            }
        }

        for (int a = 0; a < rawSino.numAngles; ++a) {
            for (int r = 0; r < rawSino.numRays; ++r) {
                double val = 0.0;
                for (int k = 0; k < kSize; ++k) {
                    int idx = r + (k - mid);
                    if (idx >= 0 && idx < rawSino.numRays) val += rawSino.data[a][idx] * kernel[k];
                }
                filtSino.data[a][r] = val;
            }
        }
        return filtSino;
    }

    // 3. Back Projection (Reconstruction)
    static ImageMatrix backProject(const SinogramData& filtSino, int size, double fanAngle, double srcRad) {
        ImageMatrix recon(size, size);
        double cX = size / 2.0, cY = size / 2.0, maxV = -1e9, minV = 1e9;
        for (int y = 0; y < size; ++y) {
            for (int x = 0; x < size; ++x) {
                double pixelVal = 0.0;
                for (int a = 0; a < filtSino.numAngles; ++a) {
                    double rad = a * M_PI / 180.0;
                    double dx = x - (cX + srcRad * std::cos(rad)), dy = y - (cY + srcRad * std::sin(rad));
                    double distSq = dx * dx + dy * dy;
                    double ang = std::atan2(dy, dx), diff = ang - (rad + M_PI);
                    while (diff > M_PI) diff -= 2.0 * M_PI; while (diff < -M_PI) diff += 2.0 * M_PI;
                    double rIdx = (diff + (fanAngle / 2.0)) / fanAngle * filtSino.numRays;
                    if (rIdx >= 0 && rIdx < filtSino.numRays - 1) {
                        int i1 = (int)rIdx; double w = rIdx - i1;
                        pixelVal += ((1 - w) * filtSino.data[a][i1] + w * filtSino.data[a][i1 + 1]) / distSq;
                    }
                }
                recon.setPixel(x, y, pixelVal);
                if (pixelVal > maxV) maxV = pixelVal; if (pixelVal < minV) minV = pixelVal;
            }
        }
        if (maxV > minV) for (auto& v : recon.data) v = (v - minV) / (maxV - minV);
        return recon;
    }
};

#endif