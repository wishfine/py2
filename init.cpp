#include "init.h"
#include "arrange.h"
#include <iostream>
using namespace std;

//��ʼ������
void show_UI()
{	//��Ҫ��Tab
	cout << endl;
	cout << "             ===========��ѧ�ƻ�����ϵͳ=============" << endl;
	cout << "            |                                       |" << endl;
	cout << "            |       1.����γ̵������Ϣ            |" << endl;
	cout << "            |       2.��ӡ�γ̼ƻ�ͬʱ������ļ�    |" << endl;
	cout << "            |       3.�˳�                          |" << endl;
	cout << "            |                                       |" << endl;
	cout << "            =======��������:1051818459@qq.com=======" << endl;
}

void choose()
{
	Topo* t = new Topo;
	while (true)
	{
		cout << "ѡ��:";
		int choose;
		cin >> choose;

		switch (choose)
		{
		case 1:
			delete t;
			t = new Topo;
			set_informations(t);
			cout << "\n" << endl;
			break;
		case 2:
			arrange(t);
			cout << "\n" << endl;
			break;
		case 3:
			exit(0);
		default:
			exit(0);
		}
	}
}

void set_informations(Topo* t)
{
	int num_term = 0, credits_limit = 0, num_subject = 0;

	cout << "����γ�����:";
	cin >> num_subject;

	cout << "����ѧ������:";
	cin >> num_term;

	cout << "����һѧ��ѧ������:";
	cin >> credits_limit;

	if (check_sum(num_subject) && check_term(num_term))
	{
		t->set_num_of_subject(num_subject);
		t->set_num_of_term(num_term);
		t->set_limit_of_credit(credits_limit);
	}
	else
		system("pause");

	cout << "\n";

	//�������ж�ȡ�γ���Ϣ
	readin_subject_information(t, num_subject);

	if (!init_topo(t))
	{
		cout << "���ܹ����������У� ��ֹ����\n\n";
		system("pause");
	}
}

//�������޿γ��ַ���
vector<int> parse_course(const string& s)
{
	int sum = 0;
	int len = s.length();
	vector<int> res;
	for (int i = 0; i < len; i++)
	{
		if (s[i] >= '0' && s[i] <= '9')
		{
			sum = 10 * sum + s[i] - '0';
			if (i + 1 == len || !(s[i + 1] >= '0' && s[i + 1] <= '9'))
			{
				res.push_back(sum);
				sum = 0;
			}
		}
	}

	return res;
}

bool check_sum(int sum)
{
	if (sum > MAXSUBJECTS)
	{
		cout << "�γ���������100!" << endl;
		return false;
	}
	return true;
}


bool check_term(int term)
{
	if (term > MAXTERMS)
	{
		cout << "ѧ��������12!" << endl;
		return false;
	}
	return true;
}

//��ȡ����Ŀγ���Ϣ
void readin_subject_information(Topo* t, int num)
{
	cout << "    ������γ�(����ΪC0--" << "C" << num - 1 << ")" << ",��û�����޿γ���--����\n" << endl;
	cout << "  �γ̴���      �γ�����			ѧ��    ���޿γ�" << endl;
	int i = 0;
	while (i < num)
	{
		string mark;
		string name;
		float credit;
		string before;

		cout << ++i << ". ";
		cin >> mark;
		cin >> name;
		cin >> credit >> before;

		//����γ���Ϣ
		Subject s(mark, name, credit);
		t->readin_subject(s);

		//���޿γ�
		vector<int> vec = parse_course(before);
		t->readin_before_subject(vec);
	}
}

//��ʼ��topo���� ���ý������ж��ܷ񹹳���������
//���ж�AOE�����Ƿ��л�
bool init_topo(Topo* t)
{
	t->compute_indegree_table();
	t->compute_outdegree_table();
	return t->topo_sort() ? true : false;
}
