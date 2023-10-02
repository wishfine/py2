/*
*		�γ̽ṹ�嶨���Topo�ඨ��
*
*/


#ifndef INFO_H_
#define INFO_H_

#include <string>
#include <vector>
#include <unordered_map>
using namespace std;

//ѧ����Ϣ
struct Subject
{
	string		subjectMark;	//�γ̴���
	string		subjectName;	//�γ���
	float		credit;			//ѧ��

	Subject(string mark, string name, float cre) :
		subjectMark(mark), subjectName(name), credit(cre) {}
	//���������ؼ��������������ظ�
	//�������������
	friend ostream& operator << (ostream& out, Subject& s);
};

class Topo
{
public:
	bool topo_sort();								//����������
	void readin_before_subject(vector<int> vec);	//�����޿γ̶���v��
	void compute_indegree_table();					//�������
	void compute_outdegree_table();					//������߱�
	void compute_credits();							//����ѧ������

	void set_num_of_subject(int num);				//����ѧ������
	void set_num_of_term(int num);					//����ѧ������
	void set_limit_of_credit(int limit);			//����ѧ������
	void readin_subject(Subject s);					//����ѧ����Ϣ
	void  print_topo_order();						//��ӡ��������������
	void  show_arrangement(vector<vector<int> >& v); //��ʾ�γ̰���
	vector<vector<int> > arrange_schedule1();		//ʹ��������
	vector<vector<int> > arrange_schedule2();		//����������ǰ����ѧ��
private:
	int								numOfSubject;	//�γ�����
	int								numOfTerm;		//ѧ������
	float							limitOfCredit;	//ѧ������
	float							credits;		//ѧ������
	vector<vector<int> >			v;				//�洢ÿ���ε����޿γ���ű�
	vector<Subject>					subjects;		//��������Ŀγ�������̬����
	unordered_map<int, int>			indegreeTable;	//��ȱ� firstΪ����ţ�secondΪ���
	unordered_map<int, vector<int> > outdegreeTable;//���߱� firstΪ��ţ�secondΪ��������
	vector<int>						topoOrder;		//�����������Ŀγ�����
};


#endif#pragma once
