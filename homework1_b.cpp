#pragma GCC diagnostic ignored "-Wwrite-strings"

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;

/// Global variables
char* source_window = "Source image";
char* warp_window = "(2) Decrease size by half";
char* warp_rotate_window = "(3) Rotate 30 degrees";
char* warp_translate_window = "(4) Translate 5pixels left-bottom";

/** @function main */
 int main( int argc, char** argv )
 {
   Point2f srcTri[3];
   Point2f dstTri[3];

   Mat rot_mat( 2, 3, CV_32FC1 );
   Mat warp_mat( 2, 3, CV_32FC1 );
   Mat src, warp_dst, warp_rotate_dst;

   // (1) GET INPUT IMAGE
   src = imread( argv[1], 1 );

   // (2) DECREASE ITS SIZE BY HALF
   int dest_rows = src.rows; // / 2;
   int dest_cols = src.cols; // / 2;
   warp_dst = Mat::zeros( dest_rows, dest_cols, src.type() );

   // Set your 3 points to calculate the Affine Transform
   srcTri[0] = Point2f( 0,0 );
   srcTri[1] = Point2f( src.cols - 1, 0 );
   srcTri[2] = Point2f( 0, src.rows - 1 );

   // Decrease its size by half
   dstTri[0] = Point2f( 0, 0 );
   dstTri[1] = Point2f( dest_cols - 1, 0 );
   dstTri[2] = Point2f( 0, dest_rows - 1 );

   // Get the Affine Transform
   warp_mat = getAffineTransform( srcTri, dstTri );

   // Apply the Affine Transform just found to the src image
   warpAffine( src, warp_dst, warp_mat, warp_dst.size() );

   /** Rotating the image after Warp */

   // (3) ROTATE 30 DEGREES
   // Compute a rotation matrix with respect to the center of the image
   Point center = Point( warp_dst.cols/2, warp_dst.rows/2 );
   double angle = 30.0;
   double scale = 0.5; // (2) DECREASE ITS SIZE BY HALF

   /// Get the rotation matrix with the specifications above
   rot_mat = getRotationMatrix2D( center, angle, scale );

   /// Rotate the warped image
   warpAffine( warp_dst, warp_rotate_dst, rot_mat, warp_dst.size() );

   /** Translating the image*/
   // (4) TRANSLATE 50 pixels left-bottom.
   dstTri[0] = Point2f( 0 - 50, 0 + 50 );
   dstTri[1] = Point2f( src.cols - 1 - 100, 0 + 50 );
   dstTri[2] = Point2f( 0 - 50, src.rows - 1 + 50 );

   // Get the Affine Transform
   Mat translate_mat = getAffineTransform( srcTri, dstTri );

   // Apply the Affine Transform just found to the rotated image
   Mat warp_translate_dst;
   warpAffine( warp_rotate_dst, warp_translate_dst, translate_mat, warp_rotate_dst.size() );

   /// Show what you got
   namedWindow( source_window, CV_WINDOW_AUTOSIZE );
   imshow( source_window, src );

   namedWindow( warp_window, CV_WINDOW_AUTOSIZE );
   imshow( warp_window, warp_dst );


   namedWindow( warp_rotate_window, CV_WINDOW_AUTOSIZE );
   imshow( warp_rotate_window, warp_rotate_dst );
   //
   imwrite( strcat(argv[1], "_rotate_a.jpg"), warp_rotate_dst);


   namedWindow( warp_translate_window, CV_WINDOW_AUTOSIZE );
   imshow( warp_translate_window, warp_translate_dst );
   //
   imwrite( strcat(argv[1], "_translate_a.jpg"), warp_translate_dst);

   /// Wait until user exits the program
   waitKey(0);

   return 0;
  }