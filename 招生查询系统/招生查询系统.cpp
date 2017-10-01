#include "stdafx.h"

int main()
{
	COORD size = { SCR_COL,SCR_ROW };                  /*窗口缓冲区大小*/
	gh_std_out = GetStdHandle(STD_OUTPUT_HANDLE);      /*获取标准输出设备句柄*/
	gh_std_in = GetStdHandle(STD_INPUT_HANDLE);        /*获取标准输入设备句柄*/
	SetConsoleTitle(gp_sys_name);                      /*设置窗口标题*/
	SetConsoleScreenBufferSize(gh_std_out, size);      /*设置窗口缓冲区大小为80*25*/
	Start();                                           /*欢迎界面*/
	InitInterface();                                   /*初始化页面*/
	LoadList(&phead,FileName);                         /*加载信息*/
	RunSys(&phead);                                    /*运行系统*/
	return 0;
}

