#include <cpptest.h>

class FrameTestSuite: public Test::Suite
{
private:
public:
	FrameTestSuite();
	~FrameTestSuite();
	void test_frame_serialisation();
};
