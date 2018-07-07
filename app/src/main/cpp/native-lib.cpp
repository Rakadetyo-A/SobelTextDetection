#include <jni.h>
#include <string>
#include <cmath>
#include <D:/Data/opencv-3.4.1-android-sdk/sdk/native/jni/include/opencv2/core.hpp>
#include <D:/Data/opencv-3.4.1-android-sdk/sdk/native/jni/include/opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

#pragma pack(push, 2)
struct RGB {        //members are in "bgr" order!
    uchar red;
    uchar green;
    uchar blue;
    uchar alfa;
};

extern "C"

JNIEXPORT jstring JNICALL
Java_com_example_rakad_skripsi_1c_1realtime_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_rakad_skripsi_1c_1realtime_RealTimeDetection_validate(JNIEnv *env, jobject, jlong frame){
    Rect();
    Mat();

    string hello = "C++ Loaded";
    return  env->NewStringUTF(hello.c_str());
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_rakad_skripsi_1c_1realtime_RealTimeDetection_getPixelValue(JNIEnv *env, jobject, jlong frame){
    Mat& Frame = *(Mat*)frame;

    uchar intensity = Frame.ptr<uchar>(5)[5];
    RGB& frm = Frame.ptr<RGB>(5)[5];
    int red = frm.red;
    int green = frm.green;
    int blue = frm.blue;
    int alfa = Frame.ptr<uchar> (5)[5];
    int channel = Frame.channels();

    string hello = "\n Red = " + to_string(red) + "\n" +
                   " Green = " + to_string(green) + "\n" +
                   " Blue = " + to_string(blue) + "\n" +
                   " Alfa = " + to_string(alfa) + "\n" +
                   " Channel = " + to_string(channel) + "\n" +
                   " Intensity = " + to_string((uchar)intensity);
    return  env->NewStringUTF(hello.c_str());
}


extern "C"
JNIEXPORT void JNICALL
Java_com_example_rakad_skripsi_1c_1realtime_RealTimeDetection_gscale(JNIEnv *env, jobject, jlong frame){
    Mat& Frame = *(Mat*)frame;
    Mat u16;

    int row = Frame.rows;
    int col = Frame.cols;

    //cvtColor(Frame, Frame, CV_RGBA2GRAY);
    GaussianBlur( Frame, Frame, Size( 7, 7), 0, 0 ); // Optional Noise Reduction

    for (int y = 0; y < row; y++) {
        for (int x = 0; x < col; x++) {
            //GET PIXEL VALUE
            RGB& frm = Frame.ptr<RGB>(y)[x];
            uchar gray = (frm.blue * 0.114) + (frm.green * 0.587) + (frm.red * 0.299); //Grayscale

            //REPLACE PIXEL VALUE
            frm.red = gray;
            frm.blue = gray;
            frm.green = gray;
        }
    }
    //cvtColor(Frame, Frame, CV_RGBA2GRAY);
}


extern "C"
JNIEXPORT void JNICALL
Java_com_example_rakad_skripsi_1c_1realtime_RealTimeDetection_sobel(JNIEnv *env, jobject, jlong frame){
    Mat& Frame = *(Mat*)frame;

    int row = Frame.rows;
    int col = Frame.cols;

    /*SOBEL OPERATOR MATRIX

        Gx =    [   -1   0   1  ]       Gy =    [    1    2    1   ]
                [   -2   0   2  ]               [    0    0    0   ]
                [   -1   0   1  ]               [   -1   -2   -1   ]
     */

    /*IMAGE MATRIX

      Image =   [   1   2   3   ]
                [   4   5   6   ]
                [   7   8   9   ]
     */

    int l = 0;
    for (int x = 0; x < row; x++) {
        for (int y = 0; y < col; y++) {
            int gradX, gradY, grad = 0;


            if (x == 0 || x == row-1 || y == 0 || y == col-1){

                gradX = gradY = 0;

            } else {

                RGB& px1 = Frame.ptr<RGB>(x-1)[y-1];   RGB& px2 = Frame.ptr<RGB>(x-1)[y];     RGB& px3 = Frame.ptr<RGB>(x-1)[y+1];
                RGB& px4 = Frame.ptr<RGB>(x)[y-1];                                            RGB& px6 = Frame.ptr<RGB>(x)[y+1];
                RGB& px7 = Frame.ptr<RGB>(x+1)[y-1];   RGB& px8 = Frame.ptr<RGB>(x+1)[y];     RGB& px9 = Frame.ptr<RGB>(x+1)[y+1];

                gradX = (px1.blue * (-1)) + (px4.blue * (-2)) + (px7.blue * (-1)) + (px3.blue) + (px6.blue * 2) + (px9.blue);

                gradY = (px7.blue * (-1)) + (px8.blue * (-2)) + (px9.blue * (-1)) + (px1.blue) + (px2.blue * 2) + (px3.blue);

            }

            grad = sqrt((gradX*gradX) + (gradY*gradY));
            //grad = grad / abs(gradX + gradY);
            uchar fnl = 0;
            if(grad > 255) fnl = 0;
            else if(grad < 0) fnl = 0;
            else fnl = grad;

            RGB& mid = Frame.ptr<RGB>(x)[y];
            mid.red = fnl;
            mid.green = fnl;
            mid.blue = fnl;
        }
    }
}

uchar getBlue(uchar color[], int idx){
    return color[4 * idx];
}
uchar getGreen(uchar color[], int idx){
    return color[(4 * idx) + 1];
}
uchar getRed(uchar color[], int idx){
    return color[(4 * idx) + 2];
}
uchar getColor(uchar color[], int idx){
    return getGreen(color, idx);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_example_rakad_skripsi_1c_1realtime_RealTimeDetection_sobelNew(JNIEnv *env, jobject, jlong frame){
    Mat& Frame = *(Mat*)frame;

    int row = Frame.rows;
    int col = Frame.cols;
    for (int x = 0; x < row; x++) {
        for (int y = 0; y < col; y++) {
            int gradX, gradY, grad = 0;

            if (x == 0 || x == row-1 || y == 0 || y == col-1){
                gradX = gradY = 255;

            } else {
                uchar px1 = getColor(Frame.ptr<uchar>(x-1), y-1);   uchar px2 = getColor(Frame.ptr<uchar>(x-1), y);     uchar px3 = getColor(Frame.ptr<uchar>(x-1), y+1);
                uchar px4 = getColor(Frame.ptr<uchar>(x), y-1);                                                        uchar px6 = getColor(Frame.ptr<uchar>(x), y+1);
                uchar px7 = getColor(Frame.ptr<uchar>(x+1), y-1);   uchar px8 = getColor(Frame.ptr<uchar>(x+1), y);     uchar px9 = getColor(Frame.ptr<uchar>(x+1), y+1);

                gradX = (px1 * -1) + (px4 * -2) + (px7 * -1) + px3 + (px6 * 2) + px9;
                gradY = (px7 * -1) + (px8 * -2) + (px9 * -1) + px1 + (px2 * 2) + px3;
            }

            grad = sqrt((gradX*gradX) + (gradY*gradY));
            uchar aa = 0;
            if(grad > 255) aa = 0;
            else if(grad < 0) aa = 255;
            else aa = grad;

            Frame.ptr<uchar>(x)[4*y] = aa;
            Frame.ptr<uchar>(x)[(4*y)+1] = aa;
            Frame.ptr<uchar>(x)[(4*y) + 2] = aa;
        }
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_rakad_skripsi_1c_1realtime_RealTimeDetection_sobelLib(JNIEnv *env, jobject, jlong frame){
    Mat& Frame = *(Mat*)frame;

    /// Generate grad_x and grad_y
    Mat grad_x, grad_y;
    Mat abs_grad_x, abs_grad_y;

    /// Gradient X
    Sobel( Frame, grad_x, CV_16S, 1, 0, 3, 1, 0, BORDER_DEFAULT );
    convertScaleAbs( grad_x, abs_grad_x );

    /// Gradient Y
    Sobel( Frame, grad_y, CV_16S, 0, 1, 3, 1, 0, BORDER_DEFAULT );
    convertScaleAbs( grad_y, abs_grad_y );

    /// Total Gradient (approximate)
    addWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5, 0, Frame );
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_rakad_skripsi_1c_1realtime_RealTimeDetection_kmeansLib(JNIEnv *env, jobject, jlong frame){
    Mat& Frame = *(Mat*)frame;

    const unsigned int singleLineSize = Frame.rows * Frame.cols;
    const unsigned int K = 2;

    Mat data = Frame.reshape(1, singleLineSize);
    data.convertTo(data, CV_32F);

    vector<int> labels;
    Mat1f colors;
    kmeans(data, K ,labels,TermCriteria(TermCriteria::EPS+TermCriteria::COUNT, 10, 1.),2,KMEANS_RANDOM_CENTERS,colors);

    for (unsigned int i = 0 ; i < singleLineSize ; i++ ){

        data.ptr<float>(i)[0] = colors(labels[i], 0);
        data.ptr<float>(i)[1] = colors(labels[i], 1);
        data.ptr<float>(i)[2] = colors(labels[i], 2);
    }

    Mat OutFrame = data.reshape(4, Frame.rows);
    OutFrame.convertTo(OutFrame, CV_8U);

    Frame = OutFrame;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_rakad_skripsi_1c_1realtime_RealTimeDetection_DetectionLib(JNIEnv *env, jobject, jlong Rgba, jlong Gray){
    Mat& mRgba = *(Mat*)Rgba;
    Mat& mGray = *(Mat*)Gray;


}

