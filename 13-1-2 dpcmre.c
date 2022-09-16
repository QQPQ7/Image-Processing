#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <math.h>
#include <Windows.h>
#include <wingdi.h>

void Binary(int num);
int main()
{
	BITMAPFILEHEADER bmpFile;
	BITMAPINFOHEADER bmpInfo;
	BITMAPFILEHEADER bmpFile2;
	BITMAPINFOHEADER bmpInfo2;
	FILE* inputFile = NULL;
	FILE* inputFile2 = NULL;
	FILE* txtoutputFile = NULL;
	inputFile = fopen("AICenterY.bmp", "rb");//rb 바이너리로 읽기
	inputFile2 = fopen("UPsampledoutput.bmp", "rb");//rb 바이너리로 읽기

	txtoutputFile = fopen("DPencodtest.txt", "wb");

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

	unsigned char* inputImg = NULL, * outputImg = NULL, * inputImg2 = NULL, * Y4 = NULL;
	char* Y1 = NULL, * Y2 = NULL;
	char* Y3 = NULL;
	inputImg = (unsigned char*)calloc(size, sizeof(unsigned char));
	inputImg2 = (unsigned char*)calloc(size, sizeof(unsigned char));
	outputImg = (unsigned char*)calloc(size, sizeof(unsigned char));
	fread(inputImg, sizeof(unsigned char), size, inputFile);
	fread(inputImg2, sizeof(unsigned char), size, inputFile2); //inputImg2가 오른쪽으로 밀려서 저장됨


	double Y, Cr, Cb, R, G, B, I;// Y1, Y2;
	double Diff_Y, MSE = 0, PSNR;
	double a, b, Grad;
	Y1 = (char*)calloc(width * height, sizeof(char));
	Y2 = (char*)calloc(width * height, sizeof(char));
	Y3 = (char*)calloc(width * height, sizeof(char));
	Y4 = (unsigned char*)calloc(width * height, sizeof(unsigned char));
	double pi = 3.1415926535;
	double theta = pi / 4;
	int cy = width / 2;
	int cx = height / 2;
	int x2, y2;
	int edge[3] = { -1, 0, 1 };
	int mean = 0;
	int Dilation[9] = { 0,0,0,0,0,0,0,0,0 };
	int Erosion[9] = { 255, 255, 255, 255, 255, 255, 255, 255, 255 };
	char* bi[8];
	bi[0] = "000";
	bi[1] = "001";
	bi[2] = "010";
	bi[3] = "011";
	bi[4] = "100";
	bi[5] = "101";
	bi[6] = "110";
	bi[7] = "111";

	int div;
	int Qan[16] = { NULL };

	int mid = 128;

	//for (int j = 0;j<8;j++)
	//{
	//	printf("%s \n", bi[j]);
	//}

	//scanf("%d", &div);
	div = 5;


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

			if (i == 0)
			{
				Y2[j * width + i] = Y1[j * width + i] - 128;
			}
			else
			{
				Y2[j * width + i] = Y1[j * width + i] - Y1[j * width + i - 1];
				//printf("%d ", Y2[j * width + i]);
			}

			Y2[j * width + i] = Y2[j * width + i] / div;

			//printf("%d ", Y2[j * width + i]);
		}

	}

	//-7~7
	/*
	-3 < 000
	7 =1111
	*/

	//인코딩 기록
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			//fprintf(txtoutputFile, "%d ", Y2[j * width + i]);
			if (Y2[j * width + i] < -3)
				fprintf(txtoutputFile, "%s ", bi[0]);
			else if (Y2[j * width + i] == -3)
				fprintf(txtoutputFile, "%s ", bi[1]);
			else if (Y2[j * width + i] == -2)
				fprintf(txtoutputFile, "%s ", bi[2]);
			else if (Y2[j * width + i] == -1)
				fprintf(txtoutputFile, "%s ", bi[3]);
			else if (Y2[j * width + i] == 0)
				fprintf(txtoutputFile, "%s ", bi[4]);
			else if (Y2[j * width + i] == 1)
				fprintf(txtoutputFile, "%s ", bi[5]);
			else if (Y2[j * width + i] == 2)
				fprintf(txtoutputFile, "%s ", bi[6]);
			else if (Y2[j * width + i] > 2)
				fprintf(txtoutputFile, "%s ", bi[7]);
		}
	}
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			//fprintf(txtoutputFile, "%d ", Y2[j * width + i]);
			if (Y2[j * width + i] < -3)
				Y2[j * width + i] = -4;

			else if (Y2[j * width + i] > 2)
				Y2[j * width + i] = 3;
		}
	}



	fclose(txtoutputFile);

	FILE* txtoutputFile2 = NULL;
	FILE* txtoutputFile3 = NULL;
	txtoutputFile2 = fopen("DPencodtest.txt", "r");
	txtoutputFile3 = fopen("DPencodtest2.txt", "wb");
	char* test[10];

	//디코딩읽기
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			//fread(test, 3, 1, txtoutputFile2);
			fscanf(txtoutputFile2, "%s", test); // &Y3[j * width + i]);
			//getchar();
			//printf("%s %s\n", test, bi[1]); 
			//printf("%s ", test); 

			fprintf(txtoutputFile3, "%s ", test);

			if (strcmp(test, bi[0]) == 0)
			{
				//printf("1 ");
				Y3[j * width + i] = (-4);
				//printf("%d ", Y3[j * width + i]);
			}
			else if (strcmp(test, bi[1]) == 0)
			{
				//printf("2 ");
				Y3[j * width + i] = (-3);
				//printf("%d ", Y3[j * width + i]);
			}
			else if (strcmp(test, bi[2]) == 0)
			{
				//printf("3 ");
				Y3[j * width + i] = (-2);
				//printf("%d ", Y3[j * width + i]);
			}
			else if (strcmp(test, bi[3]) == 0)
			{
				//printf("4 ");
				Y3[j * width + i] = (-1);
				//printf("%d ", Y3[j * width + i]);
			}
			else if (strcmp(test, bi[4]) == 0)
			{
				//printf("5 ");
				Y3[j * width + i] = 0;
				//printf("%d ", Y3[j * width + i]);
			}
			else if (strcmp(test, bi[5]) == 0)
			{
				//printf("6 ");
				Y3[j * width + i] = 1;
				//printf("%d ", Y3[j * width + i]);
			}
			else if (strcmp(test, bi[6]) == 0)
			{
				//printf("7 ");
				Y3[j * width + i] = 2;
				//printf("%d ", Y3[j * width + i]);
			}
			else if (strcmp(test, bi[7]) == 0)
			{
				//printf("8 ");
				Y3[j * width + i] = 3;
				//printf("%d ", Y3[j * width + i]);
			}

			//printf("%d ", Y3[j * width + i]);

		}
	}

	//for (int j = 0; j < height; j++)
	//{
	//	for (int i = 0; i < width; i++)
	//	{

	//		fscanf(txtoutputFile2, "%d ", &Y3[j * width + i]);

	//		Y3[j * width + i] = Y3[j * width + i] * div;

	//	}
	//}


	//디코딩
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{

			Y3[j * width + i] = Y3[j * width + i] * div;
			Y2[j * width + i] = Y2[j * width + i] * div;
		}
	}

	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{

			//printf("%d ", Y3[j * width + i]);
			if (i == 0)
			{
				Y4[j * width + i] = Y2[j * width + i] + 128;
				//printf("%d ", Y4[j * width + i]);
			}
			else
			{
				if (Y4[j * width + i - 1] + Y2[j * width + i]> 255)
				{
					Y4[j * width + i] = 255;
					//mean = 255;
					//printf("%d ", Y4[j * width + i]);
				}
				else if (Y4[j * width + i - 1] + Y2[j * width + i] < 0)
				{
					//mean = 0;
					//printf("%d ", mean);
					Y4[j * width + i] = 0;
					//printf("%d ", Y4[j * width + i]);
				}
				else
				{
					//mean = Y4[j * width + i-1 ] + Y2[j * width + i];
					Y4[j * width + i] = Y4[j * width + i - 1] + Y2[j * width + i];
					//printf("%d ", Y4[j * width + i]);
				}
				//Y4[j * width + i] =mean;
				//printf("%d %d", Y3[j * width + i-1], Y3[j * width + i]);

			}


			//printf("%d ", Y4[j * width + i]);

		}
	}
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			MSE = MSE + (double)((Y2[j * width + i] - Y3[j * width + i]) * (Y2[j * width + i] - Y3[j * width + i])); //비교시 psnr 31 나옴 손실 전후고려하면 비슷한거 같은데
		}
	}
	MSE = MSE / (double)(height * width);
	printf("MSE: %lf\n", MSE);
	PSNR = 10 * log10(255 * 255 / MSE);
	PSNR = MSE != 0.0 ? 10.0 * log10(255 * 255 / MSE) : 99.99;
	printf("PSNR: %lf\n", PSNR);


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

	FILE* outputFile = fopen("OutputDPCMtest.bmp", "wb"); //출력체크용
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
	fclose(txtoutputFile2);

	return 0;
}

//void Binary(int num)
//{
//	if (num == 0)
//	{
//		return;
//	}
//	PrintBinary2(num / 2);
//	printf("%d", num % 2);
//}
//아래거참고한듯
// https://blog.naver.com/PostView.nhn?isHttpsRedirect=true&blogId=dic1224&logNo=220840877845&redirect=Dlog&widgetTypeCall=true&directAccess=false
// https://jeak.tistory.com/20?category=827380
//https://m.blog.naver.com/liveforu/222030359973
