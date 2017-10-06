#ifndef _MAZE_
#define _MAZE_

#include <iostream>
using namespace std;

//#include "D:\Github\STL\stack.h"
#include <stack>

struct Pos
{
	Pos(){}
	Pos(int x, int y):_row(x),_col(y){}
	int _row;
	int _col;
};


template<size_t ROW, size_t COL>
class Maze
{
public:
	Maze(int arr[ROW][COL])
	{
		for (size_t i = 0; i < ROW; i++){
			for (size_t j = 0; j < COL; j++){
				_map[i][j] = arr[i][j];
			}
		}
	}

	void Show()
	{
		for (size_t i = 0; i < ROW; i++){
			for (size_t j = 0; j < COL; j++){
				cout << _map[i][j] << ' ';
			}
			cout << endl;
		}
	}
/////////////////////////////////////
	//�ǵݹ��ҵ�һ�����ڣ������·��
	bool GetPath(Pos& entry)
	{
		stack<Pos> path;

		_map[entry._row][entry._col] = 2;
		path.push(entry);
		
		Pos next = path.Top();

		//���߹��ĵ㱣����ջ��
		while (!path.empty())
		{
			if (_IsOut(path.Top())){
				cout << "����:" << '(' << path.Top()._row << ',' << path.Top()._col << ')' << endl;
				break;
			}

			//��
			next = path.Top();
			next._row -= 1;
			if (_IsAccess(next)){
				_map[next._row][next._col] = 2;
				path.push(next);
				continue;
			}
			//��
			next = path.Top();
			next._col += 1;
			if (_IsAccess(next)){
				_map[next._row][next._col] = 2;
				path.push(next);
				continue;
			}

			//��
			next = path.Top();
			next._col -= 1;
			if (_IsAccess(next)){
				_map[next._row][next._col] = 2;
				path.push(next);
				continue;
			}

			//��
			next = path.Top();
			next._row += 1;
			if (_IsAccess(next)){
				_map[next._row][next._col] = 2;
				path.push(next);
				continue;
			}
			//�ߵ����˵���õ��������Ҷ��߲�ͨ����Ѹõ�pop������ѭ���ٴο�ʼʱ�ж�ǰһ����
			path.pop();
		}

		//ջΪ����˵��û���ҵ�����
		if (path.empty()){
			cout << "û�г��ڣ�" << endl;
			return false;
		}
		return true;
	}

////////////////////////////////////////////////
	//�ݹ鷽���ҵ��������
	void GetPath_R(Pos& entry)
	{
		_map[entry._row][entry._col] = 2;

		if (_IsOut(entry)){
			cout << "����:" << '(' << entry._row << ',' << entry._col << ')' << endl;
			return;
		}
		//Show();
		//��
		Pos next = entry;
		next._row -= 1;
		if (_IsAccess(next)){
			//_map[next._row][next._col] = 2;
			GetPath_R(next);
		}

		//��
		next = entry;
		next._col += 1;
		if (_IsAccess(next)){
			//_map[next._row][next._col] = 2;
			GetPath_R(next);
		}

		//��
		next = entry;
		next._row += 1;
		if (_IsAccess(next)){
			//_map[next._row][next._col] = 2;
			GetPath_R(next);
		}

		//��
		next = entry;
		next._col -= 1;
		if (_IsAccess(next)){
			//_map[next._row][next._col] = 2;
			GetPath_R(next);
		}
	}

//////////////////////////////////////////
	//ʹ�á��ݹ���������߶ȡ���˼�������·��,
	//�ҵ������򷵻����·���ĳ��ȣ���û��ͨ·�򷵻����͵����ֵ��
	int GetShortPath(Pos& entry, int cur = 2)
	{
		_map[entry._row][entry._col] = cur;

		
		if (_IsOut(entry)){
			cout << "����:" << '(' << entry._row << ',' << entry._col << ')' << endl;
			return 1;
		}
		else{
			int up = INT_MAX-1;
			int down = INT_MAX-1;
			int left = INT_MAX-1;
			int right = INT_MAX-1;

			//��
			Pos next = entry;
			next._row -= 1;
			if (_IsAccess(next, entry))
				up = GetShortPath(next, cur+1) + 1;

			//��
			next = entry;
			next._col += 1;
			if (_IsAccess(next, entry))
				right = GetShortPath(next, cur+1) + 1;

			//��
			next = entry;
			next._row += 1;
			if (_IsAccess(next, entry))
				down = GetShortPath(next, cur+1) + 1;

			//��
			next = entry;
			next._col -= 1;
			if (_IsAccess(next, entry))
				left = GetShortPath(next, cur+1) + 1;

			return _GetMin(up, right, down, left);
		}
	}

///////////////////////////////////
//ʹ������ջ����·����������·��
	void GetShortPath(Pos& entry, stack<Pos>& shortPath, stack<Pos>& path)
	{
		_GetShortPath(entry, shortPath, path, 2);
	}

protected:
	void _GetShortPath(Pos& entry, stack<Pos>& shortPath, stack<Pos>& path, int cur)
	{
		_map[entry._row][entry._col] = cur;
		path.push(entry);
		//cout << "ǰ��"<<'(' << entry._row << ',' << entry._col << ')' << endl;;


		if (_IsOut(entry)){
			cout << "����:" << '(' << entry._row << ',' << entry._col << ')' << endl;
			if (shortPath.empty() || path.size() < shortPath.size()){
				shortPath = path;
			}	
		}

		//��
		Pos next = entry;
		next._row -= 1;
		if (_IsAccess(next, entry)){
			_GetShortPath(next, shortPath, path, cur+1);
		}

		//��
		next = entry;
		next._col += 1;
		if (_IsAccess(next, entry)){
			_GetShortPath(next, shortPath, path, cur+1);
		}


		//��
		next = entry;
		next._row += 1;
		if (_IsAccess(next, entry)){
			_GetShortPath(next, shortPath, path, cur+1);
		}

		//��
		next = entry;
		next._col -= 1;
		if (_IsAccess(next, entry)){
			_GetShortPath(next, shortPath, path, cur+1);
		}
		
		path.pop();
		//cout << "����" << '(' << entry._row << ',' << entry._col << ')'<<endl;;

	}

protected:
	//���ĸ��������Сֵ
	int _GetMin(int up, int right, int down, int left)
	{
		int min = up;
		if (min > right)
			min = right;

		if (min > down)
			min = down;

		if (min > left)
			min = left;

		return min;
	}

