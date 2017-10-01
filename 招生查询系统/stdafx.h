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

#define SCR_COL 80          /*��Ļ����*/
#define SCR_ROW 25          /*��Ļ����*/

/*�����������ṹ*/
typedef struct recruit_node {
	char r_schoolId[10];          /*��У���*/
	char r_majorId[10];           /*רҵ���*/
	int year;                     /*�������*/
	int PlanNum;                  /*�ƻ���������*/
	int ActualNum;                /*ʵ����������*/
	int MaxScore;                 /*��߷�*/
	int MinScore;                 /*��ͷ�*/
	struct recruit_node *next;    /*ָ����һ����ָ��*/
}RecruitNode;

/*רҵ��Ϣ����ṹ*/
typedef struct major_node {
	char m_schoolId[10];          /*��У���*/
	char majorId[10];             /*רҵ���*/
	char majorName[30];           /*רҵ����*/
	struct recruit_node *r_next;  /*ָ��������ָ��*/
	struct major_node *next;      /*ָ����һרҵ��ָ��*/
}MajorNode;

/*��У��Ϣ����ṹ*/
typedef struct school_node {
	char schoolId[10];            /*��У���*/
	char schoolName[30];          /*��У����*/
	char schoolAddress[30];       /*��У��ַ*/
	char schoolPhone[15];         /*��У��ϵ�绰*/
	struct major_node *m_next;    /*ָ��רҵ��ָ��*/
	struct school_node *next;     /*ָ����һ����ָ��*/
}SchoolNode;

/*��Ļ������Ϣ�������ṹ*/
typedef struct layer_node {
	char LayerNo;            /* �������ڲ���*/
	SMALL_RECT rcArea;       /* ����������������*/
	CHAR_INFO *pContent;     /* �������������ַ���Ԫԭ��Ϣ�洢������*/
	char *pScrAtt;           /* �������������ַ���Ԫԭ����ֵ�洢������*/
	struct layer_node *next; /* ָ����һ����ָ��*/
} LAYER_NODE;

/*��ǩ���ṹ*/
typedef struct labe1_bundle {
	char **ppLabel;        /*��ǩ�ַ��������׵�ַ*/
	COORD *pLoc;           /* ��ǩ��λ�����׵�ַ*/
	int num;               /* ��ǩ����*/
} LABEL_BUNDLE;

/*�����ṹ*/
typedef struct hot_area {
	SMALL_RECT *pArea;     /*������λ�����׵�ַ*/
	char *pSort;           /* �������(�������ı���ѡ���)�����׵�ַ*/
	char *pTag;            /* ������������׵�ַ*/
	int num;               /*��������*/
} HOT_AREA;

LAYER_NODE *gp_top_layer = NULL;               /*����������Ϣ����ͷ*/
SchoolNode *phead = NULL;                      /*����ͷָ��*/

HANDLE gh_std_out;          /*��׼����豸���*/
HANDLE gh_std_in;           /*��׼�����豸���*/

char *FileName[3] = { "school.txt" ,"major.txt","recruit.txt" };                  /*�����ļ�*/
char *BeiFen_FileName[3]= { "backschool.txt" ,"backmajor.txt","backrecruit.txt" };/*���ݱ����ļ�*/

wchar_t* gp_sys_name = TEXT("������ѯϵͳ");/*ϵͳ����*/

char* ga_main_menu[] = { "�ļ�(F)","����ά��(M)","���ݲ�ѯ(Q)","����ͳ��(S)","����(H)" };
char* ga_sub_menu[] = { "[S]���ݱ���","[B]���ݱ���","[R]���ݻָ�","","[X]�˳�",
"[S]��У��Ϣά��","[M]רҵ��Ϣά��","[R]������Ϣά��",
"[S]��У��ѯ","[M]רҵ��ѯ","[R]������ѯ","","[Q]�ۺϲ�ѯ",
"[S]¼����Ϣ����","","[N]�����������",
"[H]��������","","[A]����..."};

int ga_sub_menu_count[] = { 5,3,5,3,3 };/*�����˵����Ӳ˵��ĸ���*/
int gi_sel_menu = 1;                    /*��ѡ�е����˵���ţ���ʼΪ1*/
int gi_sel_sub_menu = 0;                /*��ѡ�е��Ӳ˵���ţ���ʼΪ0����ʾδѡ��*/

CHAR_INFO* gp_buff_menubar_info = NULL;     /*��Ų˵�����Ļ���ַ���Ϣ�Ļ�����*/
CHAR_INFO* gp_buff_stateBar_info = NULL;    /*���״̬����Ļ���ַ���Ϣ�Ļ�����*/

