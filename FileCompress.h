//���û���������ʵ�ּ򵥵��ļ�ѹ��

#define _CRT_SECURE_NO_WARNINGS

#ifndef __MY_FILE_COMPRESS_H__
#define __MY_FILE_COMPRESS_H__

#include <iostream>
#include <algorithm>
#include <string>

#include "D:\Github\STL\HuffmanTree.h"

const int CHARSIZE = 256;
const int BUFFSIZE = 1024;

//�ļ�������ָ��,�Զ��ر��ļ�ָ��
class FilePtr
{
public:
	FILE* _p;

	FilePtr(FILE* p)
	{
		assert(p);
		_p = p;
	}
	~FilePtr()
	{
		fclose(_p);
	}
private:
	FilePtr(const FilePtr&);
	FilePtr operator=(const FilePtr&);
};

struct CharInfor
{
	char _ch;
	int _count;
	string _huffmanCode;

	bool operator >(const CharInfor& c)const
	{
		return _count > c._count;
	}
	bool operator <(const CharInfor& c)const
	{
		return _count < c._count;
	}
	CharInfor operator+(const CharInfor& c)
	{
		CharInfor temp;
		temp._count = _count + c._count;
		return temp;
	}
	bool operator == (const CharInfor& c)const
	{
		return _count == c._count;
	}

	bool operator != (const CharInfor& c)const
	{
		return _count != c._count;
	}
};

class FileCompress
{
	typedef HuffmanTreeNode<CharInfor> Node;
public:
	FileCompress()
	{
		for (size_t i = 0; i < CHARSIZE; ++i){
			_infor[i]._ch = i;
			_infor[i]._count = 0;
		}
	}

