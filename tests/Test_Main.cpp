#include <cpptest.h>
#include "Test_Buffer.h"

int main(int argc, char *argv[])
{
	Test::TextOutput output(Test::TextOutput::Verbose);
	BufferTestSuite bts;
	return bts.run(output) ? EXIT_SUCCESS : EXIT_FAILURE;

}
