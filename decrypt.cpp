#include <iostream>
#include <cstring>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "structures.h"

using namespace std;

/* --- CÁC HÀM XỬ LÝ GIẢI MÃ --- */

void SubRoundKey(unsigned char * state, unsigned char * roundKey) {
    for (int i = 0; i < 16; i++) state[i] ^= roundKey[i];
}

void InverseMixColumns(unsigned char * state) {
    unsigned char tmp[16];
    for (int i = 0; i < 4; i++) {
        int off = i * 4;
        tmp[off + 0] = (unsigned char)(mul14[state[off + 0]] ^ mul11[state[off + 1]] ^ mul13[state[off + 2]] ^ mul9[state[off + 3]]);
        tmp[off + 1] = (unsigned char)(mul9[state[off + 0]] ^ mul14[state[off + 1]] ^ mul11[state[off + 2]] ^ mul13[state[off + 3]]);
        tmp[off + 2] = (unsigned char)(mul13[state[off + 0]] ^ mul9[state[off + 1]] ^ mul14[state[off + 2]] ^ mul11[state[off + 3]]);
        tmp[off + 3] = (unsigned char)(mul11[state[off + 0]] ^ mul13[state[off + 1]] ^ mul9[state[off + 2]] ^ mul14[state[off + 3]]);
    }
    for (int i = 0; i < 16; i++) state[i] = tmp[i];
}

void ShiftRows(unsigned char * state) {
    unsigned char tmp[16];
    tmp[0] = state[0];   tmp[1] = state[13];  tmp[2] = state[10];  tmp[3] = state[7];
    tmp[4] = state[4];   tmp[5] = state[1];   tmp[6] = state[14];  tmp[7] = state[11];
    tmp[8] = state[8];   tmp[9] = state[5];   tmp[10] = state[2];  tmp[11] = state[15];
    tmp[12] = state[12]; tmp[13] = state[9];  tmp[14] = state[6];  tmp[15] = state[3];
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
    for (int i = 8; i >= 0; i--) Round(state, expandedKey + (16 * (i + 1)));
    SubRoundKey(state, expandedKey);
    for (int i = 0; i < 16; i++) decryptedMessage[i] = state[i];
}

/* --- HÀM MAIN --- */

int main() {
    buildTables();

    ifstream infile("message.aes", ios::in | ios::binary);
    if (!infile.is_open()) return 1;

    Header head;
    infile.read((char*)&head, sizeof(Header));
    int cipherLen = head.ciphertext_len;
    
    // Kiểm tra độ dài cipherLen hợp lệ (chia hết cho 16)
    if (cipherLen <= 0 || cipherLen % 16 != 0) return 1;

    unsigned char * encryptedMessage = new unsigned char[cipherLen];
    infile.read((char*)encryptedMessage, cipherLen);
    infile.close();

    unsigned char key[16] = {0};
    ifstream keyfile("keyfile");
    if (keyfile.is_open()) {
        string keystr; getline(keyfile, keystr);
        istringstream iss(keystr);
        unsigned int c;
        for (int i = 0; i < 16 && (iss >> hex >> c); i++) key[i] = (unsigned char)c;
        keyfile.close();
    }

    unsigned char expandedKey[176];
    KeyExpansion(key, expandedKey);

    unsigned char * decryptedData = new unsigned char[cipherLen];
    for (int i = 0; i < cipherLen; i += 16) {
        AESDecrypt(encryptedMessage + i, expandedKey, decryptedData + i);
    }

    // --- KIỂM TRA PADDING PKCS#7 NGHIÊM NGẶT ---
    int paddingVal = (int)decryptedData[cipherLen - 1];
    bool isValid = (paddingVal >= 1 && paddingVal <= 16);
    
    if (isValid) {
        for (int i = 0; i < paddingVal; i++) {
            if (decryptedData[cipherLen - 1 - i] != (unsigned char)paddingVal) {
                isValid = false; 
                break;
            }
        }
    }

    // Nếu dữ liệu bị hỏng (Padding sai), giải phóng bộ nhớ và thoát ngay
    if (!isValid) {
        delete[] encryptedMessage;
        delete[] decryptedData;
        return 1; 
    }

    // --- CHỈ IN KẾT QUẢ KHI DỮ LIỆU CHUẨN ---
    cout << "=============================" << endl;
    cout << " 128-bit AES Decryption Tool " << endl;
    cout << "=============================" << endl;
    
    int actualLen = cipherLen - paddingVal;
    
    cout << "Decrypted message (hex): ";
    for (int i = 0; i < actualLen; i++) {
        cout << hex << setw(2) << setfill('0') << (int)decryptedData[i] << " ";
    }
    cout << endl;

    cout << "Decrypted message: ";
    for (int i = 0; i < actualLen; i++) {
        cout << (char)decryptedData[i];
    }
    cout << endl;

    delete[] encryptedMessage;
    delete[] decryptedData;
    return 0;
}
