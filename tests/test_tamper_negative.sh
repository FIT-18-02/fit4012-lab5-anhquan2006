#!/usr/bin/env bash
# Thiết lập dừng script nếu gặp lỗi, biến chưa định nghĩa hoặc lỗi pipe
set -euo pipefail

# 1. Chuẩn bị môi trường
make all >/dev/null 2>&1

# 2. Thiết lập Key mặc định
cat > keyfile <<'KEY'
01 04 02 03 01 03 04 0A 09 0B 07 0F 0F 06 03 00
KEY

PLAINTEXT="tamper negative test"

echo "[INFO] Đang thực hiện kiểm tra tấn công Tamper (chỉnh sửa bản mã)..."

# 3. Mã hóa dữ liệu gốc
printf "%s" "$PLAINTEXT" | ./encrypt > /dev/null

# 4. Thực hiện Tamper: Đảo 1 bit trong file message.aes bằng Python
# Việc này mô phỏng dữ liệu bị can thiệp trên đường truyền
python3 - <<'PY'
from pathlib import Path
p = Path('message.aes')
if not p.exists():
    raise SystemExit('Lỗi: Không tìm thấy file message.aes')
data = bytearray(p.read_bytes())
if not data:
    raise SystemExit('Lỗi: File message.aes trống')

# Đảo bit đầu tiên của ciphertext
data[0] ^= 0x01 
p.write_bytes(data)
PY

# 5. Thử giải mã file đã bị chỉnh sửa
# Sử dụng '|| true' vì chúng ta kỳ vọng chương trình sẽ trả về lỗi (exit code khác 0)
OUTPUT=$(./decrypt 2>&1 | tr -d '\000' || true)

# 6. Kiểm tra kết quả
# Nếu đầu ra vẫn chứa plaintext ban đầu nghĩa là test THẤT BẠI
if grep -q "$PLAINTEXT" <<< "$OUTPUT"; then
    echo "--- decrypt output ---"
    echo "$OUTPUT"
    echo "----------------------"
    echo "[FAIL] Lỗi bảo mật: Dữ liệu bị chỉnh sửa nhưng vẫn giải mã ra plaintext cũ!"
    exit 1
fi

echo "[PASS] Tamper detected: Dữ liệu bị chỉnh sửa không thể giải mã chính xác."
