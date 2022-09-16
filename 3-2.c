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
	inputFile = fopen("AICenter.bmp", "rb");//rb 바이너리로 읽기
	fread(&bmpFile, sizeof(BITMAPFILEHEADER), 1, inputFile);
	fread(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, inputFile);

	int width = bmpInfo.biWidth;
	int height = bmpInfo.biHeight;
	int size = bmpInfo.biSizeImage;
	int bitCnt = bmpInfo.biBitCount;
	int stride = (((bitCnt / 8) * width) + 3) / 4 * 4; //+3 해주는건 읽기용 조치임
	printf("W: %d(%d)\nH: %d\nS: %d\nD: %d\n", width, stride, height, size, bitCnt);

	unsigned char* inputImg = NULL, * outputImg = NULL,*Y=NULL, *Cr=NULL,*Cb=NULL,*R=NULL,*G=NULL,*B=NULL;
	inputImg = (unsigned char*)calloc(size, sizeof(unsigned char));
	outputImg = (unsigned char*)calloc(size, sizeof(unsigned char));
	fread(inputImg, sizeof(unsigned char), size, inputFile);








	/*
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			outputImg[j * stride + 3 * i + 0] = inputImg[j * stride + 3 * i + 0];
			outputImg[j * stride + 3 * i + 1] = inputImg[j * stride + 3 * i + 1];
			outputImg[j * stride + 3 * i + 2] = inputImg[j * stride + 3 * i + 2];
		}
	}
	*/

	//위아래뒤집기
	/*
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			outputImg[j * stride + 3 * i + 0] = inputImg[(height - j) * stride + 3 * i + 0];
			outputImg[j * stride + 3 * i + 1] = inputImg[(height - j) * stride + 3 * i + 1];
			outputImg[j * stride + 3 * i + 2] = inputImg[(height - j) * stride + 3 * i + 2];
		}
	}
		*/

	//위아래 좌우 뒤집
	/*
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			outputImg[j * stride + 3 * i + 0] = inputImg[(height-j) * stride + 3 * (width-i) + 0];
			outputImg[j * stride + 3 * i + 1] = inputImg[(height - j) * stride + 3 * (width - i) + 1];
			outputImg[j * stride + 3 * i + 2] = inputImg[(height - j) * stride + 3 * (width - i) + 2];
		}
	}
	*/

	/*
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			outputImg[j * stride + 3 * i + 0] = inputImg[(height - j) * stride + 3 * i + 0];
			outputImg[j * stride + 3 * i + 1] = inputImg[(height - j) * stride + 3 * i + 1];
			outputImg[j * stride + 3 * i + 2] = inputImg[(height - j) * stride + 3 * i + 2];
			if (j < 100 && i < 100)
			{
				outputImg[j * stride + 3 * i + 0] = 0;
				outputImg[j * stride + 3 * i + 1] = 0;
				outputImg[j * stride + 3 * i + 2] = 0;
			}
			if (j > 400 && i > 400)
			{
				outputImg[j * stride + 3 * i + 0] = 255;
				outputImg[j * stride + 3 * i + 1] = 255;
				outputImg[j * stride + 3 * i + 2] = 255;
			}
		}
	}
	*/
		for (int j = 0; j < height; j++)
		{
			for (int i = 0; i < width; i++)
			{
				Y = (unsigned char)(0.299 * inputImg[j * stride + 3 * i + 2] + 0.587 * inputImg[j * stride + 3 * i + 1]+ 0.114 * inputImg[j * stride + 3 * i + 0]);
				Cr = (unsigned char)(-0.169 * inputImg[j * stride + 3 * i + 2] - 0.331 * inputImg[j * stride + 3 * i + 1] + 0.114 * inputImg[j * stride + 3 * i + 0]);
				Cb = (unsigned char)(0.299 * inputImg[j * stride + 3 * i + 2] + 0.587 * inputImg[j * stride + 3 * i + 1] + 0.114 * inputImg[j * stride + 3 * i + 0]);

				outputImg[j * stride + 3 * i + 0] = Y;
				outputImg[j * stride + 3 * i + 1] = Y; #define _CRT_SECURE_NO_WARNINGS
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
					inputFile = fopen("AICenter.bmp", "rb");//rb 바이너리로 읽기
					fread(&bmpFile, sizeof(BITMAPFILEHEADER), 1, inputFile);
					fread(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, inputFile);

					int width = bmpInfo.biWidth;
					int height = bmpInfo.biHeight;
					int size = bmpInfo.biSizeImage;
					int bitCnt = bmpInfo.biBitCount;
					int stride = (((bitCnt / 8) * width) + 3) / 4 * 4; //+3 해주는건 읽기용 조치임
					printf("W: %d(%d)\nH: %d\nS: %d\nD: %d\n", width, stride, height, size, bitCnt);

					unsigned char* inputImg = NULL, * outputImg = NULL;// , * Y = NULL, * Cr = NULL, * Cb = NULL, * R = NULL, * G = NULL, * B = NULL;
					inputImg = (unsigned char*)calloc(size, sizeof(unsigned char));
					outputImg = (unsigned char*)calloc(size, sizeof(unsigned char));
					double Y, Cr, Cb, R, G, B;
					/*
					Y = (unsigned char*)calloc(size, sizeof(unsigned char));
					Cb = (unsigned char*)calloc(size, sizeof(unsigned char));
					Cr = (unsigned char*)calloc(size, sizeof(unsigned char));
					R = (unsigned char*)calloc(size, sizeof(unsigned char));
					G = (unsigned char*)calloc(size, sizeof(unsigned char));
					B = (unsigned char*)calloc(size, sizeof(unsigned char));
					*/
					fread(inputImg, sizeof(unsigned char), size, inputFile);








					/*
					for (int j = 0; j < height; j++)
					{
						for (int i = 0; i < width; i++)
						{
							outputImg[j * stride + 3 * i + 0] = inputImg[j * stride + 3 * i + 0];
							outputImg[j * stride + 3 * i + 1] = inputImg[j * stride + 3 * i + 1];
							outputImg[j * stride + 3 * i + 2] = inputImg[j * stride + 3 * i + 2];
						}
					}
					*/

					//위아래뒤집기
					/*
					for (int j = 0; j < height; j++)
					{
						for (int i = 0; i < width; i++)
						{
							outputImg[j * stride + 3 * i + 0] = inputImg[(height - j) * stride + 3 * i + 0];
							outputImg[j * stride + 3 * i + 1] = inputImg[(height - j) * stride + 3 * i + 1];
							outputImg[j * stride + 3 * i + 2] = inputImg[(height - j) * stride + 3 * i + 2];
						}
					}
						*/

						//위아래 좌우 뒤집
						/*
						for (int j = 0; j < height; j++)
						{
							for (int i = 0; i < width; i++)
							{
								outputImg[j * stride + 3 * i + 0] = inputImg[(height-j) * stride + 3 * (width-i) + 0];
								outputImg[j * stride + 3 * i + 1] = inputImg[(height - j) * stride + 3 * (width - i) + 1];
								outputImg[j * stride + 3 * i + 2] = inputImg[(height - j) * stride + 3 * (width - i) + 2];
							}
						}
						*/

						/*
						for (int j = 0; j < height; j++)
						{
							for (int i = 0; i < width; i++)
							{
								outputImg[j * stride + 3 * i + 0] = inputImg[(height - j) * stride + 3 * i + 0];
								outputImg[j * stride + 3 * i + 1] = inputImg[(height - j) * stride + 3 * i + 1];
								outputImg[j * stride + 3 * i + 2] = inputImg[(height - j) * stride + 3 * i + 2];
								if (j < 100 && i < 100)
								{
									outputImg[j * stride + 3 * i + 0] = 0;
									outputImg[j * stride + 3 * i + 1] = 0;
									outputImg[j * stride + 3 * i + 2] = 0;
								}
								if (j > 400 && i > 400)
								{
									outputImg[j * stride + 3 * i + 0] = 255;
									outputImg[j * stride + 3 * i + 1] = 255;
									outputImg[j * stride + 3 * i + 2] = 255;
								}
							}
						}
						*/
					for (int j = 0; j < height; j++)
					{
						for (int i = 0; i < width; i++)
						{
							Y = (unsigned char)(0.299 * inputImg[j * stride + 3 * i + 2] + 0.587 * inputImg[j * stride + 3 * i + 1] + 0.114 * inputImg[j * stride + 3 * i + 0]);
							Cr = (unsigned char)(-0.169 * inputImg[j * stride + 3 * i + 2] - 0.331 * inputImg[j * stride + 3 * i + 1] + 0.114 * inputImg[j * stride + 3 * i + 0]);
							Cb = (unsigned char)(0.299 * inputImg[j * stride + 3 * i + 2] + 0.587 * inputImg[j * stride + 3 * i + 1] + 0.114 * inputImg[j * stride + 3 * i + 0]);

							R = Y + 1.403 * Cr;
							G = Y - 0.344 * Cb - 0.714 * Cr;
							B = Y + 1.770 * Cb;


							outputImg[j * stride + 3 * i + 0] = Y;
							outputImg[j * stride + 3 * i + 1] = Y;
							outputImg[j * stride + 3 * i + 2] = Y;


						}
					}


					FILE* outputFile = fopen("Output.bmp", "wb");
					fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile);
					fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile);
					fwrite(outputImg, sizeof(unsigned char), size, outputFile);

					free(outputImg);
					free(inputImg);
					fclose(inputFile);
					fclose(outputFile);

					return 0;
				}
				outputImg[j * stride + 3 * i + 2] = Y;
				//R = Y + 1.403 * Cr; 
				//G = Y - 0.344*Cb - 0.714*Cr;
				//B = Y + 1.770*Cb;

			}
		}


	FILE* outputFile = fopen("Output.bmp", "wb");
	fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile);
	fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile);
	fwrite(outputImg, sizeof(unsigned char), size, outputFile);

	free(outputImg);
	free(inputImg);
	fclose(inputFile);
	fclose(outputFile);

	return 0;
}