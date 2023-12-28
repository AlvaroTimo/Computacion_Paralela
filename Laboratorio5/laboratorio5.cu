#include <iostream>
#include <opencv2/opencv.hpp>

#define BLUR_SIZE 10

// Kernel para convertir la imagen a escala de grises
__global__ void rgbToGray(unsigned char* inputImage, unsigned char* outputImage, int width, int height) {
    int x = blockIdx.x * blockDim.x + threadIdx.x;
    int y = blockIdx.y * blockDim.y + threadIdx.y;

    if (x < width && y < height) {
        int grayOffset = y * width + x;
        int rgbOffset = grayOffset * 3;

        unsigned char r = inputImage[rgbOffset];
        unsigned char g = inputImage[rgbOffset + 1];
        unsigned char b = inputImage[rgbOffset + 2];

        outputImage[grayOffset] = 0.21f * r + 0.72f * g + 0.07f * b;
    }
}

// Kernel para aplicar un filtro de desenfoque
__global__
void blurKernel(unsigned char* in, unsigned char* out, int w, int h) {
    int Col = blockIdx.x * blockDim.x + threadIdx.x;
    int Row = blockIdx.y * blockDim.y + threadIdx.y;

    if (Col < w && Row < h) {
        int pixVal = 0;
        int pixels = 0;

        // Obtener el promedio del cuadro BLUR_SIZE x BLUR_SIZE
        for (int blurRow = -BLUR_SIZE; blurRow < BLUR_SIZE + 1; ++blurRow) {
            for (int blurCol = -BLUR_SIZE; blurCol < BLUR_SIZE + 1; ++blurCol) {
                int curRow = Row + blurRow;
                int curCol = Col + blurCol;
                // Verificar que tengamos un píxel de imagen válido
                if (curRow > -1 && curRow < h && curCol > -1 && curCol < w) {
                    pixVal += in[curRow * w + curCol];
                    pixels++; // Mantener un registro del número de píxeles en el promedio
                }
            }
        }
        // Escribir nuestro nuevo valor de píxel
        out[Row * w + Col] = (unsigned char)(pixVal / pixels);
    }
}

int main() {
    std::string inputImagePath = "C:/Users/alvaro/Documents/Capturas/input.jpg";
    std::string outputImagePathGray = "C:/Users/alvaro/Documents/Capturas/output_gray.jpg";
    std::string outputImagePathBlur = "C:/Users/alvaro/Documents/Capturas/output_blur.jpg";

    // Cargar la imagen de entrada
    cv::Mat inputImage = cv::imread(inputImagePath);
    if (inputImage.empty()) {
        std::cerr << "Error: Couldn't load the image." << std::endl;
        return -1;
    }

    int width = inputImage.cols;
    int height = inputImage.rows;

    size_t imageSize = width * height * inputImage.channels();
    size_t grayImageSize = width * height * sizeof(unsigned char);

    unsigned char* devInputImage, * devOutputImageGray, * devOutputImageBlur;
    cudaMalloc((void**)&devInputImage, imageSize);
    cudaMalloc((void**)&devOutputImageGray, grayImageSize);
    cudaMalloc((void**)&devOutputImageBlur, grayImageSize);

    cudaMemcpy(devInputImage, inputImage.data, imageSize, cudaMemcpyHostToDevice);

    dim3 threadsPerBlock(16, 16);
    dim3 numBlocks((width + threadsPerBlock.x - 1) / threadsPerBlock.x,
        (height + threadsPerBlock.y - 1) / threadsPerBlock.y);

    // Convertir a escala de grises
    rgbToGray << <numBlocks, threadsPerBlock >> > (devInputImage, devOutputImageGray, width, height);

    // Aplicar el filtro de desenfoque
    blurKernel << <numBlocks, threadsPerBlock >> > (devOutputImageGray, devOutputImageBlur, width, height);

    unsigned char* outputImageGray = new unsigned char[grayImageSize];
    unsigned char* outputImageBlur = new unsigned char[grayImageSize];

    // Copiar los resultados de la GPU a la memoria del host
    cudaMemcpy(outputImageGray, devOutputImageGray, grayImageSize, cudaMemcpyDeviceToHost);
    cudaMemcpy(outputImageBlur, devOutputImageBlur, grayImageSize, cudaMemcpyDeviceToHost);

    // Crear imágenes Mat a partir de los datos obtenidos
    cv::Mat grayImage(height, width, CV_8UC1, outputImageGray);
    cv::Mat blurImage(height, width, CV_8UC1, outputImageBlur);

    // Mostrar las imágenes procesadas
    cv::imshow("Original Image", inputImage);
    cv::imshow("Blurred Image", blurImage);
    cv::imshow("Gray Image", grayImage);

    cv::waitKey(0);

    // Guardar las imágenes en archivos de salida
    cv::imwrite(outputImagePathGray, grayImage);
    cv::imwrite(outputImagePathBlur, blurImage);

    // Liberar la memoria
    cudaFree(devInputImage);
    cudaFree(devOutputImageGray);
    cudaFree(devOutputImageBlur);
    delete[] outputImageGray;
    delete[] outputImageBlur;

    return 0;
}
