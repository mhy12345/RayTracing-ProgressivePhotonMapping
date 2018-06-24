#include <iostream>
#include <Eigen/Dense>
using namespace Eigen;
using namespace std;

int main() {
	Vector3d Z(1,1,1);
    AngleAxisd t_V(M_PI / 4, Vector3d(0, 0, 1));
    Matrix3d t_R = t_V.matrix();
    Quaterniond t_Q(t_V);
    AngleAxisd V1(M_PI / 4, Vector3d(0, 0, 1));//以（0,0,1）为旋转轴，旋转45度
    cout << "Rotation_vector1" << endl << V1.matrix() << endl;

}
