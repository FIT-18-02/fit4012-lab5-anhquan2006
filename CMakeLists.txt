cmake_minimum_required(VERSION 3.10)
project(fit4012_lab5_aes LANGUAGES CXX)

# Thiết lập chuẩn C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Tìm tất cả các file header để theo dõi thay đổi
set(DEPS structures.h)

# Cấu hình cho chương trình mã hóa (encrypt)
add_executable(encrypt encrypt.cpp ${DEPS})
target_compile_options(encrypt PRIVATE -Wall -Wextra -pedantic -O2)

# Cấu hình cho chương trình giải mã (decrypt)
add_executable(decrypt decrypt.cpp ${DEPS})
target_compile_options(decrypt PRIVATE -Wall -Wextra -pedantic -O2)

# Đảm bảo các file output được đặt đúng thư mục gốc để các script test có thể tìm thấy
set_target_properties(encrypt decrypt PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/..")

# (Tùy chọn) Thêm chỉ dẫn để CMake biết về thư mục chứa các file mã nguồn nếu cần
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
