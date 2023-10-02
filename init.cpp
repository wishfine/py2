#include "init.h"
#include "arrange.h"
#include <iostream>
using namespace std;

//初始化界面
void show_UI()
{	//不要用Tab
	cout << endl;
	cout << "             ===========教学计划编制系统=============" << endl;
	cout << "            |                                       |" << endl;
	cout << "            |       1.输入课程的相关信息            |" << endl;
	cout << "            |       2.打印课程计划同时输出到文件    |" << endl;
	cout << "            |       3.退出                          |" << endl;
	cout << "            |                                       |" << endl;
	cout << "            =======作者邮箱:1051818459@qq.com=======" << endl;
}

void choose()
{
	Topo* t = new Topo;
	while (true)
	{
		cout << "选择:";
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

	cout << "输入课程总数:";
	cin >> num_subject;

	cout << "输入学期总数:";
	cin >> num_term;

	cout << "输入一学期学分上限:";
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

	//从输入中读取课程信息
	readin_subject_information(t, num_subject);

	if (!init_topo(t))
	{
		cout << "不能构成拓扑序列！ 终止程序！\n\n";
		system("pause");
	}
}

//解析先修课程字符串
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
		cout << "课程数量超过100!" << endl;
		return false;
	}
	return true;
}


bool check_term(int term)
{
	if (term > MAXTERMS)
	{
		cout << "学期数大于12!" << endl;
		return false;
	}
	return true;
}

//读取输入的课程信息
void readin_subject_information(Topo* t, int num)
{
	cout << "    请输入课程(代号为C0--" << "C" << num - 1 << ")" << ",若没有先修课程用--代替\n" << endl;
	cout << "  课程代号      课程名称			学分    先修课程" << endl;
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

		//读入课程信息
		Subject s(mark, name, credit);
		t->readin_subject(s);

		//先修课程
		vector<int> vec = parse_course(before);
		t->readin_before_subject(vec);
	}
}

//初始化topo序列 作用仅仅是判断能否构成拓扑排序
//即判断AOE网中是否有环
bool init_topo(Topo* t)
{
	t->compute_indegree_table();
	t->compute_outdegree_table();
	return t->topo_sort() ? true : false;
}
