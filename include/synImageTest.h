#pragma once

#include <stdint.h>

//Generic parameter
#define SYN_MAXROW				400 //200
#define SYN_MAXCOL				400 //200

//SNR parameter
#define SYN_QNTY_SNR_ZONES		7

/*
 * Bubble parameter
 */
#define KERNAL_SIZE		3
#define SYN_BUBBLE_AREA 5

/*
 * PixelUniformity parameter
 */
#define SYN_UNIFORMITY_AREA 16

#ifdef SYNIMAGETEST_EXPORTS
	#define SYNIMAGINGTEST __declspec(dllexport)
#else
	#define SYNIMAGINGTEST __declspec(dllimport)
#endif

#ifdef __cplusplus
	#define SYNA_IMAGING_API extern "C" SYNIMAGINGTEST
#else 
	#define SYNA_IMAGING_API SYNIMAGINGTEST
#endif

/*
 * SNR Test
 * int16_t and uint8_t image : without finger and with finger
 */
SYNA_IMAGING_API unsigned int synSNRTest(int16_t *arrNoFingerImage, int16_t *arrFingerImage, unsigned int rowNumber, unsigned int columnNumber, unsigned int frameNumbers, int16_t guard, 
											int arrSingal[SYN_QNTY_SNR_ZONES], float arrNoise[SYN_QNTY_SNR_ZONES], double arrSNR[SYN_QNTY_SNR_ZONES]);

SYNA_IMAGING_API unsigned int synSNRTestAlpha(uint8_t *arrNoFingerImage, uint8_t *arrFingerImage, unsigned int rowNumber, unsigned int columnNumber, unsigned int frameNumbers, 
												int arrSingal[SYN_QNTY_SNR_ZONES], float arrNoise[SYN_QNTY_SNR_ZONES], double arrSNR[SYN_QNTY_SNR_ZONES]);

/*
 * Imperfection Test
 * uint8_t image : with finger
 */
SYNA_IMAGING_API unsigned int synImperfectionTest(uint8_t *arrFingerImage, unsigned int rowNumber, unsigned int columnNumber, int peggedThreshold, int flooredThreshold, unsigned int failLimit, 
													int *consecutive_pegged_rows, int *consecutive_pegged_cols);

/*
 * Sharpness Test
 * uint8_t image : with finger
 */
SYNA_IMAGING_API unsigned int synSharpnessTest(uint8_t *arrFingerImage, unsigned int rowNumber, unsigned int columnNumber, int Gx[SYN_MAXROW][SYN_MAXCOL], int Gy[SYN_MAXROW][SYN_MAXCOL], 
												int h[SYN_MAXCOL], int g[SYN_MAXROW][SYN_MAXCOL], int pImg_t[SYN_MAXROW][SYN_MAXCOL], float SHARPNESS[4], float *percent);

/*
 * Bubble Test(new Imperfection Test)
 * int16_t image : with finger
 */
SYNA_IMAGING_API unsigned int synBubbleTest(int16_t *finger_image, unsigned int rowNumber, unsigned int columnNumber, double gradient_image[SYN_MAXROW][SYN_MAXCOL]);

/*
 * PixelUniformityTest(integrate FlooredPixel Test, PeggedPixel Test and PixelUniformity Test)
 * int16_t image : without finger
 */
SYNA_IMAGING_API unsigned int synPixelUniformityTest(int16_t *arr_image, unsigned int rowNumber, unsigned int columnNumber, 
														int floored_limit, int pegged_limit, int row_fail_counts, int column_fail_counts, int area_fail_counts, unsigned int *op_result);