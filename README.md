# 🚀 CT Scanner Simulator: OpenMP & FBP Reconstruction
# ############################################################################### Author Ahmet Uzoglu ##############################################################
A high-performance Computed Tomography (CT) simulation built in C++. This project simulates the complete medical imaging pipeline: from firing virtual X-rays (Forward Projection) to reconstructing the internal structure using the **FBP (Filtered Back Projection)** algorithm. 

By leveraging **OpenMP**, the heavy matrix computations are parallelized across all CPU cores, reducing reconstruction time from minutes to seconds.

---

## 📌 Features
* **Forward Projection (Radon Transform):** Simulates X-ray attenuation through a 2D density matrix.
* **Analytical Reconstruction (FBP):** Uses Filtered Back Projection to analytically invert the Radon transform and reconstruct the image from sinogram data.
* **Multi-threading:** Fully optimized with OpenMP for real-time parallel processing.
* **Custom Phantoms:** Includes a built-in `PhantomGenerator` to create various test subjects (Shepp-Logan, Medical Lungs, Bio-Mechanical structures).
* **Python Visualization:** A dedicated Python script to convert output CSVs into visual heatmaps.

---

## 📂 Project Structure

    📦 fbp_sim
     ┣ 📂 src
     ┃ ┣ 📜 main.cpp               # Main execution and pipeline control
     ┃ ┣ 📜 Common.h               # Core data structures (ImageMatrix, SinogramData)
     ┃ ┣ 📜 CTProcessor.h          # OpenMP-powered scanning and FBP algorithms
     ┃ ┗ 📜 PhantomGenerator.h     # Library of test objects (Phantoms)
     ┣ 📜 .gitignore               # Ignoring unnecessary files
     ┣ 📜 CMakeLists.txt           # Build configuration
     ┣ 📜 data_analysis.py         # Python script to plot the CSV outputs
     ┗ 📜 README.md                # Project documentation

---

## ⚙️ Prerequisites

Before you begin, ensure you have the following installed:
1. **C++ Compiler:** GCC/MinGW or MSVC with C++17 support.
2. **CMake:** Version 3.10 or higher.
3. **OpenMP:** Usually bundled with GCC/MinGW. 
4. **Python 3.x:** With `matplotlib` and `numpy` installed.
   pip install matplotlib numpy

---

## 🛠️ Build Configuration (CMake)

This project uses CMake to handle dependencies and build paths. Open your terminal in the root directory of the project and run the following commands:

**1. Going to correct directory:**
    cd fbp_sim

**2. Configure the CMake project:**
    cmake -G "MinGW Makefiles" -B build

**3. Compile the executable (utilizing multiple CPU cores for a faster build):**
    cmake --build build -j 8

---

## 🚀 Getting Started

### Step 1: Run the C++ Simulation
Once compiled, run the executable generated in the `build` folder. 
    ./build/scan.exe

**What happens here?**
The program will generate the phantom, perform the virtual CT scan to create a sinogram, run the FBP algorithm, and finally output three files inside your `build/` directory:
* `build/phantom.csv`: The ground-truth original object.
* `build/sinogram.csv`: The raw X-ray projection data.
* `build/reconstructed.csv`: The final image calculated by the FBP algorithm.

### Step 2: Visualize the Results
Run the Python analysis script to read the generated `.csv` files and plot the results side-by-side. (Make sure your Python script is configured to read from the `build/` folder).
    python data_analysis.py

This will open a Matplotlib window displaying the Original Object, the Sinogram, and the Reconstructed Image.

---

## 🧠 How to Change the Phantom (Test Object)

You can easily change the object being scanned by modifying one line in `src/main.cpp`. 

Open `src/main.cpp` and locate the phantom initialization step:
    // Change this line to use a different phantom
    ImageMatrix phantom = PhantomGenerator::createSheppLogan(resolution);

# Necessary for cleaning old build
Remove-Item -Recurse -Force build
**Available Phantoms in `PhantomGenerator.h`:**
* `createSheppLogan(size)`: The medical standard for testing reconstruction accuracy.
* `createCancerousLung(size)`: A simulated lung with a high-density tumor.
* `createStickHuman(size)`: A geometric stick figure to test line aliasing.
* `createSentinel(size)`: A complex bio-mechanical robot structure.

### Adding Your Own Phantom
To draw a custom object, add a new static method in `src/PhantomGenerator.h` and use the built-in drawing tools:
    static ImageMatrix createMyCustomObject(int size) {
        ImageMatrix img(size, size);
        // Draw a circle at center (cx, cy) with radius 20 and density 1.0
        drawOrganicTissue(img, size/2, size/2, 20, 1.0); 
        return img;
    }

---

## 🏎️ Engineering Notes: FBP (Filtered Back Projection)

This project utilizes the **Filtered Back Projection (FBP)** method, the gold standard in analytical tomographic reconstruction. 

Unlike simple back projection which results in heavily blurred images (1/r blurring), FBP applies a high-pass ramp filter to the sinogram data in the frequency domain before back-projecting it into the image space. This mathematical inversion of the Radon Transform ensures sharp boundaries and high-contrast resolution, making it computationally efficient and highly effective for medical imaging simulations.

---
*Developed as a high-performance numerical analysis and signal processing simulation.*
