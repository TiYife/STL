/*
���ݣ����ú���ʵ��
ʱ�䣺2017.10.27
*/

#ifndef __FUNCTION_H__
#define __FUNCTION_H__

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include "D:\Github\STL\Map.h"

//����

//���������ʼ������
void RandomArrayEqual(int a[], size_t len, int begin, int end);		//��������ͬ����
void RandomArrayUnique(int a[], size_t len, int begin, int end);	//û����ͬ����
//��ӡ����
void PrintArray(int a[], size_t len);
//��ӡ�Զ�������
template<class Container>
void PrintContainer(const Container& l);

//�Ƚ���طº���
template<class T>
struct Less{
	bool operator()(const T& l, const T& r){
		return l < r; 
	}
};

template<class T>
struct Greater{
	bool operator()(const T& l, const T& r){ 
		return l > r; 
	}
};


//����
///////////////////////////////////////////////////////////////////////

void RandomArrayEqual(int a[], size_t len, int begin = 0, int end = 100)
{
	srand((unsigned int)time(NULL));

	int interval = end - begin;
	for (size_t i = 0; i < len; ++i)
		a[i] = rand() % interval + begin;
	//PrintArray(a, len);
}

void RandomArrayUnique(int a[], size_t len, int begin = 0, int end = INT_MAX)
{
	srand((unsigned int)time(NULL));

	int interval = end - begin;	//���������ĸ���
	int randNum;					//���ɵ������

	for (size_t i = 0; i < len; ++i){
		while (1){
			randNum = rand() % interval + begin;
			size_t j = 0;
			for (; j < i; ++j){
				if (randNum == a[j])
					break;
			}
			if (j == i)
				break;
		}
		a[i] = randNum;
	}
	//PrintArray(a, len);
}

void PrintArray(int a[], size_t len)
{
	for (size_t i = 0; i < len; ++i)
		printf("%d ", a[i]);
	printf("\n");
}

template<class Container>
void PrintContainer(const Container& l)
{
	Container::ConstIterator it = l.Begin();
	for (; it != l.End(); ++it)
		std::cout << *it << ' ';
	std::cout << std::endl;
}


//����
///////////////////////////////////////////////////////////////////////

#if 0
void TestRandomArray()
{
	int a[100];
	RandomArrayEqual(a, 100, 0, 500);
	PrintArray(a, 100);
}

#endif

#endif