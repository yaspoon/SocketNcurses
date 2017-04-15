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
	void test_write_read_true_bool();
	void test_write_read_false_bool();
	void test_write_read_byte();
	void test_write_read_short();
	void test_write_read_int32();
	void test_write_read_int64();
	void test_write_read_multiple();
	void test_write_read_multiple_reversed();
	void test_write_read_multiple_random();
};

#endif
