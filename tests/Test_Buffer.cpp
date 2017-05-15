#include "Test_Buffer.h"

BufferTestSuite::BufferTestSuite()
{
	TEST_ADD(BufferTestSuite::test_write_read_true_bool);
	TEST_ADD(BufferTestSuite::test_write_read_false_bool);
	TEST_ADD(BufferTestSuite::test_write_read_byte);
	TEST_ADD(BufferTestSuite::test_write_read_int16);
	TEST_ADD(BufferTestSuite::test_write_read_int32);
	TEST_ADD(BufferTestSuite::test_write_read_int64);
	TEST_ADD(BufferTestSuite::test_write_read_multiple);
	TEST_ADD(BufferTestSuite::test_write_read_multiple_reversed);
	TEST_ADD(BufferTestSuite::test_write_read_multiple_random);

	TEST_ADD(BufferTestSuite::test_write_read_helper_true_bool);
	TEST_ADD(BufferTestSuite::test_write_read_helper_false_bool);
	TEST_ADD(BufferTestSuite::test_write_read_helper_int8);
	TEST_ADD(BufferTestSuite::test_write_read_helper_uint8);
	TEST_ADD(BufferTestSuite::test_write_read_helper_int16);
	TEST_ADD(BufferTestSuite::test_write_read_helper_uint16);
	TEST_ADD(BufferTestSuite::test_write_read_helper_int32);
	TEST_ADD(BufferTestSuite::test_write_read_helper_uint32);
	TEST_ADD(BufferTestSuite::test_write_read_helper_int64);
	TEST_ADD(BufferTestSuite::test_write_read_helper_uint64);
	TEST_ADD(BufferTestSuite::test_write_read_helper_multiple);
	TEST_ADD(BufferTestSuite::test_write_read_helper_multiple_reversed);
	TEST_ADD(BufferTestSuite::test_write_read_helper_multiple_random);
}

BufferTestSuite::~BufferTestSuite()
{
}

void BufferTestSuite::test_write_read_true_bool()
{
	std::cout << "Testing true bool write read" << std::endl;
	TEST_ASSERT(buf.totalBytes() == 0);
	TEST_ASSERT(buf.bytesLeft() == 0);
	buf.write(true_bool, 1);
	TEST_ASSERT(buf.totalBytes() == 1);
	TEST_ASSERT(buf.bytesLeft() == 1);
	buf.flush();
	TEST_ASSERT(buf.totalBytes() == 1);
	TEST_ASSERT(buf.bytesLeft() == 1);
	TEST_ASSERT(((bool)buf.read(1)) == true_bool);
	TEST_ASSERT(buf.totalBytes() == 1);
	TEST_ASSERT(buf.bytesLeft() == 0);
	buf.reset();
	TEST_ASSERT(buf.totalBytes() == 0);
	TEST_ASSERT(buf.bytesLeft() == 0);

}

void BufferTestSuite::test_write_read_false_bool()
{
	std::cout << "Testing false bool write read" << std::endl;
	buf.write(false_bool, 1);
	buf.flush();
	TEST_ASSERT(((bool)buf.read(1)) == false_bool);
	buf.reset();
}

void BufferTestSuite::test_write_read_byte()
{
	std::cout << "Testing byte write read" << std::endl;
	buf.write(byte_value, noBitsInByte);
	buf.flush();
	TEST_ASSERT(((typeof(byte_value))buf.read(noBitsInByte)) == byte_value);
	buf.reset();
}

void BufferTestSuite::test_write_read_int16()
{
	std::cout << "Testing short write read" << std::endl;
	buf.write(short_value, noBitsInShort);
	buf.flush();
	TEST_ASSERT(((typeof(short_value))buf.read(noBitsInShort)) == short_value);
	buf.reset();
}

void BufferTestSuite::test_write_read_int32()
{
	std::cout << "Testing int32 write read" << std::endl;
	buf.write(uint32, noBitsInInt32);
	buf.flush();
	TEST_ASSERT(((typeof(uint32))buf.read(noBitsInInt32)) == uint32);
	buf.reset();
}

void BufferTestSuite::test_write_read_int64()
{
	std::cout << "Testing int64 write read" << std::endl;
	buf.write(uint64, noBitsInInt64);
	buf.flush();
	TEST_ASSERT(((typeof(uint64))buf.read(noBitsInInt64)) == uint64);
	buf.reset();
}

void BufferTestSuite::test_write_read_multiple()
{
	std::cout << "Testing multiple write/read" << std::endl;
	buf.write(true_bool, noBitsInBool);
	buf.write(false_bool, noBitsInBool);
	buf.write(byte_value, noBitsInByte);
	buf.write(short_value, noBitsInShort);
	buf.write(uint32, noBitsInInt32);
	buf.write(uint64, noBitsInInt64);
	buf.write(byte_value, noBitsInByte);
	buf.flush();
	TEST_ASSERT(((typeof(true_bool))buf.read(noBitsInBool)) == true_bool);
	TEST_ASSERT(((typeof(false_bool))buf.read(noBitsInBool)) == false_bool);
	TEST_ASSERT(((typeof(byte_value))buf.read(noBitsInByte)) == byte_value);
	TEST_ASSERT(((typeof(short_value))buf.read(noBitsInShort)) == short_value);
	TEST_ASSERT(((typeof(uint32))buf.read(noBitsInInt32)) == uint32);
	TEST_ASSERT(((typeof(uint64))buf.read(noBitsInInt64)) == uint64);
	TEST_ASSERT(((typeof(byte_value))buf.read(noBitsInByte)) == byte_value);
	buf.reset();
}

