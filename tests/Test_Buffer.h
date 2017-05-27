#ifndef TEST_BUFFER_H
#define TEST_BUFFER_H
#include <cpptest.h>
#include <Buffer.h>

class BufferTestSuite: public Test::Suite
{
public:
	BufferTestSuite();
	~BufferTestSuite();
private:
	Buffer buf;
	bool true_bool = true;
	bool false_bool = false;
	int8_t signed_byte_value = 0xAB;
	uint8_t byte_value = 0xAB;
	int16_t signed_short_value = 0xABCD;
	uint16_t short_value = 0xABCD;
	int32_t int32 = 0xFEDCBA98;
	uint32_t uint32 = 0xFEDCBA98;
	int64_t int64 = 0xFEDCBA9876543211;
	uint64_t uint64 = 0xFEDCBA9876543211;
	size_t noBitsInBool = 1;
	size_t noBitsInByte = sizeof(byte_value) * 8;
	size_t noBitsInShort = sizeof(short_value) * 8;
	size_t noBitsInInt32 = sizeof(uint32) * 8;
	size_t noBitsInInt64 = sizeof(uint64) * 8;

	/*Testing the base write/read functions*/
	void test_write_read_true_bool();
	void test_write_read_false_bool();
	void test_write_read_byte();
	void test_write_read_int16();
	void test_write_read_int32();
	void test_write_read_int64();
	void test_write_read_multiple();
	void test_write_read_multiple_reversed();
	void test_write_read_multiple_random();

	/*Testing the convenience write wrapper functions*/
	void test_write_read_helper_true_bool();
	void test_write_read_helper_false_bool();
	void test_write_read_helper_int8();
	void test_write_read_helper_uint8();
	void test_write_read_helper_int16();
	void test_write_read_helper_uint16();
	void test_write_read_helper_int32();
	void test_write_read_helper_uint32();
	void test_write_read_helper_int64();
	void test_write_read_helper_uint64();
	void test_write_read_helper_multiple();
	void test_write_read_helper_multiple_reversed();
	void test_write_read_helper_multiple_random();
	void test_write_read_lots();
};

#endif
