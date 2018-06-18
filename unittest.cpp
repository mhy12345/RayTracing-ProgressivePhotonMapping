#include "gtest/gtest.h"
#include "core/sceneloader.h"
#include "core/vector.h"
#include "core/light.h"


TEST(VectorTest,BasicOperation)
{
	Vector a(1,2,2),b(3,4,4);
	EXPECT_TRUE(b-a == Vector(2,2,2));
}

TEST(SceneLoaderTest, Vector) 
{
	Vector b;
	SceneLoader a("configures/config_test_vector.txt");
	b.accept(a);
	EXPECT_EQ(b.getX(),1);
	EXPECT_EQ(b.getY(),2);
	EXPECT_EQ(b.getZ(),3);
}

TEST(SceneLoaderTest, Light) 
{
	Light l;
	SceneLoader a("configures/config_test_light.txt");
	l.accept(a);
	EXPECT_EQ(l.getPosition().getX(),1);
}
