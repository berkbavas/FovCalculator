# FovCalculator

A Field of View (FOV) calculator for detection, observation, recognition, and identification limits, based on the IEC EN62676-4:2015 International Standard.

## Build Instructions

1. **Install dependencies:**
    - [CMake](https://cmake.org/) 3.25.1 or newer
    - [Visual Studio 2022](https://visualstudio.microsoft.com/vs/) with MSVC v143 C++ Compiler
    - [Qt 6.7.3 MSVC2022 64bit](https://www.qt.io/download)

2. **Set environment variable:**

    - `Qt6_DIR` to `C:\Qt\6.7.3\msvc2022_64`

3. **Clone the repository:**

    ```sh
    git clone https://github.com/berkbavas/FovCalculator.git
    ```

4. **Create and enter build directory:**

    ```sh
    mkdir build
    cd build
    ```

5. **Configure project with CMake:**

    ```sh
    cmake ..
    ```

6. **Open solution in Visual Studio:**

   - Open `FovCalculator.sln` in Visual Studio 2022
   - Build and run the project

## Demo Video

[Project Demo](https://github.com/user-attachments/assets/00f376d7-56a9-4999-aeeb-04ef076b29eb)

## License

This project is licensed under the [MIT License](LICENSE).
