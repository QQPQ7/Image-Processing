#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <math.h>
#include <Windows.h>
#include <wingdi.h>


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


	int ratio = 2;
	int width2 = bmpInfo.biWidth >> ratio;
	int height2 = bmpInfo.biHeight >> ratio;
	int stride2 = (((bitCnt / 8) * width2) + 3) / 4 * 4;
	int size2 = stride2 * height2;

	unsigned char* inputImg = NULL, * outputImg = NULL, * inputImg2 = NULL, * Y1 = NULL, * Y2 = NULL;
	inputImg = (unsigned char*)calloc(size, sizeof(unsigned char));
	inputImg2 = (unsigned char*)calloc(size, sizeof(unsigned char));

	outputImg = (unsigned char*)calloc(size2, sizeof(unsigned char));
	fread(inputImg, sizeof(unsigned char), size, inputFile);
	fread(inputImg2, sizeof(unsigned char), size, inputFile2); //inputImg2가 오른쪽으로 밀려서 저장됨



	double  Cr, Cb, R, G, B, I;// Y1, Y2;
	int* Y;
	double Diff_Y, MSE = 0, PSNR;
	double a, b;
	int Err, k = 0;
	Y = (int*)calloc(width * height, sizeof(int));
	Y1 = (unsigned char*)calloc(width * height, sizeof(unsigned char));

	Y2 = (unsigned char*)calloc(width2 * height2, sizeof(unsigned char));

	//canf("%d", &Err);
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			Y1[j * width + i] = inputImg[j * stride + 3 * i + 0];
		}
	}



	for (int j = 0; j < height2; j++)
	{
		for (int i = 0; i < width2; i++)
		{
			Y2[j * width2 + i] = Y1[(j<<ratio) * width +( i<<ratio)];
		}
	}



	for (int j = 0; j < height2; j++)
	{
		for (int i = 0; i < width2; i++)
		{

			//MSE = MSE + (double)((Y1[j * width + i] - Y2[j * width + i]) * (Y1[j * width + i] - Y2[j * width + i]));
			//printf("%lf\n",MSE);
			outputImg[j * stride2 + 3 * i + 0] = Y2[j * width2 + i];
			outputImg[j * stride2 + 3 * i + 1] = Y2[j * width2 + i];
			outputImg[j * stride2 + 3 * i + 2] = Y2[j * width2 + i];

		}
	}


	FILE* outputFile = fopen("Downsampledoutput.bmp", "wb"); //출력체크용
	bmpInfo.biWidth= width2;
	bmpInfo.biHeight= height2;
	bmpInfo.biSizeImage= size2;
	bmpFile.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + size2;


	fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile);
	fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile);
	fwrite(outputImg, sizeof(unsigned char), size2, outputFile);

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

