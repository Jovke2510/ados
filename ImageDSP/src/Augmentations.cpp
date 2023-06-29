
#include "Augmentations.h"
#define _USE_MATH_DEFINES
#define PI 3.14159265359
using namespace std;


void myAugmentation1(const uchar input[], int x_size, int y_size, uchar output[], int m, int n, double angle) {
    //Rotation for 180 degrees around center of the image

    double angle_rad = angle*PI/180;
    for (int x = 0; x < x_size; x++){
		for(int y = 0; y < y_size; y++){
			
			int newX = floor(x * cos(angle_rad) - y * sin(angle_rad) - m * cos(angle_rad) + n * sin(angle_rad) + m);
			int newY = floor(y * cos(angle_rad) + x * sin(angle_rad) - m * sin(angle_rad) - n * cos(angle_rad) + n);
			
			if(newX > x_size || newX < 0 || newY > y_size || newY < 0){
				output[y * 3 * x_size + x * 3] = 255;
				output[y * 3 * x_size + x * 3 + 1] = 255;
				output[y * 3 * x_size + x * 3 + 2] = 255;
			}else{
				output[y * 3 * x_size + x * 3] = input[newY * 3 * x_size + newX * 3];
				output[y * 3 * x_size + x * 3 + 1] = input[newY * 3 * x_size + newX * 3 + 1];
				output[y * 3 * x_size + x * 3 + 2] = input[newY * 3 * x_size + newX * 3 + 2];
			}
			 
		}
	}
}

void myAugmentation2(const uchar input[], int x_size, int y_size, uchar output[], double increment){
	//Changing Hue
	for(int y = 0; y < y_size; y++){
		for(int x = 0; x < x_size; x++){
			int r = input[(y * x_size + x)*3 + 0];
			int g = input[(y * x_size + x)*3 + 1];
			int b = input[(y * x_size + x)*3 + 2];
			
			
			double h, s, l;
			RGBtoHSL(r, g, b, h, s, l);
			
			
			h += increment;
			if(h >= 360){
				h -= 360;
			}
			
			//convert pixel back to rgb
			HSLtoRGB(h, s, l, r, g, b);
			
			//then compose a new output
			output[(y * x_size + x)*3 + 0] = r;
			output[(y * x_size + x)*3 + 1] = g;
			output[(y * x_size + x)*3 + 2] = b;
			
		}
	}

}

void myAugmentation3(
	const uchar input[],
	int x_size,
	int y_size, 
	uchar output[], 
	int occlusion_size
) {
	//Occlusion
	memcpy(output, input, x_size * y_size * 3);
	
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<int> disX(0, x_size - occlusion_size);
	uniform_int_distribution<int> disY(0, y_size - occlusion_size);
	
	int occlusionX = disX(gen);
	int occlusionY = disY(gen);
	
	for(int y = occlusionY; y < occlusionY + occlusion_size; y++){
		for(int x = occlusionX; x < occlusionX + occlusion_size; x++){
			output[(y * x_size + x) * 3 + 0] = 255;
			output[(y * x_size + x) * 3 + 1] = 155;
			output[(y * x_size + x) * 3 + 2] = 55;
		}
	} 	
}
//HELPER FUNCTIONS FOR DCT
void GenerateDCTmatrix(double* DCTKernel, int order)
{
	int i, j;
	int N;
	N = order;
	double alpha;
	double denominator;
	for (j = 0; j <= N - 1; j++)
	{
		DCTKernel[0, j] = sqrt(1 / (double)N);
	}
	alpha = sqrt(2 / (double)N);
	denominator = (double)2 * N;
	for (j = 0; j <= N - 1; j++)
		for (i = 1; i <= N - 1; i++)
		{
			DCTKernel[i*N + j] = alpha * cos(((2 * j + 1) * i * PI) / denominator);
		}
}

void DCT(const uchar input[], short output[], int N, double* DCTKernel)
{
	double* temp = new double[N*N];
	double* DCTCoefficients = new double[N*N];

	double sum;
	for (int i = 0; i <= N - 1; i++)
	{
		for (int j = 0; j <= N - 1; j++)
		{
			sum = 0;
			for (int k = 0; k <= N - 1; k++)
			{
				sum = sum + DCTKernel[i*N+k] * (input[k*N+j]-128.0);
			}
			temp[i*N + j] = sum;
		}
	}

	for (int i = 0; i <= N - 1; i++)
	{
		for (int j = 0; j <= N - 1; j++)
		{
			sum = 0;
			for (int k = 0; k <= N - 1; k++)
			{
				sum = sum + temp[i*N+k] * DCTKernel[j*N+k];
			}
			DCTCoefficients[i*N+j] = sum;
		}
	}
	
	for(int i = 0; i < N*N; i++)
	{
		output[i] = floor(DCTCoefficients[i]+0.5);
	}
		
	delete[] temp;
	delete[] DCTCoefficients;

	return;
}
    
void IDCT(const short input[], uchar output[], int N, double* DCTKernel)
{
	/* TO DO */
    double* temp = new double[N*N];
    double* DCTCoefficients = new double[N*N];

    double sum;
    for (int i = 0; i <= N - 1; i++)
    {
        for (int j = 0; j <= N - 1; j++)
        {
            sum = 0;
            for (int k = 0; k <= N - 1; k++)
            {
                sum = sum + DCTKernel[k*N+i] * (input[k*N+j]);
            }
            temp[i*N + j] = sum;
        }
    }

    for (int i = 0; i <= N - 1; i++)
    {
        for (int j = 0; j <= N - 1; j++)
        {
            sum = 0;
            for (int k = 0; k <= N - 1; k++)
            {
                sum = sum + temp[i*N+k] * DCTKernel[k*N+j];
            }
            DCTCoefficients[i*N+j] = sum;
        }
    }

    for(int i = 0; i < N*N; i++)
    {
        output[i] = floor(DCTCoefficients[i]+0.5)+128;
    }

    delete[] temp;
    delete[] DCTCoefficients;

    return;
}
   
