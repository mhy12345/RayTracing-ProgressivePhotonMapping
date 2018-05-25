#include "gtest/gtest.h"
#include "core/sceneloader.h"
#include "core/vector.h"


TEST(VectorTest,BasicOperationTest)
{
	Vector a(1,2,2),b(3,4,4);
	EXPECT_TRUE(b-a == Vector(2,2,2));
}

TEST(VisitorTest, VisitVectorTest) 
{
	Vector b;
	SceneLoader a("config_test.txt");
	b.accept(&a);
	EXPECT_EQ(b.getX(),1);
	EXPECT_EQ(b.getY(),2);
	EXPECT_EQ(b.getZ(),3);
}
