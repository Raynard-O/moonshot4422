
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
char *test = "../testb.jpeg";

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
    cout<<"\nImage saved at liveImage.jpeg"<<endl;
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
    //system("../build2/msPicture");

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

            int f1 = ms_imagequality("./image_01.wsq");
            imageQtest = (f1 < 3);
            //check image quality
            if (f1 < 3)
            {cout<<"Image saved at image.jpeg"<<f1<<endl;
            }
        }else if (MODE == "THUMB") {
            int f1, f2;
            f1 = ms_imagequality("./image_11.wsq");
            f2 = ms_imagequality("./image_12.wsq");


            imageQtest = (f1 < 3 &&
                          f2 < 3);
            //check image quality
            if (f1 >= 3)
            {std::cerr <<"image_11 : error:"<<f1<<endl;
            }if (f2 >= 3) {
                std::cerr << "image_12 : error:" <<f2<< endl;
            }
        }else if (MODE == "LEFTHAND") {


            int f1,f2,f3,f4;

            f1 = ms_imagequality("./image_07.wsq");
            f2 = ms_imagequality("./image_08.wsq");
            f3 = ms_imagequality("./image_09.wsq");
            f4 = ms_imagequality("./image_10.wsq");


            imageQtest = (f1 < 3&&
                          f2 < 3&&
                          f3< 3&&
                          f4< 3);
            //check image quality
            if (f1 >= 3)
            {std::cerr <<"image_02 : error:"<<f1<<endl;
            }if (f2 >= 3){
                std::cerr <<"image_03 : error:"<<f2<<endl;
            }if (f3>= 3){
                std::cerr <<"image_04 : error:"<<f3<<endl;
            }if (f4 >= 3){
                std::cerr <<"image_05 : error:"<<f4<<endl;}

        } if (MODE == "RIGHTHAND") {

            int f1,f2,f3,f4;

            f1 = ms_imagequality("./image_02.wsq");
            f2 = ms_imagequality("./image_03.wsq");
            f3 = ms_imagequality("./image_04.wsq");
            f4 = ms_imagequality("./image_05.wsq");
            std::cerr <<"imagescode: "<<f4<<f2<<f3<<f1<<endl;
            imageQtest = (f1 < 3&&
                          f2 < 3&&
                          f3 < 3&&
                          f4 < 3 );
            //check image quality
            if (f1 >= 3)
            {std::cerr <<"image_02 : error: " <<f1<<endl;
            }if (f2>= 3){
                std::cerr <<"image_03 : error: "<<f2<<endl;
            }if (f3 >= 3){
                std::cerr <<"image_04 : error: "<<f3<<endl;
            }if (f4 >= 3) {
                std::cerr <<"image_05 : error: "<<f4<<endl;
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
