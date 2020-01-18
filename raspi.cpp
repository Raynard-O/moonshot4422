#include <raspicam/raspicam_cv.h>
#include <iostream>

int main ( int argc,char **argv ) {

    raspicam::RaspiCam_Cv Camera;
    Camera.set( CV_CAP_PROP_FORMAT, CV_8UC1 );

    cv::Mat image;

    if (!Camera.open()) {return -1;}

    Camera.set(CV_CAP_PROP_FRAME_WIDTH, 1600); // <-- If i remove this and the next line, the images are "normal"
    Camera.set(CV_CAP_PROP_FRAME_HEIGHT, 1200);

    Camera.grab();
    Camera.retrieve(image);

    Camera.release();

    //save image
    cv::imwrite("image.jpeg",image);
    std::cout<<"Image saved at image.jpeg"<<std::endl;
    return 0;
}