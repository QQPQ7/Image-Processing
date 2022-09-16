#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <math.h>
#include <Windows.h>
#include <wingdi.h>

int wid = 512;
int hei = 512;
void filter(int j, unsigned char* Y);

int main()
{
	BITMAPFILEHEADER bmpFile;
	BITMAPINFOHEADER bmpInfo;
	BITMAPFILEHEADER bmpFile2;
	BITMAPINFOHEADER bmpInfo2;
	FILE* inputFile = NULL;
	FILE* inputFile2 = NULL;
	inputFile = fopen("AICenterY.bmp", "rb");//rb 바이너리로 읽기
	inputFile2 = fopen("AICenterY.bmp", "rb");//rb 바이너리로 읽기

	fread(&bmpFile, sizeof(BITMAPFILEHEADER), 1, inputFile);  //
	fread(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, inputFile); //
	fread(&bmpFile2, sizeof(BITMAPFILEHEADER), 1, inputFile2);  //
	fread(&bmpInfo2, sizeof(BITMAPINFOHEADER), 1, inputFile2); //

	int width = bmpInfo.biWidth;
	int height = bmpInfo.biHeight;
	int size = bmpInfo.biSizeImage;
	int bitCnt = bmpInfo.biBitCount;
	int stride = (((bitCnt / 8) * width) + 3) / 4 * 4;
	printf("W: %d(%d)\nH: %d\nS: %d\nD: %d\n", width, stride, height, size, bitCnt);
	//#define wid = width


	unsigned char* inputImg = NULL, * outputImg = NULL, * inputImg2 = NULL, * Y1 = NULL, * Y2 = NULL, * Y3 = NULL, * sumHist = NULL, * Y4 = NULL;
	inputImg = (unsigned char*)calloc(size, sizeof(unsigned char));
	inputImg2 = (unsigned char*)calloc(size, sizeof(unsigned char));
	outputImg = (unsigned char*)calloc(size, sizeof(unsigned char));
	fread(inputImg, sizeof(unsigned char), size, inputFile);
	fread(inputImg2, sizeof(unsigned char), size, inputFile2); //inputImg2가 오른쪽으로 밀려서 저장됨
	//Y2 = (unsigned char*)calloc(width*height, sizeof(unsigned char));

	//for (int j = 0; j < height; j++)
	//{
	//	for (int i = 0; i < width; i++)
	//	{
	//		Y2[j * width + i] = inputImg2[j * stride + 3 * i + 0];
	//	}
	//}

	//for (int j = 0; j < height; j++)
	//{
	//	for (int i = 0; i < width; i++)
	//	{
	//		outputImg[j * stride + 3 * i + 0] = Y2[j * width + i];
	//		outputImg[j * stride + 3 * i + 1] = Y2[j * width + i];
	//		outputImg[j * stride + 3 * i + 2] = Y2[j * width + i];

	//	}
	//}

	double  Cr, Cb, R, G, B, I;// Y1, Y2;
	int* Y, * intY2, sum = 0;
	float gamma;
	double Diff_Y, MSE = 0, PSNR;
	double a, b;
	int Err, k = 0;
	Y = (int*)calloc(width * height, sizeof(int));
	intY2 = (int*)calloc(width * height, sizeof(int));
	Y1 = (unsigned char*)calloc(width * height, sizeof(unsigned char));
	Y2 = (unsigned char*)calloc(width * height, sizeof(unsigned char));
	Y3 = (unsigned char*)calloc(width * height, sizeof(unsigned char));
	Y4 = (unsigned char*)calloc(width * height, sizeof(unsigned char));
	sumHist = (unsigned char*)calloc(width * height, sizeof(unsigned char));

	//canf("%d", &Err);
	scanf("%f",&gamma);

	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			Y1[j * width + i] = inputImg[j * stride + 3 * i + 0];

		}
	}

	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			Y2[j * width + i] = (unsigned char)(255 * pow(Y1[j * width + i]/255.0,gamma) );

		}
	}





	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{

			//MSE = MSE + (double)((Y1[j * width + i] - Y2[j * width + i]) * (Y1[j * width + i] - Y2[j * width + i]));
			//printf("%lf\n",MSE);
			outputImg[j * stride + 3 * i + 0] = Y2[j * width + i];
			outputImg[j * stride + 3 * i + 1] = Y2[j * width + i];
			outputImg[j * stride + 3 * i + 2] = Y2[j * width + i];

		}
	}



	//FILE* outputFile = fopen("CumulativeHisto.bmp", "wb"); //출력체크용
	FILE* outputFile = fopen("GAMMA AICenterY.bmp", "wb"); //출력체크용
	fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile);
	fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile);
	fwrite(outputImg, sizeof(unsigned char), size, outputFile);

	free(Y1);
	free(Y2);
	free(outputImg);
	free(inputImg);
	free(inputImg2);
	fclose(inputFile);
	fclose(inputFile2);
	fclose(outputFile);

	return 0;
}

/*
Historgram Equalization이란 콘트라스트를 평평? 공평하게 만드는것임 단순 밝기를 줄이고늘이고가 아니라 전제적으로 균등? 하게 만든다는거
축적히스토그램으로 표현 하겠다?
*/