void extendBorders(uchar* input, int xSize, int ySize, int N, uchar** p_output, int* newXSize, int* newYSize)
{
	int deltaX = N-xSize%N;
	int deltaY = N-ySize%N;

	*newXSize = xSize+deltaX;
	*newYSize = ySize+deltaY;

	uchar* output = new uchar[(xSize+deltaX)*(ySize+deltaY)];

	for (int i=0; i<ySize; i++)
	{
		memcpy(&output[i*(xSize+deltaX)], &input[i*(xSize)], xSize);
		if(deltaX != 0)
		{
			memset(&output[i*(xSize+deltaX)+xSize], output[i*(xSize+deltaX)+xSize - 1], deltaX);  
		}
	}

	for (int i=0; i<deltaY; i++)
	{
		memcpy(&output[(i + ySize)*(xSize+deltaX)], &output[(ySize)*(xSize+deltaX)], xSize+deltaX);
	}

	*p_output = output;
}

void cropImage(uchar* input, int xSize, int ySize, uchar* output, int newXSize, int newYSize)
{
	for (int i=0; i<newYSize; i++)
	{
		memcpy(&output[i*(newXSize)], &input[i*(xSize)], newXSize);
	}
}

static void doZigZag(short block[], int N, int B)
{
	/* TO DO */
    int currNum = 0;
    int currDiagonalWidth = 1;
    int i;
    int row;
    int col;

    while (currDiagonalWidth<N)
    {
        for (i = 0; i<currDiagonalWidth; i++)
        {
            if (currDiagonalWidth % 2 == 1)
            {
                row = currDiagonalWidth - i - 1;
                col = i;
            }
            else
            {
                row = i;
                col = currDiagonalWidth - i - 1;
            }
            // OBRADA PIKSELA SA KOORDINATAMA row I col
            if(currNum > B)
                block[row*N + col] = 0;
            currNum++;
        }
        currDiagonalWidth++;
    }
    while (currDiagonalWidth> 0)
    {
        for (i = currDiagonalWidth; i> 0; i--)
        {
            if (currDiagonalWidth % 2 == 1){
                row = N - currDiagonalWidth + i - 1;
                col = N - i;
            }
            else
            {
                row = N - i;
                col = N - currDiagonalWidth + i - 1;
            }
            // OBRADA PIKSELA SA KOORDINATAMA row I col
            if(currNum > B)
                block[row*N + col] = 0;
            currNum++;
        }

        currDiagonalWidth--;
    }

}


/* perform masking of DCT coeffs */
void performMaskDCTCoeffs(uchar input[], int xSize, int ySize, int N, int B)
{
	/* TO DO */
    /* Create NxN buffer for one input block */
    uchar* inBlock = new uchar[N*N];

    /* Generate DCT kernel */
    double* DCTKernel = new double[N*N];
    GenerateDCTmatrix(DCTKernel, N);

    /* Create buffer for DCT coefficients */
    short* dctCoeffs = new short[N*N];

    /* Extend image borders */
    int xSize2, ySize2;
    uchar* input2;
    extendBorders(input, xSize , ySize, N, &input2, &xSize2, &ySize2);

    for (int y = 0; y < ySize2/N; y++)
    {
        for (int x = 0; x < xSize2/N; x++)
        {

            /* Fill input block buffer */
            for (int j=0; j<N; j++)
            {
                for (int i=0; i<N; i++)
                {
                    inBlock[j*N+i] = input2[(N*y+j)*(xSize2)+N*x+i];
                }
            }

            /* Invoke DCT */
            DCT(inBlock, dctCoeffs, N, DCTKernel);

            doZigZag(dctCoeffs, N, B);

            /* Invoke IDCT */
            IDCT(dctCoeffs, inBlock, N, DCTKernel);

            /* Write output values to output image */
            for (int j=0; j<N; j++)
            {
                for (int i=0; i<N; i++)
                {
                    input2[(N*y+j)*(xSize2)+N*x+i] = inBlock[j*N+i];
                }
            }
        }
    }

    cropImage(input2, xSize2, ySize2, input, xSize, ySize);

    /* Delete used memory buffers coefficients */
    delete[] dctCoeffs;
    delete[] inBlock;
    delete[] DCTKernel;
    delete[] input2;

}

void unsharpMasking(uchar orig[], uchar blured[], int x_size, int y_size, double strength){
	
	uchar* vf_img = new uchar[x_size * y_size];
	uchar* sharp_img = new uchar[x_size * y_size];
	for(int y = 0; y < y_size; y++){
		for(int x = 0; x < x_size; x++){
			vf_img[y*x_size + x] = orig[y*x_size + x] - blured[y*x_size + x];
			sharp_img[y*x_size + x] = orig[y*x_size + x] + strength*vf_img[y*x_size + x];
		}
	}
	
    memcpy(blured, sharp_img, x_size*y_size);
}

void myAugmentation4(
	uchar input[],
	int x_size,
	int y_size,  
	int B,
	double strength
){
	//DCT lowpass Unsharp masking
	int N = 8;
	uchar* orig = new uchar[x_size * y_size];
	
	memcpy(orig, input, x_size*y_size);
	
	performMaskDCTCoeffs(input, x_size, y_size, N, B);
	
	unsharpMasking(orig, input, x_size, y_size, strength);	
}















