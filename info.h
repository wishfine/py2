/*
*		课程结构体定义和Topo类定义
*
*/


#ifndef INFO_H_
#define INFO_H_

#include <string>
#include <vector>
#include <unordered_map>
using namespace std;

//学科信息
struct Subject
{
	string		subjectMark;	//课程代号
	string		subjectName;	//课程名
	float		credit;			//学分

	Subject(string mark, string name, float cre) :
		subjectMark(mark), subjectName(name), credit(cre) {}
	//操作符重载简化输出，避免代码重复
	//可以用于输出流
	friend ostream& operator << (ostream& out, Subject& s);
};

class Topo
{
public:
	bool topo_sort();								//拓扑排序函数
	void readin_before_subject(vector<int> vec);	//将先修课程读入v中
	void compute_indegree_table();					//计算入度
	void compute_outdegree_table();					//计算出边表
	void compute_credits();							//计算学分总数

	void set_num_of_subject(int num);				//设置学科总数
	void set_num_of_term(int num);					//设置学期总数
	void set_limit_of_credit(int limit);			//设置学分上线
	void readin_subject(Subject s);					//读入学科信息
	void  print_topo_order();						//打印拓扑排序后的序列
	void  show_arrangement(vector<vector<int> >& v); //显示课程安排
	vector<vector<int> > arrange_schedule1();		//使负担均匀
	vector<vector<int> > arrange_schedule2();		//尽量安排在前几个学期
private:
	int								numOfSubject;	//课程总数
	int								numOfTerm;		//学期总数
	float							limitOfCredit;	//学分上限
	float							credits;		//学分总数
	vector<vector<int> >			v;				//存储每个课的先修课程序号表
	vector<Subject>					subjects;		//根据输入的课程量来动态分配
	unordered_map<int, int>			indegreeTable;	//入度表 first为课序号，second为入度
	unordered_map<int, vector<int> > outdegreeTable;//出边表 first为序号，second为出边序列
	vector<int>						topoOrder;		//按拓扑排序后的课程序列
};


#endif#pragma once
