//
// Created by ZenYuan on 2020/7/1.
//
#include "../common.h"
const char* url = "E:\\code\\c++_project\\SDL_demo\\result\\out.yuv";
const int w = 1920;
const int h = 1080;

int split_420_yuv()
{
    FILE* fp = fopen(url, "rb+");
    FILE* fp_Y = fopen("E:\\code\\c++_project\\SDL_demo\\result\\out_Y.y", "wb+");
    FILE* fp_U = fopen("E:\\code\\c++_project\\SDL_demo\\result\\out_U.y", "wb+");
    FILE* fp_V = fopen("E:\\code\\c++_project\\SDL_demo\\result\\out_V.y", "wb+");

    char *pic = static_cast<char*>(malloc(sizeof *pic * w*h*3/2));

    for(int i = 0; i < 500; i++)
    {
        fread(pic, 1, w * h * 3 / 2, fp);
        fwrite(pic, 1, w * h, fp_Y);
        fwrite(pic + w * h, 1, w * h / 4, fp_U);
        fwrite(pic + w * h * 5 / 4, 1, w * h / 4, fp_V);
    }
    free(pic);
    fclose(fp);
    fclose(fp_V);
    fclose(fp_U);
    fclose(fp_Y);
    return 0;
}

int split_444_yuv()
{
    FILE* fp = fopen(url, "rb+");
    FILE* fp_Y = fopen("E:\\code\\c++_project\\SDL_demo\\result\\out_Y.y", "wb+");
    FILE* fp_U = fopen("E:\\code\\c++_project\\SDL_demo\\result\\out_U.y", "wb+");
    FILE* fp_V = fopen("E:\\code\\c++_project\\SDL_demo\\result\\out_V.y", "wb+");

    char *pic = static_cast<char*>(malloc(sizeof *pic * w*h*3/2));

    for(int i = 0; i < 500; i++)
    {
        fread(pic, 1, w * h * 3, fp);
        fwrite(pic, 1, w * h, fp_Y);
        fwrite(pic + w * h, 1, w * h, fp_U);
        fwrite(pic + w * h*2, 1, w * h, fp_V);
    }
    free(pic);
    fclose(fp);
    fclose(fp_V);
    fclose(fp_U);
    fclose(fp_Y);
    return 0;
}

int split_422_yuv()
{
    FILE* fp = fopen(url, "rb+");
    FILE* fp_Y = fopen("E:\\code\\c++_project\\SDL_demo\\result\\out_Y.y", "wb+");
    FILE* fp_U = fopen("E:\\code\\c++_project\\SDL_demo\\result\\out_U.y", "wb+");
    FILE* fp_V = fopen("E:\\code\\c++_project\\SDL_demo\\result\\out_V.y", "wb+");

    char *pic = static_cast<char*>(malloc(sizeof *pic * w*h*3));
    for(int i = 0; i < 500; i++)
    {
        fread(pic, 1, w * h * 2, fp);
        fwrite(pic, 1, w * h, fp_Y);
        fwrite(pic + w * h, 1, w * h/2, fp_U);
        fwrite(pic + w * h*3/2, 1, w * h/2, fp_V);
    }
    free(pic);
    fclose(fp);
    fclose(fp_V);
    fclose(fp_U);
    fclose(fp_Y);
    return 0;
}