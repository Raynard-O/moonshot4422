/********************************************************************************
 * Created by MOONSHOT TEAM ON 8/01/2020
 * PROJECT:
 *      Four-Finger-Print Scanner Device
 * CONTENT:
 *      Functions used for camera and LED control
 * FUNCTIONS:
 *      ms_led_on()
 *      ms_led_off()
 *      ms_ledInit()
 *      ms_camInit()
 *      ms_cam_close()
 *      ms_take_pic()
 *      ms_process_img()
 * private function:
 *      ms_resCheck()
 *  
***********************************************************************************/

#include <iostream>
#include "ms_func.h"
#include <wiringPi.h>
#include <raspicam/raspicam_cv.h>
#include <opencv2/opencv.hpp>

/* Declare Variables */
raspicam::RaspiCam_Cv Camera;
cv::Mat image;


/****************************************************************
 * NAME:
 *      ms_ledInit()
 * FUNCTION: 
 *      Initialize LED pins to output 
 * INPUT:
 *      None
 * OUTPUT:
 *      None
******************************************************************/
void ms_ledInit() {
    /* Set all LED pins to output */
    pinMode(LEDGREEN, OUTPUT);
    pinMode(LEDGREEN_1, OUTPUT);
    pinMode(LEDGREEN_2, OUTPUT);
    pinMode(LEDGREEN_3, OUTPUT);
}

/****************************************************************
 * NAME:
 *      ms_led_on()
 * FUNCTION: 
 *      Set LED pins to high output 
 * INPUT:
 *      None
 * OUTPUT:
 *      None
******************************************************************/

void ms_led_on(){
    /* Switch on all LED's */
    digitalWrite(LEDGREEN, HIGH);
    digitalWrite(LEDGREEN_1, HIGH);
    digitalWrite(LEDGREEN_2, HIGH);
    digitalWrite(LEDGREEN_3, HIGH);
}


/****************************************************************
 * NAME:
 *      ms_led_off()
 * FUNCTION: 
 *      Set LED pins to low output 
 * INPUT:
 *      None
 * OUTPUT:
 *      None
******************************************************************/

void ms_led_off(){
    /* Switch on all LED's */
    digitalWrite(LEDGREEN, LOW);
    digitalWrite(LEDGREEN_1, LOW);
    digitalWrite(LEDGREEN_2, LOW);
    digitalWrite(LEDGREEN_3, LOW);
}


/****************************************************************
 * NAME:
 *      ms_camInit()
 * FUNCTION: 
 *      Initialize raspicam to WIDTH ADN HEIGHT
 * INPUT:
 *      None
 * OUTPUT:
 *      None
******************************************************************/

void ms_camInit() {
    /* Initializes Camera for operation */

    Camera.set(CV_CAP_PROP_FRAME_WIDTH, WIDTH);
    Camera.set(CV_CAP_PROP_FRAME_HEIGHT, HEIGHT);
    Camera.set(CV_CAP_PROP_FORMAT, CV_8UC1); //Sets image to grayscale

    /* Open The Camera */
    if (!Camera.open())
    {
        std::cerr << "CAMERA INIT FALIED\n";
        exit(ERROR_MSG);
    }

}


/****************************************************************
 * NAME:
 *      ms_cam_close()
 * FUNCTION: 
 *      Closes raspicam 
 * INPUT:
 *      None
 * OUTPUT:
 *      None
******************************************************************/

void ms_cam_close() {
    /* Close Camera from operation */

    Camera.release();
    if (Camera.isOpened())
    {
        std::cerr << "CAMERA NOT CLOSED\n";
        exit(ERROR_MSG);
    }  

}


/****************************************************************
 * NAME:
 *      ms_resCheck()
 * FUNCTION: 
 *      Checks image resolution  
 * INPUT:
 *      None
 * OUTPUT:
 *      None
******************************************************************/

void ms_resCheck() {  //Supposed to be private to just this file 
    int16_t img_width = 0;
    int16_t img_height = 0;

    if (img_width != WIDTH || img_height != HEIGHT)
    {
        std::cerr << "IMAGE RESOLUTION INCORRECT\n";
        exit(ERROR_MSG);
    }
    
}

/****************************************************************
 * NAME:
 *      ms_take_pic()
 * FUNCTION: 
 *      Takes Fingerprint picture  
 * INPUT:
 *      None
 * OUTPUT:
 *      None
******************************************************************/

void ms_take_pic() {
    /* Takes Fingerprint Image */

    int8_t count = 3;

    for (int8_t i = 0; i < count; i++)  //Take Picture 3 times for stability 
    {
        if (!Camera.grab())
        {
        std::cerr << "CANNOT TAKE PIC\n";
        exit(ERROR_MSG);    
        }
    }
    Camera.retrieve(image);
    /* CHECK IMAGE RESOLUTION */
   // ms_resCheck();
    cv::imwrite("image.jpeg", image);
    std::cout << "Success\n";
}


/****************************************************************
 * NAME:
 *      ms_process_img()
 * FUNCTION: 
 *      Processes Fingerprint Image for Print processing by NIST library  
 * INPUT:
 *      None
 * OUTPUT:
 *      Produces an image file in jpeg format and stores it the root folder
******************************************************************/



void raypics(){

    raspicam::RaspiCam_Cv Camera;
    Camera.set( CV_CAP_PROP_FORMAT, CV_8UC1 );

    cv::Mat image;

    if (!Camera.open()) {
        std::cerr << "CANNOT TAKE PIC\n";
    }

    Camera.set(CV_CAP_PROP_FRAME_WIDTH, 1600); // <-- If i remove this and the next line, the images are "normal"
    Camera.set(CV_CAP_PROP_FRAME_HEIGHT, 1200);

    Camera.grab();
    Camera.retrieve(image);

    Camera.release();

    //save image
    cv::imwrite("image.jpeg",image);
    std::cout<<"Imageeee saved at image.jpeg"<<std::endl;

}