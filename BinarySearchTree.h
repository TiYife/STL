/*
���ݣ�����������
ʱ�䣺2017.10.27
*/

#ifndef __BINARY_SEARCH_TREE__
#define __BINARY_SEARCH_TREE__

#pragma once

#include <iostream>
using namespace std;
#include <assert.h>

#ifndef NULL
#define NULL 0
#endif

template<class T>
struct BinarySearchTreeNode
{
	BinarySearchTreeNode(const T& key)
	:_key(key)
	,_parent(NULL)
	,_left(NULL)
	,_right(NULL)
	{}
	typedef BinarySearchTreeNode<T> Node;
	T _key;
	Node *_left;
	Node *_right;
	Node *_parent;
};

template<class T>
class BinarySearchTree
{
	typedef BinarySearchTreeNode<T> Node;
public:
	BinarySearchTree()
		:_root(NULL)
	{}

	//����
	bool Insert(const T& key)
	{
		//�����Ϊ����ֱ�ӹ���һ��Node����
		if (NULL == _root){
			_root = new Node(key);
			return true;
		}
		//�������ҵ�����λ�ã��ڹ���ڵ����
		else{
			Node *cur = _root;
			Node *parent = NULL;
			while (cur){
				if (key < cur->_key){
					parent = cur;
					cur = cur->_left;
				}
				else if (key > cur->_key){
					parent = cur;
					cur = cur->_right;
				}
				else
					return false;
			}

			cur = new Node(key);
			if (key < parent->_key)
				parent->_left = cur;
			else
				parent->_right = cur;
			cur->_parent = parent;

			return true;
		}
	}

	//����
	Node* Find(const T& key)
	{
		Node *cur = _root;
		while (cur){
			if (key < cur->_key)
				cur = cur->_left;
			else if (key > cur->_key)
				cur = cur->_right;
			else
				return cur;
		}

		return NULL;
	}

	bool Earse(const T& key)
	{
		Node *cur = _root;
		Node *parent = NULL;

		/*
		1. ���ҵ�Ҫɾ���Ľڵ�
		2. ɾ��
			1) ��ɾ���ڵ����Ҷ���Ϊ��,�����������ҽڵ㣨����������ڵ㣩�滻(���⴦����ڵ�)
			2) ��ɾ���ڵ���Ϊ�գ��ø��������ɾ���ڵ�������
			3����ɾ���ڵ���Ϊ�գ��ø��������ɾ���ڵ�������
		*/
		while (cur){
			//��ɾ���ڵ�
			if (key < cur->_key){
				parent = cur;
				cur = cur->_left;
			}
			else if (key > cur->_key){
				parent = cur;
				cur = cur->_right;
			}
			else{//�ҵ���
				if (cur->_left && cur->_right){//���Ҷ���Ϊ��
					Node *subLMostR = cur->_left;
					while (subLMostR->_right)//�����������ҽڵ�
						subLMostR = subLMostR->_right;

					cur->_key = subLMostR->_key;//�滻

					if (subLMostR == subLMostR->_parent->_right)
						subLMostR->_parent->_right = subLMostR->_left;
					else
						subLMostR->_parent->_left = subLMostR->_left;

					if (subLMostR->_left)
						subLMostR->_left->_parent = subLMostR->_parent;

					delete subLMostR;
					subLMostR = NULL;
					return true;
				}
				else{// ��Ϊ�� ���� ��Ϊ�� ���������Ҷ�Ϊ�գ�
					Node* linkNode = NULL;
					if (NULL == cur->_left)//��Ϊ��
						linkNode = cur->_right;
					else if (NULL == cur->_right)//��Ϊ��
						linkNode = cur->_left;

					if (cur != _root){
						if (cur == cur->_parent->_right)
							cur->_parent->_right = linkNode;
						else
							cur->_parent->_left = linkNode;
					}
					else
						_root = linkNode;

					if(linkNode)//���¸��ڵ�
						linkNode->_parent = cur->_parent;

					delete cur;
					cur = NULL;
					return true;
				}
				

				/*
				//��Ϊ��
				if (NULL == cur->_left){
					if (cur != _root){
						if (cur == parent->_left)
							parent->_left = cur->_right;
						else
							parent->_right = cur->_right;

						//delete cur;
						//cur = NULL;
						//return true;
					}
					else{
						_root = cur->_right;
						//delete cur;
						//cur = NULL;
						//return true;
					}

					if (cur->_right)
						cur->_right->_parent = parent;

					delete cur;
					cur = NULL;
					return true;
				}
				//��Ϊ��
				else if (NULL == cur->_right){
					if (cur != _root){
						if (cur == parent->_left)
							parent->_left = cur->_left;
						else
							parent->_right = cur->_left;

						//delete cur;
						//cur = NULL;
						//return true;
					}
					else{
						_root = cur->_left;
						//delete cur;
						//cur = NULL;
						//return true;
					}
					if (cur->_left)
						cur->_left->_parent = parent;
					delete cur;
					cur = NULL;
					return true;
				}
				//���Ҷ�����
				else{
					Node *subLMostR = cur->_left;
					while (subLMostR->_right)//�����������ҽڵ�
						subLMostR = subLMostR->_right;

					cur->_key = subLMostR->_key;
					Node *subLMostRP = subLMostR->_parent;
					if (subLMostR == subLMostRP->_right)
						subLMostRP->_right = subLMostR->_left;
					else
						subLMostRP->_left = subLMostR->_left;

					if (subLMostR->_left)
						subLMostR->_left->_parent = subLMostRP;

					delete subLMostR;
					subLMostR = NULL;
					return true;
				}
				*/
			}
		}
		return false;
	}

	void InOrder()
	{
		_InOrder(_root);
		cout << endl;
	}

	~BinarySearchTree()
	{
		_Destroy(_root);
		_root = NULL;
	}
protected:
	void _Destroy(Node* root)
	{
		if (NULL == root)
			return;

		_Destroy(root->_left);
		_Destroy(root->_right);
		delete root;
		root = NULL;
	}
	void _InOrder(Node* root)
	{
		if (NULL == root)
			return;

		_InOrder(root->_left);
		cout << root->_key << ' ';
		_InOrder(root->_right);
	}

protected:
	Node *_root;
};


#if 1

#include "D:\Github\STL\Function.h"
void TestBSTree()
{
#if 1
	int a[100];
	RandArrayUnique(a, sizeof(a)/sizeof(int));//�������������� a[]
#else
	int a[] = {5, 3, 4, 1, 7, 8, 2, 6, 0, 9};
#endif

	BinarySearchTree<int> t;
	for (size_t i = 0; i < sizeof(a) / sizeof(int); ++i)
		t.Insert(a[i]);

	cout << t.Find(a[1])->_key << endl;

	t.InOrder();
	t.Earse(a[3]);
	t.Earse(a[2]);
	t.Earse(a[0]);
	t.Earse(a[1]);
	t.Earse(a[5]);
	t.Earse(a[7]);
	t.Earse(a[8]);
	t.Earse(a[6]);
	t.Earse(a[9]);
	t.Earse(a[4]);
	t.InOrder();

}

#endif

#endif