#include "ColorSpaces.h"
#include <bits/stdc++.h>
using namespace std;

void processing_RGB(
	const uchar rgbInputImg[],
	int w,
	int h,
	uchar rgbOutputImg[],
	double R,
	double G,
	double B
) {
	for(int y = 0; y < h; y++){
		for(int x = 0; x < w; x++){
			rgbOutputImg[y*w*3+x*3 + 0] = R*rgbInputImg[(y*w + x)*3 + 0];
			rgbOutputImg[y*w*3+x*3 + 1] = G*rgbInputImg[(y*w + x)*3 + 1];
			rgbOutputImg[y*w*3+x*3 + 2] = B*rgbInputImg[(y*w + x)*3 + 2];
		}
	}
}

void RGBtoYUV444(
	const uchar rgbImg[],
	int w,
	int h,
	uchar Y_buff[],
	char U_buff[],
	char V_buff[]
) {
	uchar R, G, B;
	for(int y = 0; y < h; y++){
		for(int x = 0; x < w; x++){
			R = rgbImg[(y*w + x)*3 + 0];
			G = rgbImg[(y*w + x)*3 + 1];
			B = rgbImg[(y*w + x)*3 + 2];

			Y_buff[y*w + x] =  0.299*R + 0.587*G + 0.114*B;
			U_buff[y*w + x] =  - 0.14713*R - 0.28886*G + 0.436*B;
			V_buff[y*w + x] =  0.615*R - 0.51499*G - 0.10001*B;
		}
	}
}

void RGBtoHSL(
	int R,
	int G,
	int B,
	double &h,
	double &s,
	double &l
) {
	double r, g, b;
	
	r = R/255.0;
	g = G/255.0;
	b = B/255.0;
	
	double max = qMax(qMax(r, b), g);
	double min = qMin(qMin(r, b), g);
	double delta = max - min;
	
	l = (max + min) / 2;
	
	if(delta == 0)
		s = 0;
	else{
		if(l <= 0.5)
			s = (max - min) / (max + min);
		else
			s = (max - min) / (2.0 - max - min);	
	}
	
	if(delta == 0){
		h = 0;
	}else if(max == r){
		h = 60 * (g - b)/delta;
	}else if(max == g){
		h = 60 * (2.0 + (b - r)/delta);
	}else if(max == b){
		h = 60 * (4.0 + (r - g)/delta);
	}
	
	if(h < 0)
		h += 360;
	  	
}


void HSLtoRGB(double h, double s, double l, int& r, int& g, int& b) {
    
    double C = (1 - abs(2*l - 1)) * s;
    double H = h/60;
    double X = C * (1 - abs(fmod(H, 2) - 1));
    
    double R = 0;
    double G = 0;
    double B = 0;
    
    if(H >= 0 && H < 1){
    	R = C;
    	B = X;
    	G = 0;
    }else if(H >= 1 && H < 2){
    	R = X;
    	B = C;
    	G = 0;
    }else if(H >= 2 && H < 3){
    	R = 0;
    	B = C;
    	G = X;
    }else if(H >= 3 && H < 4){
    	R = 0;
    	B = X;
    	G = C;
    }else if(H >= 4 && H < 5){
    	R = X;
    	B = 0;
    	G = C;
    }else if(H >= 5 && H <= 6){
    	R = C;
    	B = 0;
    	G = X;
    }
    
    double m = l - (C/2);
    
    r = ceil((R + m) * 255);
    g = ceil((G + m) * 255);
    b = ceil((B + m) * 255);	
    	
}
    

void YUV444toRGB(
	const uchar Y_buff[],
	const char U_buff[],
	const char V_buff[],
	int w,
	int h,
	uchar rgbImg[]
) {
	double R, G, B;
	double Y, U, V;
	for(int y = 0; y < h; y++){
		for(int x = 0; x < w; x++){
			Y = Y_buff[y*w + x];
			U = U_buff[y*w + x];
			V = V_buff[y*w + x];

			R = Y + 1.13983*V;
			G = Y - 0.39465*U - 0.58060*V;
			B = Y + 2.03211*U;
			
			if(R < 0){
				R = 0;
			}else if(R > 255){
				R = 255;
			}
			if(G < 0){
				G = 0;
			}else if(G > 255){
				G = 255;
			}
			if(B < 0){
				B = 0;
			}else if(B > 255){
				B = 255;
			}

			rgbImg[(y*w + x)*3 + 0] = R;
			rgbImg[(y*w + x)*3 + 1] = G;
			rgbImg[(y*w + x)*3 + 2] = B;
	
		}
	}
}

void procesing_YUV444(
	uchar Y_buff[],
	char U_buff[],
	char V_buff[],
	int w,
	int h,
	double Y,
	double U,
	double V
) {
	for(int y = 0; y < h; y++){
		for(int x = 0; x < w; x++){
			Y_buff[y*w + x] *= Y; 
			U_buff[y*w + x] *= U; 
			V_buff[y*w + x] *= V; 
		}
	}
}

