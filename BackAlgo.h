#ifndef __BACKAGLO_H__
#define __BACKAGLO_H__
#include <iostream>
using namespace std;

#define M 5 //�������
#define N 3 //������Դ
int Available[N];//ϵͳ��������Դ��Ŀ
int Max[M][N];//����������
int Allocation[M][N];//�Ѿ���ȡ����Դ��Ŀ�ľ���
int Need[M][N];//���������Դ��Ŀ�ľ���
int Work[N];//��ǰϵͳ�п���ʹ�õ���Դ��Ŀ
bool Finish[N];//����ִ��������

void Print();//��ӡ��Դ������Ϣ
void Init();//��ʼ����Դ��Ϣ
bool Safe();//��ȫ�Լ��

//�����ǰ��Դ������Ϣ
void Print(int max[M][N], int allo[M][N], int need[M][N], int work[N], int pnum)
{
	if (pnum == 0)
		cout << "      Max	  Allo     Need	         Work" << endl;

	//pnum�Ž����������
	cout << "P" << pnum << "  ";
	for (size_t j = 0; j < N; ++j)
		cout << " " << max[pnum][j];

	//pnum�Ž����Ѿ����䵽����Դ
	cout << "	";
	for (size_t j = 0; j < N; ++j)
		cout << " " << allo[pnum][j];

	//pnum�Ž��̻���Ҫ�������Դ
	cout << "    ";
	for (size_t j = 0; j < N; ++j)
		cout << " " << need[pnum][j];

	//��ǰ���õ���Դ��Ŀ
	cout << "	";
	for (size_t j = 0; j < N; ++j)
		cout << " " << work[j];
	cout << endl;
}

//��ʼ����Դ��Ϣ
void Init()
{
	//��������
	//	3 3 2
	cout << "�����������Դ������Ŀ��" << endl;
	for (size_t i = 0; i < N; ++i)
		cin >> Available[i];

	//	{ 3, 3, 2 },
	//	{ 5, 3, 2 },
	//	{ 7, 4, 3 },
	//	{ 7, 4, 5 },
	//	{ 10, 4, 7 }
	cout << "������������������" << endl;
	for (size_t i = 0; i < M; ++i){
		cout << "P" << i << endl;
		for (size_t j = 0; j < N; ++j)
			cin >> Max[i][j];
	}

	//	{ 2, 0, 0 },
	//	{ 2, 1, 1 },
	//	{ 0, 0, 2 },
	//	{ 3, 0, 2 },
	//	{ 0, 1, 0 }
	cout << "������������Ѿ����䵽��Դ��Ŀ��" << endl;
	for (size_t i = 0; i < M; ++i){
		cout << "P" << i << endl;
		for (size_t j = 0; j < N; ++j)
			cin >> Allocation[i][j];
	}

	for (size_t i = 0; i < M; ++i){
		for (size_t j = 0; j < N; ++j)
			Need[i][j] = Max[i][j] - Allocation[i][j];
	}

	for (size_t i = 0; i<N; i++)
		Work[i] = Available[i];
}

//��ȫ�Լ��
bool Safe()
{
	size_t i = 0;
	size_t j = 0;
	size_t k = 0;
	bool flg = false;

	for (i = 0; i < M; ++i){
		//�����н�����ѡ��һ������������ִ��
		for (j = i; j < M; ++j){
			if (Finish[j] == true)
				break;

			//��������������Դ���Ƿ񳬹���ǰ��ʹ����Դ��
			for (k = 0; k < N; ++k){
				//�������������һ������
				if (Need[i][k] > Work[k])
					break;
			}
			if (k < N){
				if (j == M)
					return false;
				else
					continue;
			}
			else{
				//�ҵ�һ��������������ִ�еĽ���
				Finish[j] = true;
				for (size_t index = 0; index < N; ++index)
					Work[index] += Allocation[j][index];

				//ģ�����ִ�в���ӡ������Ϣ
				Print(Max, Allocation, Need, Work, j);
				break;
			}
		}
	}

	return true;
}

void test()
{
	Init();
	bool ret = Safe();
	if (ret == true)
		cout << "��ȫ��" << endl;
	else
		cout << "����ȫ��" << endl;
}

int main()
{
	test();
	return 0;
}
#endif