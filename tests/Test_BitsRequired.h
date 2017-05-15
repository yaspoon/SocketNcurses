#ifndef TEST_BITSREQUIRED_H
#define TEST_BITSREQUIRED_H
#include <cpptest.h>

class BitsRequiredTestSuite: public Test::Suite
{
private:
	void test_bitsrequired_bool();
	void test_bitsrequired_uint8();
	void test_bitsrequired_uint16();
	void test_bitsrequired_uint32();
	void test_bitsrequired_uint64();
public:
	BitsRequiredTestSuite();
	~BitsRequiredTestSuite();
};

#endif
