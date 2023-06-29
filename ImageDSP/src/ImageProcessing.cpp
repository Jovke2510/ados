
#include "ImageProcessing.h"
#include "ColorSpaces.h"
#include "Augmentations.h"

#include <QDebug>

void imageProcessingFun(
	const QString& prog_name,
	QImage& out_img,
	const QImage& in_img,
	const QVector<double>& params
) {
    int x_size = 0;
    int y_size = 0;
    x_size = in_img.width();
    y_size = in_img.height();
    
    if(prog_name == "Augmentation 1"){
    	int angle = params[0];
    	out_img = QImage(x_size, y_size, in_img.format());
        myAugmentation1(in_img.bits(), x_size, y_size, out_img.bits(), x_size/2, y_size/2, angle);
        
    }else if(prog_name == "Augmentation 2"){
	out_img = QImage(x_size, y_size, in_img.format());    
    	double increment = params[0];
    	
    	myAugmentation2(in_img.bits(), x_size, y_size, out_img.bits(), increment);
    }else if(prog_name == "Augmentation 3"){
    	out_img = QImage(x_size, y_size, in_img.format()); 
    	int occlusion_size = params[0];
    	
    	myAugmentation3(in_img.bits(), x_size, y_size, out_img.bits(), occlusion_size);
    }else if(prog_name == "Augmentation 4"){
    	int B = params[0];
    	double strength = params[1];
    	out_img = QImage(x_size, y_size, in_img.format());
    	
    	
    	uchar* Y_buff = new uchar[x_size*in_img.height()];
	char* U_buff = new char[x_size*in_img.height()/4];
	char* V_buff = new char[x_size*in_img.height()/4];

	// Convert input image to YUV420 image.
	RGBtoYUV420(in_img.bits(), x_size, y_size, Y_buff, U_buff, V_buff);
    	
    	myAugmentation4(Y_buff, x_size, y_size, B, strength);
    	
    	procesing_YUV420(Y_buff, U_buff, V_buff, x_size, y_size, 1, 1, 1);

	// Convert YUV image back to RGB.
	YUV420toRGB(Y_buff, U_buff, V_buff, x_size, y_size, out_img.bits());
	

	// Delete used memory buffers.
	delete[] Y_buff;
	delete[] U_buff;
	delete[] V_buff;
    }

}


























