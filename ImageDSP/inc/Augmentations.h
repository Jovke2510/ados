
#pragma once

#include <QDebug>
#include <cmath>
#include <ColorSpaces.h>
#include <iostream>
#include <random>


void myAugmentation1(
	const uchar input[], 
	int x_size, 
	int y_size, 
	uchar output[], 
	int m, 
	int n, 
	double angle);
    
void myAugmentation2(
	const uchar input[],
	int x_size,
	int y_size,
	uchar output[],
	double increment);

void myAugmentation3(
	const uchar input[],
	int x_size,
	int y_size, 
	uchar output[], 
	int occlusion_size);

void GenerateDCTmatrix(double* DCTKernel, int order);
void DCT(const uchar input[], short output[], int N, double* DCTKernel);
void IDCT(const short input[], uchar output[], int N, double* DCTKernel);
void extendBorders(uchar* input, int xSize, int ySize, int N, uchar** p_output, int* newXSize, int* newYSize);
void cropImage(uchar* input, int xSize, int ySize, uchar* output, int newXSize, int newYSize);
static void doZigZag(short block[], int N, int B);
void performMaskDCTCoeffs(uchar input[], int xSize, int ySize, int N, int B);
void unsharpMasking(uchar orig[], uchar blured[], int x_size, int y_size, double strenght);

void myAugmentation4(
	uchar input[],
	int x_size,
	int y_size, 
	int B,
	double strength
);
	
	
	
	
	
	
	
	
