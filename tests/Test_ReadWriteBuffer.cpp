#include "Test_ReadWriteBuffer.h"

#include "ReadBuffer.h"
#include "WriteBuffer.h"
#include "Stream.h"

#include <stdint.h>

ReadWriteBufferTestSuite::ReadWriteBufferTestSuite()
{
	TEST_ADD(ReadWriteBufferTestSuite::TestReadWriteBool);
	TEST_ADD(ReadWriteBufferTestSuite::TestReadWriteUint32);
}

ReadWriteBufferTestSuite::~ReadWriteBufferTestSuite()
{
}

void ReadWriteBufferTestSuite::TestReadWriteBool()
{
	ReadBuffer rb;
	WriteBuffer wb;

	TEST_ASSERT(wb.SerialiseUint(false, bits_required(0, 1)));
	rb.fillFromBuffer(wb.bytes(), wb.totalBytes());
	uint64_t tmp;
	rb.SerialiseUint(tmp, bits_required(0, 1));
	TEST_ASSERT(((bool)tmp) == false);

}

void ReadWriteBufferTestSuite::TestReadWriteUint32()
{
	ReadBuffer rb;
	WriteBuffer wb;

	TEST_ASSERT(wb.SerialiseUint(0xFEDCBA98, bits_required(0, UINT32_MAX)));
	wb.flush();
	rb.fillFromBuffer(wb.bytes(), wb.totalBytes());
	uint64_t tmp;
	TEST_ASSERT(rb.SerialiseUint(tmp, bits_required(0, UINT32_MAX)));
	TEST_ASSERT(((uint32_t)tmp) == 0xFEDCBA98);
}
