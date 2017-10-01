#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <wincon.h>
#include <conio.h>
#include <string.h>
#include <io.h>
#include <fcntl.h>
#include <sys\stat.h>
#include <ctype.h>
#include <time.h>

#ifndef TYPE_H_INCLUDED
#define TYPE_H_INCLUDED

#define SCR_COL 80          /*屏幕列数*/
#define SCR_ROW 25          /*屏幕行数*/

/*招生情况链表结构*/
typedef struct recruit_node {
	char r_schoolId[10];          /*高校编号*/
	char r_majorId[10];           /*专业编号*/
	int year;                     /*招生年份*/
	int PlanNum;                  /*计划招生人数*/
	int ActualNum;                /*实际招生人数*/
	int MaxScore;                 /*最高分*/
	int MinScore;                 /*最低分*/
	struct recruit_node *next;    /*指向下一结点的指针*/
}RecruitNode;

/*专业信息链表结构*/
typedef struct major_node {
	char m_schoolId[10];          /*高校编号*/
	char majorId[10];             /*专业编号*/
	char majorName[30];           /*专业名称*/
	struct recruit_node *r_next;  /*指向招生的指针*/
	struct major_node *next;      /*指向下一专业的指针*/
}MajorNode;

/*高校信息链表结构*/
typedef struct school_node {
	char schoolId[10];            /*高校编号*/
	char schoolName[30];          /*高校名称*/
	char schoolAddress[30];       /*高校地址*/
	char schoolPhone[15];         /*高校联系电话*/
	struct major_node *m_next;    /*指向专业的指针*/
	struct school_node *next;     /*指向下一结点的指针*/
}SchoolNode;

/*屏幕窗口信息链结点结点结构*/
typedef struct layer_node {
	char LayerNo;            /* 弹出窗口层数*/
	SMALL_RECT rcArea;       /* 弹出窗口区域坐标*/
	CHAR_INFO *pContent;     /* 弹出窗口区域字符单元原信息存储缓冲区*/
	char *pScrAtt;           /* 弹出窗口区域字符单元原属性值存储缓冲区*/
	struct layer_node *next; /* 指向下一结点的指针*/
} LAYER_NODE;

/*标签束结构*/
typedef struct labe1_bundle {
	char **ppLabel;        /*标签字符串数组首地址*/
	COORD *pLoc;           /* 标签定位数组首地址*/
	int num;               /* 标签个数*/
} LABEL_BUNDLE;

/*热区结构*/
typedef struct hot_area {
	SMALL_RECT *pArea;     /*热区定位数组首地址*/
	char *pSort;           /* 热区类别(按键、文本框、选项框)数组首地址*/
	char *pTag;            /* 热区序号数组首地址*/
	int num;               /*热区个数*/
} HOT_AREA;

LAYER_NODE *gp_top_layer = NULL;               /*弹出窗口信息链链头*/
SchoolNode *phead = NULL;                      /*主链头指针*/

HANDLE gh_std_out;          /*标准输出设备句柄*/
HANDLE gh_std_in;           /*标准输入设备句柄*/

char *FileName[3] = { "school.txt" ,"major.txt","recruit.txt" };                  /*数据文件*/
char *BeiFen_FileName[3]= { "backschool.txt" ,"backmajor.txt","backrecruit.txt" };/*数据备份文件*/

wchar_t* gp_sys_name = TEXT("招生查询系统");/*系统名称*/

char* ga_main_menu[] = { "文件(F)","数据维护(M)","数据查询(Q)","数据统计(S)","帮助(H)" };
char* ga_sub_menu[] = { "[S]数据保存","[B]数据备份","[R]数据恢复","","[X]退出",
"[S]高校信息维护","[M]专业信息维护","[R]招生信息维护",
"[S]高校查询","[M]专业查询","[R]招生查询","","[Q]综合查询",
"[S]录入信息总数","","[N]年份招生人数",
"[H]帮助主题","","[A]关于..."};

int ga_sub_menu_count[] = { 5,3,5,3,3 };/*个主菜单下子菜单的个数*/
int gi_sel_menu = 1;                    /*被选中的主菜单项号，初始为1*/
int gi_sel_sub_menu = 0;                /*被选中的子菜单项号，初始为0，表示未选中*/

