#include <iostream>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <array>
#include <immintrin.h> 
#include <x86intrin.h> 

using namespace std;

uint8_t MASTER_KEY[16];
uint8_t roundKeys[11][4][4];
const uint8_t RC[10] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1B, 0x36};

__m128i roundKeys_NI[11];
__m128i decRoundKeys_NI[11];

const uint8_t sbox[256] = {
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
};

const uint8_t rsbox[256] = {
    0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
    0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
    0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
    0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
    0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
    0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
    0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
    0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
    0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
    0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
    0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
    0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
    0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
    0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
    0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
    0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d
};

void shiftRows(uint8_t state[4][4]) {
    uint8_t temp[4][4];
    for (size_t i = 0; i < 4; ++i)
        for (size_t j = 0; j < 4; ++j) 
            temp[i][j] = state[i][(i + j) % 4];
    for (size_t i = 0; i < 4; ++i)
        for (size_t j = 0; j < 4; ++j) 
            state[i][j] = temp[i][j];
}

void invShiftRows(uint8_t state[4][4]) {
    uint8_t temp[4][4];
    for (size_t i = 0; i < 4; ++i)
        for (size_t j = 0; j < 4; ++j) 
            temp[i][j] = state[i][(j - i + 4) % 4];
    for (size_t i = 0; i < 4; ++i)
        for (size_t j = 0; j < 4; ++j) 
            state[i][j] = temp[i][j];
}

void keySchedule() {
    for (size_t i = 0; i < 16; ++i) 
        roundKeys[0][i % 4][i / 4] = MASTER_KEY[i];

    for (size_t round = 1; round <= 10; ++round) {
        for (size_t row = 0; row < 4; ++row) 
            for (size_t col = 0; col < 4; ++col) 
                roundKeys[round][row][col] = roundKeys[round - 1][row][col];
        shiftRows(roundKeys[round]);
        roundKeys[round][0][0] ^= RC[round - 1]; 
    }
}

