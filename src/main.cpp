#include <iostream>
#include <fstream>
#include "Common.h"
#include "PhantomGenerator.h"
#include "CTProcessor.h"

void exportCSV(const std::vector<double>& data, int w, int h, const std::string& name) {
    std::ofstream f(name);
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) f << data[y * w + x] << (x == w - 1 ? "" : ",");
        f << "\n";
    }
}

int main() {
    const int resolution = 256;
    const double fanAngle = M_PI / 3.0;
    const double sourceRadius = resolution * 1.5;

    std::cout << "[Step 1] Creating Phantom..." << std::endl;
    ImageMatrix phantom = PhantomGenerator::createSheppLogan(resolution);

    std::cout << "[Step 2] Scanning..." << std::endl;
    SinogramData rawSino = CTProcessor::forwardProject(phantom, fanAngle, sourceRadius);

    std::cout << "[Step 3] Filtering..." << std::endl;
    SinogramData filtSino = CTProcessor::applyFilter(rawSino);

    std::cout << "[Step 4] Reconstruction..." << std::endl;
    ImageMatrix reconstruction = CTProcessor::backProject(filtSino, resolution, fanAngle, sourceRadius);

    std::cout << "[Step 5] Saving..." << std::endl;
    exportCSV(phantom.data, resolution, resolution, "build/phantom.csv");
    exportCSV(reconstruction.data, resolution, resolution, "build/reconstructed.csv");
    
    std::ofstream fS("build/sinogram.csv");
    for (const auto& row : rawSino.data) {
        for (size_t i = 0; i < row.size(); ++i) fS << row[i] << (i == row.size() - 1 ? "" : ",");
        fS << "\n";
    }

    std::cout << "Done!" << std::endl;
    return 0;
}