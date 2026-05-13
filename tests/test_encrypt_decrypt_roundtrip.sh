#!/usr/bin/env bash
# Thiết lập dừng script nếu gặp lỗi, biến chưa định nghĩa hoặc lỗi pipe
set -euo pipefail

# 1. Đảm bảo môi trường sạch trước khi test
make all > /dev/null 2>&1

# 2. Tạo file key mẫu để test (khớp với logic đọc key của chương trình)
cat > keyfile <<'KEY'
01 04 02 03 01 03 04 0A 09 0B 07 0F 0F 06 03 00
KEY

PLAINTEXT="hello FIT4012 AES"

# 3. Thực hiện mã hóa
# Gửi plaintext vào encrypt, kết quả sẽ tạo ra file message.aes
echo "[INFO] Đang thực hiện mã hóa thử nghiệm..."
printf "%s" "$PLAINTEXT" | ./encrypt > /dev/null

# 4. Thực hiện giải mã và kiểm tra kết quả
echo "[INFO] Đang thực hiện giải mã thử nghiệm..."
# Lọc bỏ các ký tự null và lấy dòng kết quả cuối cùng
OUTPUT=$(./decrypt 2>/dev/null | grep "Decrypted message:" | sed 's/Decrypted message: //')

# 5. So sánh kết quả
if [[ "$OUTPUT" == *"$PLAINTEXT"* ]]; then
    echo "[PASS] Round-trip encrypt/decrypt recovers plaintext."
    exit 0
else
    echo "--- Kết quả giải mã thực tế ---"
    echo "$OUTPUT"
    echo "-------------------------------"
    echo "[FAIL] Round-trip encrypt/decrypt did not recover plaintext."
    exit 1
fi
