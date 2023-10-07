// ParallelProgrammingTask1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <iostream>
#include <windows.h>

const int imageSize = 1900; //размер изображения в байтах
const int coefficient = 1;
const int maxThreads = 8;
const int threadStep = imageSize / maxThreads;
unsigned char f[1900];


typedef struct thread_parameters { //параметры функции для потока
    int start;
    int end;
    int threadNum;
};
DWORD __stdcall change_bytes(void* args) {
    thread_parameters* params = (thread_parameters*)args;

    for (int i = params->start; i > params->end; i -= threadStep) {
        f[i] += (i * coefficient) & 255;
    }
    return 0;
}
HANDLE handles[maxThreads];
DWORD thId[maxThreads];
thread_parameters params[maxThreads];
int main()
{
    FILE* original_file;
    FILE* new_file;

    original_file = fopen("C://Users//707//Downloads//corrupted_binary.jpg", "rb");
    fread(f, sizeof(f), 1, original_file);
    
    for (int i = 1; i < maxThreads + 1; i++) { //идем по потокам

        params[i - 1].start = imageSize - i;
        params[i - 1].end = 0;
        params[i - 1].threadNum = i;

        //первый поток расшифровывает от N-1, N-1-N/B, второй N-2, N-2-N/B, N-2-2*N/B...
        //N - размер файла, B = max_threads


        handles[i - 1] = CreateThread(NULL, 0, change_bytes, &params[i-1], CREATE_SUSPENDED, &thId[i-1]);
        ResumeThread(handles[i - 1]);
    }
    WaitForMultipleObjects(maxThreads, handles, true, INFINITE);

    new_file = fopen("C://Users//707//Downloads//changed_binary_createthread.jpg", "wb");
    fwrite(f, sizeof(f), 1, new_file);

    fclose(original_file);
    fclose(new_file);
}
//оригинальный
/*int main()
{
    FILE* original_file;
    FILE* new_file;
    const int size = 1900;
    unsigned char f[1900];
    int x = 8;

    original_file = fopen("C://Users//707//Downloads//binary.jpg", "rb");
    fread(f, sizeof(f), 1, original_file);

    for (int i = 0; i < size; i++) {
        printf("%u ", f[i]);
    }
    std::cout << "\n\n\n";

    for (int i = 0; i < size; i++) {
        f[i] -= (i * x) & 255;
        printf("%u ", f[i]);
    }

    new_file = fopen("C://Users//707//Downloads//corrupt_image.jpg", "wb");
    fwrite(f, sizeof(f), 1, new_file);

    fclose(original_file);
    fclose(new_file);
}*/

//оригинальная задача + цикл где проходит от N-1, N-1-N/B и т.д
/*#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <iostream>

const int imageSize = 1900; //размер изображения в байтах
const int coefficient = 1;
const int maxThreads = 8;
const int threadStep = imageSize / maxThreads;

void change_bytes(int start, int end, unsigned char buffer[1900]) {
    for (int i = start; i > end; i -= threadStep) {
        buffer[i] += (i * coefficient) & 255;
        std::cout << (int)buffer[i] << " ";
    }
}


int main()
{
    FILE* original_file;
    FILE* new_file;
    unsigned char f[1900];


    original_file = fopen("C://Users//707//Downloads//corrupted_binary.jpg", "rb");
    fread(f, sizeof(f), 1, original_file);

    for (int i = 1; i < maxThreads + 1; i++) { //идем по потокам

        //первый поток расшифровывает от N-1, N-1-N/B, второй N-2, N-2-N/B, N-2-2*N/B...
        //N - размер файла, B = max_threads

        change_bytes(imageSize - i, 0, f);

    }

    new_file = fopen("C://Users//707//Downloads//changed_binary.jpg", "wb");
    fwrite(f, sizeof(f), 1, new_file);

    fclose(original_file);
    fclose(new_file);
}*/
