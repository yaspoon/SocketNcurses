#include "Test_Buffer.h"

BufferTestSuite::BufferTestSuite()
{
	TEST_ADD(BufferTestSuite::test_write_read_true_bool);
	TEST_ADD(BufferTestSuite::test_write_read_false_bool);
	TEST_ADD(BufferTestSuite::test_write_read_byte);
	TEST_ADD(BufferTestSuite::test_write_read_short);
	TEST_ADD(BufferTestSuite::test_write_read_int32);
	TEST_ADD(BufferTestSuite::test_write_read_int64);
	TEST_ADD(BufferTestSuite::test_write_read_multiple);
	TEST_ADD(BufferTestSuite::test_write_read_multiple_reversed);
	TEST_ADD(BufferTestSuite::test_write_read_multiple_random);
}

BufferTestSuite::~BufferTestSuite()
{
}

void BufferTestSuite::test_write_read_true_bool()
{
	bool true_bool = true;

	std::cout << "Testing true bool write read" << std::endl;
	buf.write(true_bool, 1);
	buf.flush();
	TEST_ASSERT(((bool)buf.read(1)) == true_bool);
	buf.reset();

}

void BufferTestSuite::test_write_read_false_bool()
{
	std::cout << "Testing false bool write read" << std::endl;
	bool false_bool = false;
	buf.write(false_bool, 1);
	buf.flush();
	TEST_ASSERT(((bool)buf.read(1)) == false_bool);
	buf.reset();
}

void BufferTestSuite::test_write_read_byte()
{
	std::cout << "Testing byte write read" << std::endl;
	uint8_t byte_value = 0xAB;
	size_t noBitsInByte= sizeof(byte_value) * 8;
	buf.write(byte_value, noBitsInByte);
	buf.flush();
	TEST_ASSERT(((typeof(byte_value))buf.read(noBitsInByte)) == byte_value);
	buf.reset();
}

void BufferTestSuite::test_write_read_short()
{
	std::cout << "Testing short write read" << std::endl;
	uint16_t short_value = 0xABCD;
	size_t noBitsInShort = sizeof(short_value) * 8;
	buf.write(short_value, noBitsInShort);
	buf.flush();
	TEST_ASSERT(((typeof(short_value))buf.read(noBitsInShort)) == short_value);
	buf.reset();
}

void BufferTestSuite::test_write_read_int32()
{
	std::cout << "Testing int32 write read" << std::endl;
	uint32_t uint32 = 0xFEDCBA98;
	size_t noBitsInInt32 = sizeof(uint32) * 8;
	buf.write(uint32, noBitsInInt32);
	buf.flush();
	TEST_ASSERT(((typeof(uint32))buf.read(noBitsInInt32)) == uint32);
	buf.reset();
}

void BufferTestSuite::test_write_read_int64()
{
	std::cout << "Testing int64 write read" << std::endl;
	uint64_t uint64 = 0xFEDCBA9876543211;
	size_t noBitsInInt64 = sizeof(uint64) * 8;
	buf.write(uint64, noBitsInInt64);
	buf.flush();
	TEST_ASSERT(((typeof(uint64))buf.read(noBitsInInt64)) == uint64);
	buf.reset();
}

void BufferTestSuite::test_write_read_multiple()
{
	std::cout << "Testing multiple write/read" << std::endl;
	bool true_bool = true;
	bool false_bool = false;
	uint8_t byte_value = 0xAB;
	uint16_t short_value = 0xABCD;
	uint32_t uint32 = 0xFEDCBA98;
	uint64_t uint64 = 0xFEDCBA9876543211;
	size_t noBitsInBool = 1;
	size_t noBitsInByte = sizeof(byte_value) * 8;
	size_t noBitsInShort = sizeof(short_value) * 8;
	size_t noBitsInInt32 = sizeof(uint32) * 8;
	size_t noBitsInInt64 = sizeof(uint64) * 8;
	buf.write(true_bool, noBitsInBool);
	buf.write(false_bool, noBitsInBool);
	buf.write(byte_value, noBitsInByte);
	buf.write(short_value, noBitsInShort);
	buf.write(uint32, noBitsInInt32);
	buf.write(uint64, noBitsInInt64);
	buf.flush();
	TEST_ASSERT(((typeof(true_bool))buf.read(noBitsInBool)) == true_bool);
	TEST_ASSERT(((typeof(false_bool))buf.read(noBitsInBool)) == false_bool);
	TEST_ASSERT(((typeof(byte_value))buf.read(noBitsInByte)) == byte_value);
	TEST_ASSERT(((typeof(short_value))buf.read(noBitsInShort)) == short_value);
	TEST_ASSERT(((typeof(uint32))buf.read(noBitsInInt32)) == uint32);
	TEST_ASSERT(((typeof(uint64))buf.read(noBitsInInt64)) == uint64);
	buf.reset();
}

