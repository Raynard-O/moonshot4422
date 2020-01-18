
#include <iostream>
#include <raspicam/raspicam.h>
#include <raspicam/raspicam_cv.h>
#include <opencv2/opencv.hpp>
#include <wiringPi.h>
#include "ms_func.h"
#include "thread"
#include <atomic>
#include <chrono>
#include <unistd.h>
int debug = 0;


int fgp;
int imageQtest;
char* test = "./image.jpeg";

using namespace std;


void liveStreamImage () {
    raspicam::RaspiCam Camera; //Cmaera object
    //Open camera
//    if ( !Camera.open()) {
//        cerr<<"Error opening camera"<<endl;
//        exit(1);
//    }
    //capture
    Camera.grab();
    //allocate memory
    unsigned char *data=new unsigned char[  Camera.getImageTypeSize ( raspicam::RASPICAM_FORMAT_RGB )];
    //extract the image in rgb format
    Camera.retrieve ( data,raspicam::RASPICAM_FORMAT_RGB );//get camera image
    //save
    std::ofstream outFile ( "liveImage.jpeg",std::ios::binary );
    outFile<<"P6\n"<<Camera.getWidth() <<" "<<Camera.getHeight() <<" 255\n";
    outFile.write ( ( char* ) data, Camera.getImageTypeSize ( raspicam::RASPICAM_FORMAT_RGB ) );
    cout<<"Image saved at liveImage.jpeg"<<endl;
    //free resrources
    delete data;
}

void livestream(std::atomic<bool >& imageTest){
    while(imageTest) {
        //system("../build2/msPicture");
        liveStreamImage();
    }
}

int main() {
    char *MODE;
    wiringPiSetup(); //Initialize wiringPi
    ms_ledInit();
    ms_led_on();
    system("../build2/msPicture");

    std::atomic<bool> running {true};


    std::thread liveFeedThread(livestream, ref(running));





    MODE = "RIGHTHAND";

    if (MODE == "RIGHTHAND"){
        fgp = 13;
        printf("%s\n",MODE);

    } else if(MODE == "LEFTHAND"){
        fgp = 10;
        printf("%s\n",MODE);
    } else if(MODE == "THUMB"){
        fgp = 15;
        printf("%s\n",MODE);
    }else if(MODE == "SINGLEFINGER"){
        fgp = 1;
        printf("%s\n",MODE);
    }else{
        printf("error set correct MODE");
    }
    //ms_detect_finger(fgp, "../testb.jpeg");
    do {
        system("../build2/msPicture");
       // sleep(5);
        //pick an image

        //segment image
        ms_detect_finger(fgp, test);


        if (MODE == "SINGLEFINGER") {

            imageQtest = ((ms_imagequality("./image_01.wsq") < 3));
            //check image quality
            if ((ms_imagequality("./image_01.wsq") < 3))
            {cout<<"Image saved at image.jpeg"<<endl;
            }
        }else if (MODE == "THUMB") {

            imageQtest = ((ms_imagequality("./image_11.wsq") < 3) &&
                          (ms_imagequality("./image_12.wsq") < 3));
            //check image quality
            if (!(ms_imagequality("./image_02.wsq") < 3))
            {std::cerr <<"image_11 : error"<<endl;
            }if (!(ms_imagequality("./image_02.wsq") < 3)) {
                std::cerr << "image_12 : error" << endl;
            }
        }else if (MODE == "LEFTHAND") {

            imageQtest = ((ms_imagequality("./image_07.wsq") < 3) &&
                          (ms_imagequality("./image_08.wsq") < 3)
                          && (ms_imagequality("./image_09.wsq") < 3) &&
                          (ms_imagequality("./image_10.wsq") < 3));
            //check image quality
            if (!(ms_imagequality("./image_07.wsq") < 3))
            {std::cerr <<"image_02 : error"<<endl;
            }if (!(ms_imagequality("./image_08.wsq") < 3)){
                std::cerr <<"image_03 : error"<<endl;
            }if (!(ms_imagequality("./image_09.wsq") < 3)){
                std::cerr <<"image_04 : error"<<endl;
            }if (!(ms_imagequality("./image_10.wsq") < 3)){
                std::cerr <<"image_05 : error"<<endl;}

        } if (MODE == "RIGHTHAND") {

            imageQtest = ((ms_imagequality("./image_02.wsq") < 3) &&
                          (ms_imagequality("./image_03.wsq") < 3)
                          && (ms_imagequality("./image_04.wsq") < 3) &&
                          (ms_imagequality("./image_05.wsq") < 3));
            //check image quality
            if (!(ms_imagequality("./image_02.wsq") < 3))
            {std::cerr <<"image_02 : error"<<endl;
            }if (!(ms_imagequality("./image_02.wsq") < 3)){
                std::cerr <<"image_03 : error"<<endl;
            }if (!(ms_imagequality("./image_02.wsq") < 3)){
                std::cerr <<"image_04 : error"<<endl;
            }if (!(ms_imagequality("./image_02.wsq") < 3)) {
                std::cerr <<"image_05 : error\n" ;
            }
        }


//run nfiq to confirm image quality

        if (imageQtest){

            printf("Good Image\n");
            running = false;
            // end thread

        } else {
            printf("\n\n\nBad image\n");
        }

    }while (!imageQtest);



    liveFeedThread.join();

    return 0;


}
