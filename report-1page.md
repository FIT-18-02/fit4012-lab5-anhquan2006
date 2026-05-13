# Report 1 page - Lab 4 AES-128
## Thông tin sinh viên
Họ và tên: Phạm Anh Quân

Mã sinh viên: 1871020471

Lớp: CNT18-02
## Mục tiêu
Bài thực hành giúp sinh viên hiểu sâu về quy trình mã hóa và giải mã AES-128 bit, bao gồm việc tự triển khai các hàm biến đổi trạng thái (State), mở rộng khóa (Key Expansion) và cơ chế đóng gói gói tin (Packet encapsulation) để truyền dữ liệu an toàn.
## Cách làm / Method
Hệ thống được triển khai bằng ngôn ngữ C++ với các thành phần chính:

**structures.h**: Định nghĩa cấu trúc Header (chứa IV và độ dài dữ liệu) giúp bên nhận xử lý chính xác bản mã. Chứa các bảng tra cứu (S-box, MixColumns) và hàm KeyExpansion.

**encrypt.cpp**: Thực hiện mã hóa AES-CBC. Sử dụng chuẩn PKCS#7 Padding để đảm bảo dữ liệu luôn là bội số của 16 byte và an toàn hơn so với null-padding.

**decrypt.cpp**: Đọc Header từ file message.aes, thực hiện giải mã ngược và loại bỏ padding để khôi phục plaintext nguyên bản.

**Testing**: Sử dụng các script trong thư mục tests/ để kiểm tra tính đúng đắn qua nhiều kịch bản (Multi-block, Wrong key, Tamper dữ liệu).

Repo được cấu trúc lại theo mẫu starter repo của FIT4012 Lab 4: có `Makefile`, `CMakeLists.txt`, thư mục `tests/`, `logs/`, `scripts/` và GitHub Actions CI.

## Kết quả / Result
**Biên dịch**: Thành công bằng Makefile và CMake.

**Mã hóa/Giải mã**: Đã kiểm thử với chuỗi plaintext dài, chương trình khôi phục chính xác 100% dữ liệu gốc nhờ cơ chế PKCS#7.

**Negative Test**:

Khi dùng Wrong Key, chương trình phát hiện sai lệch dữ liệu sau giải mã.

Khi Tamper (chỉnh sửa file message.aes), chương trình báo lỗi do sai cấu trúc hoặc lỗi unpadding.

**Logs**: Các minh chứng thực hiện được lưu đầy đủ trong thư mục logs/

## Kết luận / Conclusion
Bài lab đã hoàn thành đầy đủ các yêu cầu về bảo mật cơ bản. Việc sử dụng struct Header và chuẩn padding quốc tế (PKCS#7) giúp hệ thống hoạt động ổn định và chuyên nghiệp hơn. Đây là nền tảng quan trọng để tiếp cận các giao thức truyền tin an toàn trong môn học Cybersecurity.
