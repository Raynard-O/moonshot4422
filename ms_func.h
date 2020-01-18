/*****************************************************************************
 * Created by the MOONSHOT TEAM on 8/01/2020
*/

#ifndef NIBS_MS_FUNC_H
#define NIBS_MS_FUNC_H

/* Define Error Message AND Constants */
#define ERROR_MSG   -657  //A random negative number to indicate error 
#define WIDTH   640
#define HEIGHT  480


/* Pin Declarations for LED */
#define LEDGREEN    0
#define LEDGREEN_1  2
#define LEDGREEN_2  22
#define LEDGREEN_3  1    

/* Function Declarations */
void ms_ledInit(void); //Initializes the led to output
void ms_led_on(void); //Sets the led to high output
void ms_led_off(void); //Sets the led to low output
void ms_camInit(void);  //Initalizes Raspicam and sets resolution to WIDTH ANND HEIGHT
void ms_cam_close(void);  //Closes camera from operation
void ms_take_pic(void);  //Takes finger print picture
void raypics(void);
/* C-style written code */
#ifdef __cplusplus 
extern "C" {
#endif

int ms_imagequality(char *);  //Returns image quality value 
int ms_detect_finger(int, char*); //Produces images of the print


#ifdef __cplusplus
}
#endif

#endif //NIBS_MS_FUNC_H