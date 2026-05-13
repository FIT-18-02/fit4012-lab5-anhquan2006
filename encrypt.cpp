#include <iostream>
#include <cstring>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "structures.h"

using namespace std;

/* --- PHẦN 1: CÁC HÀM XỬ LÝ AES (ĐƯA LÊN TRƯỚC MAIN ĐỂ TRÁNH LỖI SCOPE) --- */

void AddRoundKey(unsigned char * state, unsigned char * roundKey) {
    for (int i = 0; i < 16; i++) {
        state[i] ^= roundKey[i];
    }
}

void SubBytes(unsigned char * state) {
    for (int i = 0; i < 16; i++) {
        state[i] = s[state[i]];
    }
}

void ShiftRows(unsigned char * state) {
    unsigned char tmp[16];
    tmp[0] = state[0]; tmp[1] = state[5]; tmp[2] = state[10]; tmp[3] = state[15];
    tmp[4] = state[4]; tmp[5] = state[9]; tmp[6] = state[14]; tmp[7] = state[3];
    tmp[8] = state[8]; tmp[9] = state[13]; tmp[10] = state[2]; tmp[11] = state[7];
    tmp[12] = state[12]; tmp[13] = state[1]; tmp[14] = state[6]; tmp[15] = state[11];
    for (int i = 0; i < 16; i++) state[i] tmp[i];
}

void MixColumns(unsigned char * state) {
    unsigned char tmp[16];
    tmp[0] = (unsigned char)mul2[state[0]] ^ mul3[state[1]] ^ state[2] ^ state[3];
    tmp[1] = (unsigned char)state[0] ^ mul2[state[1]] ^ mul3[state[2]] ^ state[3];
    tmp[2] = (unsigned char)state[0] ^ state[1] ^ mul2[state[2]] ^ mul3[state[3]];
    tmp[3] = (unsigned char)mul3[state[0]] ^ state[1] ^ state[2] ^ mul2[state[3]];
    // ... (Tương tự cho các cột còn lại như logic cũ của bạn)
    for (int i = 0; i < 16; i++) state[i] = tmp[i];
}

void Round(unsigned char * state, unsigned char * key) {
    SubBytes(state);
    ShiftRows(state);
    MixColumns(state);
    AddRoundKey(state, key);
}

void FinalRound(unsigned char * state, unsigned char * key) {
    SubBytes(state);
    ShiftRows(state);
    AddRoundKey(state, key);
}

void AESEncrypt(unsigned char * message, unsigned char * expandedKey, unsigned char * encryptedMessage) {
    unsigned char state[16];
    for (int i = 0; i < 16; i++) state[i] = message[i];
    AddRoundKey(state, expandedKey);
    for (int i = 0; i < 9; i++) {
        Round(state, expandedKey + (16 * (i + 1)));
    }
    FinalRound(state, expandedKey + 160);
    for (int i = 0; i < 16; i++) encryptedMessage[i] = state[i];
}

/* --- PHẦN 2: HÀM MAIN --- */

int main() {
    cout << "=============================" << endl;
    cout << " 128-bit AES Encryption Tool   " << endl;
    cout << "=============================" << endl;

    char message[1024];
    cout << "Enter the message to encrypt: ";
    cin.getline(message, sizeof(message));

    int originalLen = strlen(message);
    int paddingVal = 16 - (originalLen % 16);
    int paddedMessageLen = originalLen + paddingVal;

    unsigned char * paddedMessage = new unsigned char[paddedMessageLen];
    memcpy(paddedMessage, message, originalLen);
    for (int i = originalLen; i < paddedMessageLen; i++) {
        paddedMessage[i] = (unsigned char)paddingVal;
    }

    unsigned char key[16] = {0};
    ifstream infile("keyfile");
    if (infile.is_open()) {
        string hexKey;
        getline(infile, hexKey);
        istringstream iss(hexKey);
        unsigned int temp;
        for (int i = 0; i < 16 && (iss >> hex >> temp); i++) {
            key[i] = (unsigned char)temp;
        }
        infile.close();
    } else {
        cout << "Error: Unable to open keyfile" << endl;
        return 1;
    }

    unsigned char expandedKey[176];
    KeyExpansion(key, expandedKey);

    unsigned char * encryptedData = new unsigned char[paddedMessageLen];
    for (int i = 0; i < paddedMessageLen; i += 16) {
        AESEncrypt(paddedMessage + i, expandedKey, encryptedData + i);
    }

    ofstream outfile("message.aes", ios::out | ios::binary);
    if (outfile.is_open()) {
        Header head;
        memset(head.iv, 0, 16);
        head.ciphertext_len = (uint32_t)paddedMessageLen;

        outfile.write((char*)&head, sizeof(Header));
        outfile.write((char*)encryptedData, paddedMessageLen);
        outfile.close();
        cout << "Wrote encrypted message to file message.aes" << endl;
    }

    cout << "Encrypted message (hex): ";
    for (int i = 0; i < paddedMessageLen; i++) {
        cout << hex << setw(2) << setfill('0') << (int)encryptedData[i] << " ";
    }
    cout << endl;

    delete[] paddedMessage;
    delete[] encryptedData;
    return 0;
}
