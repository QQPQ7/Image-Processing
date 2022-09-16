#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <math.h>
#include <Windows.h>
#include <wingdi.h>

int wid = 512;
int hei = 512;
void filter(int j, unsigned char *Y);

int main()
{
	BITMAPFILEHEADER bmpFile;
	BITMAPINFOHEADER bmpInfo;
	BITMAPFILEHEADER bmpFile2;
	BITMAPINFOHEADER bmpInfo2;
	FILE* inputFile = NULL;
	FILE* inputFile2 = NULL;
	inputFile = fopen("AICenterY_CombinedNoise.bmp", "rb");//rb 바이너리로 읽기
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


	unsigned char* inputImg = NULL, * outputImg = NULL, * inputImg2 = NULL, * Y1 = NULL, * Y2 = NULL, *Y3=NULL;
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

	double Y, Cr, Cb, R, G, B, I;// Y1, Y2;
	double Diff_Y, MSE = 0, PSNR;
	double a, b;
	int Err,d=0,tmp;
	unsigned char* sort= NULL;
	Y1 = (unsigned char*)calloc(width * height, sizeof(unsigned char));
	Y2 = (unsigned char*)calloc(width * height, sizeof(unsigned char));
	Y3 = (unsigned char*)calloc(width * height, sizeof(unsigned char));
	sort = (unsigned char*)calloc(9, sizeof(unsigned char));


	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			Y1[j * width + i] = inputImg[j * stride + 3 * i + 0];
			Y3[j * width + i] = inputImg2[j * stride + 3 * i + 0];
		}
	}


	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{

			//if (i == 0 || j == 0 || i == height - 1 || j == width - 1)
			//{
			//	Y2[j * width + i] = Y1[j * width + i]; //위의 조건일 때 입력 이미지 값을 그대로출력 이미지 값에 넣는다.


			//}

			//else
			//{
				for (int b = -1; b < 2; b++)       //중간 값을 찾기 위한 9개의 배열 값을 임의의 배열에 저장
				{
					for (int a = 0; a < 3; a++)
					{
						sort[d] = Y1[(j + b) * width + (i - 1) + a];
						d++;
					}
				}
				d = 0;

				for (int k = 0; k < 9; k++)     //임의의 배열에 저장 된 9개의 값을 Bubble Sorting
				{
					for (int n = 0; n < 8; n++)
					{
						if (sort[n] > sort[n + 1])
						{
							tmp = sort[n];
							sort[n] = sort[n + 1];
							sort[n + 1] = tmp;
						}
					}
				}
				Y2[j * width + i] = sort[4];
			//}
		}
	}


	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{

			MSE = MSE + (double)((Y3[j * width + i] - Y2[j * width + i]) * (Y3[j * width + i] - Y2[j * width + i]));
			//printf("%lf\n",MSE);
			outputImg[j * stride + 3 * i + 0] = Y2[j * width + i];
			outputImg[j * stride + 3 * i + 1] = Y2[j * width + i];
			outputImg[j * stride + 3 * i + 2] = Y2[j * width + i];

		}
	}
	MSE = MSE / (double)(height * width);
	printf("MSE: %lf\n", MSE);
	PSNR = 10 * log10(255 * 255 / MSE);
	//PSNR = MSE !=0.0? 10.0 * log10(255 * 255 / MSE):99.99;
	printf("PSNR: %lf\n", PSNR);
	//Diff_Y = Diff_Y / (512 * 512);
	//print("%lf", Diff_Y);

	FILE* outputFile = fopen("OutputFilter.bmp", "wb"); //출력체크용
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


void  filter(int k,int wid, int hei,unsigned char *Y)
{
	int i, j;
	unsigned char sum=0;
	
	if (wid > 3 && hei>3)
	{
		for (j = -1; j < 2; j++)
		{
			for (i = -1; i < 2; i++)
			{
				sum += Y[k+i+j*wid];
			}
		}
		sum - Y[k];
		Y[k] = sum / 8;
	}
	else
		return Y[k];
}





