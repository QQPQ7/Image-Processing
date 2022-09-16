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
	BITMAPFILEHEADER bmpFile2;
	BITMAPINFOHEADER bmpInfo2;
	FILE* inputFile = NULL;
	FILE* inputFile2 = NULL;
	inputFile = fopen("AICenterY.bmp", "rb");//rb 바이너리로 읽기
	inputFile2 = fopen("UPsampledoutput.bmp", "rb");//rb 바이너리로 읽기

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

	unsigned char* inputImg = NULL, * outputImg = NULL, * inputImg2 = NULL, * Y1 = NULL, * Y2 = NULL, * Y3 = NULL, * Y4 = NULL;
	inputImg = (unsigned char*)calloc(size, sizeof(unsigned char));
	inputImg2 = (unsigned char*)calloc(size, sizeof(unsigned char));
	outputImg = (unsigned char*)calloc(size, sizeof(unsigned char));
	fread(inputImg, sizeof(unsigned char), size, inputFile);
	fread(inputImg2, sizeof(unsigned char), size, inputFile2); //inputImg2가 오른쪽으로 밀려서 저장됨


	double Y, Cr, Cb, R, G, B, I;// Y1, Y2;
	double Diff_Y, MSE = 0, PSNR;
	double a, b, Grad;
	Y1 = (unsigned char*)calloc(width * height, sizeof(unsigned char));
	Y2 = (unsigned char*)calloc(width * height, sizeof(unsigned char));
	Y3 = (unsigned char*)calloc(width * height, sizeof(unsigned char));
	Y4 = (unsigned char*)calloc(width * height, sizeof(unsigned char));
	double pi = 3.1415926535;
	double theta = pi / 4;
	int cy = width / 2;
	int cx = height / 2;
	int x2, y2;
	int edge[3] = { -1, 0, 1 };
	int mean = 0;

	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			Y1[j * width + i] = inputImg[j * stride + 3 * i + 0];
			Y2[j * width + i] = 0;
			Y3[j * width + i] = 0;

		}
	}
	//IX
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			if (j == 0 || i == 0 || j == height - 1 || i == width - 1)
			{
				if (j == 0 && i == 0)
					mean = (-Y1[(j)*width + i] + Y1[(j)*width + i + 1] - 2*Y1[j * width + i] + 2*Y1[j * width + i + 1] - Y1[(j + 1) * width + i] + Y1[(j + 1) * width + i + 1]);
				else if (j == height - 1 && i == width - 1)
					mean = (-Y1[(j - 1) * width + i - 1] + Y1[(j - 1) * width + i] - 2*Y1[j * width + i - 1] +2*Y1[j * width + i] - Y1[(j)*width + i - 1] + Y1[(j)*width + i]);
				else if (j == 0)
					mean = (-Y1[(j)*width + i - 1] + Y1[(j)*width + i + 1] - 2*Y1[j * width + i - 1] + 2*Y1[j * width + i + 1] - Y1[(j + 1) * width + i - 1] + Y1[(j + 1) * width + i + 1]);
				else if (i == 0)
					mean = (-Y1[(j - 1) * width + i] + Y1[(j - 1) * width + i + 1] - 2*Y1[j * width + i] + 2*Y1[j * width + i + 1] - Y1[(j + 1) * width + i] + Y1[(j + 1) * width + i + 1]);
				else if (j == height - 1)
					mean = (-Y1[(j - 1) * width + i - 1] + Y1[(j - 1) * width + i + 1] - 2*Y1[j * width + i - 1] + Y1[j * width + i + 1] - Y1[(j)*width + i - 1] + Y1[(j)*width + i + 1]);
				else if (i == width - 1)
					mean = (-Y1[(j - 1) * width + i - 1] + Y1[(j - 1) * width + i] - 2*Y1[j * width + i - 1] + 2*Y1[j * width + i + 1] - Y1[(j + 1) * width + i - 1] + Y1[(j + 1) * width + i]);
			}
			else
			{
				mean = (-Y1[(j - 1) * width + i - 1] + Y1[(j - 1) * width + i + 1] - 2*Y1[j * width + i - 1] + 2*Y1[j * width + i + 1] - Y1[(j + 1) * width + i - 1] + Y1[(j + 1) * width + i + 1]);
			}
			if (mean > 255)
				mean = 255;
			else if (mean < 0)
				mean = 0;

			Y2[j * width + i] = mean;
		}
	}
	//IY
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{

			if (j == 0 || i == 0 || j == height - 1 || i == width - 1)
			{
				if (j == 0 && i == 0)
					mean = (-Y1[(j)*width + i] -2*Y1[(j)*width + i] - Y1[j * width + i+1] + Y1[(j + 1) * width + i + 1] +2* Y1[(j + 1) * width + i] + Y1[(j + 1) * width + i + 1]);
				else if (j == height - 1 && i == width - 1)
					mean = (-Y1[(j - 1) * width + i - 1] - 2*Y1[(j - 1) * width + i] - Y1[j * width + i] + Y1[j * width + i-1] +2*Y1[(j)*width + i ] + Y1[(j)*width + i]);
				else if (j == 0)
					mean = (-Y1[(j)*width + i - 1] - 2*Y1[(j)*width + i + 1] - Y1[j * width + i + 1] + Y1[j * width + i + 1] +2*Y1[(j + 1) * width + i - 1] + Y1[(j + 1) * width + i + 1]);
				else if (i == 0)
					mean = (-Y1[(j - 1) * width + i] -2*Y1[(j - 1) * width + i] - Y1[j * width + i+1] + Y1[j * width + i + 1] + 2*Y1[(j + 1) * width + i] + Y1[(j + 1) * width + i + 1]);
				else if (j == height - 1)
					mean = (-Y1[(j - 1) * width + i - 1] - 2*Y1[(j - 1) * width + i] - Y1[j * width + i + 1] + Y1[j * width + i - 1] + 2*Y1[(j)*width + i] + Y1[(j)*width + i + 1]);
				else if (i == width - 1)
					mean = (-Y1[(j - 1) * width + i - 1] - 2*Y1[(j - 1) * width + i] - Y1[j * width + i ] + Y1[(j+1) * width + i + 1] + 2*Y1[(j + 1) * width + i - 1] + Y1[(j + 1) * width + i]);
			}
			else
			{
				mean = (-Y1[(j - 1) * width + i - 1] - 2*Y1[(j - 1) * width + i] - Y1[(j - 1) * width + i + 1] + Y1[(j + 1) * width + i - 1] +2*Y1[(j + 1) * width + i] + Y1[(j + 1) * width + i + 1]);
			}
			if (mean > 255)
				mean = 255;
			else if (mean < 0)
				mean = 0;

			Y3[j * width + i] = mean;
		}
	}
	//gradient
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{

			Grad = sqrt(Y2[j * width + i] * Y2[j * width + i] + Y3[j * width + i] * Y3[j * width + i]);
			

			//if (Grad > 255)
			//	Grad = 255;
			//else if (Grad < 0)
			//	Grad = 0;

			//theshold만들기
			/*if (Grad < 100)
				Grad = 0;*/
			
			if (Grad > 255)
				Grad = 255;
			else if (Grad < 0)
				Grad = 0;


			Y4[j * width + i] = Grad;
		}
	}

	double gx, gy,g;

	for (int j = 1; j < height - 1; j++) {
		for (int i = 1; i < width - 1; i++) {
			gx = Y1[(j - 1) * width + i + 1] + 2 * Y1[(j)*width + i + 1] + Y1[(j + 1) * width + i + 1] - Y1[(j - 1) * width + i - 1] - 2 * Y1[(j)*width + i - 1] - Y1[(j + 1) * width + i - 1];
			gy = Y1[(j + 1) * width + i - 1] + 2 * Y1[(j + 1) * width + i] + Y1[(j + 1) * width + i - 1] - Y1[(j - 1) * width + i - 1] - 2 * Y1[(j - 1) * width + i] - Y1[(j - 1) * width + i + 1];
			g = sqrt(gx * gx + gy * gy);
			Y4[j * width + i] = (unsigned char)(g > 255 ? 255 : (g < 0 ? 0 : g));
		}
	}













	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{

			//MSE = MSE + (double)((Y1[j * width + i] - Y2[j * width + i]) * (Y1[j * width + i] - Y2[j * width + i]));
			//printf("%lf\n",MSE);
			outputImg[j * stride + 3 * i + 0] = Y4[j * width + i];
			outputImg[j * stride + 3 * i + 1] = Y4[j * width + i];
			outputImg[j * stride + 3 * i + 2] = Y4[j * width + i];

		}
	}
	//MSE = MSE / (double)(height * width);
	//printf("MSE: %lf\n", MSE);
	//PSNR = 10 * log10(255 * 255 / MSE);
	//PSNR = MSE !=0.0? 10.0 * log10(255 * 255 / MSE):99.99;
	//printf("PSNR: %lf\n", PSNR);
	//Diff_Y = Diff_Y / (512 * 512);
	//print("%lf", Diff_Y);

	FILE* outputFile = fopen("OutputSobelFilter.bmp", "wb"); //출력체크용
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
//아래거참고한듯
//https://birthxmas.tistory.com/235