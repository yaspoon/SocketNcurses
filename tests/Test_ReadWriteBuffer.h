#ifndef TEST_READWRITE_BUFFER_H
#define TEST_READWRITE_BUFFER_H
#include <cpptest.h>

class ReadWriteBufferTestSuite: public Test::Suite
{
private:
public:
	ReadWriteBufferTestSuite();
	~ReadWriteBufferTestSuite();

	void TestReadWriteBool();
	void TestReadWriteUint32();
};
#endif
