//
// Created by wjl on 21-3-28.
//

#include "ImageAlgorithm.h"

BEGIN_NAMESPACE(VulkanEngine)

void ImageAlgorithm::turnRGB2RGBA(uint8_t *inData, uint8_t *outData, int width,
                                  int height) {
    assert(inData and outData);
    int imgSize = width * height;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            outData[(x + width * y) * 4 + 0] = inData[(x + width * y) * 3 + 0];
            outData[(x + width * y) * 4 + 1] = inData[(x + width * y) * 3 + 1];
            outData[(x + width * y) * 4 + 2] = inData[(x + width * y) * 3 + 2];
            outData[(x + width * y) * 4 + 3] = 255;
        }
    }
}

void ImageAlgorithm::mirrorX(uint8_t *data, int width, int height) {
    for (int c = 0; c < 4; c++) {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width / 2; x++) {
                uint8_t temp = at(data, x, y, c, width, height);
                at(data, x, y, c, width, height) = at(data, width - 1 - x, y, c, width, height);
                data[((width - 1 - x) + width * y) * 4 + c] = temp;
            }
        }
    }
}

void ImageAlgorithm::mirror_X_Y(uint8_t *data, int width, int height) {
    uint8_t *temp = new uint8_t[width * height * 4];
    memcpy(temp, data, width * height * 4);
    for (int c = 0; c < 4; c++) {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                at(data, x, y, c, width, height) = at(temp, height - 1 - y, width - 1 - x, c, width,
                                                      height);
            }
        }
    }
    delete_array(temp);
}

void ImageAlgorithm::mirrorXY(uint8_t *data, int width, int height) {
    uint8_t *temp = new uint8_t[width * height * 4];
    memcpy(temp, data, width * height * 4);
    for (int c = 0; c < 4; c++) {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                at(data, x, y, c, width, height) = at(temp, y, x, c, width, height);
            }
        }
    }
    delete_array(temp);
}

uint8_t &ImageAlgorithm::at(uint8_t *data, int x, int y, int c, int width, int height) {
    x = clamp(x, 0, width - 1);
    y = clamp(y, 0, height - 1);
    return data[(y * height + x) * 4 + c];
}

END_NAMESPACE(VulkanEngine)