/********************************************************************************
 * Created by MOONSHOT TEAM ON 8/01/2020
 * PROJECT:
 *      Four-Finger-Print Scanner Device
 * CONTENT:
 *      Functions using the NIST LIBRARY for finger segmentation and Print quality check
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

#include "ms_func.h"
#include <stdio.h>
#include <imgdecod.h>
#include <log.h>
#include <nfiq.h>
#include <nfseg.h>
#include <lfs.h>
#include <libgen.h>

/* Declarations needed to make NIST function calls work */
typedef struct opt_flags_s {
  int verbose;
  int old_mode;
  int has_attribs;
} OPT_FLAGS;

MINUTIAE *minutiae;
int *direction_map, *low_contrast_map, *low_flow_map;
int *high_curve_map, *quality_map;
int map_w, map_h;
seg_rec_coords *fing_boxes;
OPT_FLAGS flags = {0, 0, 0};
int bthr_adj = 0, rot_search = 0, rot_seg = 0, comp = 1;

int img_type, ilen, iw,ih, id, ippi, nfiq, bw, bh, w, h, lossyflag =0, ppi;
int dlen, d;
float conf;
unsigned char *idata, *bdata, *data, **pdata;

/*
**********************************************************************
 * Function name:
 *          ms_imagequality
 * Description: 
 *          Checks fingerprint quality using NIST nfiq library
 * Input:
 *          imagedata: String of finger print image 
 * Output:
 *          Returns int 1 to 5 where 1 is the best print quality and 5 is the worst
 **********************************************************************
 */
int ms_imagequality(char *imagedata) {
    int ret;
    if((ret = read_and_decode_grayscale_image(imagedata, &img_type,&idata, &ilen, &iw, &ih, &id, &ippi))) 
        {
            if(ret == -3) /* UNKNOWN_IMG */
                fprintf(stderr, "Hint: Use -raw for raw images\n");
            exit(ret);
        }

    //Compute NFIQ value
    ret = comp_nfiq(&nfiq, &conf, idata, iw, ih, id, ippi, &flags.verbose);
    if(ret < 0) {
        free(idata);
        exit(ret);
  }

  return nfiq;
    
}

/*
**********************************************************************
 * Function name:
 *          ms_detect_finger
 * Desscription: 
 *          Checks fingerprint image and crops out the needed prints from the image
 * Input:
 *          FGP: A value to determine how many prints are needed
 *          imagedata: String of finger print image 
 * Output:
 *          wsq image format of fingerprint images stored in the executables root folder
 **********************************************************************
 */

int ms_detect_finger(int FGP, char *imagedata) {
  char *filename;
  int ret, nf;
  if (FGP < 15 && FGP > 12)
    nf = 4;
  else if (FGP == 15)
    nf = 2;
  else
    nf = 1;

  if ((ret = read_and_decode_grayscale_image(imagedata, &img_type, &data, &dlen, &w,
                                             &h, &d, &ppi)))
    exit(ret);

  if ((ret = lfs_detect_minutiae_V2(&minutiae, &direction_map,
                                    &low_contrast_map, &low_flow_map,
                                    &high_curve_map, &map_w, &map_h, &bdata,
                                    &bw, &bh, data, w, h, &lfsparms))) {
    return (ret);
  }
  //trim_histtails_contrast_boost(data, w, h);
  /* TRY TO SEGMENT FINGER IMAGE */
  if ((ret = segment_fingers(data, w, h, &fing_boxes, nf, FGP, bthr_adj,
                             rot_search)))
    exit(ret);

  /* PARSE FINGERS FROM ORIGINAL FINGER IMAGE */
  if ((ret = parse_segfing(&pdata, data, w, h, fing_boxes, nf, rot_seg)))
    exit(ret);
  //trim_histtails_contrast_boost(data, w, h);
  free(data);
  /* OUTPUT RESULTS TO FILE */
  filename = basename("image");
  if ((ret = write_parsefing(filename, -1, FGP, comp, ppi, lossyflag, pdata,
                             fing_boxes, nf, rot_seg)))
    exit(ret);

  free(fing_boxes);
  for (int i = 0; i < nf; i++)
    free(pdata[i]);
  free(pdata);

  /* If system error ... */
  if (ret < 0) {
    free(bdata);
    exit(ret);
    return 0;
  }
  return 0;
}