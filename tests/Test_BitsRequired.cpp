#include "Test_BitsRequired.h"
#include "Stream.h"
#include <stdint.h>

BitsRequiredTestSuite::BitsRequiredTestSuite()
{
	TEST_ADD(BitsRequiredTestSuite::test_bitsrequired_bool);
	TEST_ADD(BitsRequiredTestSuite::test_bitsrequired_uint8);
	TEST_ADD(BitsRequiredTestSuite::test_bitsrequired_uint16);
	TEST_ADD(BitsRequiredTestSuite::test_bitsrequired_uint32);
	TEST_ADD(BitsRequiredTestSuite::test_bitsrequired_uint64);
}

BitsRequiredTestSuite::~BitsRequiredTestSuite()
{
}

void BitsRequiredTestSuite::test_bitsrequired_bool()
{
	std::cout << "Testing bitsrequired for bool" << std::endl;
	TEST_ASSERT(bits_required(0, 1) == 1);
}

void BitsRequiredTestSuite::test_bitsrequired_uint8()
{
	std::cout << "Testing bitsrequired for uint8_t" << std::endl;
	TEST_ASSERT(bits_required(0, UINT8_MAX) == 8);
}

void BitsRequiredTestSuite::test_bitsrequired_uint16()
{
	std::cout << "Testing bitsrequired for uint16_t" << std::endl;
	TEST_ASSERT(bits_required(0, UINT16_MAX) == 16);
}

void BitsRequiredTestSuite::test_bitsrequired_uint32()
{
	std::cout << "Testing bitsrequired for uint32_t" << std::endl;
	TEST_ASSERT(bits_required(0, UINT32_MAX) == 32);
}

void BitsRequiredTestSuite::test_bitsrequired_uint64()
{
	std::cout << "Testing bitsrequired for uint64_t" << std::endl;
	TEST_ASSERT(bits_required(0, UINT64_MAX) == 64);
}
