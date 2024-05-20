#include <iostream>
#include <opencv2/opencv.hpp>

cv::Vec3b bilinear_interpolation(cv::Mat& image, int row, int col) {
    int col0 = col, row0 = row;
    int col1 = col0 + 1, row1 = row0 + 1;

    cv::Vec3b q00 = image.at<cv::Vec3b>(row0, col0);
    cv::Vec3b q10 = image.at<cv::Vec3b>(row0, col1);
    cv::Vec3b q01 = image.at<cv::Vec3b>(row1, col0);
    cv::Vec3b q11 = image.at<cv::Vec3b>(row1, col1);

    double wa = (col1 - col) / static_cast<double>(col1 - col0);
    double wb = (row1 - row) / static_cast<double>(row1 - row0);

    cv::Vec3b result;
    for (int i = 0; i < 3; i++) {
        result[i] = q00[i] * (1 - wa) * (1 - wb) + q10[i] * wa * (1 - wb) +
                    q01[i] * (1 - wa) * wb + q11[i] * wa * wb;
    }

    return result;
}

cv::Mat mipmap(cv::Mat& image) {
    int ori_width = image.cols;
    int ori_height = image.rows;
    int channels = image.channels();

    int fin_width = ori_width / 2;
    int fin_height = ori_height / 2;

    cv::Mat fin_image(fin_height, fin_width, CV_8UC3);

    for (int row = 0; row < fin_height; row++) {
        for (int col = 0; col < fin_width; col++) {
            cv::Vec3b interpolated = bilinear_interpolation(image, 2 * row, 2 * col);
            fin_image.at<cv::Vec3b>(row, col) = interpolated;
        }
    }

    return fin_image;
}

void show_save_img(cv::Mat& img) {
    cv::imshow("Image", img);
    cv::imwrite("./mipmap_imgs/" + std::to_string(img.size().width) + ".png", img);
    cv::waitKey(0);
}

int main() {
    cv::Mat img = cv::imread("./test.png");
    int pow = static_cast<int>(log2(1024));

    for (int i = 0; i <= pow; i++) {
        show_save_img(img);
        img = mipmap(img);
    }

    return 0;
}
