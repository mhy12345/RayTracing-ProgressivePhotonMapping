#include "gtest/gtest.h"
#include "Core/Vector.h"


TEST(VectorTest,BasicOperationTest)
{
	Vector a(1,2,2),b(3,4,4);
	EXPECT_TRUE(b-a == Vector(2,2,2));
}

