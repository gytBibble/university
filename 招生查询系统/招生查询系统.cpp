#include "stdafx.h"

int main()
{
	COORD size = { SCR_COL,SCR_ROW };                  /*���ڻ�������С*/
	gh_std_out = GetStdHandle(STD_OUTPUT_HANDLE);      /*��ȡ��׼����豸���*/
	gh_std_in = GetStdHandle(STD_INPUT_HANDLE);        /*��ȡ��׼�����豸���*/
	SetConsoleTitle(gp_sys_name);                      /*���ô��ڱ���*/
	SetConsoleScreenBufferSize(gh_std_out, size);      /*���ô��ڻ�������СΪ80*25*/
	Start();                                           /*��ӭ����*/
	InitInterface();                                   /*��ʼ��ҳ��*/
	LoadList(&phead,FileName);                         /*������Ϣ*/
	RunSys(&phead);                                    /*����ϵͳ*/
	return 0;
}

