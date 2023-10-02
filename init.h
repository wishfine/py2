/**
*		与解析输入和初始化相关的函数
*/

#ifndef INIT_H_
#define INIT_H_

#include "info.h"
#include <vector>
using namespace std;

#define MAXSUBJECTS 100
#define MAXTERMS	12

//显示初始化界面
void show_UI();

//选择1或者2 在里面执行具体操作
void choose();

//设置学期数，每学期最大学分上限
void set_informations(Topo* t);

//字符串先修课程中提取整数
vector<int> parse_course(const string& s);

//检查课程数 学期数
bool check_sum(int sum);
bool check_term(int term);

//读入课程信息
void readin_subject_information(Topo* t, int num);

//根据读入的subject信息初始化入边表，出边表，计算topo排序序列
bool init_topo(Topo* t);

#endif
#pragma once