void BufferTestSuite::test_write_read_multiple_reversed()
{
	std::cout << "Testing multiple write/read reversed" << std::endl;
	bool true_bool = true;
	bool false_bool = false;
	uint8_t byte_value = 0xAB;
	uint16_t short_value = 0xABCD;
	uint32_t uint32 = 0xFEDCBA98;
	uint64_t uint64 = 0xFEDCBA9876543211;
	size_t noBitsInBool = 1;
	size_t noBitsInByte = sizeof(byte_value) * 8;
	size_t noBitsInShort = sizeof(short_value) * 8;
	size_t noBitsInInt32 = sizeof(uint32) * 8;
	size_t noBitsInInt64 = sizeof(uint64) * 8;
	buf.write(uint64, noBitsInInt64);
	buf.write(uint32, noBitsInInt32);
	buf.write(short_value, noBitsInShort);
	buf.write(byte_value, noBitsInByte);
	buf.write(false_bool, noBitsInBool);
	buf.write(true_bool, noBitsInBool);
	buf.flush();
	TEST_ASSERT(((typeof(uint64))buf.read(noBitsInInt64)) == uint64);
	TEST_ASSERT(((typeof(uint32))buf.read(noBitsInInt32)) == uint32);
	TEST_ASSERT(((typeof(short_value))buf.read(noBitsInShort)) == short_value);
	TEST_ASSERT(((typeof(byte_value))buf.read(noBitsInByte)) == byte_value);
	TEST_ASSERT(((typeof(false_bool))buf.read(noBitsInBool)) == false_bool);
	TEST_ASSERT(((typeof(true_bool))buf.read(noBitsInBool)) == true_bool);
	buf.reset();
}

/*By random I mean I generated some random numbers and then selected
 * what to write based on that number. 0 being true_bool 1 being false_bool
 * etc.
 */
void BufferTestSuite::test_write_read_multiple_random()
{
	std::cout << "Testing multiple write/read reversed" << std::endl;
	bool true_bool = true;
	bool false_bool = false;
	uint8_t byte_value = 0xAB;
	uint16_t short_value = 0xABCD;
	uint32_t uint32 = 0xFEDCBA98;
	uint64_t uint64 = 0xFEDCBA9876543211;
	size_t noBitsInBool = 1;
	size_t noBitsInByte = sizeof(byte_value) * 8;
	size_t noBitsInShort = sizeof(short_value) * 8;
	size_t noBitsInInt32 = sizeof(uint32) * 8;
	size_t noBitsInInt64 = sizeof(uint64) * 8;
	buf.write(uint32, noBitsInInt32);
	buf.write(byte_value, noBitsInByte);
	buf.write(short_value, noBitsInShort);
	buf.write(false_bool, noBitsInBool);
	buf.write(uint64, noBitsInInt64);
	buf.write(true_bool, noBitsInBool);

	buf.write(short_value, noBitsInShort);
	buf.write(byte_value, noBitsInByte);
	buf.write(true_bool, noBitsInBool);
	buf.write(short_value, noBitsInShort);
	buf.write(uint64, noBitsInInt64);
	buf.flush();
	TEST_ASSERT(((typeof(uint32))buf.read(noBitsInInt32)) == uint32);
	TEST_ASSERT(((typeof(byte_value))buf.read(noBitsInByte)) == byte_value);
	TEST_ASSERT(((typeof(short_value))buf.read(noBitsInShort)) == short_value);
	TEST_ASSERT(((typeof(false_bool))buf.read(noBitsInBool)) == false_bool);
	TEST_ASSERT(((typeof(uint64))buf.read(noBitsInInt64)) == uint64);
	TEST_ASSERT(((typeof(true_bool))buf.read(noBitsInBool)) == true_bool);

	TEST_ASSERT(((typeof(short_value))buf.read(noBitsInShort)) == short_value);
	TEST_ASSERT(((typeof(byte_value))buf.read(noBitsInByte)) == byte_value);
	TEST_ASSERT(((typeof(true_bool))buf.read(noBitsInBool)) == true_bool);
	TEST_ASSERT(((typeof(short_value))buf.read(noBitsInShort)) == short_value);
	TEST_ASSERT(((typeof(uint64))buf.read(noBitsInInt64)) == uint64);
	buf.reset();
}
