#!/usr/bin/env bash
# Thiết lập dừng script nếu gặp lỗi, biến chưa định nghĩa hoặc lỗi pipe
set -euo pipefail

# Đảm bảo thư mục logs tồn tại
mkdir -p logs

echo "--- Đang biên dịch mã nguồn (make all) ---"
make all

PLAINTEXT="hello FIT4012 AES"

{
  echo "=========================================="
  echo "[INFO] Thời gian chạy: $(date)"
  echo "[INFO] Plaintext gốc: ${PLAINTEXT}"
  echo "------------------------------------------"
  
  echo "[INFO] Đang chạy mã hóa (./encrypt)..."
  # Gửi plaintext vào chương trình encrypt
  printf "%s\n" "$PLAINTEXT" | ./encrypt
  
  echo
  echo "[INFO] Đang chạy giải mã (./decrypt)..."
  # Chạy chương trình decrypt để khôi phục dữ liệu từ message.aes
  ./decrypt
  
  # Kiểm tra trạng thái thoát của lệnh cuối cùng (decrypt)
  if [ $? -eq 0 ]; then
    echo -e "\n[SUCCESS] Quá trình mã hóa/giải mã hoàn tất thành công."
  else
    echo -e "\n[ERROR] Phát hiện lỗi trong quá trình giải mã (có thể do sai Padding/Tamper)."
  fi
  echo "=========================================="
} | tee logs/sample-run.log