	void Compress(const char* filePath)
	{
		string fileName(filePath);//��ȡ�ļ���
		_GetFileName(filePath, fileName);

		//1. ͳ���ַ�����
		FilePtr _fpOut(fopen(fileName.c_str(), "r"));//ʹ������ָ��
		FILE* fpOut = _fpOut._p;
		//FILE* fpOut = fopen(fileName.c_str(), "r");


		char ch = getc(fpOut);
		while (!feof(fpOut)){//ch != EOF
			++_infor[(unsigned char)ch]._count;
			ch = getc(fpOut);
		}

		//2. �����շ�����
		CharInfor invalue;
		invalue._count = 0;//���ִ���Ϊ0���ַ����������Ĺ���
		HuffmanTree<CharInfor> ht(_infor, CHARSIZE, invalue);

		//3. ��ȡ����������
		Node *root = ht.GetRoot();
		//string code;
		_GetHuffmanCode(root);

		/////////////////////////////////////////
		//cout << "################################" << endl;
		//_PreOrder(root);


		//4. ѹ���ļ�

		//����ѹ���ļ���
		string compressFileName(fileName);
		size_t idx = compressFileName.rfind('_');
		compressFileName.erase(idx, compressFileName.size() - idx);
		compressFileName += ".hzp";

		FilePtr _fpIn(fopen(compressFileName.c_str(), "w"));
		FILE* fpIn = _fpIn._p;
		//FILE* fpIn = fopen(compressFileName.c_str(), "w");

		////ѹ���ļ���ͷ����Ϣ���������ʽ(�ļ���׺+�ַ�������+ÿ���ַ��ĸ���)
		string headInforBuff;

		//��1����ȡ�ļ���׺
		string filePostfix;
		_GetFilePostfix(fileName, filePostfix);
		filePostfix += '\n';
		//д���ļ���׺
		fwrite(filePostfix.c_str(), 1, filePostfix.size(), fpIn);

		//��2����ȡ�ַ����ָ���
		string charInforBuff;//�ַ���Ϣ
		int count = 0;//�ַ�������
		fseek(fpOut, 0, SEEK_SET);//�����ļ�ָ�����ļ���ͷ
		for (size_t i = 0; i < CHARSIZE; ++i){
			if (0 != _infor[i]._count){
				++count;
				charInforBuff += _infor[i]._ch;
				charInforBuff += ':';

				char countBuff[32]; //ÿ���ַ����ָ���
				_itoa(_infor[i]._count, countBuff, 10);

				charInforBuff += countBuff;
				charInforBuff += '\n';
			}
		}
		char speciesBuff[32];//�ַ�������
		_itoa(count, speciesBuff, 10);
		string countBuff(speciesBuff);
		countBuff += '\n';
		//д��count
		fwrite(countBuff.c_str(), 1, countBuff.size(), fpIn);

		//д��charInfor
		fwrite(charInforBuff.c_str(), 1, charInforBuff.size(), fpIn);

		//��3��ѹ���ַ�
		fseek(fpOut, 0, SEEK_SET);//�����ļ�ָ�����ļ���ͷ
		ch = getc(fpOut);
		char value = 0;
		int pos = 0;
		int c = 0;
		while (ch != EOF){
			string code = _infor[(unsigned char)ch]._huffmanCode;
			for (size_t i = 0; i < code.size(); ++i){
				if ('0' == code[i])
					value &= ~(1 << pos);//����posλ����Ϊ0
				else if ('1' == code[i])
					value |= 1 << pos;//����posλ����Ϊ1
				else//error
					assert(false);

				++pos;

				if (8 == pos){//������˸�λ��д���ļ�
					fputc(value, fpIn);
					value = 0;
					pos = 0;
				}
			}

			ch = fgetc(fpOut);
			cout << ch << ' ';
			++c;
		}

		if (0 != pos)
			fputc(value, fpIn);

		//fclose(fpOut);
		//fclose(fpIn);
	}
	void Decompress(const char *filePath)
	{
		//// ��ȡ��ѹ�ļ���
		string fileName;
		_GetFileName(filePath, fileName);

		FilePtr _fpOut(fopen(fileName.c_str(), "r"));
		FILE* fpOut = _fpOut._p;

		string decompressFileName(fileName);
		int idx = decompressFileName.rfind('.');
		decompressFileName.erase(idx, decompressFileName.size() - idx);
		string filePostfix;
		_GetLine(fpOut, filePostfix);
		decompressFileName += "_out";
		decompressFileName += filePostfix;

		//1. ��ȡ�ַ�������Ϣ
		string sizeBuff;
		_GetLine(fpOut, sizeBuff);
		size_t size = atoi(sizeBuff.c_str());

		string charInfor;
		string charCount;
		for (size_t i = 0; i < size; ++i)
		{
			charInfor = "";
			_GetLine(fpOut, charInfor);
			if (0 != charInfor.size()){
				_infor[(unsigned char)charInfor[0]]._ch = charInfor[0];
				charCount = charInfor.substr(2, charInfor.size() - 2);
				_infor[(unsigned char)charInfor[0]]._count = atoi(charCount.c_str());
			}
			else{
				_GetLine(fpOut, charInfor);
				_infor[(unsigned char)charInfor[0]]._ch = '\n';
				charCount = charInfor.substr(1, charInfor.size() - 1);
				_infor[(unsigned char)('\n')]._count = atoi(charCount.c_str());
			}
		}

		//2. ������������
		CharInfor invalue;
		invalue._count = 0;
		HuffmanTree<CharInfor> ht(_infor, CHARSIZE, invalue);


		//3. ��ȡ����������
		Node *root = ht.GetRoot();
		_GetHuffmanCode(root);
		
		//cout << "################################" << endl;
		//_PreOrder(root);

		//4. ��ѹ��
		FilePtr _fpIn(fopen(decompressFileName.c_str(), "w"));
		FILE* fpIn = _fpIn._p;
		//FILE *fpIn = fopen(decompressFileName.c_str(), "w");//�򿪽�ѹ����ļ�

		Node *cur = root;
		char ch = fgetc(fpOut);
		long long count = root->_w._count;

		while (count){
			for (size_t pos = 0; pos < 8; ++pos){
				if (0 == (ch & (1 << pos)))//�����posλ��0
					cur = cur->_left;
				else
					cur = cur->_right;

				if (NULL == cur->_left && NULL == cur->_right){
					fputc(cur->_w._ch, fpIn);
					cout << cur->_w._ch << ' ';

					--count;
					cur = root;
				}
			}
			if (0 == count)
				break;
			ch = fgetc(fpOut);
		}
		//fclose(fpOut);
		//fclose(fpIn);
	}
	~FileCompress()
	{}
protected:
	void _PreOrder(Node* root)
	{
		if (NULL == root)
			return;

		cout << root->_w._ch << ' ' << root->_w._count << ' ' << root->_w._huffmanCode << endl;

		_PreOrder(root->_left);
		_PreOrder(root->_right);
	}
	//��ȡһ��
	void _GetLine(FILE* fp, string& buff)
	{
		assert(fp);

		char ch;
		while (!feof(fp)){
			ch = getc(fp);
			if ('\n' == ch)
				break;
			else
				buff.push_back(ch);
		}
	}
	//��ȡ�ļ�����׺
	void _GetFilePostfix(const string& fileName, string& filePostfix)
	{
		size_t pos = fileName.find_last_of('.');
		filePostfix = fileName.substr(pos, fileName.size()-pos);
	}
	//��ȡ�ļ���
	void _GetFileName(const string& filePath, string &fileName)
	{
		//D:\\VS2013\\STL\FileCompress\\FileCompress\\test.txt
		size_t pos = filePath.find_last_of('\\');
		if (pos == string::npos)
			fileName = filePath;
		else
			fileName = filePath.substr(pos+1, filePath.size()-pos-1);
	}

