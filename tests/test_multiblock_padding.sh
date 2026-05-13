#!/usr/bin/env bash
# Thiết lập dừng script nếu gặp lỗi, biến chưa định nghĩa hoặc lỗi pipe
set -euo pipefail

# 1. Biên dịch lại hệ thống
make all > /dev/null 2>&1

# 2. Thiết lập Key mặc định
cat > keyfile <<'KEY'
01 04 02 03 01 03 04 0A 09 0B 07 0F 0F 06 03 00
KEY

# 3. Sử dụng chuỗi dài hơn 1 block (32 bytes - vừa khít 2 blocks)
# Trong AES, nếu dữ liệu vừa khít block, PKCS#7 vẫn sẽ thêm 1 block padding mới (16 bytes)
PLAINTEXT="AES message longer than one block"

echo "[INFO] Đang test dữ liệu dài (Multi-block)..."

# 4. Thực hiện mã hóa và lưu log tạm
# Sử dụng printf %s để tránh ký tự \n tự động làm sai lệch độ dài block
printf "%s" "$PLAINTEXT" | ./encrypt > /dev/null

# 5. Giải mã và lọc output
# Lấy dòng chứa kết quả đã giải mã, loại bỏ các ký tự null
OUTPUT=$(./decrypt 2>/dev/null | grep "Decrypted message:" | sed 's/Decrypted message: //')

# 6. Kiểm tra tính toàn vẹn
if [[ "$OUTPUT" == "$PLAINTEXT" ]]; then
    echo "[PASS] Multi-block plaintext with padding is recovered correctly."
    exit 0
else
    echo "--- Lỗi giải mã Multi-block ---"
    echo "Gốc: $PLAINTEXT"
    echo "Nhận: $OUTPUT"
    echo "--------------------------------"
    echo "[FAIL] Multi-block plaintext was not recovered correctly."
    exit 1
fi
