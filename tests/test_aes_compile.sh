#!/usr/bin/env bash
# Thiết lập dừng script nếu gặp lỗi hoặc biến chưa định nghĩa
set -euo pipefail

echo "[INFO] Đang thực hiện dọn dẹp và biên dịch lại hệ thống..."

# Sử dụng Makefile để biên dịch nhằm đảm bảo các flag đồng nhất
make clean > /dev/null 2>&1
make all > /dev/null 2>&1

# Kiểm tra sự tồn tại và quyền thực thi của các file binary
if [[ -x "./encrypt" ]] && [[ -x "./decrypt" ]]; then
    echo "[PASS] AES encrypt/decrypt programs compile successfully."
    exit 0
else
    echo "[FAIL] Lỗi biên dịch: Không tìm thấy file thực thi 'encrypt' hoặc 'decrypt'."
    exit 1
fi