	void _GetHuffmanCode(Node *root)//��ȡ����������
	{
		if (NULL == root)
			return;

		_GetHuffmanCode(root->_left);
		_GetHuffmanCode(root->_right);

		if (NULL == root->_left && NULL == root->_right){
			string& code = _infor[(unsigned char)root->_w._ch]._huffmanCode;
			Node *cur = root;
			Node *parent = cur->_parent;
			while (parent){
				if (cur == parent->_left)
					code += '0';
				else
					code += '1';

				cur = parent;
				parent = cur->_parent;
			}

			reverse(code.begin(), code.end());
		}
	}
	void _GetHuffmanCode(Node *root, string code)
	{
		if (NULL == root)
			return;
		
		if (NULL == root->_left && NULL == root->_right){
			_infor[(unsigned char)root->_w._ch]._huffmanCode = code;
			return;
		}
		_GetHuffmanCode(root->_left, code+'0');
		_GetHuffmanCode(root->_right, code+'1');
	}
protected:
	CharInfor _infor[CHARSIZE];
};

#endif

void TestFileCompress()
{
	FileCompress fc;
	fc.Compress("test_in.txt");
	cout << endl << "########################################" << endl << endl << endl;



	//char buff[32] = "�Ұ�ѧϰ";

	//fc.Compress("timg_in.jpg");
	//fc.Decompress("timg.huffman");

	//char ch[] = "Linux��iӢ�﷢����/?l?n?ks/ LIN-?ks����һ�����ɺͿ���Դ�������UNIX����ϵͳ��\
	//	�ò���ϵͳ���ں�������˹����������1991��10��5���״η��������ڼ����û��ռ��Ӧ \
	//	�ó���֮�󣬳�ΪLinux����ϵͳ��LinuxҲ����������Ϳ���Դ���������չ�������� \
	//	�����ӡ�ֻҪ��ѭGNUͨ�ù������֤���κθ��˺ͻ������������ɵ�ʹ��Linux������ \
	//	�ײ�Դ���룬Ҳ�������ɵ��޸ĺ��ٷ����������Linuxϵͳ���������ṩGUI��X Wind \
	//	ow֮��ĳ��򡣳���һ����ר��֮�⣬������˶���ֱ��ʹ��Linux���а棬�������Լ� \
	//	ѡ��ÿһ��������������á�";

	//cout << '\\' << endl;
	//FilePtr _fpOut(fopen("test_in.txt", "r"));
	//FILE* fpOut = _fpOut.GetPtr();
} 
void TestFileDecompress()
{
	FileCompress fc;
	fc.Decompress("test.hzp");
	getchar();
}