CHAR_INFO* gp_buff_menubar_info = NULL;     /*存放菜单条屏幕区字符信息的缓冲区*/
CHAR_INFO* gp_buff_stateBar_info = NULL;    /*存放状态条屏幕区字符信息的缓冲区*/

char* gp_scr_att = NULL;                    /*存放屏幕上字符单元属性值的缓冲区*/

inline void Delay(void);		            /**< 时间暂延0.05s*/
inline void  ScreenFresh(void);		        /**< 刷屏函数*/
void goto_xy(int x, int y);                 /*定位光标*/
void Start(void);	    					/**< 开始进入系统欢迎界面.*/

unsigned long written_num=0;
void ShowMenu(void);                        /*在屏幕上显示主菜单，并设置热区，在主菜单第一项上置选中标记*/
void PopMenu(int num);                      /*显示下拉菜单*/
void PopOff(void);                          /*关闭顶层弹出窗口*/
void PopUp(SMALL_RECT* pRc, WORD att, LABEL_BUNDLE* pLabel, HOT_AREA* pHotArea);    /*弹出窗口屏幕信息维护*/
void LocSubMenu(int num, SMALL_RECT* parea);/*主菜单下拉菜单定位*/
void DrawBox(SMALL_RECT* parea);            /*绘制边框*/     
void TagMainMenu(int num);                  /*标记被选中的主菜单项*/
void TagSubMenu(int num);                   /*标记被选中的子菜单项*/
void ClearScreen(void);                     /*清除屏幕信息*/
void InitInterface(void);                   /*系统界面初始化*/
void RunSys(SchoolNode **phead);            /*系统功能模块的选择与运行*/
void CloseSys(SchoolNode *phead);           /*退出系统*/
void SetHotPoint(HOT_AREA *pHotArea, int iHot);             /*设置热区*/							
int DealInput(HOT_AREA *pHotArea, int *piHot);              /*控制台输入处理*/
BOOL ExeFunction(int main_menu_num, int sub_menu_num);      /*功能模块的调用*/

void SaveList(SchoolNode *phead, char *fn[3]);              /**< 将链表的内容存盘*/
void LoadList(SchoolNode  **phead, char *fn[3]);            /**< 加载文件中的信息*/
BOOL LoadData(void);                  /*数据加载*/
BOOL SaveData(void);                  /*保存数据*/
BOOL BackupData(void);                /*备份数据*/
BOOL RestoreData(void);               /*恢复数据*/
BOOL ExitSys(void);                   /*退出系统*/
BOOL HelpTopic(void);                 /*帮助主题*/
BOOL AboutDorm(void);                 /*关于系统*/

BOOL QuerySchInfo(void);              /*查询高校信息*/
BOOL QueryMajorInfo(void);            /*查询专业信息*/
BOOL QueryRecInfo(void);              /*查询招生信息*/
BOOL QueryMajorInfoM(void);           /*综合查询相应的专业*/

BOOL StatSchNum(void);                /*统计高校数据总数*/
BOOL StatRecNum(void);                /*年份招生人数统计*/

BOOL MaintainSchInfo(void);           /*维护高校信息*/
BOOL MaintainMajorInfo(void);         /*维护专业信息*/
BOOL MaintainRecInfo(void);           /*维护招生信息*/

SchoolNode *SeekSchNode(SchoolNode *hd, char *sch);/*查找高校信息结点*/
MajorNode *SeekMajorNode(SchoolNode *hd, char *major_id);/*查找专业基本信息结点*/
RecruitNode *SeekRecNode(char *sch, char *maj, int year);/*查找招生信息结点*/
BOOL InsertSchNode(SchoolNode **hd, SchoolNode *psch_node);/*插入高校信息结点*/
BOOL InsertMajorNode(SchoolNode *hd, MajorNode *pmajor_node);/*插入专业信息结点*/
BOOL InsertRecNode(SchoolNode *hd, RecruitNode *prec_node);/*插入招生信息结点*/
int DelSchNode(SchoolNode **hd, char *sch_id);/*删除高校信息结点*/
int DelMajorNode(SchoolNode *hd, char *major_id);/*删除专业信息结点*/
int DelRecNode(char *sch, char *maj, int year);/*删除招生信息结点*/
wchar_t* AnsiToUnicode(const char* szStr);
#endif