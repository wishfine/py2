#include "info.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <iomanip>
using namespace std;


ostream& operator << (ostream& out, Subject& s)
{
	out << "          ";
	out << setw(14) << setiosflags(ios::left) << s.subjectMark;
	out << setw(16) << setiosflags(ios::left) << s.subjectName;
	out << setw(4) << setiosflags(ios::left) << s.credit << endl;

	return out;
}

//计算总学分
void Topo::compute_credits()
{
	float sum = 0;
	for (auto it = subjects.begin(); it != subjects.end(); it++)
		sum += it->credit;

	credits = sum;
}
//根据v 来建入度表
void Topo::compute_indegree_table()
{
	//初始化入度，全为0
	for (int i = 0; i < numOfSubject; i++)
		indegreeTable[i] = 0;

	for (int i = 0; i < numOfSubject; i++)
	{
		vector<int> temp = v[i];
		if (!temp.empty())
		{
			indegreeTable[i] = temp.size();
		}
	}
}

//计算出边表
void Topo::compute_outdegree_table()
{
	//初始化出边表
	for (int i = 0; i < numOfSubject; i++)
		outdegreeTable[i] = {};
	//构建出边表
	for (int i = 0; i < numOfSubject; i++)
	{
		vector<int> temp = v[i];
		if (!temp.empty())
		{
			for (auto it = temp.begin(); it != temp.end(); it++)
				outdegreeTable[*it].push_back(i);
		}
	}
}

/*			拓扑排序: 从入边表中找入度为0的顶点，然后删除该顶点的
*					  所有出边，即以该顶点为索引找到该顶点所有出边
*					  再将出边顶点的入度减一，最后擦除该顶点
*/
bool Topo::topo_sort()
{	//按照拓扑序列将结果依次存入topoOrder
	for (int i = 0; i < numOfSubject; i++)
	{
		auto it = indegreeTable.begin();
		for (; it != indegreeTable.end();it++)
		{
			if (it->second == 0)
			{
				int m = it->first;
				topoOrder.push_back(m);
				if (!outdegreeTable[m].empty())
				{
					for (auto t = outdegreeTable[m].begin();
						t != outdegreeTable[m].end(); t++)
						indegreeTable[*t]--;
				}

				break;
			}
		}

		indegreeTable.erase(it);
	}

	return indegreeTable.empty() ? true : false;
}

void Topo::readin_before_subject(vector<int> vec)
{
	v.push_back(vec);
}

void Topo::set_num_of_subject(int num)
{
	numOfSubject = num;
}

void Topo::set_num_of_term(int num)
{
	numOfTerm = num;
}

void Topo::set_limit_of_credit(int limit)
{
	limitOfCredit = limit;
}

void Topo::readin_subject(Subject s)
{
	subjects.push_back(s);
}

void Topo::print_topo_order()
{
	for (auto a : topoOrder)
		cout << a << " ";
}

//学习负担尽量均匀，即每学期科目均匀
vector<vector<int> > Topo::arrange_schedule1()
{
	//重建入度表因为在拓扑排序中将其删除
	compute_indegree_table();
	int n = 0;			//学期
	int average_sub = numOfSubject / numOfTerm;	//平均每学期应修门数
	int remainder = numOfSubject % numOfTerm;	//学科总数除以学期的余数
	int count = 0;		//对余数计数						
	vector<vector<int> > res;

	while (n++ < numOfTerm)
	{
		int subjects_per_term = 0;
		float credits_sum = 0;
		vector<int> temp;

		for (auto it = indegreeTable.begin(); it != indegreeTable.end();)
		{	//对于每学期只能是入度为零的顶点
			if (it->second == 0)
			{
				int i = it->first;
				//加上此门课小于限制学分
				if (subjects[i].credit + credits_sum <= limitOfCredit)
				{
					//当前已选科目小于平均，或者等于平均但remainder还有
					if ((subjects_per_term < average_sub) ||
						(subjects_per_term == average_sub && count < remainder))
					{
						subjects_per_term++;
						credits_sum += subjects[i].credit;
						temp.push_back(i);
						//如果是后者
						if (subjects_per_term == average_sub + 1)
							count++;

						it = indegreeTable.erase(it);
					}
					else
						it++;
				}
				else
					it++;
			}
			else
				it++;
		}

		//和拓扑排序相同
		//以该顶点为索引找到该顶点所有出边,再将出边顶点的入度减一
		if (!temp.empty())
		{
			for (auto p = temp.begin(); p != temp.end(); p++)
				if (!outdegreeTable[*p].empty())
				{
					for (auto t = outdegreeTable[*p].begin();
						t != outdegreeTable[*p].end();t++)
						indegreeTable[*t]--;
				}
		}

		res.push_back(temp);
	}

	//统计res中的课程总数
	int sum = 0;
	for (auto it = res.begin(); it != res.end(); it++)
		sum += it->size();

	if (sum < numOfSubject)
		return { {} };
	return res;
}


