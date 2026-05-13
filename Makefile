# Trình biên dịch và các cờ
CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -pedantic -O2

# Tên các file thực thi
ENCRYPT_TARGET := encrypt
DECRYPT_TARGET := decrypt

# Danh sách các file header để theo dõi thay đổi
DEPS := structures.h

# Các mục tiêu ảo
.PHONY: all clean run encrypt-sample decrypt-sample test

# Mặc định biên dịch tất cả
all: $(ENCRYPT_TARGET) $(DECRYPT_TARGET)

# Biên dịch chương trình mã hóa
$(ENCRYPT_TARGET): encrypt.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) encrypt.cpp -o $(ENCRYPT_TARGET)

# Biên dịch chương trình giải mã
$(DECRYPT_TARGET): decrypt.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) decrypt.cpp -o $(DECRYPT_TARGET)

# Chạy kịch bản mẫu
run: all
	@mkdir -p logs
	bash scripts/run_sample.sh

# Thử nghiệm nhanh mã hóa
encrypt-sample: $(ENCRYPT_TARGET)
	printf "hello FIT4012 AES\n" | ./$(ENCRYPT_TARGET)

# Thử nghiệm nhanh giải mã
decrypt-sample: $(DECRYPT_TARGET)
	./$(DECRYPT_TARGET)

# Chạy toàn bộ các bài test lab
test: all
	@echo "--- Đang chạy các bài kiểm tra tự động ---"
	bash tests/test_aes_compile.sh
	bash tests/test_encrypt_decrypt_roundtrip.sh
	bash tests/test_multiblock_padding.sh
	bash tests/test_tamper_negative.sh
	bash tests/test_wrong_key_negative.sh

# Dọn dẹp các file rác và file đã biên dịch
clean:
	rm -f $(ENCRYPT_TARGET) $(DECRYPT_TARGET) message.aes keyfile
	rm -rf build logs
	@echo "Đã dọn dẹp sạch sẽ."
