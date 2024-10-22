#include<cmath>
#include<eigen3/Eigen/Core>
#include<eigen3/Eigen/Dense>
#include<iostream>

const double pi = std::acos(-1);

// inverse rotate spot for degrees
Eigen::Vector3f inverseRotate(Eigen::Vector3f* spot, double* degree) {
    Eigen::Matrix3f rotateMatrix;
    rotateMatrix << std::cos(*degree), -std::sin(*degree), 0,
                    std::sin(*degree), std::cos(*degree), 0,
                    0, 0, 1;
    return rotateMatrix * *spot;
}

// move spot in vector
Eigen::Vector3f translation(Eigen::Vector3f* spot, Eigen::Vector3f* trVector) {
    Eigen::Matrix3f moveMatrix;
    moveMatrix << 1, 0, (*trVector)[0],
                  0, 1, (*trVector)[1],
                  0, 0, 1;
    return moveMatrix * *spot;
}

int main(){
    /* examples:
    // Basic Example of cpp
    std::cout << "Example of cpp \n";
    float a = 1.0, b = 2.0;
    // added explanation of result
    std::cout << "output float number a: \n";
    std::cout << a << std::endl;
    std::cout << "output a divide by b:  \n";
    std::cout << a/b << std::endl;
    std::cout << "output square root of b: \n";
    std::cout << std::sqrt(b) << std::endl;
    std::cout << "output result of arccos(-1): \n";
    std::cout << std::acos(-1) << std::endl;
    std::cout << "output result of sine 30 degrees: \n";
    std::cout << std::sin(30.0/180.0*acos(-1)) << std::endl;

    // Example of vector
    std::cout << "Example of vector \n";
    // vector definition
    Eigen::Vector3f v(1.0f,2.0f,3.0f);
    Eigen::Vector3f w(1.0f,0.0f,0.0f);
    // vector output
    std::cout << "Example of output \n";
    std::cout << v << std::endl;
    // vector add
    std::cout << "Example of add \n";
    std::cout << v + w << std::endl;
    // vector scalar multiply
    std::cout << "Example of scalar multiply \n";
    std::cout << v * 3.0f << std::endl;
    std::cout << 2.0f * v << std::endl;
    // vector dot product
    std::cout << "Example of dot product \n";
    std::cout << v.dot(w) >> std:;endl;

    // Example of matrix
    std::cout << "Example of matrix \n";
    // matrix definition
    Eigen::Matrix3f i,j;
    i << 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0;
    j << 2.0, 3.0, 1.0, 4.0, 6.0, 5.0, 9.0, 7.0, 8.0;
    // matrix output
    std::cout << "Example of output \n";
    std::cout << i << std::endl;
    // matrix add i + j
    std::cout << "matrix add i + j = ";
    std::cout << i + j << std::endl;
    std::cout << "matrix minus i - j = ";
    std::cout << i - j << std::endl;
    // matrix scalar multiply i * 2.0
    std::cout << "matrix scalar multiply i * 2.0 = ";
    std::cout << i * 2.0f << std::endl;
    // matrix multiply i * j
    std::cout << "matrix multiply i * j = ";
    std::cout << i * j << std::endl;
    // matrix multiply vector i * v
    std::cout << "matrix multiply vector i * v = ";
    std::cout << i * v << std::endl;
    */
    // spot p
    Eigen::Vector3f p(2.0f, 1.0f, 1.0f);
    // theta angle
    double angleTheta = pi / 4;
    // rotate
    Eigen::Vector3f rotatedSpot = inverseRotate(&p, &angleTheta);
    // move vector
    Eigen::Vector3f moveVector(1.0f, 2.0f, 0.0f);
    Eigen::Vector3f movedSpot = translation(&rotatedSpot, &moveVector);
    // output result
    std::cout << "the result of spot P is: P=(" << movedSpot[0] << "," << movedSpot[1] << ")" << std::endl;
    return 0;
}