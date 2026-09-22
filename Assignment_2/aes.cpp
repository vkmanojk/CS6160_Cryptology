#include <iostream>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <array>

using namespace std;

uint8_t MASTER_KEY[4][4];
uint8_t roundKeys[11][4][4];
const uint8_t RC[10] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1B, 0x36};

// ShiftRows
void shiftRows(const uint8_t input[4][4], uint8_t output[4][4]) {
    for (size_t i = 0; i < 4; ++i) {
        for (size_t j = 0; j < 4; ++j) {
            output[i][j] = input[i][(i + j) % 4];
        }
    }
}

// Key Schedule
void runKeySchedule() {
    // Generate MASTER_KEY
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            MASTER_KEY[i][j] = static_cast<uint8_t>(std::rand() % 256);
        }
    }

    // k0 = K
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            roundKeys[0][i][j] = MASTER_KEY[i][j];
        }
    }

    // ki = ShiftRows(k(i-1)) XOR RCi
    for (int round = 1; round <= 10; round++) {
        uint8_t shiftedKey[4][4];
        shiftRows(roundKeys[round - 1], shiftedKey);

        // Copy ShiftRows result into roundKeys[round]
        for (int row = 0; row < 4; row++) {
            for (int col = 0; col < 4; col++) {
                roundKeys[round][row][col] = shiftedKey[row][col];
            }
        }
        roundKeys[round][0][0] ^= RC[round - 1]; // XOR
    }
}

int main() {
    srand(time(nullptr));

    // Plain Text Generation
    const std::array<size_t, 5> plainTextSize = {
        1 * 1024,
        2 * 1024,
        4 * 1024,
        32 * 1024,
        64 * 1024
    };
    
    vector<vector<uint8_t>> plainText(5);

    for (size_t i = 0; i < 5; ++i) {
        plainText[i].resize(plainTextSize[i]);
        for (size_t j = 0; j < plainTextSize[i]; j++) {
            plainText[i][j] = static_cast<uint8_t>(std::rand() % 256);
        }
    }

    /*
    YET TO BE WORKED
    */
    
    return 0;
}