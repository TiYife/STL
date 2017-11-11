#ifndef __MY_AVLTREE_H__
#define __MY_AVLTREE_H__

#include <iostream>
using namespace std;
#include <assert.h>

template<class K, class V>
struct AVLTreeNode//AVLTrees���ڵ㣬������
{
	AVLTreeNode(const K& key, const V& value)
	:_bf(0)
	,_key(key)
	, _value(value)
	, _left(NULL)
	, _right(NULL)
	, _parent(NULL)
	{}
	int _bf;//ƽ������ = �Ҹ߶� - ��߶�
	K _key;
	V _value;
	AVLTreeNode<K, V> *_left;
	AVLTreeNode<K, V> *_right;
	AVLTreeNode<K, V> *_parent;
};

template<class K, class V>
class AVLTree
{
	typedef AVLTreeNode<K, V> Node;
public:
	AVLTree()
		:_root(NULL)
	{}

	bool Insert(const K& key, const V& value)
	{
		if (NULL == _root){
			_root = new Node(key, value);
			return true;
		}

		Node *cur = _root;
		Node *parent = NULL;

		//�Ҳ���λ��
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

		//������
		cur = new Node(key, value);
		if (key < parent->_key)
			parent->_left = cur;
		else
			parent->_right = cur;
		cur->_parent = parent;

		//����ƽ������
		while (parent){
			if (cur == parent->_left)
				--parent->_bf;
			else
				++parent->_bf;

			if (0 == parent->_bf)
				break;
			else if (1 == parent->_bf || -1 == parent->_bf){//	+/-1		�������ϵ���
				cur = parent;
				parent = parent->_parent;
			}
			else{//	+/-2		������ת��
				if (2 == parent->_bf){//	2
					if (1 == cur->_bf)//	2	1	��-��
						_RotateL(parent);
					else//	2	-1		��-��
						_RotateRL(parent);
				}
				else{//	-2
					if (-1 == cur->_bf)//-2	-1	��-��
						_RotateR(parent);
					else//-2	1	��-��
						_RotateLR(parent);
				}
				break;
			}
		}
		return true;
	}

	void InOrder()
	{
		_InOrder(_root);
		cout << endl;
	}

	bool IsBlance()
	{
		int height = 0;
		return _IsBlance(_root, height);
	}

	~AVLTree()
	{
		_Destroy(_root);
		_root = NULL;
	}
protected:
	bool _IsBlance(Node *root, int& height)
	{
		if (NULL == root){
			height = 0;
			return true;
		}

		int leftHeight = 0;
		int rightHeight = 0;

		if (!_IsBlance(root->_left, leftHeight) 
			|| !_IsBlance(root->_right, rightHeight))
			return false;

		if ((rightHeight - leftHeight) > 1
			|| (rightHeight - leftHeight) < -1){
			cout << "ƽ�����ӳ���"<< root->_key <<"->"<<root->_bf<<'('<<rightHeight-leftHeight<<')'<< endl;
			return false;
		}
		height = (leftHeight > rightHeight) ? (leftHeight + 1) : (rightHeight + 1);
		return true;
	}
	void _RotateL(Node *parent)//����
	{
		Node* subR = parent->_right;
		Node* subRL = subR->_left;

		parent->_right = subRL;
		if (subRL)
			subRL->_parent = parent;

		Node* pparent = parent->_parent;
		subR->_parent = pparent;
		
		if (NULL == pparent){
			_root = subR;
			subR->_parent = NULL;
		}
		else if (parent == pparent->_left)
			pparent->_left = subR;
		else
			pparent->_right = subR;

		subR->_left = parent;
		parent->_parent = subR;

		subR->_bf = 0;
		parent->_bf = 0;
	}
	void _RotateR(Node *parent)//����
	{
		Node *subL = parent->_left;
		Node *subLR = subL->_right;

		parent->_left = subLR;
		if (subLR)
			subLR->_parent = parent;

		Node *pparent = parent->_parent;
		subL->_parent = pparent;

		if (NULL == pparent){
			_root = subL;
			subL->_parent = NULL;
		}
		else if (parent == pparent->_left)
			pparent->_left = subL;
		else
			pparent->_right = subL;

		subL->_right = parent;
		parent->_parent = subL;

		subL->_bf = 0;
		parent->_bf = 0;
	}
	void _RotateLR(Node *parent)//����˫��
	{
		Node *subL = parent->_left;
		Node *subLR = subL->_right;
		int bf = subLR->_bf;

		_RotateL(parent->_left);
		_RotateR(parent);

		if (1 == bf)
			subL->_bf = -1;
		else if(-1 == bf)
			parent->_bf = 1;
	}
	void _RotateRL(Node *parent)//����˫��
	{
		Node *subR = parent->_right;
		Node *subRL = subR->_left;
		int bf = subRL->_bf;

		_RotateR(parent->_right);
		_RotateL(parent);
	
		if (1 == bf)
			parent->_bf = -1;
		else if(-1 == bf)
			subR->_bf = 1;
	}
	void _InOrder(Node *root)
	{
		if (NULL == root)
			return;
		_InOrder(root->_left);
		cout << root->_key << "->" << root->_value <<"->"<< root->_bf<< endl;
		_InOrder(root->_right);
	}
	void _Destroy(Node *root)
	{
		if (NULL == root)
			return;

		_Destroy(root->_left);
		_Destroy(root->_right);
		delete root;
	}

protected:
	Node *_root;
};

#if 0

#include "D:\Github\STL\Function.h"

void TestAVLTree()
{
	int arr[100];
	RandomArrayUnique(arr, sizeof(arr)/sizeof(int), 0, 1000000);
	AVLTree<int, int> t;

	for (size_t i = 0; i < sizeof(arr) / sizeof(int); ++i){
		t.Insert(arr[i], i);
		cout << "IsBlance? " << t.IsBlance() << endl;;
	}

	t.InOrder();
}

#endif
#endif