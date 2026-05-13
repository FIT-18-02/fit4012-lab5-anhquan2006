/* "structures.h" defines the look-up tables and KeyExpansion function 
 * used in encrypt.cpp and decrypt.cpp
 */
#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <stdint.h> // Thêm thư viện này để dùng uint8_t, uint32_t

// --- PHẦN 1: CẤU TRÚC GÓI TIN (QUÂN CẦN THÊM VÀO ĐỂ LÀM LAB 5) ---
#define AES_BLOCK_SIZE 16

#pragma pack(push, 1) 
struct Header {
    uint8_t iv[AES_BLOCK_SIZE]; // Vector khởi tạo (IV)
    uint32_t ciphertext_len;    // Độ dài dữ liệu sau mã hóa (bao gồm cả padding)
};

struct Packet {
    Header header;
    uint8_t payload[];         // Dữ liệu mã hóa (Ciphertext) sẽ nằm ở đây
};
#pragma pack(pop)

// --- PHẦN 2: CÁC BẢNG TRA CỨU AES (DỮ NGUYÊN TỪ NỘI DUNG BẠN GỬI) ---

// Encryption: Forward Rijndael S-box 
unsigned char s[256] = {
    0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
    // ... (Giữ nguyên toàn bộ mảng s của bạn ở đây)
    0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16
};

// ... (Giữ nguyên các mảng mul2, mul3, rcon, inv_s, mul9, mul11, mul13, mul14 từ file bạn gửi)

// --- PHẦN 3: CÁC HÀM HỖ TRỢ KEY EXPANSION (GIỮ NGUYÊN) ---

void KeyExpansionCore(unsigned char * in, unsigned char i) {
    unsigned char t = in[0];
    in[0] = in[1];
    in[1] = in[2];
    in[2] = in[3];
    in[3] = t;

    in[0] = s[in[0]];
    in[1] = s[in[1]];
    in[2] = s[in[2]];
    in[3] = s[in[3]];

    in[0] ^= rcon[i];
}

void KeyExpansion(unsigned char inputKey[16], unsigned char expandedKeys[176]) {
    for (int i = 0; i < 16; i++) {
        expandedKeys[i] = inputKey[i];
    }

    int bytesGenerated = 16; 
    int rconIteration = 1; 
    unsigned char tmpCore[4]; 

    while (bytesGenerated < 176) {
        for (int i = 0; i < 4; i++) {
            tmpCore[i] = expandedKeys[i + bytesGenerated - 4];
        }

        if (bytesGenerated % 16 == 0) {
            KeyExpansionCore(tmpCore, rconIteration++);
        }

        for (unsigned char a = 0; a < 4; a++) {
            expandedKeys[bytesGenerated] = expandedKeys[bytesGenerated - 16] ^ tmpCore[a];
            bytesGenerated++;
        }
    }
}

#endif /* STRUCTURES_H */
