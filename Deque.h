#ifndef __MY_DEQUE_H__
#define __MY_DEQUE_H__

#include <iostream>
using namespace std;
#include <assert.h>

template<class T, class Ref, class Ptr, size_t BuffSize = 8>
struct __DequeIterator
{
	typedef T**										MapPointer;
	typedef __DequeIterator<T, T&, T*, BuffSize>	Iterator;
	typedef __DequeIterator<T, Ref, Ptr, BuffSize>	Self;

	//typedef __DequeIterator Self;

	//���캯��
	__DequeIterator()
	:_cur(NULL)
	,_first(NULL)
	,_last(NULL)
	,_node(NULL)
	{}

	__DequeIterator(T* cur, MapPointer node)
		:_cur(cur)
		,_first(*node)
		,_last(_first + BuffSize)
		,_node(node)
	{}
	
	__DequeIterator(const Iterator& s)
		:_cur(s._cur)
		,_first(s._first)
		,_last(s._last)
		,_node(s._node)
	{}

	//��������
	Self& operator++()
	{
		++_cur;
		if (_cur == _last){
			SetNode(_node + 1);
			_cur = _first;
		}
		return *this;
	}

	Self operator++(int)
	{
		Self temp(*this);
		++(*this);
		return temp;
	}

	Self& operator--()
	{
		if (_cur == _first){
			SetNode(_node - 1);
			_cur = _last;
		}
		--_cur;
		
		return *this;
	}

	Self operator--(int)
	{
		Self temp(*this);
		--(*this);
		return temp;
	}

	//�Ƚ�
	bool operator!=(const Self& s)const
	{
		return _cur != s._cur;
	}

	bool operator==(const Self& s)const
	{
		return !(operator!=(s));
	}

	//�����ú�ȡַ
	Ref operator *()const
	{
		return *_cur;
	}

	Ptr operator->()const
	{
		return &(operator*());
	}


	//����λ��
	void SetNode(MapPointer newNode)
	{
		_node = newNode;
		_first = *newNode;
		_last = *newNode + BuffSize;
	}

	T* _cur;
	T* _first;
	T* _last;
	MapPointer _node;
};

template<class T, size_t BuffSize = 8>
class Deque
{
	typedef T** MapPointer;
public:
	typedef __DequeIterator<T, T&, T*, BuffSize>				Iterator;
	typedef __DequeIterator<T, const T&, const T*, BuffSize>	ConstIterator;
	typedef T& Ref;
	typedef const T& ConstRef;
public:
	//construction
	Deque()
		:_map(NULL)
		,_mapSize(0)
		,_size(0)
	{
		assert(BuffSize > 2);
	}

	void PushBack(const T& value)//β��
	{
		if (NULL == _map || _finish._cur == _finish._last - 1)	//���_mapΪ�գ���һ�����ã���ǰ������ֻʣһ���ռ䣬
			_PushBackAux(value);								//��ʹ�ñ���β�巽��
		else{
			*(_finish._cur) = value;//��ǰ�����������������ռ�
			++_finish._cur;
			++_size;
		}
	}

	void PushFront(const T& value)//ͷ��
	{
		if (NULL == _map || _start._cur == _start._first)
			_PushFrontAux(value);
		else{
			*(_start._cur-1) = value;
			--_start._cur;
			++_size;
		}
	}

	void PopBack()
	{
		--_finish;
		if (_finish._cur == _finish._last){
			delete[] * (_finish._node - 1);
			*(_finish._node - 1) = NULL;
		}

		if (_finish == _start){
			delete[] _map;
			_map = NULL;
		}
		--_size;
	}

	void PopFront()
	{
		++_start;
		if (_start._cur == _start._first){
			delete[] * (_start._node - 1);
			*(_start._node - 1) = NULL;
		}

		if (_finish == _start){
			delete _map;
			_map = NULL;
		}
		--_size;
	}

	//Iteration
	Iterator Begin()
	{
		return _start;
	}

	ConstIterator Begin()const
	{
		return _start;
	}

	Iterator End()
	{
		return _finish;
	}

	ConstIterator End()const
	{
		return _finish;
	}

	//capacity
	size_t Size()
	{
		return _size;
	}

	bool Empty()
	{
		return _start == _finish;
	}

	T& Back()
	{
		assert(0 != _size);
		if (_finish._cur != _finish._first)
			return *(_finish._cur - 1);
		else{
			Iterator it(_finish);
			--it;
			return *(it._cur);
		}
	}

