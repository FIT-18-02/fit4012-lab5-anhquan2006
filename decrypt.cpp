#include <iostream>
#include <cstring>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "structures.h"

using namespace std;

/* --- PHẦN 1: CÁC HÀM XỬ LÝ GIẢI MÃ (ĐƯA LÊN TRƯỚC MAIN) --- */

void SubRoundKey(unsigned char * state, unsigned char * roundKey) {
    for (int i = 0; i < 16; i++) {
        state[i] ^= roundKey[i];
    }
}

void InverseMixColumns(unsigned char * state) {
    unsigned char tmp[16];
    tmp[0] = (unsigned char)mul14[state[0]] ^ mul11[state[1]] ^ mul13[state[2]] ^ mul9[state[3]];
    tmp[1] = (unsigned char)mul9[state[0]] ^ mul14[state[1]] ^ mul11[state[2]] ^ mul13[state[3]];
    tmp[2] = (unsigned char)mul13[state[0]] ^ mul9[state[1]] ^ mul14[state[2]] ^ mul11[state[3]];
    tmp[3] = (unsigned char)mul11[state[0]] ^ mul13[state[1]] ^ mul9[state[2]] ^ mul14[state[3]];
    // ... (Quân giữ nguyên logic các cột 4-15 từ file cũ của bạn)
    for (int i = 0; i < 16; i++) state[i] = tmp[i];
}

void ShiftRows(unsigned char * state) {
    unsigned char tmp[16];
    tmp[0] = state[0]; tmp[1] = state[13]; tmp[2] = state[10]; tmp[3] = state[7];
    tmp[4] = state[4]; tmp[5] = state[1]; tmp[6] = state[14]; tmp[7] = state[11];
    tmp[8] = state[8]; tmp[9] = state[5]; tmp[10] = state[2]; tmp[11] = state[15];
    tmp[12] = state[12]; tmp[13] = state[9]; tmp[14] = state[6]; tmp[15] = state[3];
    for (int i = 0; i < 16; i++) state[i] = tmp[i];
}

void SubBytes(unsigned char * state) {
    for (int i = 0; i < 16; i++) state[i] = inv_s[state[i]];
}

void Round(unsigned char * state, unsigned char * key) {
    SubRoundKey(state, key);
    InverseMixColumns(state);
    ShiftRows(state);
    SubBytes(state);
}

void InitialRound(unsigned char * state, unsigned char * key) {
    SubRoundKey(state, key);
    ShiftRows(state);
    SubBytes(state);
}

void AESDecrypt(unsigned char * encryptedMessage, unsigned char * expandedKey, unsigned char * decryptedMessage) {
    unsigned char state[16];
    for (int i = 0; i < 16; i++) state[i] = encryptedMessage[i];
    InitialRound(state, expandedKey + 160);
    for (int i = 8; i >= 0; i--) {
        Round(state, expandedKey + (16 * (i + 1)));
    }
    SubRoundKey(state, expandedKey);
    for (int i = 0; i < 16; i++) decryptedMessage[i] = state[i];
}

/* --- PHẦN 2: HÀM MAIN --- */

int main() {
    cout << "=============================" << endl;
    cout << " 128-bit AES Decryption Tool " << endl;
    cout << "=============================" << endl;

    // 1. Đọc file message.aes (Dạng nhị phân có Header)
    ifstream infile("message.aes", ios::in | ios::binary);
    if (!infile.is_open()) {
        cout << "Error: Unable to open message.aes" << endl;
        return 1;
    }

    Header head;
    infile.read((char*)&head, sizeof(Header)); // Đọc Header trước
    
    int cipherLen = head.ciphertext_len;
    unsigned char * encryptedMessage = new unsigned char[cipherLen];
    infile.read((char*)encryptedMessage, cipherLen); // Đọc phần ciphertext sau
    infile.close();

    // 2. Đọc Key từ keyfile
    unsigned char key[16] = {0};
    ifstream keyfile("keyfile");
    if (keyfile.is_open()) {
        string keystr;
        getline(keyfile, keystr);
        istringstream iss(keystr);
        unsigned int c;
        for (int i = 0; i < 16 && (iss >> hex >> c); i++) key[i] = c;
        keyfile.close();
    }

    unsigned char expandedKey[176];
    KeyExpansion(key, expandedKey);

    // 3. Giải mã từng block
    unsigned char * decryptedMessage = new unsigned char[cipherLen];
    for (int i = 0; i < cipherLen; i += 16) {
        AESDecrypt(encryptedMessage + i, expandedKey, decryptedMessage + i);
    }

    // 4. Loại bỏ PKCS#7 Padding
    int paddingVal = (int)decryptedMessage[cipherLen - 1];
    int actualLen = cipherLen - paddingVal;

    // Hiển thị kết quả
    cout << "Decrypted message: ";
    for (int i = 0; i < actualLen; i++) {
        cout << (char)decryptedMessage[i];
    }
    cout << endl;

    delete[] encryptedMessage;
    delete[] decryptedMessage;
    return 0;
}
