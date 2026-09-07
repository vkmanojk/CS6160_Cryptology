#include<stdio.h>
#include <stdint.h>
#include<stdlib.h>
#include<time.h>

uint8_t MASTER_KEY [4][4];
uint8_t roundKeys [11][4][4];
const uint8_t RC[10] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1B, 0x36};

//ShiftRows
void shiftRows(uint8_t input[4][4], uint8_t output[4][4])
{
    for (size_t i = 0; i < 4; ++i) {
        for (size_t j = 0; j < 4; ++j) {
            output[i][j] = input[i][(i+j) % 4];
        }
    }
}

void runKeySchedule(void)
{
    // Generate MASTER_KEY
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            MASTER_KEY[i][j] =
                (uint8_t)(rand() % 256);
        }
    }

    // k0 = K
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            roundKeys[0][i][j] = MASTER_KEY[i][j];
        }
    }

    // ki = ShiftRows(k(i-1)) XOR RCi
    for (int i = 1; i <= 10; i++) {
        uint8_t shiftedKey[4][4];
        shiftRows(roundKeys[i - 1], shiftedKey);

        // Copy ShiftRows result into roundKeys[i]
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                roundKeys[i][i][j] =
                    shiftedKey[i][j];
            }
        }
        roundKeys[i][0][0] ^= RC[i - 1]; //XOR
    }
}

int main() {
    srand((size_t)time(NULL));

    //Plain Text Generation
    const size_t plainTextSize[5] = {
        1 * 1024,
        2 * 1024,
        4 * 1024,
        32 * 1024,
        64 * 1024
    };
    uint8_t *plainText[5];

    for (size_t i = 0; i < 5; ++i) {
        plainText[i] = malloc(plainTextSize[i]);
        for (size_t j = 0; j < plainTextSize[i]; j++) {
            plainText[i][j] = (uint8_t)(rand() % 256);
        }
    }
    /*
    YET TO BE WORKED
    */
    //Free Plain Texts
    for (int i = 0; i < 5; i++) {
        free(plainText[i]);
    }


    return 0;
}