void BufferTestSuite::test_write_read_multiple_reversed()
{
	std::cout << "Testing multiple write/read reversed" << std::endl;
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
	std::cout << "Testing multiple write/read random" << std::endl;
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

void BufferTestSuite::test_write_read_helper_true_bool()
{
	std::cout << "Testing write/read true bool helper" << std::endl;
	buf.writeBool(true_bool);
	buf.flush();
	TEST_ASSERT(buf.readBool() == true_bool);
	buf.reset();
}

void BufferTestSuite::test_write_read_helper_false_bool()
{
	std::cout << "Testing write/read false bool helper" << std::endl;
	buf.writeBool(false_bool);
	buf.flush();
	TEST_ASSERT(buf.readBool() == false_bool);
	buf.reset();
}

void BufferTestSuite::test_write_read_helper_int8()
{
	std::cout << "Testing int8 write/read helper" << std::endl;
	buf.writeInt8(signed_byte_value);
	buf.flush();
	TEST_ASSERT(buf.readInt8() == signed_byte_value);
	buf.reset();
}

void BufferTestSuite::test_write_read_helper_uint8()
{
	std::cout << "Testing uint8 write/read helper" << std::endl;
	buf.writeUint8(byte_value);
	buf.flush();
	TEST_ASSERT(buf.readUint8() == byte_value);
	buf.reset();
}

void BufferTestSuite::test_write_read_helper_int16()
{
	std::cout << "Testing int16 write/read helper" << std::endl;
	buf.writeInt16(signed_short_value);
	buf.flush();
	TEST_ASSERT(buf.readInt16() == signed_short_value);
	buf.reset();
}

void BufferTestSuite::test_write_read_helper_uint16()
{
	std::cout << "Testing uint16 write/read helper" << std::endl;
	buf.writeUint16(short_value);
	buf.flush();
	TEST_ASSERT(buf.readUint16() == short_value);
	buf.reset();
}

void BufferTestSuite::test_write_read_helper_int32()
{
	std::cout << "Testing int32 write/read helper" << std::endl;
	buf.writeInt32(int32);
	buf.flush();
	TEST_ASSERT(buf.readInt32() == int32);
	buf.reset();
}

void BufferTestSuite::test_write_read_helper_uint32()
{
	std::cout << "Testing uint32 write/read helper" << std::endl;
	buf.writeUint32(uint32);
	buf.flush();
	TEST_ASSERT(buf.readUint32() == uint32);
	buf.reset();
}

void BufferTestSuite::test_write_read_helper_int64()
{
	std::cout << "Testing int64 write/read helper" << std::endl;
	buf.writeInt64(int64);
	buf.flush();
	TEST_ASSERT(buf.readInt64() == int64);
	buf.reset();
}

void BufferTestSuite::test_write_read_helper_uint64()
{
	std::cout << "Testing uint64 write/read helper" << std::endl;
	buf.writeUint64(uint64);
	buf.flush();
	TEST_ASSERT(buf.readUint64() == uint64);
	buf.reset();
}

void BufferTestSuite::test_write_read_helper_multiple()
{
	std::cout << "Testing multiple write/read helper" << std::endl;
	buf.writeBool(true_bool);
	buf.writeBool(false_bool);
	buf.writeUint8(byte_value);
	buf.writeUint16(short_value);
	buf.writeUint32(uint32);
	buf.writeUint64(uint64);
	buf.flush();
	TEST_ASSERT(buf.readBool() == true_bool);
	TEST_ASSERT(buf.readBool() == false_bool);
	TEST_ASSERT(buf.readUint8() == byte_value);
	TEST_ASSERT(buf.readUint16() == short_value);
	TEST_ASSERT(buf.readUint32() == uint32);
	TEST_ASSERT(buf.readUint64() == uint64);
	buf.reset();
}

void BufferTestSuite::test_write_read_helper_multiple_reversed()
{
	std::cout << "Testing multiple write/read reversed helper" << std::endl;
	buf.writeUint64(uint64);
	buf.writeUint32(uint32);
	buf.writeUint16(short_value);
	buf.writeUint8(byte_value);
	buf.writeBool(false_bool);
	buf.writeBool(true_bool);
	buf.flush();
	TEST_ASSERT(buf.readUint64() == uint64);
	TEST_ASSERT(buf.readUint32() == uint32);
	TEST_ASSERT(buf.readUint16() == short_value);
	TEST_ASSERT(buf.readUint8() == byte_value);
	TEST_ASSERT(buf.readBool() == false_bool);
	TEST_ASSERT(buf.readBool() == true_bool);
	buf.reset();
}

void BufferTestSuite::test_write_read_helper_multiple_random()
{
	std::cout << "Testing multiple write/read random helper" << std::endl;
	buf.writeUint32(uint32);
	buf.writeUint8(byte_value);
	buf.writeUint16(short_value);
	buf.writeBool(false_bool);
	buf.writeUint64(uint64);
	buf.writeBool(true_bool);

	buf.writeUint16(short_value);
	buf.writeUint8(byte_value);
	buf.writeBool(true_bool);
	buf.writeUint16(short_value);
	buf.writeUint64(uint64);
	buf.flush();
	TEST_ASSERT(buf.readUint32() == uint32);
	TEST_ASSERT(buf.readUint8() == byte_value);
	TEST_ASSERT(buf.readUint16() == short_value);
	TEST_ASSERT(buf.readBool() == false_bool);
	TEST_ASSERT(buf.readUint64() == uint64);
	TEST_ASSERT(buf.readBool() == true_bool);

	TEST_ASSERT(buf.readUint16() == short_value);
	TEST_ASSERT(buf.readUint8() == byte_value);
	TEST_ASSERT(buf.readBool() == true_bool);
	TEST_ASSERT(buf.readUint16() == short_value);
	TEST_ASSERT(buf.readUint64() == uint64);
	buf.reset();
}
