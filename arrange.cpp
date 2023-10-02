#include "arrange.h"
#include <iostream>

using namespace std;

void arrange(Topo* t)
{
	cout << "               请选择课程安排的方式			" << endl;
	cout << endl;
	cout << "           1.各学期的学习负担尽量均匀" << endl;
	cout << endl;
	cout << "           2.课程尽可能集中在前几个学期" << endl;
	cout << endl;

	cout << "选择1 or 2:";
	int n;
	cin >> n;

	vector<vector<int> > v;
	switch (n)
	{
	case 1:
		v = t->arrange_schedule1();
		t->show_arrangement(v);
		break;
	case 2:
		v = t->arrange_schedule2();
		t->show_arrangement(v);
		break;
	default:
		cout << "选择错误" << endl;
		system("pause");
	}
}