void RGBtoYUV422(const uchar rgbImg[], int x, int y, uchar Y_buff[], char U_buff[], char V_buff[]) 
{
	uchar R, G, B;
	for(int i = 0; i<x; i+=2)
	{
		for(int j = 0; j<y; j++)
		{
			R = rgbImg[j*3*x+i*3];
			G = rgbImg[j*3*x+i*3 + 1];
			B = rgbImg[j*3*x+i*3 + 2];

			Y_buff[j*x+i] =  0.299*R + 0.587*G + 0.114*B;
			U_buff[j*x/2+i/2] =  (- 0.14713*R - 0.28886*G + 0.436*B)/2;
			V_buff[j*x/2+i/2] =  (R*0.615 - 0.51499*G - 0.10001*B)/2;

			R = rgbImg[j*3*x+(i+1)*3];
			G = rgbImg[j*3*x+(i+1)*3 + 1];
			B = rgbImg[j*3*x+(i+1)*3 + 2];

			Y_buff[j*x+(i+1)] =  0.299*R + 0.587*G + 0.114*B;
			U_buff[j*x/2+i/2] +=  (- 0.14713*R - 0.28886*G + 0.436*B)/2;
			V_buff[j*x/2+i/2] +=  (R*0.615 - 0.51499*G - 0.10001*B)/2;
		}
	}
}


void YUV422toRGB(const uchar Y_buff[], const char U_buff[], const char V_buff[], int x, int y, uchar rgbImg[]) 
{
	double R,G,B;
	double Y, U, V;
	for(int i = 0; i<x; i+=2)
	{
		for(int j = 0; j<y; j++)
		{
			U = U_buff[j*x/2+i/2];
			V = V_buff[j*x/2+i/2];

			Y = Y_buff[j*x+i];

			R = Y + 1.13983*V;
			G = Y -0.39465*U - 0.58060*V;
			B = Y + 2.03211*U;

			if (R < 0)
				R = 0;
			else if (R > 255)
				R = 255;
			if (G < 0)
				G = 0;
			else if (G> 255)
				G = 255;
			if (B < 0)
				B = 0;
			else if (B > 255)
				B = 255;

			rgbImg[j*3*x+i*3] =  R;
			rgbImg[j*3*x+i*3 + 1] = G;
			rgbImg[j*3*x+i*3 + 2] = B;

			Y = Y_buff[j*x+(i+1)];

			R = Y + 1.13983*V;
			G = Y -0.39465*U - 0.58060*V;
			B = Y + 2.03211*U;

			if (R < 0)
				R = 0;
			else if (R > 255)
				R = 255;
			if (G< 0)
				G = 0;
			else if (G> 255)
				G = 255;
			if (B < 0)
				B = 0;
			else if (B > 255)
				B = 255;

			rgbImg[j*3*x+(i+1)*3] =  R;
			rgbImg[j*3*x+(i+1)*3 + 1] = G;
			rgbImg[j*3*x+(i+1)*3 + 2] = B;
		}
	}
}

void procesing_YUV422(uchar Y_buff[], char U_buff[], char V_buff[], int x, int y, double Y, double U, double V)
{
	for(int i = 0; i<x; i+=2)
	{
		for(int j = 0; j<y; j++)
		{
			Y_buff[j*x+i] *= Y; 
			Y_buff[j*x+i+1] *= Y; 
			U_buff[j*x/2+i/2] *= U; 
			V_buff[j*x/2+i/2] *= V; 
		}
	}

}

void RGBtoYUV420(const uchar rgbImg[], int x, int y, uchar Y_buff[], char U_buff[], char V_buff[]) 
{
	uchar R, G, B;
	double U, V;
	for(int i = 0; i<x; i+=2)
	{
		for(int j = 0; j<y; j+=2)
		{	
			R = rgbImg[j*3*x+i*3];
			G = rgbImg[j*3*x+i*3 + 1];
			B = rgbImg[j*3*x+i*3 + 2];

			Y_buff[j*x+i] =  0.299*R + 0.587*G + 0.114*B;
			U =  (- 0.14713*R - 0.28886*G + 0.436*B);
			V =  (R*0.615 - 0.51499*G - 0.10001*B);

			R = rgbImg[j*3*x+(i+1)*3];
			G = rgbImg[j*3*x+(i+1)*3 + 1];
			B = rgbImg[j*3*x+(i+1)*3 + 2];

			Y_buff[j*x+(i+1)] =  0.299*R + 0.587*G + 0.114*B;
			U +=  (- 0.14713*R - 0.28886*G + 0.436*B);
			V +=  (R*0.615 - 0.51499*G - 0.10001*B);

			R = rgbImg[(j+1)*3*x+i*3];
			G = rgbImg[(j+1)*3*x+i*3 + 1];
			B = rgbImg[(j+1)*3*x+i*3 + 2];

			Y_buff[(j+1)*x+i] =  0.299*R + 0.587*G + 0.114*B;
			U +=  (- 0.14713*R - 0.28886*G + 0.436*B);
			V +=  (R*0.615 - 0.51499*G - 0.10001*B);

			R = rgbImg[(j+1)*3*x+(i+1)*3];
			G = rgbImg[(j+1)*3*x+(i+1)*3 + 1];
			B = rgbImg[(j+1)*3*x+(i+1)*3 + 2];

			Y_buff[(j+1)*x+(i+1)] =  0.299*R + 0.587*G + 0.114*B;
			U +=  (- 0.14713*R - 0.28886*G + 0.436*B);
			V +=  (R*0.615 - 0.51499*G - 0.10001*B);

			U_buff[(j/2)*x/2+i/2] =  U/4;
			V_buff[(j/2)*x/2+i/2] =  V/4;
		}
	}
}

