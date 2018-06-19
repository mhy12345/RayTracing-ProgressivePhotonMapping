#include "gtest/gtest.h"
#include "core/vector.h"
#include "core/light.h"
#include "json/json.h"
#include <iostream>
#include <fstream>


TEST(VectorTest,BasicOperation)
{
	Vector a(1,2,2),b(3,4,4);
	EXPECT_TRUE(b-a == Vector(2,2,2));
}

TEST(SceneLoaderTest, Vector) 
{
	Vector b;
	std::ifstream ifs("configures/config_test_vector.json");
	Json::CharReaderBuilder reader;
	Json::Value root;  
	JSONCPP_STRING errs;
	Json::parseFromStream(reader, ifs, &root, &errs);
	b.accept(root);
	EXPECT_EQ(b.getX(),1);
	EXPECT_EQ(b.getY(),2);
	EXPECT_EQ(b.getZ(),3);
}

TEST(SceneLoaderTest, Light) 
{
	Light *l = new AreaLight();
	std::ifstream ifs("configures/config_test_light.json");
	Json::CharReaderBuilder reader;
	Json::Value root;  
	JSONCPP_STRING errs;
	Json::parseFromStream(reader, ifs, &root, &errs);
	l->accept(root);
	EXPECT_EQ(l->getColor().getR(),1);
}

TEST(CameraTest, ShowAPicture)
{
}
