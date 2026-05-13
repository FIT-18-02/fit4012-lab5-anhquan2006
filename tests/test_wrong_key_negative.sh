#!/usr/bin/env bash
# Thiết lập dừng script nếu gặp lỗi, biến chưa định nghĩa hoặc lỗi pipe
set -euo pipefail

# 1. Chuẩn bị môi trường sạch
make all >/dev/null 2>&1

# 2. Thiết lập khóa ĐÚNG để mã hóa
cat > keyfile <<'KEY'
01 04 02 03 01 03 04 0A 09 0B 07 0F 0F 06 03 00
KEY

PLAINTEXT="wrong key negative test"

echo "[INFO] Đang test trường hợp dùng sai khóa (Wrong Key)..."

# 3. Mã hóa với khóa đúng
printf "%s" "$PLAINTEXT" | ./encrypt > /dev/null

# 4. Ghi đè file keyfile bằng một khóa SAI (toàn số 0)
cat > keyfile <<'KEY'
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
KEY

# 5. Thử giải mã với khóa sai
# Dùng '|| true' vì chương trình sẽ trả về lỗi (exit code khác 0) khi không khớp Padding
OUTPUT=$(./decrypt 2>&1 | tr -d '\000' || true)

# 6. Kiểm tra kết quả
if grep -q "$PLAINTEXT" <<< "$OUTPUT"; then
  echo "--- decrypt output ---"
  echo "$OUTPUT"
  echo "----------------------"
  echo "[FAIL] Lỗi bảo mật: Dùng sai khóa vẫn giải mã ra nội dung gốc!"
  exit 1
fi

echo "[PASS] Wrong key test: Khóa sai không thể khôi phục plaintext ban đầu."
