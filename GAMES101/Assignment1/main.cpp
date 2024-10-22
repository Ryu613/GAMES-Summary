#include "Triangle.hpp"
#include "rasterizer.hpp"
#include <eigen3/Eigen/Eigen>
#include <iostream>
#include <opencv2/opencv.hpp>

constexpr double MY_PI = 3.1415926;

Eigen::Matrix4f get_view_matrix(Eigen::Vector3f eye_pos)
{
    Eigen::Matrix4f view = Eigen::Matrix4f::Identity();

    Eigen::Matrix4f translate;
    translate << 1, 0, 0, -eye_pos[0], 0, 1, 0, -eye_pos[1], 0, 0, 1,
        -eye_pos[2], 0, 0, 0, 1;

    view = translate * view;

    return view;
}

Eigen::Matrix4f get_model_matrix(float rotation_angle)
{
    Eigen::Matrix4f model = Eigen::Matrix4f::Identity();

    // Create the model matrix for rotating the triangle around the Z axis.
    // Then return it.
    Eigen::Matrix4f rotate;
    rotate << std::cos(rotation_angle / 180.0 * MY_PI), -std::sin(rotation_angle / 180.0 * MY_PI), 0, 0,
            std::sin(rotation_angle / 180.0 * MY_PI), std::cos(rotation_angle / 180.0 * MY_PI), 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1;
    model = rotate * model;
    return model;
}

/* zNear����ԭ������ĵ��z����(n)
   zFar: ��ԭ����Զ�ĵ��z����(f)
   eye_fov: ��ֱ���ӽǶ�
   aspect_ratio: �����
*/

Eigen::Matrix4f get_projection_matrix(float eye_fov, float aspect_ratio,
                                      float zNear, float zFar)
{

    Eigen::Matrix4f projection = Eigen::Matrix4f::Identity();

    // Create the projection matrix for the given parameters.
    // Then return it.

    // ����͸��ת������������ͶӰ��ƽ�ƣ���׼����
    // Զƽ��x,y����ѹ����
    Eigen::Matrix4f mPerspToOrtho;
    mPerspToOrtho << zNear, 0, 0, 0,
        0, zNear, 0, 0,
        0, 0, zNear + zFar, -zNear * zFar,
        0, 0, 1, 0;
    // ����ͶӰ��
    // 1: �������ƶ���ԭ��
    // 2: ��׼��
    // ����fov���t�����ݿ�߱����r������l,b
    double t = -std::tan(eye_fov / 2 / 180 * MY_PI) * zNear; // ��-z����,zNearĬ����һ����ֵ���������tĬ����һ������
    double b = -t;
    double r = t * aspect_ratio;
    double l = -r;
    Eigen::Matrix4f mOrthoTranslate;
    Eigen::Matrix4f mOrthoScale;
    //���ŷ�����ΪzNear, zFar�Ǹ���
    mOrthoTranslate << 1, 0, 0, -(r + l) / 2,
        0, 1, 0, -(t + b) / 2,
        0, 0, 1, -(zNear + zFar) / 2,
        0, 0, 0, 1;
    mOrthoScale << 2 / (r - l), 0, 0, 0,
        0, 2 / (t - b), 0, 0,
        0, 0, 2 / (zNear - zFar), 0,
        0, 0, 0, 1;

    projection = mOrthoScale * mOrthoTranslate * mPerspToOrtho * projection;

    return projection;
}

// ������������axis����תangle��
// ����Rodriguez��ת��ʽ
Eigen::Matrix4f get_rotation(Eigen::Vector3f axis, float angle)
{
    Eigen::Matrix3f rotationMatrix = Eigen::Matrix3f::Identity();
    // ���ù�ʽ��I��ʾ��λ����,Rn���任������N����
    Eigen::Matrix3f I = Eigen::Matrix3f::Identity();
    Eigen::Matrix3f Rn;
    Rn << 0, -axis[2], axis[1],
        axis[2], 0, -axis[0],
        -axis[1], axis[0], 0;
    double angleRad = angle / 180.0 * MY_PI;
    rotationMatrix = std::cos(angleRad) * I + (1 - std::cos(angleRad)) * axis * axis.transpose() + std::sin(angleRad) * Rn;
    // �任Ϊ4f����
    Eigen::Matrix4f rMatrix;
    rMatrix << rotationMatrix(0, 0), rotationMatrix(0, 1), rotationMatrix(0, 2), 0,
        rotationMatrix(1, 0), rotationMatrix(1, 1), rotationMatrix(1, 2), 0,
        rotationMatrix(2, 0), rotationMatrix(2, 1), rotationMatrix(2, 2), 0,
        0, 0, 0, 1;
    return rMatrix;
}

int main(int argc, const char** argv)
{
    //������������(Ĭ����z��)
    // ע�⣺Ŀǰ�Ƶ�����ӽǱ����ܿ���
    Eigen::Vector3f axis = { 0.0, 0.0, 1.0 };
    float angle = 0;
    bool command_line = false;
    std::string filename = "output.png";

    if (argc >= 3) {
        command_line = true;
        angle = std::stof(argv[2]); // -r by default
        if (argc == 4) {
            filename = std::string(argv[3]);
        }
        //fix: �����ļ���ʱ��Ӧ��ֱ���˳����򣬶���ʹ��Ĭ���ļ�������
        //else
        //    return 0;
    }

    rst::rasterizer r(700, 700);

    Eigen::Vector3f eye_pos = {0, 0, 5};

    std::vector<Eigen::Vector3f> pos{{2, 0, -2}, {0, 2, -2}, {-2, 0, -2}};

    std::vector<Eigen::Vector3i> ind{{0, 1, 2}};

    auto pos_id = r.load_positions(pos);
    auto ind_id = r.load_indices(ind);

    int key = 0;
    int frame_count = 0;

    if (command_line) {
        r.clear(rst::Buffers::Color | rst::Buffers::Depth);

        r.set_model(get_model_matrix(angle));
        r.set_view(get_view_matrix(eye_pos));
        r.set_projection(get_projection_matrix(45, 1, 0.1, 50));
        r.draw(pos_id, ind_id, rst::Primitive::Triangle);
        cv::Mat image(700, 700, CV_32FC3, r.frame_buffer().data());
        image.convertTo(image, CV_8UC3, 1.0f);

        cv::imwrite(filename, image);

        return 0;
    }

    while (key != 27) {
        r.clear(rst::Buffers::Color | rst::Buffers::Depth);

        //r.set_model(get_model_matrix(angle));
        r.set_model(get_rotation(axis, angle));
        r.set_view(get_view_matrix(eye_pos));
        r.set_projection(get_projection_matrix(45, 1, 0.1, 50));
        if (angle / 100.0 == -1) {
            std::cout << "break point";
        }
        r.draw(pos_id, ind_id, rst::Primitive::Triangle);

        cv::Mat image(700, 700, CV_32FC3, r.frame_buffer().data());
        image.convertTo(image, CV_8UC3, 1.0f);
        cv::imshow("image", image);
        key = cv::waitKey(10);

        std::cout << "frame count: " << frame_count++ << '\n';

        if (key == 'a') {
            angle += 10;
        }
        else if (key == 'd') {
            angle -= 10;
        }
        else if (key == 'x') {
            axis = {1.0, 0, 0};
        }
        else if (key == 'y') {
            axis = { 0, 1.0, 0 };
        }
        else if (key == 'z') {
            axis = { 0, 0, 1.0 };
        }
    }

    return 0;
}