	//�жϳ���
	bool _IsOut(Pos& p)
	{
		if (ROW - 1 == p._row)
			return true;
		return false;
	}

	//�ж��ܷ���ͨ
	bool _IsAccess(Pos& p)
	{
		if (p._col >= 0 && p._col < 10 && p._row >= 0 && p._row < 10 \
			&& 0 == _map[p._row][p._col])
			return true;
		return false;
	}

	//�ж��ܷ���ͨ�������հ�
	//����һΪ����Ҫ�ߵĵ㣬������Ϊ��ǰ��
	bool _IsAccess(Pos& p, Pos& pre)
	{
		if (p._col >= 0 && p._col < 10 && p._row >= 0 && p._row < 10 \
			&& (0 == _map[p._row][p._col] || _map[p._row][p._col]  > _map[pre._row][pre._col]))
			return true;
		return false;
	}

private:
	int _map[ROW][COL];
};

#endif

void TestMaze()
{
	int arr[10][10] =
	{
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
		{ 0, 0, 0, 0, 0, 1, 0, 0, 1, 1 },
		{ 1, 1, 0, 1, 1, 1, 1, 0, 1, 1 },
		{ 1, 1, 0, 1, 1, 1, 1, 0, 1, 1 },
		{ 1, 1, 0, 1, 1, 1, 1, 0, 1, 1 },
		{ 1, 1, 0, 1, 1, 1, 1, 0, 1, 1 },
		{ 1, 1, 0, 1, 1, 1, 1, 0, 1, 1 },
		{ 1, 1, 0, 0, 0, 0, 0, 0, 1, 1 },
		{ 1, 1, 1, 0, 1, 1, 1, 1, 1, 1 },
	};
	//int arr[10][10] =
	//{
	//	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	//	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	//	{ 0, 0, 1, 1, 1, 1, 1, 1, 1, 1 },
	//	{ 1, 0, 1, 1, 1, 1, 1, 1, 1, 1 },
	//	{ 1, 0, 0, 0, 0, 0, 0, 0, 1, 1 },
	//	{ 1, 0, 1, 1, 1, 1, 1, 0, 1, 1 },
	//	{ 1, 0, 1, 1, 1, 1, 1, 0, 1, 1 },
	//	{ 1, 0, 1, 1, 1, 1, 1, 0, 1, 1 },
	//	{ 1, 0, 1, 1, 1, 1, 1, 0, 1, 1 },
	//	{ 1, 0, 1, 1, 1, 1, 1, 0, 1, 1 },
	//};

	Maze<10, 10> m(arr);
	stack<Pos> shortPath;
	stack<Pos> path;

	m.Show();
	m.GetShortPath(Pos(2,0), shortPath, path);
	std::cout << shortPath.size() << endl;
	//m.GetPath(Pos(2,0));
	//m.GetPath_R(Pos(2,0));
	//cout <<"���·���ĳ��ȣ�" << m.GetShortPath(Pos(2, 0)) << endl;
	m.Show();
}