	T& Front()
	{
		assert(0 != _size);
		return *(_start._cur);
	}
	~Deque()//�ͷſռ�
	{
		if (_map){
			T** cur = _start._node;
			for (; cur != _finish._node; ++cur){
				if (*cur){
					delete[] * cur;
					*cur = NULL;
				}
			}

			if (*cur){
				delete[] * cur;
				*cur = NULL;
			}
			delete[] _map;
			_map = NULL;
		}
	}

protected:
	void _PushBackAux(const T& value)
	{
		if (NULL == _map || _map + _mapSize - 1 == _finish._node){//����п���������������п�������
			size_t newSize = _mapSize == 0 ? 2 : _mapSize * 2;
			MapPointer temp = new T*[newSize];//�����¿ռ�
			for (size_t i = 0; i < newSize; ++i)
				*(temp + i) = NULL;
			
			size_t addToNode = _mapSize / 2;	//0
			for (size_t i = 0; i < _mapSize; ++i)//�������п���
				temp[addToNode + i] = _map[i];

			size_t oldStartNode = _start._node - _map;//0
			size_t oldFinishNode = _finish._node - _map;//0

			if (_map)//�ͷžɵ��п���
				delete[] _map;

			_map = temp;//�޸��µ��п���

			if (NULL != _finish._cur){
				_start.SetNode(temp + addToNode + oldStartNode);
				_finish.SetNode(temp + addToNode + oldFinishNode);
			}
			else{//˵���ǵ�һ�����ݣ��ڴ�֮ǰ��û�пռ䣩
				*(_map) = new T[BuffSize];

				_finish.SetNode(temp);
				_start.SetNode(temp);
				_finish._cur = *(_map) + BuffSize / 2;
				_start._cur = *(_map) + BuffSize / 2;
				*(_finish._cur++) = value;
				++_size;
				_mapSize = newSize;
				return;
			}
			_mapSize = newSize;
		}

		*(_finish._cur) = value;
		*(_finish._node + 1) = new T[BuffSize];
		_finish.SetNode(_finish._node + 1);
		_finish._cur = _finish._first;
		++_size;
	}
	void _PushFrontAux(const T& value)
	{
		if (NULL == _map || _map == _start._node){//����п���������������п�������
			size_t newSize = _mapSize == 0 ? 2 : _mapSize * 2;
			MapPointer temp = new T*[newSize];//�����¿ռ�

			size_t addToNode = _mapSize / 2;
			for (size_t i = 0; i < _mapSize; ++i)//�������п���
				temp[addToNode + i] = _map[i];

			size_t oldStartNode = _start._node - _map;
			size_t oldFinishNode = _finish._node - _map;

			if (_map)//�ͷžɵ��п���
				delete[] _map;

			_map = temp;//�޸��µ��п���

			if (NULL != _start._cur){
				_start.SetNode(temp + addToNode + oldStartNode);
				_finish.SetNode(temp + addToNode + oldFinishNode);
			}
			else{
				*(_map) = new T[BuffSize];

				_start.SetNode(_map);
				_finish.SetNode(_map);

				_start._cur = *(_map)+BuffSize / 2;
				_finish._cur = *(_map)+BuffSize / 2;

				*(_start._cur - 1) = value;
				--_start._cur;
				++_size;
				_mapSize = newSize;
				return;
			}
			_mapSize = newSize;
		}

		*(_start._node - 1) = new T[BuffSize];
		_start.SetNode(_start._node - 1);
		_start._cur = _start._last - 1;
		*(_start._cur) = value;
		++_size;
	}
protected:
	MapPointer _map;//�����п�����ָ��
	Iterator _start;//ָ�򱻹���ĵ�һ������
	Iterator _finish;//ָ�򱻹�������һ�����ݵ���һ��λ��
	size_t _mapSize;//�ж��ٸ��п���
	size_t _size;//�ж��ٸ�����
};
#if 1

#include "D:\Github\STL\Function.h"

void PrintDeque(const Deque<int>& d)
{
	Deque<int>::ConstIterator it = d.Begin();
	for (; it != d.End(); ++it)
		cout << *it << ' ';
	cout << endl;
}


void TestDeque2()
{
	const int N = 1000;
	int a[N];
	RandomArray(a, N);

	Deque<int> d;

	size_t i = 0;
	for (; i < N/2; ++i)
		d.PushBack(a[i]);
	//PrintDeque(d);
	cout << d.Size() << ' ';

	for (; i < N; ++i)
		d.PushFront(a[i]);
	//PrintDeque(d);
	cout << d.Size() << ' ';

	i = 0;
	for (; i < N / 2; ++i)
		d.PopFront();
	//PrintDeque(d);
	cout << d.Size() << ' ';

	for (; i < N; ++i)
		d.PopBack();
	//PrintDeque(d);
	cout << d.Size() << endl;

}
void TestDeque()
{
	Deque<int> d;
	d.PushBack(1);
	d.PushBack(2);
	d.PushBack(3);

	int i = d.Front();
	cout << i << ' ';
	i = d.Back();
	cout << i << endl;

	d.PushBack(4);
	d.PushBack(5);
	d.PushFront(6);
	i = d.Front();
	cout << i << ' ';
	i = d.Back();
	cout << i << endl;

	d.PushFront(7);
	d.PushFront(8);
	d.PushFront(9);
	d.PushFront(10);
	i = d.Front();
	cout << i << ' ';
	i = d.Back();
	cout << i << endl;

	PrintDeque(d);

	Deque<int>::Iterator it = d.Begin();
	while (it != d.End()){
		cout << *it << ' ';
		++it;
	}
	cout << endl;


	d.PopBack();
	d.PopBack();
	i = d.Front();
	cout << i << ' ';
	i = d.Back();
	cout << i << endl;
	it = d.Begin();
	while (it != d.End()){
		cout << *it << ' ';
		++it;
	}
	cout << endl;

	d.PopFront();
	d.PopFront();
	i = d.Front();
	cout << i << ' ';
	i = d.Back();
	cout << i << endl;
	it = d.Begin();
	while (it != d.End()){
		cout << *it << ' ';
		++it;
	}
	cout << endl;
	i = d.Front();
	cout << i << ' ';
	i = d.Back();
	cout << i << endl;

}
#include <stack>
#include <queue>
#include <deque>
stack<int> s;
queue<int> q;

void test_deque()
{
	deque<int> d;
	d.push_back(1);
	d.push_back(2);
	d.push_back(3);
	d.push_back(4);
	d.push_back(5);
	d.push_front(6);
	d.push_front(7);
	d.push_front(8);
	d.push_front(9);
	d.push_front(10);

	deque<int>::iterator it = d.begin();
	while (it != d.end()){
		cout << *it << ' ';
		++it;
	}
	cout << endl;

}
#endif

#endif