void keySchedule_NI() {
    roundKeys_NI[0] = _mm_loadu_si128((__m128i*)MASTER_KEY);
    decRoundKeys_NI[0] = roundKeys_NI[0];

    const __m128i shiftrows_mask = _mm_setr_epi8(
        0, 5, 10, 15, 
        4, 9, 14, 3, 
        8, 13, 2, 7, 
        12, 1, 6, 11
    );

    for (int round = 1; round <= 10; ++round) {
        __m128i shifted = _mm_shuffle_epi8(roundKeys_NI[round - 1], shiftrows_mask);
        __m128i rc_vec = _mm_setr_epi8(RC[round - 1], 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        roundKeys_NI[round] = _mm_xor_si128(shifted, rc_vec);

        if (round < 10) 
            decRoundKeys_NI[round] = _mm_aesimc_si128(roundKeys_NI[round]);
        else 
            decRoundKeys_NI[round] = roundKeys_NI[round];
    }
}

inline uint8_t mul2(uint8_t x) { return (x << 1) ^ ((x & 0x80) ? 0x1B : 0x00); }
inline uint8_t mul3(uint8_t x) { return mul2(x) ^ x; }
inline uint8_t mul9(uint8_t x) { return mul2(mul2(mul2(x))) ^ x; }
inline uint8_t mul11(uint8_t x) { return mul2(mul2(mul2(x))) ^ mul2(x) ^ x; }
inline uint8_t mul13(uint8_t x) { return mul2(mul2(mul2(x))) ^ mul2(mul2(x)) ^ x; }
inline uint8_t mul14(uint8_t x) { return mul2(mul2(mul2(x))) ^ mul2(mul2(x)) ^ mul2(x); }

void subBytes(uint8_t state[4][4]) {
    for (size_t i = 0; i < 4; ++i)
        for (size_t j = 0; j < 4; ++j)
            state[i][j] = sbox[state[i][j]];
}

void invSubBytes(uint8_t state[4][4]) {
    for (size_t i = 0; i < 4; ++i)
        for (size_t j = 0; j < 4; ++j)
            state[i][j] = rsbox[state[i][j]];
}

void mixColumns(uint8_t state[4][4]) {
    for (size_t c = 0; c < 4; ++c) {
        uint8_t s[4] = {state[0][c], state[1][c], state[2][c], state[3][c]};
        state[0][c] = mul2(s[0]) ^ mul3(s[1]) ^ s[2] ^ s[3];
        state[1][c] = s[0] ^ mul2(s[1]) ^ mul3(s[2]) ^ s[3];
        state[2][c] = s[0] ^ s[1] ^ mul2(s[2]) ^ mul3(s[3]);
        state[3][c] = mul3(s[0]) ^ s[1] ^ s[2] ^ mul2(s[3]);
    }
}

void invMixColumns(uint8_t state[4][4]) {
    for (size_t c = 0; c < 4; ++c) {
        uint8_t s[4] = {state[0][c], state[1][c], state[2][c], state[3][c]};
        state[0][c] = mul14(s[0]) ^ mul11(s[1]) ^ mul13(s[2]) ^ mul9(s[3]);
        state[1][c] = mul9(s[0])  ^ mul14(s[1]) ^ mul11(s[2]) ^ mul13(s[3]);
        state[2][c] = mul13(s[0]) ^ mul9(s[1])  ^ mul14(s[2]) ^ mul11(s[3]);
        state[3][c] = mul11(s[0]) ^ mul13(s[1]) ^ mul9(s[2])  ^ mul14(s[3]);
    }
}

void addRoundKey(uint8_t state[4][4], const uint8_t key[4][4]) {
    for (size_t i = 0; i < 4; ++i)
        for (size_t j = 0; j < 4; ++j)
            state[i][j] ^= key[i][j];
}

void encryptBlock(uint8_t state[4][4]) {
    addRoundKey(state, roundKeys[0]);
    for (size_t round = 1; round <= 9; ++round) {
        subBytes(state);
        shiftRows(state);
        mixColumns(state);
        addRoundKey(state, roundKeys[round]);
    }
    subBytes(state);
    shiftRows(state);
    addRoundKey(state, roundKeys[10]);
}

void decryptBlock(uint8_t state[4][4]) {
    addRoundKey(state, roundKeys[10]);
    for (size_t round = 9; round >= 1; --round) {
        invShiftRows(state);
        invSubBytes(state);
        addRoundKey(state, roundKeys[round]);
        invMixColumns(state);
    }
    invShiftRows(state);
    invSubBytes(state);
    addRoundKey(state, roundKeys[0]);
}

void encryptBlock_NI(__m128i& state) {
    state = _mm_xor_si128(state, roundKeys_NI[0]);
    for (int round = 1; round <= 9; ++round) 
        state = _mm_aesenc_si128(state, roundKeys_NI[round]);
    state = _mm_aesenclast_si128(state, roundKeys_NI[10]);
}

void decryptBlock_NI(__m128i& state) {
    state = _mm_xor_si128(state, decRoundKeys_NI[10]);
    for (int round = 9; round >= 1; --round) 
        state = _mm_aesdec_si128(state, decRoundKeys_NI[round]);
    state = _mm_aesdeclast_si128(state, decRoundKeys_NI[0]);
}

int main() {
    srand(time(nullptr));
    for (size_t i = 0; i < 16; ++i) 
        MASTER_KEY[i] = rand() % 256;
    
    keySchedule();
    keySchedule_NI();
    
    const array<size_t, 5> plainTextSize = {
        1 * 1024, 2 * 1024, 4 * 1024, 32 * 1024, 64 * 1024
    };

    for (size_t i = 0; i < 5; ++i) {
        vector<uint8_t> originalText(plainTextSize[i]);
        for (size_t j = 0; j < plainTextSize[i]; ++j) 
            originalText[j] = rand() % 256;
            
        vector<uint8_t> plainText_Soft = originalText;
        vector<uint8_t> plainText_NI = originalText;

        for (size_t block = 0; block < plainTextSize[i]; block += 16) {
            uint8_t state[4][4];
            for (size_t j = 0; j < 16; ++j) 
                state[j % 4][j / 4] = plainText_Soft[block + j];
            encryptBlock(state);
            for (size_t j = 0; j < 16; ++j)
                plainText_Soft[block + j] = state[j % 4][j / 4];
        }

        for (size_t block = 0; block < plainTextSize[i]; block += 16) {
            __m128i state_ni = _mm_loadu_si128((__m128i*)&plainText_NI[block]);
            encryptBlock_NI(state_ni);
            _mm_storeu_si128((__m128i*)&plainText_NI[block], state_ni);
        }
        
        bool cipherMatch = (plainText_Soft == plainText_NI);
        
        for (size_t block = 0; block < plainTextSize[i]; block += 16) {
            uint8_t state[4][4];
            for (size_t j = 0; j < 16; ++j) 
                state[j % 4][j / 4] = plainText_Soft[block + j];
            decryptBlock(state);
            for (size_t j = 0; j < 16; ++j)
                plainText_Soft[block + j] = state[j % 4][j / 4];
        }

        for (size_t block = 0; block < plainTextSize[i]; block += 16) {
            __m128i state_ni = _mm_loadu_si128((__m128i*)&plainText_NI[block]);
            decryptBlock_NI(state_ni);
            _mm_storeu_si128((__m128i*)&plainText_NI[block], state_ni);
        }
        
        bool plainMatch = (plainText_Soft == originalText) && (plainText_NI == originalText);
        
        cout << "File Size: " << plainTextSize[i] / 1024 << " KB\n";
        cout << "Ciphertexts Match: " << (cipherMatch ? "YES" : "NO") << "\n";
        cout << "Decrypted Texts Match Original: " << (plainMatch ? "YES" : "NO") << "\n\n";
    }
    
    return 0;
}