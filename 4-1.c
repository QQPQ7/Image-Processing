#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <math.h>
#include <Windows.h>
#include <wingdi.h>

int main()
{
	BITMAPFILEHEADER bmpFile;
	BITMAPINFOHEADER bmpInfo;
	FILE* inputFile = NULL;
	FILE *inputFile2 = NULL;
	inputFile = fopen("AICenter.bmp", "rb");//rb 바이너리로 읽기
	inputFile2 = fopen("SejongMark.bmp", "rb");//rb 바이너리로 읽기
	
	fread(&bmpFile, sizeof(BITMAPFILEHEADER), 1, inputFile);
	fread(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, inputFile);

	int width = bmpInfo.biWidth;
	int height = bmpInfo.biHeight;
	int size = bmpInfo.biSizeImage;
	int bitCnt = bmpInfo.biBitCount;
	int stride = (((bitCnt / 8) * width) + 3) / 4 * 4; //+3 해주는건 읽기용 조치임
	printf("W: %d(%d)\nH: %d\nS: %d\nD: %d\n", width, stride, height, size, bitCnt);

	unsigned char* inputImg = NULL, * outputImg = NULL, * inputImg2 = NULL;// , * Y = NULL, * Cr = NULL, * Cb = NULL, * R = NULL, * G = NULL, * B = NULL;
	inputImg = (unsigned char*)calloc(size, sizeof(unsigned char));
	inputImg2 = (unsigned char*)calloc(size, sizeof(unsigned char));
	outputImg = (unsigned char*)calloc(size, sizeof(unsigned char));
	double Y,Cr,Cb,R,G,B,I, Y1,Y2;
	double Diff_Y;
	double a, b;
	/*
	Y = (unsigned char*)calloc(size, sizeof(unsigned char));
	Cb = (unsigned char*)calloc(size, sizeof(unsigned char));
	Cr = (unsigned char*)calloc(size, sizeof(unsigned char));
	R = (unsigned char*)calloc(size, sizeof(unsigned char));
	G = (unsigned char*)calloc(size, sizeof(unsigned char));
	B = (unsigned char*)calloc(size, sizeof(unsigned char));
	*/
	fread(inputImg, sizeof(unsigned char), size, inputFile);
	fread(inputImg2, sizeof(unsigned char), size, inputFile2);
	
	scanf("%lf %lf",&a,&b );
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			Y = (unsigned char)(0.299 * inputImg[j * stride + 3 * i + 2] + 0.587 * inputImg[j * stride + 3 * i + 1] + 0.114 * inputImg[j * stride + 3 * i + 0]);
			Cr = (unsigned char)(-0.169 * inputImg[j * stride + 3 * i + 2] - 0.331 * inputImg[j * stride + 3 * i + 1] + 0.114 * inputImg[j * stride + 3 * i + 0]);
			Cb = (unsigned char)(0.299 * inputImg[j * stride + 3 * i + 2] + 0.587 * inputImg[j * stride + 3 * i + 1] + 0.114 * inputImg[j * stride + 3 * i + 0]);
			I= (unsigned char)(( inputImg[j * stride + 3 * i + 2] + inputImg[j * stride + 3 * i + 1] +  inputImg[j * stride + 3 * i + 0])/3);

			R = Y + 1.403 *Cr; 
			G = Y - 0.344*Cb - 0.714*Cr;
			B = Y + 1.770*Cb;

			Diff_Y = (Y - I)* (Y - I);
			Y1= (unsigned char)(0.299 * inputImg[j * stride + 3 * i + 2] + 0.587 * inputImg[j * stride + 3 * i + 1] + 0.114 * inputImg[j * stride + 3 * i + 0]);
			Y2 = (unsigned char)(0.299 * inputImg2[j * stride + 3 * i + 2] + 0.587 * inputImg2[j * stride + 3 * i + 1] + 0.114 * inputImg2[j * stride + 3 * i + 0]);
			
			
			Y = Y1 / a + Y2 / b;
			//Y > 255 ? 255 : Y;
			//Y < 0 ? 0 : Y;
			if (Y > 255) Y = 255;
			if (Y < 0) Y = 0;
			//printf("%lf", Y);


			outputImg[j * stride + 3 * i + 0] = Y;
			outputImg[j * stride + 3 * i + 1] = Y;
			outputImg[j * stride + 3 * i + 2] = Y;
			if (Diff_Y > 255) Diff_Y = 255;
			if (Diff_Y < 0) Diff_Y = 0;
			//outputImg[j * stride + 3 * i + 0] = I;
			//outputImg[j * stride + 3 * i + 1] = I;
			//outputImg[j * stride + 3 * i + 2] = I;


		
			//Diff_Y = Y - I;//(inputImg[j * stride + 3 * i + 2] - inputImg2[j * stride + 3 * i + 2])* (inputImg[j * stride + 3 * i + 2] - inputImg2[j * stride + 3 * i + 2]);
		}
	}
	//Diff_Y = Diff_Y / (512 * 512);
	//print("%lf", Diff_Y);

	FILE* outputFile = fopen("Output2.bmp", "wb");
	fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile);
	fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile);
	fwrite(outputImg, sizeof(unsigned char), size, outputFile);

	free(outputImg);
	free(inputImg);
	free(inputImg2);
	fclose(inputFile);
	fclose(inputFile2);
	fclose(outputFile);

	return 0;
}