void YUV420toRGB(const uchar Y_buff[], const char U_buff[], const char V_buff[], int x, int y, uchar rgbImg[]) 
{
	double R,G,B;
	double Y, U, V;

	for(int j = 0; j<y; j+=2)	
	{
		for(int i = 0; i<x; i+=2)
		{
			U = U_buff[j/2*x/2+i/2];
			V = V_buff[j/2*x/2+i/2];

			Y = Y_buff[j*x+i];

			R = Y + 1.13983*V;
			G = Y -0.39465*U - 0.58060*V;
			B = Y + 2.03211*U;

			if (R < 0)
				R = 0;
			else if (R > 255)
				R = 255;
			if (G< 0)
				G = 0;
			else if (G> 255)
				G = 255;
			if (B < 0)
				B = 0;
			else if (B > 255)
				B = 255;

			rgbImg[j*3*x+i*3] =  R;
			rgbImg[j*3*x+i*3 + 1] = G;
			rgbImg[j*3*x+i*3 + 2] = B;

			Y = Y_buff[j*x+(i+1)];

			R = Y + 1.13983*V;
			G = Y -0.39465*U - 0.58060*V;
			B = Y + 2.03211*U;

			if (R < 0)
				R = 0;
			else if (R > 255)
				R = 255;
			if (G< 0)
				G = 0;
			else if (G> 255)
				G = 255;
			if (B < 0)
				B = 0;
			else if (B > 255)
				B = 255;
		
			rgbImg[j*3*x+(i+1)*3] =  R;
			rgbImg[j*3*x+(i+1)*3 + 1] = G;
			rgbImg[j*3*x+(i+1)*3 + 2] = B;

			Y = Y_buff[(j+1)*x+i];

			R = Y + 1.13983*V;
			G = Y -0.39465*U - 0.58060*V;
			B = Y + 2.03211*U;

			if (R < 0)
				R = 0;
			else if (R > 255)
				R = 255;
			if (G < 0)
				G = 0;
			else if (G> 255)
				G = 255;
			if (B < 0)
				B = 0;
			else if (B > 255)
				B = 255;

			rgbImg[(j+1)*3*x+i*3] =  R;
			rgbImg[(j+1)*3*x+i*3 + 1] = G;
			rgbImg[(j+1)*3*x+i*3 + 2] = B;

			Y = Y_buff[(j+1)*x+(i+1)];

			R = Y + 1.13983*V;
			G = Y -0.39465*U - 0.58060*V;
			B = Y + 2.03211*U;

			if (R < 0)
				R = 0;
			else if (R > 255)
				R = 255;
			if (G< 0)
				G = 0;
			else if (G> 255)
				G = 255;
			if (B < 0)
				B = 0;
			else if (B > 255)
				B = 255;

			rgbImg[(j+1)*3*x+(i+1)*3] =  R;
			rgbImg[(j+1)*3*x+(i+1)*3 + 1] = G;
			rgbImg[(j+1)*3*x+(i+1)*3 + 2] = B;
		}
	}
}

void procesing_YUV420(uchar Y_buff[], char U_buff[], char V_buff[], int x, int y, double Y, double U, double V)
{
	for(int i = 0; i<x; i+=2)
	{
		for(int j = 0; j<y; j+=2)
		{
			Y_buff[j*x+i] *= Y; 
			Y_buff[j*x+i+1] *= Y; 
			Y_buff[(j+1)*x+i] *= Y; 
			Y_buff[(j+1)*x+i+1] *= Y; 

			U_buff[j/2*x/2+i/2] *= U; 
			V_buff[j/2*x/2+i/2] *= V; 
		}
	}

}

	

void decimate_Y(
	uchar Y_buff[],
    int w,
    int h
) {
	uchar Y;
	int YY;
	for(int y = 0; y < h; y += 2){
		for(int x = 0; x < w; x += 2){
			YY = 0;
			for(int yb = 0; yb < 2; yb++){
				for(int xb = 0; xb < 2; xb++){
					YY += Y_buff[(y+yb)*w + (x+xb)];
				}
			}
			Y = YY >> 2;
			
			for(int yb = 0; yb < 2; yb++){
				for(int xb = 0; xb < 2; xb++){
					Y_buff[(y+yb)*w + (x+xb)] = Y;
				}
			}
		}
	}
}
