#include <cpptest.h>
#include "Test_Buffer.h"
#include "Test_BitsRequired.h"
#include "Test_Frame.h"
#include "Test_ReadWriteBuffer.h"

int main(int argc, char *argv[])
{
	Test::TextOutput output(Test::TextOutput::Verbose);
	BufferTestSuite bts;
	BitsRequiredTestSuite brts;
	FrameTestSuite fts;
	ReadWriteBufferTestSuite rwbts;
	return (bts.run(output) && brts.run(output) && rwbts.run(output) && fts.run(output)) ? EXIT_SUCCESS : EXIT_FAILURE;
	

}
