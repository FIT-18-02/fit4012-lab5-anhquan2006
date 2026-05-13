[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/0pZSTN4X)
# FIT4012 - Lab 5: AES-128 Starter Repository

## 1. Cấu trúc repo

```text
.
├── .github/
│   ├── scripts/
│   │   └── check_submission.sh
│   └── workflows/
│       └── ci.yml
├── docs/
│   └── aes-code-notes.md
├── logs/
│   ├── .gitkeep
│   ├── README.md
│   └── sample-run.log
├── scripts/
│   └── run_sample.sh
├── tests/
│   ├── test_aes_compile.sh
│   ├── test_encrypt_decrypt_roundtrip.sh
│   ├── test_multiblock_padding.sh
│   ├── test_tamper_negative.sh
│   └── test_wrong_key_negative.sh
├── .gitignore
├── CMakeLists.txt
├── Makefile
├── README.md
├── decrypt.cpp
├── encrypt.cpp
├── keyfile
├── report-1page.md
└── structures.h
```

## 2. Cách chạy chương trình

### Cách 1: Dùng Makefile

```bash
make
printf "hello FIT4012 AES\n" | ./encrypt
./decrypt
```

### Cách 2: Chạy script mẫu

```bash
bash scripts/run_sample.sh
```

### Cách 3: Biên dịch trực tiếp

```bash
g++ -std=c++17 -Wall -Wextra -pedantic encrypt.cpp -o encrypt
g++ -std=c++17 -Wall -Wextra -pedantic decrypt.cpp -o decrypt
printf "hello FIT4012 AES\n" | ./encrypt
./decrypt
```

### Cách 4: Dùng CMake

```bash
cmake -S . -B build
cmake --build build
printf "hello FIT4012 AES\n" | ./build/encrypt
./build/decrypt
```

## 3. Input / Đầu vào

Chương trình mã hóa `encrypt.cpp` nhận plaintext từ bàn phím bằng `cin.getline(...)`.

Chương trình giải mã `decrypt.cpp` đọc ciphertext từ file `message.aes`.

Cả hai chương trình đều đọc khóa AES-128 từ file `keyfile`. File `keyfile` đang dùng định dạng 16 byte biểu diễn dưới dạng hex, ví dụ:

```text
01 04 02 03 01 03 04 0A 09 0B 07 0F 0F 06 03 00
```

## 4. Output / Đầu ra

- `encrypt` in ciphertext theo dạng hex ra màn hình.
- `encrypt` ghi ciphertext ra file `message.aes`.
- `decrypt` đọc `message.aes`, in plaintext dạng hex và plaintext dạng ký tự.

## 5. Padding đang dùng

**Loại Padding**: Hiện tại mã nguồn đã được nâng cấp lên PKCS#7 Padding.

**Mô tả**: AES yêu cầu dữ liệu đầu vào phải là bội số của 16 byte. Nếu dữ liệu không đủ, chương trình sẽ tự động thêm các byte có giá trị bằng chính số lượng byte cần đệm.

**Ưu điểm**: Khác với Zero Padding, PKCS#7 giúp việc giải mã xác định chính xác vị trí kết thúc của dữ liệu thật, tránh bị nhầm lẫn với các byte 0x00 có sẵn trong dữ liệu gốc.

## 6. Tests bắt buộc

Dự án bao gồm 5 kịch bản kiểm thử tự động để đảm bảo tính ổn định:

**test_aes_compile.sh**: Kiểm tra khả năng biên dịch mã nguồn.

**test_encrypt_decrypt_roundtrip.sh**: Kiểm tra tính toàn vẹn của dữ liệu sau khi mã hóa và giải mã.

**test_multiblock_padding.sh**: Kiểm tra xử lý dữ liệu dài (nhiều block 16-byte).

**test_tamper_negative.sh**: Kiểm tra phản ứng khi file ciphertext bị chỉnh sửa trái phép.

**test_wrong_key_negative.sh**: Kiểm tra lỗi khi dùng sai khóa giải mã.

Chạy toàn bộ test:

```bash
make test
```

## 7. Logs / Minh chứng

Thư mục `logs/` chứa các kết quả thực thi thực tế phục vụ cho việc chấm bài:

- Báo cáo build chương trình thành công.

- Log quá trình mã hóa/giải mã thành công.

- Minh chứng các trường hợp test lỗi (sai khóa, sửa file).

## 8. Ethics & Safe use
- Chỉ sử dụng cho mục đích học tập và nghiên cứu.

- Không sử dụng mã nguồn này cho các hệ thống bảo mật thực tế hoặc mục đích can thiệp trái phép.

- Phải trích dẫn nguồn rõ ràng nếu tham khảo từ các tài liệu bên ngoài hoặc công cụ hỗ trợ.

- Tuân thủ quy định về trung thực học thuật.

## 9. Checklist nộp bài

Trước khi nộp, sinh viên cần có:
[x] Có đầy đủ: encrypt.cpp, decrypt.cpp, structures.h.

[x] File README.md đã mô tả đúng các bước và cơ chế padding.

[x] Hoàn thiện file báo cáo nội bộ report-1page.md.

[x] Chạy thành công ít nhất 5 loại test.

[x] Thư mục logs/ có minh chứng chạy thực tế.

[x] Đã xóa toàn bộ các dòng đánh dấu TODO_STUDENT.

## 10. Lưu ý kỹ thuật
- Đã chuyển đổi thành công từ Zero Padding sang PKCS#7 Padding.

- Cấu trúc code được tổ chức lại để tránh lỗi Scope khi biên dịch.

- Cơ chế ghi file sử dụng chế độ Binary để đảm bảo tính chính xác cho các byte đặc biệt.
