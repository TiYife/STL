#ifndef __MY_AVLTREE_H__
#define __MY_AVLTREE_H__

#include <iostream>
using namespace std;

#ifndef NULL 
#define NULL 0
#endif
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
	int _bf;//ƽ������
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
		while (cur){//�Ҳ���λ��
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

		cur = new Node(key, value);//������
		if (key < parent->_key)
			parent->_left = cur;
		else{
			parent->_right = cur;
		cur->_parent = parent;

		while (parent){//����ƽ������
			int bf = parent->_bf;

			if (cur == parent->_left)
				++parent->_bf;
			else
				--parent->_bf;

			if (abs(cur->_bf) < abs(bf))
				return true;

			cur = parent;
			parent = parent->_parent;
		}
	}

	void InOrder()
	{
		_InOrder(_root);
		cout << endl;
	}

	~AVLTree()
	{
		_Destroy(_root);
		_root = NULL;
	}
protected:
	void _RevolveL(Node *parent)//����
	{
	
	}
	void _RevolveR(Node *parent)//����
	{

	}
	void _RevolveLR(Node *parent)//����˫��
	{

	}
	void _RevolveRL(Node *parent)//����˫��
	{

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

void TestAVLTree()
{
	int arr[10] = {1,3,5,7,9,10,8,6,4,2};
	AVLTree<int, int> t;

	for (size_t i = 0; i < 10; ++i)
		t.Insert(arr[i], i);

	t.InOrder();
}

#endif