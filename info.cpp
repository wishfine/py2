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

//������ѧ��
void Topo::compute_credits()
{
	float sum = 0;
	for (auto it = subjects.begin(); it != subjects.end(); it++)
		sum += it->credit;

	credits = sum;
}
//����v ������ȱ�
void Topo::compute_indegree_table()
{
	//��ʼ����ȣ�ȫΪ0
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

//������߱�
void Topo::compute_outdegree_table()
{
	//��ʼ�����߱�
	for (int i = 0; i < numOfSubject; i++)
		outdegreeTable[i] = {};
	//�������߱�
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

/*			��������: ����߱��������Ϊ0�Ķ��㣬Ȼ��ɾ���ö����
*					  ���г��ߣ����Ըö���Ϊ�����ҵ��ö������г���
*					  �ٽ����߶������ȼ�һ���������ö���
*/
bool Topo::topo_sort()
{	//�����������н�������δ���topoOrder
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

//ѧϰ�����������ȣ���ÿѧ�ڿ�Ŀ����
vector<vector<int> > Topo::arrange_schedule1()
{
	//�ؽ���ȱ���Ϊ�����������н���ɾ��
	compute_indegree_table();
	int n = 0;			//ѧ��
	int average_sub = numOfSubject / numOfTerm;	//ƽ��ÿѧ��Ӧ������
	int remainder = numOfSubject % numOfTerm;	//ѧ����������ѧ�ڵ�����
	int count = 0;		//����������						
	vector<vector<int> > res;

	while (n++ < numOfTerm)
	{
		int subjects_per_term = 0;
		float credits_sum = 0;
		vector<int> temp;

		for (auto it = indegreeTable.begin(); it != indegreeTable.end();)
		{	//����ÿѧ��ֻ�������Ϊ��Ķ���
			if (it->second == 0)
			{
				int i = it->first;
				//���ϴ��ſ�С������ѧ��
				if (subjects[i].credit + credits_sum <= limitOfCredit)
				{
					//��ǰ��ѡ��ĿС��ƽ�������ߵ���ƽ����remainder����
					if ((subjects_per_term < average_sub) ||
						(subjects_per_term == average_sub && count < remainder))
					{
						subjects_per_term++;
						credits_sum += subjects[i].credit;
						temp.push_back(i);
						//����Ǻ���
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

		//������������ͬ
		//�Ըö���Ϊ�����ҵ��ö������г���,�ٽ����߶������ȼ�һ
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

	//ͳ��res�еĿγ�����
	int sum = 0;
	for (auto it = res.begin(); it != res.end(); it++)
		sum += it->size();

	if (sum < numOfSubject)
		return { {} };
	return res;
}


/**
*	�ڶ��ַ�ʽ��һ��ȣ�����Ҫÿѧ�ڿγ̵����ƣ�ֻҪ��֤
*	ÿѧ��ѧ��������ѧ���ھ�����
*/
vector<vector<int> > Topo::arrange_schedule2()
{
	//�ؽ���ȱ�,��Ϊ�����������н���ɾ����
	compute_indegree_table();
	int n = 0;			//ѧ��

	vector<vector<int> > res;

	while (n++ < numOfTerm)
	{
		float credits_sum = 0;
		vector<int> temp;
		//v_zeros�洢�������Ϊ��Ķ���
		vector<decltype(indegreeTable.begin())> v_zeros;

		for (auto it = indegreeTable.begin(); it != indegreeTable.end();it++)
		{	//����ÿѧ��ֻ�������Ϊ��Ķ���
			if (it->second == 0)
				v_zeros.push_back(it);
		}

		//�����Ϊ0�Ķ���
		if (!v_zeros.empty())
		{
			//��ѧ����С����˳������,����lambda���ʽ
			sort(v_zeros.begin(), v_zeros.end(),
				[this](decltype(indegreeTable.begin()) a,
					decltype(indegreeTable.begin()) b) {
						return subjects[a->first].credit < subjects[b->first].credit;
				});

			//v_erase�洢��Ҫ�����Ķ���ָ��
			vector<decltype(indegreeTable.begin())> v_erase;

			//������ѧ�����С��limitOfCredits�Ŀ�Ŀ������Ӧ����
			for (auto it = v_zeros.begin();
				it != v_zeros.end(); it++)
			{
				if (credits_sum + subjects[(*it)->first].credit <= limitOfCredit)
				{
					credits_sum += subjects[(*it)->first].credit;
					temp.push_back((*it)->first);

					//����ָ���������������֮���������
					//�� ����֮�����ĳ��߶�����ȼ�һ
					v_erase.push_back(*it);
				}
			}

			/*����:	������������ͬ,�Ըö���Ϊ�����ҵ��ö������г���,
				�ٽ����߶������ȼ�һ*/
			if (!v_erase.empty())
			{
				//�޸����
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

				//��������
				for (auto t = v_erase.begin();
					t != v_erase.end(); t++)
					indegreeTable.erase(*t);
			}
		}

		res.push_back(temp);
	}

	//ͳ��res�еĿγ�����
	int sum = 0;
	for (auto it = res.begin(); it != res.end(); it++)
		sum += it->size();

	if (sum < numOfSubject)
		return{ {} };
	return res;
}

//��ʾ�γ̰���
void  Topo::show_arrangement(vector<vector<int> >& v)
{
	system("cls");

	if (v[0].empty())
	{
		cout << "����������Ϣ�޷����ţ�" << endl;
		return;
	}

	//�����ļ���
	ofstream outfile("�γ̰���.txt");

	for (int i = 0; i < v.size(); i++)
	{
		cout << "          ============��" << i + 1 << "ѧ�ڿα�=========\n\n";
		cout << "          �γ̴���      �γ�����       ѧ��" << endl;
		cout << endl;

		outfile << "          ============��" << i + 1 << "ѧ�ڿα�=========\n\n";
		outfile << "          �γ̴���      �γ�����       ѧ��" << endl;
		outfile << endl;
		for (int j = 0; j < v[i].size(); j++)
		{
			cout << subjects[v[i][j]];
			outfile << subjects[v[i][j]];
		}

		cout << endl;
		outfile << endl;
	}
	//�ر��ļ���
	outfile.close();
}
