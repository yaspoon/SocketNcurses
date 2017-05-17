#include "Test_Frame.h"
#include "Frame.h"
#include "Network.h"
#include "WriteBuffer.h"
#include "ReadBuffer.h"

FrameTestSuite::FrameTestSuite()
{
	TEST_ADD(FrameTestSuite::test_frame_serialisation);
}

FrameTestSuite::~FrameTestSuite()
{
}

void FrameTestSuite::test_frame_serialisation()
{
	Frame frame1;
	Frame frame2;
	WriteBuffer wb;
	ReadBuffer rb;

	frame1.setType(Frame::DATA);
	frame1.setId(0xFEDCBA98);
	frame1.setNum(0x5);

	TEST_ASSERT(frame1.serialise(wb));
	wb.flush();
	rb.fillFromBuffer(wb.bytes(), wb.totalBytes());
	TEST_ASSERT(frame2.serialise(rb));

	TEST_ASSERT(frame1.getType() == frame2.getType());
	TEST_ASSERT(frame1.getId() == frame2.getId());
	TEST_ASSERT(frame1.getNum() == frame2.getNum());
}