char* gp_scr_att = NULL;                    /*�����Ļ���ַ���Ԫ����ֵ�Ļ�����*/

inline void Delay(void);		            /**< ʱ������0.05s*/
inline void  ScreenFresh(void);		        /**< ˢ������*/
void goto_xy(int x, int y);                 /*��λ���*/
void Start(void);	    					/**< ��ʼ����ϵͳ��ӭ����.*/

unsigned long written_num=0;
void ShowMenu(void);                        /*����Ļ����ʾ���˵��������������������˵���һ������ѡ�б��*/
void PopMenu(int num);                      /*��ʾ�����˵�*/
void PopOff(void);                          /*�رն��㵯������*/
void PopUp(SMALL_RECT* pRc, WORD att, LABEL_BUNDLE* pLabel, HOT_AREA* pHotArea);    /*����������Ļ��Ϣά��*/
void LocSubMenu(int num, SMALL_RECT* parea);/*���˵������˵���λ*/
void DrawBox(SMALL_RECT* parea);            /*���Ʊ߿�*/     
void TagMainMenu(int num);                  /*��Ǳ�ѡ�е����˵���*/
void TagSubMenu(int num);                   /*��Ǳ�ѡ�е��Ӳ˵���*/
void ClearScreen(void);                     /*�����Ļ��Ϣ*/
void InitInterface(void);                   /*ϵͳ�����ʼ��*/
void RunSys(SchoolNode **phead);            /*ϵͳ����ģ���ѡ��������*/
void CloseSys(SchoolNode *phead);           /*�˳�ϵͳ*/
void SetHotPoint(HOT_AREA *pHotArea, int iHot);             /*��������*/							
int DealInput(HOT_AREA *pHotArea, int *piHot);              /*����̨���봦��*/
BOOL ExeFunction(int main_menu_num, int sub_menu_num);      /*����ģ��ĵ���*/

void SaveList(SchoolNode *phead, char *fn[3]);              /**< ����������ݴ���*/
void LoadList(SchoolNode  **phead, char *fn[3]);            /**< �����ļ��е���Ϣ*/
BOOL LoadData(void);                  /*���ݼ���*/
BOOL SaveData(void);                  /*��������*/
BOOL BackupData(void);                /*��������*/
BOOL RestoreData(void);               /*�ָ�����*/
BOOL ExitSys(void);                   /*�˳�ϵͳ*/
BOOL HelpTopic(void);                 /*��������*/
BOOL AboutDorm(void);                 /*����ϵͳ*/

BOOL QuerySchInfo(void);              /*��ѯ��У��Ϣ*/
BOOL QueryMajorInfo(void);            /*��ѯרҵ��Ϣ*/
BOOL QueryRecInfo(void);              /*��ѯ������Ϣ*/
BOOL QueryMajorInfoM(void);           /*�ۺϲ�ѯ��Ӧ��רҵ*/

BOOL StatSchNum(void);                /*ͳ�Ƹ�У��������*/
BOOL StatRecNum(void);                /*�����������ͳ��*/

BOOL MaintainSchInfo(void);           /*ά����У��Ϣ*/
BOOL MaintainMajorInfo(void);         /*ά��רҵ��Ϣ*/
BOOL MaintainRecInfo(void);           /*ά��������Ϣ*/

SchoolNode *SeekSchNode(SchoolNode *hd, char *sch);/*���Ҹ�У��Ϣ���*/
MajorNode *SeekMajorNode(SchoolNode *hd, char *major_id);/*����רҵ������Ϣ���*/
RecruitNode *SeekRecNode(char *sch, char *maj, int year);/*����������Ϣ���*/
BOOL InsertSchNode(SchoolNode **hd, SchoolNode *psch_node);/*�����У��Ϣ���*/
BOOL InsertMajorNode(SchoolNode *hd, MajorNode *pmajor_node);/*����רҵ��Ϣ���*/
BOOL InsertRecNode(SchoolNode *hd, RecruitNode *prec_node);/*����������Ϣ���*/
int DelSchNode(SchoolNode **hd, char *sch_id);/*ɾ����У��Ϣ���*/
int DelMajorNode(SchoolNode *hd, char *major_id);/*ɾ��רҵ��Ϣ���*/
int DelRecNode(char *sch, char *maj, int year);/*ɾ��������Ϣ���*/
wchar_t* AnsiToUnicode(const char* szStr);
#endif