#include "arrange.h"
#include <iostream>

using namespace std;

void arrange(Topo* t)
{
	cout << "               ��ѡ��γ̰��ŵķ�ʽ			" << endl;
	cout << endl;
	cout << "           1.��ѧ�ڵ�ѧϰ������������" << endl;
	cout << endl;
	cout << "           2.�γ̾����ܼ�����ǰ����ѧ��" << endl;
	cout << endl;

	cout << "ѡ��1 or 2:";
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
		cout << "ѡ�����" << endl;
		system("pause");
	}
}

