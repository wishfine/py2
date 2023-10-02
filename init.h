/**
*		���������ͳ�ʼ����صĺ���
*/

#ifndef INIT_H_
#define INIT_H_

#include "info.h"
#include <vector>
using namespace std;

#define MAXSUBJECTS 100
#define MAXTERMS	12

//��ʾ��ʼ������
void show_UI();

//ѡ��1����2 ������ִ�о������
void choose();

//����ѧ������ÿѧ�����ѧ������
void set_informations(Topo* t);

//�ַ������޿γ�����ȡ����
vector<int> parse_course(const string& s);

//���γ��� ѧ����
bool check_sum(int sum);
bool check_term(int term);

//����γ���Ϣ
void readin_subject_information(Topo* t, int num);

//���ݶ����subject��Ϣ��ʼ����߱����߱�����topo��������
bool init_topo(Topo* t);

#endif
#pragma once