/**
*	第二种方式与一相比，不需要每学期课程的限制，只要保证
*	每学期学分在限制学分内就行了
*/
vector<vector<int> > Topo::arrange_schedule2()
{
	//重建入度表,因为在拓扑排序中将其删除了
	compute_indegree_table();
	int n = 0;			//学期

	vector<vector<int> > res;

	while (n++ < numOfTerm)
	{
		float credits_sum = 0;
		vector<int> temp;
		//v_zeros存储所有入度为零的顶点
		vector<decltype(indegreeTable.begin())> v_zeros;

		for (auto it = indegreeTable.begin(); it != indegreeTable.end();it++)
		{	//对于每学期只能是入度为零的顶点
			if (it->second == 0)
				v_zeros.push_back(it);
		}

		//有入度为0的顶点
		if (!v_zeros.empty())
		{
			//按学分由小到大顺序排序,运用lambda表达式
			sort(v_zeros.begin(), v_zeros.end(),
				[this](decltype(indegreeTable.begin()) a,
					decltype(indegreeTable.begin()) b) {
						return subjects[a->first].credit < subjects[b->first].credit;
				});

			//v_erase存储将要擦除的顶点指针
			vector<decltype(indegreeTable.begin())> v_erase;

			//将满足学分相加小于limitOfCredits的科目放入相应容器
			for (auto it = v_zeros.begin();
				it != v_zeros.end(); it++)
			{
				if (credits_sum + subjects[(*it)->first].credit <= limitOfCredit)
				{
					credits_sum += subjects[(*it)->first].credit;
					temp.push_back((*it)->first);

					//将该指针放入容器，用于之后擦除顶点
					//和 把与之关联的出边顶点入度减一
					v_erase.push_back(*it);
				}
			}

			/*擦除:	和拓扑排序相同,以该顶点为索引找到该顶点所有出边,
				再将出边顶点的入度减一*/
			if (!v_erase.empty())
			{
				//修改入度
				for (auto t = v_erase.begin();
					t != v_erase.end(); t++)
				{
					if (!outdegreeTable[(*t)->first].empty())
					{
						for (auto p = outdegreeTable[(*t)->first].begin();
							p != outdegreeTable[(*t)->first].end(); p++)
							indegreeTable[*p]--;
					}
				}

				//擦除顶点
				for (auto t = v_erase.begin();
					t != v_erase.end(); t++)
					indegreeTable.erase(*t);
			}
		}

		res.push_back(temp);
	}

	//统计res中的课程总数
	int sum = 0;
	for (auto it = res.begin(); it != res.end(); it++)
		sum += it->size();

	if (sum < numOfSubject)
		return{ {} };
	return res;
}

//显示课程安排
void  Topo::show_arrangement(vector<vector<int> >& v)
{
	system("cls");

	if (v[0].empty())
	{
		cout << "根据输入信息无法安排！" << endl;
		return;
	}

	//创建文件流
	ofstream outfile("课程安排.txt");

	for (int i = 0; i < v.size(); i++)
	{
		cout << "          ============第" << i + 1 << "学期课表=========\n\n";
		cout << "          课程代号      课程名称       学分" << endl;
		cout << endl;

		outfile << "          ============第" << i + 1 << "学期课表=========\n\n";
		outfile << "          课程代号      课程名称       学分" << endl;
		outfile << endl;
		for (int j = 0; j < v[i].size(); j++)
		{
			cout << subjects[v[i][j]];
			outfile << subjects[v[i][j]];
		}

		cout << endl;
		outfile << endl;
	}
	//关闭文件流
	outfile.close();
}
