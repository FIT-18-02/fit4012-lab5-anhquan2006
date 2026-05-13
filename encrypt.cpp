#include <iostream>
#include <cstring>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "structures.h"

using namespace std;

// ... (Giữ nguyên các hàm AddRoundKey, SubBytes, ShiftRows, MixColumns, Round, FinalRound, AESEncrypt) ...

int main() {
    cout << "=============================" << endl;
    cout << " 128-bit AES Encryption Tool   " << endl;
    cout << "=============================" << endl;

    char message[1024];
    cout << "Enter the message to encrypt: ";
    cin.getline(message, sizeof(message));

    int originalLen = strlen(message);
    
    // 1. PKCS#7 Padding: AES yêu cầu dữ liệu luôn là bội số của 16
    // Nếu dữ liệu là 16 byte, vẫn phải thêm 1 block padding 16 byte mới.
    int paddingVal = 16 - (originalLen % 16);
    int paddedMessageLen = originalLen + paddingVal;

    unsigned char * paddedMessage = new unsigned char[paddedMessageLen];
    memcpy(paddedMessage, message, originalLen);
    for (int i = originalLen; i < paddedMessageLen; i++) {
        paddedMessage[i] = (unsigned char)paddingVal; // PKCS#7 điền giá trị bằng số byte padding
    }

    // 2. Đọc Key từ file
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

    // 3. Mã hóa từng block 16 byte
    unsigned char * encryptedData = new unsigned char[paddedMessageLen];
    for (int i = 0; i < paddedMessageLen; i += 16) {
        AESEncrypt(paddedMessage + i, expandedKey, encryptedData + i);
    }

    // 4. Đóng gói Header và ghi file "message.aes" theo chuẩn Lab
    // Cấu trúc: [Header (IV + ciphertext_len)] + [Ciphertext]
    ofstream outfile("message.aes", ios::out | ios::binary);
    if (outfile.is_open()) {
        Header head;
        memset(head.iv, 0, 16); // Trong bài này chúng ta tạm để IV = 0 cho đơn giản
        head.ciphertext_len = (uint32_t)paddedMessageLen;

        // Ghi Header trước
        outfile.write((char*)&head, sizeof(Header));
        // Ghi Ciphertext sau
        outfile.write((char*)encryptedData, paddedMessageLen);
        
        outfile.close();
        cout << "Wrote encrypted message to file message.aes" << endl;
    }

    // Xuất Hex ra màn hình để kiểm tra
    cout << "Encrypted message (hex): ";
    for (int i = 0; i < paddedMessageLen; i++) {
        cout << hex << setw(2) << setfill('0') << (int)encryptedData[i] << " ";
    }
    cout << endl;

    delete[] paddedMessage;
    delete[] encryptedData;
    return 0;
}
