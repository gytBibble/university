#include "stdafx.h"
/**
* ��������: InitInterface
* ��������: ��ʼ������.
* �������: ��
* �������: ��
* �� �� ֵ: ��
* ����˵��:
*/
void InitInterface(void)
{
	WORD att = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY | BACKGROUND_BLUE;

	SetConsoleTextAttribute(gh_std_out, att);          /*���û�ɫǰ������ɫ����*/
	ClearScreen();                                     /*����*/
	/*��������������Ϣ��ջ������ʼ�������Ļ������һ�㵯������*/
	gp_scr_att = (char*)calloc(SCR_COL*SCR_ROW, sizeof(char));/*��Ļ�ַ�����*/
	int posa = 0, posb = 0;
	for (int i = 0; i<5; i++)
	{
		if (i != 0)
		{
			posa += strlen(ga_main_menu[i - 1]) + 8;
		}
		posb = posa + strlen(ga_main_menu[i]) + 8;
		for (int j = posa; j<posb; j++)
		{
			gp_scr_att[j] = ((i + 1) << 2);
		}
	}
	gp_top_layer = (LAYER_NODE*)malloc(sizeof(LAYER_NODE));
	gp_top_layer->LayerNo = 0;                                /*�������ڵĲ��Ϊ0*/
	gp_top_layer->rcArea.Left = 0;                            /*�������ڵ�����Ϊ������Ļ����*/
	gp_top_layer->rcArea.Top = 0;
	gp_top_layer->rcArea.Right = SCR_COL - 1;
	gp_top_layer->rcArea.Bottom = SCR_ROW - 1;
	gp_top_layer->pContent = NULL;
	gp_top_layer->pScrAtt = gp_scr_att;
	gp_top_layer->next = NULL;
	ShowMenu();                                        /*��ʾϵͳ���˵�*/


}

/**
* ��������: ShowMenu
* ��������: ����Ļ����ʾ���˵�, ����������, �����˵���һ������ѡ�б��.
* �������: ��
* �������: ��
* �� �� ֵ: ��
* ����˵��:
*/
void ShowMenu()
{
	CONSOLE_SCREEN_BUFFER_INFO bInfo;
	CONSOLE_CURSOR_INFO lpCur;
	COORD size;
	COORD pos = { 0,0 };
	int i;
	int PosA = 2;
	char ch;
	GetConsoleScreenBufferInfo(gh_std_out, &bInfo);
	size.X = bInfo.dwSize.X;
	size.Y = 1;
	SetConsoleCursorPosition(gh_std_out, pos);
	for (i = 0; i < 5; i++) /*�ڴ��ڵ�һ�е�һ�д�������˵���*/
	{
		printf("    %s    ", ga_main_menu[i]);
	}
	GetConsoleCursorInfo(gh_std_out, &lpCur);
	lpCur.bVisible = FALSE;
	SetConsoleCursorInfo(gh_std_out, &lpCur);  /*���ع��*/
	/*���붯̬�洢����Ϊ��Ų˵�����Ļ���ַ���Ϣ�Ļ�����*/
	gp_buff_menubar_info = (CHAR_INFO *)malloc(size.X * size.Y * sizeof(CHAR_INFO));
	SMALL_RECT rcMenu = { 0, 0, size.X - 1, 0 };
	/*�����ڵ�һ�е����ݶ��뵽��Ų˵�����Ļ���ַ���Ϣ�Ļ�������*/
	ReadConsoleOutput(gh_std_out, gp_buff_menubar_info, size, pos, &rcMenu);
	/*����һ����Ӣ����ĸ��Ϊ��ɫ�������ַ���Ԫ��Ϊ�׵׺���*/
	for (i = 0; i<size.X; i++)
	{
		(gp_buff_menubar_info + i)->Attributes = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;
		ch = (char)((gp_buff_menubar_info + i)->Char.AsciiChar);
	    (gp_buff_menubar_info + i)->Attributes |= FOREGROUND_RED;
	}
	/*�޸ĺ�Ĳ˵����ַ���Ϣ��д�����ڵĵ�һ��*/
	WriteConsoleOutput(gh_std_out, gp_buff_menubar_info, size, pos, &rcMenu);
	TagMainMenu(gi_sel_menu);  /*��ѡ�����˵���������ǣ�gi_sel_menu��ֵΪ1*/
	return;
}

/**
* ��������: TagMainMenu
* ��������: ��ָ�����˵�������ѡ�б�־.
* �������: num ѡ�е����˵����
* �������: ��
* �� �� ֵ: ��
* ����˵��:
*/
void TagMainMenu(int num)
{
	CONSOLE_SCREEN_BUFFER_INFO bInfo;
	COORD size;
	COORD pos = { 0, 0 };
	int PosA = 2, PosB;
	char ch;
	int i;
	if (num == 0) /*numΪ0ʱ������ȥ�����˵���ѡ�б��*/
	{
		PosA = 0;
		PosB = 0;
	}
	else  /*���򣬶�λѡ�����˵������ֹλ��: PosAΪ��ʼλ��, PosBΪ��ֹλ��*/
	{
		for (i = 1; i<num; i++)
		{
			PosA += strlen(ga_main_menu[i - 1]) + 4;
		}
		PosB = PosA + strlen(ga_main_menu[num - 1]);
	}
	GetConsoleScreenBufferInfo(gh_std_out, &bInfo);
	size.X = bInfo.dwSize.X;
	size.Y = 1;
	/*ȥ��ѡ�в˵���ǰ��Ĳ˵���ѡ�б��*/
	for (i = 0; i<PosA; i++)
	{
		(gp_buff_menubar_info + i)->Attributes = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;
		ch = (gp_buff_menubar_info + i)->Char.AsciiChar;
		if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'))
		{
			(gp_buff_menubar_info + i)->Attributes |= FOREGROUND_RED;
		}
	}
	/*��ѡ�в˵���������ǣ��ڵװ���*/
	for (i = PosA; i<PosB+2; i++)
	{
		(gp_buff_menubar_info + i)->Attributes = FOREGROUND_BLUE | FOREGROUND_GREEN
			| FOREGROUND_RED;
	}
	/*ȥ��ѡ�в˵������Ĳ˵���ѡ�б��*/
	for (i = PosB+2; i<bInfo.dwSize.X; i++)
	{
		(gp_buff_menubar_info + i)->Attributes = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;
		ch = (char)((gp_buff_menubar_info + i)->Char.AsciiChar);
		if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'))
		{
			(gp_buff_menubar_info + i)->Attributes |= FOREGROUND_RED;
		}
	}
	/*�����ñ�ǵĲ˵�����Ϣд�����ڵ�һ��*/
	SMALL_RECT rcMenu = { 0, 0, size.X - 1, 0 };
	WriteConsoleOutput(gh_std_out, gp_buff_menubar_info, size, pos, &rcMenu);
	return;
}

/**
* ��������: ClearScreen
* ��������: �����Ļ��Ϣ.
* �������: ��
* �������: ��
* �� �� ֵ: ��
* ����˵��:
*/
void ClearScreen(void)
{
	CONSOLE_SCREEN_BUFFER_INFO bInfo;
	COORD home = { 0, 1 };
	unsigned long size;
	GetConsoleScreenBufferInfo(gh_std_out, &bInfo);/*ȡ��Ļ��������Ϣ*/
	size = bInfo.dwSize.X * bInfo.dwSize.Y; /*������Ļ�������ַ���Ԫ��*/
	/*����Ļ���������е�Ԫ���ַ���������Ϊ��ǰ��Ļ�������ַ�����*/
	FillConsoleOutputAttribute(gh_std_out, bInfo.wAttributes, size, home, &written_num);
	/*����Ļ���������е�Ԫ���Ϊ�ո��ַ�*/
	FillConsoleOutputCharacter(gh_std_out, ' ', size, home, &written_num);
	goto_xy(0, 1);

	return;
}

/**
* ��������: PopMenu
* ��������: ����ָ�����˵����Ӧ���Ӳ˵�
* �������: num ָ�������˵����
* �������: ��
* �� �� ֵ: ��
* ����˵��:
*/
void PopMenu(int num)
{
	LABEL_BUNDLE labels;
	HOT_AREA areas;
	SMALL_RECT rcPop;
	COORD pos;
	WORD att;
	char *pCh;
	int i, j, loc = 0;
	if (num != gi_sel_menu)                  /*���ָ�����˵�������ѡ�в˵�*/
	{
		if (gp_top_layer->LayerNo != 0)      /*�����ǰ�����Ӳ˵�����*/
		{
			PopOff();
			gi_sel_sub_menu = 0;
		}
	}
	else if (gp_top_layer->LayerNo != 0)     /*���ѵ������Ӳ˵����򷵻�*/
	{
		return;
	}


	gi_sel_menu = num;                       /*��ѡ�����˵�����Ϊָ�������˵���*/
	TagMainMenu(gi_sel_menu);                /*��ѡ�е����˵����������*/
	LocSubMenu(gi_sel_menu, &rcPop);         /*���㵯���Ӳ˵�������λ��, �����rcPop��*/

	/*������Ӳ˵��еĵ�һ�����Ӳ˵��ַ��������е�λ��(�±�)*/
	for (i = 1; i<gi_sel_menu; i++)
	{
		loc += ga_sub_menu_count[i - 1];
	}

	/*�������Ӳ˵������������ǩ���ṹ����*/
	labels.ppLabel = ga_sub_menu + loc;      /*��ǩ����һ����ǩ�ַ����ĵ�ַ*/
	labels.num = ga_sub_menu_count[gi_sel_menu - 1]; /*��ǩ���б�ǩ�ַ����ĸ���*/
    /*����һ���������飬���ÿ����ǩ�ַ������λ�õ�����*/
	COORD *aLoc;
	aLoc = (COORD*)calloc(labels.num, sizeof(COORD));
	
	for (i = 0; i<labels.num; i++)           /*ȷ����ǩ�ַ��������λ�ã����������������*/
	{
		aLoc[i].X = rcPop.Left + 2;
		aLoc[i].Y = rcPop.Top + i + 1;
	}
	labels.pLoc = aLoc;                      /*ʹ��ǩ���ṹ����labels�ĳ�ԱpLocָ�������������Ԫ��*/
	/*����������Ϣ*/
	areas.num = labels.num;                  /*�����ĸ��������ڱ�ǩ�ĸ��������Ӳ˵�������*/
    /*������������������λ��*/
	SMALL_RECT *aArea;
	aArea = (SMALL_RECT*)calloc(areas.num, sizeof(SMALL_RECT));
    /*��������������������Ӧ���*/
	char *aSort;               
	aSort = (char*)calloc(areas.num, sizeof(char));
    /*����������ÿ�������ı��*/
	char *aTag;                    
	aTag= (char*)calloc(areas.num, sizeof(char));
	for (i = 0; i<areas.num; i++)
	{
		aArea[i].Left = rcPop.Left + 2;      /*������λ*/
		aArea[i].Top = rcPop.Top + i + 1;
		aArea[i].Right = rcPop.Right - 2;
		aArea[i].Bottom = aArea[i].Top;
		aSort[i] = 0;                        /*�������Ϊ0(��ť��)*/
		aTag[i] = i + 1;                     /*������˳����*/
	}
	areas.pArea = aArea;       /*ʹ�����ṹ����areas�ĳ�ԱpAreaָ������λ��������Ԫ��*/
	areas.pSort = aSort;       /*ʹ�����ṹ����areas�ĳ�ԱpSortָ���������������Ԫ��*/
	areas.pTag = aTag;         /*ʹ�����ṹ����areas�ĳ�ԱpTagָ���������������Ԫ��*/
    /*�׵׺���*/
	att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  
	PopUp(&rcPop, att, &labels, &areas);
	DrawBox(&rcPop);  /*���������ڻ��߿�*/
	pos.X = rcPop.Left + 2;
	for (pos.Y = rcPop.Top + 1; pos.Y<rcPop.Bottom; pos.Y++)
	{ /*��ѭ�������ڿմ��Ӳ˵���λ�û����γɷָ�����ȡ���˲˵������������*/
		pCh = ga_sub_menu[loc + pos.Y - rcPop.Top - 1];
		if (strlen(pCh) == 0) /*����Ϊ0������Ϊ�մ�*/
		{   /*���Ȼ�����*/
			FillConsoleOutputCharacter(gh_std_out, '-', rcPop.Right - rcPop.Left - 3, pos, &written_num);
			for (j = rcPop.Left + 2; j<rcPop.Right - 1; j++)
			{   /*ȡ���������ַ���Ԫ����������*/
				gp_scr_att[pos.Y*SCR_COL + j] &= 3; /*��λ��Ľ�������˵���λ*/
			}
		}
	}

	/*���Ӳ˵���Ĺ��ܼ���Ϊ�׵׺���*/
	pos.X = rcPop.Left + 3;
	att = FOREGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;
	for (pos.Y = rcPop.Top + 1; pos.Y<rcPop.Bottom; pos.Y++)
	{
		if (strlen(ga_sub_menu[loc + pos.Y - rcPop.Top - 1]) == 0)
		{
			continue;  /*�����մ�*/
		}
		FillConsoleOutputAttribute(gh_std_out, att, 1, pos, &written_num);
	}
	return;
}

/**
* ��������: DrawBox
* �������� : ��ָ�����򻭱߿�.
* ������� : pRc �������λ����Ϣ�ĵ�ַ
* ������� : ��
* �� �� ֵ : ��
* ����˵�� :
*/
void DrawBox(SMALL_RECT* pRc)
{
	wchar_t chBox[] = { '+','-','|' };  /*�����õ��ַ�*/
	COORD pos = { pRc->Left, pRc->Top };  /*��λ����������Ͻ�*/

    /*���߿����Ͻ�*/
	WriteConsoleOutputCharacter(gh_std_out, &chBox[0], 1, pos, &written_num);
	for (pos.X = pRc->Left + 1; pos.X < pRc->Right; pos.X++)
	{   /*��ѭ�����ϱ߿����*/
		WriteConsoleOutputCharacter(gh_std_out, &chBox[1], 1, pos, &written_num);
	}
	pos.X = pRc->Right;

    /*���߿����Ͻ�*/
	WriteConsoleOutputCharacter(gh_std_out, &chBox[0], 1, pos, &written_num);
	for (pos.Y = pRc->Top + 1; pos.Y < pRc->Bottom; pos.Y++)
	{   /*��ѭ�����߿�����ߺ��ұ���*/
		pos.X = pRc->Left;
		WriteConsoleOutputCharacter(gh_std_out, &chBox[2], 1, pos, &written_num);
		pos.X = pRc->Right;
		WriteConsoleOutputCharacter(gh_std_out, &chBox[2], 1, pos, &written_num);
	}
	pos.X = pRc->Left;
	pos.Y = pRc->Bottom;

    /*���߿����½�*/
	WriteConsoleOutputCharacter(gh_std_out, &chBox[0], 1, pos, &written_num);
	for (pos.X = pRc->Left + 1; pos.X < pRc->Right; pos.X++)
	{   /*���±߿����*/
		WriteConsoleOutputCharacter(gh_std_out, &chBox[1], 1, pos, &written_num);
	}
	pos.X = pRc->Right;

    /*���߿����½�*/
	WriteConsoleOutputCharacter(gh_std_out, &chBox[0], 1, pos, &written_num);
	return;
}

/**
* ��������: PopOff
* ��������: �رն��㵯������, �ָ���������ԭ��ۺ��ַ���Ԫԭ����.
* �������: ��
* �������: ��
* �� �� ֵ: ��
* ����˵��:
*/
void PopOff(void)
{
	LAYER_NODE *nextLayer;
	COORD size;
	COORD pos = { 0, 0 };
	char *pCh;
	int i, j;
	if ((gp_top_layer->next == NULL) || (gp_top_layer->pContent == NULL))
	{   /*ջ�״�ŵ���������Ļ��Ϣ�����ùر�*/
		return;
	}
	nextLayer = gp_top_layer->next;

	/*�ָ�������������ԭ���*/
	size.X = gp_top_layer->rcArea.Right - gp_top_layer->rcArea.Left + 1;
	size.Y = gp_top_layer->rcArea.Bottom - gp_top_layer->rcArea.Top + 1;
	WriteConsoleOutput(gh_std_out, gp_top_layer->pContent, size, pos, &(gp_top_layer->rcArea));
	
	/*�ָ��ַ���Ԫԭ����*/
	pCh = gp_top_layer->pScrAtt;
	for (i = gp_top_layer->rcArea.Top; i <= gp_top_layer->rcArea.Bottom; i++)
	{
		for (j = gp_top_layer->rcArea.Left; j <= gp_top_layer->rcArea.Right; j++)
		{
			gp_scr_att[i*SCR_COL + j] = *pCh;
			pCh++;
		}
	}
	free(gp_top_layer->pContent);    /*�ͷŶ�̬�洢��*/
	free(gp_top_layer->pScrAtt);
	free(gp_top_layer);
	gp_top_layer = nextLayer;
	gi_sel_sub_menu = 0;
	return;
}

/**
* ��������: PopUp
* ��������: ��ָ�������������������Ϣ, ͬʱ��������, ����������λ����Ϣ��ջ.
* �������: pRc ��������λ�����ݴ�ŵĵ�ַ,att �������������ַ�����
*          pLabel ���������б�ǩ����Ϣ��ŵĵ�ַ
*          pHotArea ����������������Ϣ��ŵĵ�ַ
* �������: ��
* �� �� ֵ: ��
* ����˵��:
*/
void PopUp(SMALL_RECT* pRc, WORD att, LABEL_BUNDLE* pLabel, HOT_AREA* pHotArea)
{
	LAYER_NODE *nextLayer;
	COORD size;
	COORD pos = { 0, 0 };
	char *pCh;
	int i, j, row;
	/*������������λ���ַ���Ԫ��Ϣ��ջ*/
	size.X = pRc->Right - pRc->Left + 1;    /*�������ڵĿ��*/
	size.Y = pRc->Bottom - pRc->Top + 1;    /*�������ڵĸ߶�*/

	/*�����ŵ������������Ϣ�Ķ�̬�洢��*/
	nextLayer = (LAYER_NODE *)malloc(sizeof(LAYER_NODE));
	nextLayer->next = gp_top_layer;
	nextLayer->LayerNo = gp_top_layer->LayerNo + 1;
	nextLayer->rcArea = *pRc;
	nextLayer->pContent = (CHAR_INFO *)malloc(size.X*size.Y * sizeof(CHAR_INFO));
	nextLayer->pScrAtt = (char *)malloc(size.X*size.Y * sizeof(char));
	pCh = nextLayer->pScrAtt;

	/*���������ڸ���������ַ���Ϣ���棬�����ڹرյ�������ʱ�ָ�ԭ��*/
	ReadConsoleOutput(gh_std_out, nextLayer->pContent, size, pos, pRc);
	for (i = pRc->Top; i <= pRc->Bottom; i++)
	{   /*�˶���ѭ�����������ַ���Ԫ��ԭ������ֵ���붯̬�洢���������Ժ�ָ�*/
		for (j = pRc->Left; j<= pRc->Right; j++)
		{
			*pCh = gp_scr_att[i*SCR_COL + j];
			pCh++;
		}
	}
	gp_top_layer = nextLayer;  /*��ɵ������������Ϣ��ջ����*/

	/*���õ������������ַ���������*/
	pos.X = pRc->Left;
	pos.Y = pRc->Top;
	for (i = pRc->Top; i <= pRc->Bottom; i++)
	{
		FillConsoleOutputAttribute(gh_std_out, att, size.X, pos, &written_num);
		pos.Y++;
	}

	/*����ǩ���еı�ǩ�ַ������趨��λ�����*/
	for (i = 0; i<pLabel->num; i++)
	{
		pCh = pLabel->ppLabel[i];
		wchar_t*a = AnsiToUnicode(pCh);
		if (wcslen(a) != 0)
		{
	
			WriteConsoleOutputCharacter(gh_std_out,a, wcslen(a),pLabel->pLoc[i], &written_num);
		}
	}

	/*���õ������������ַ���Ԫ��������*/
	for (i = pRc->Top; i <= pRc->Bottom; i++)
	{   /*�˶���ѭ�������ַ���Ԫ�Ĳ��*/
		for (j = pRc->Left; j <= pRc->Right; j++)
		{
			gp_scr_att[i*SCR_COL + j] = gp_top_layer->LayerNo;
		}
	}

	for (i = 0; i<pHotArea->num; i++)
	{   /*�˶���ѭ�����������������ַ���Ԫ���������ͺ��������*/
		row = pHotArea->pArea[i].Top;
		for (j = pHotArea->pArea[i].Left; j <= pHotArea->pArea[i].Right; j++)
		{
			gp_scr_att[row*SCR_COL + j] |= (pHotArea->pSort[i] << 6)
				| (pHotArea->pTag[i] << 2);
		}
	}
	return;

}

/**
* ��������: TagSubMenu
* ��������: ��ָ���Ӳ˵�������ѡ�б��.
* �������: num ѡ�е��Ӳ˵����
* �������: ��
* �� �� ֵ: ��
* ����˵��:
*/
void TagSubMenu(int num)
{
	SMALL_RECT rcPop;
	COORD pos;
	WORD att;
	int width;

    /*���㵯���Ӳ˵�������λ��, �����rcPop��*/
	LocSubMenu(gi_sel_menu, &rcPop);  
	if ((num<1) || (num == gi_sel_sub_menu) || (num>rcPop.Bottom - rcPop.Top - 1))
	{   /*����Ӳ˵����Խ�磬������Ӳ˵��ѱ�ѡ�У��򷵻�*/
		return;
	}
	pos.X = rcPop.Left + 2;
	width = rcPop.Right - rcPop.Left - 3;
	if (gi_sel_sub_menu != 0) /*����ȡ��ԭѡ���Ӳ˵����ϵı��*/
	{
		pos.Y = rcPop.Top + gi_sel_sub_menu;
		att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*�׵׺���*/
		FillConsoleOutputAttribute(gh_std_out, att, width, pos, &written_num);
		pos.X += 1;
		att |= FOREGROUND_RED;/*�׵׺���*/
		FillConsoleOutputAttribute(gh_std_out, att, 1, pos, &written_num);
	}

	/*���ƶ��Ӳ˵�������ѡ�б��*/
	pos.X = rcPop.Left + 2;
	pos.Y = rcPop.Top + num;
	att = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;  /*�ڵװ���*/
	FillConsoleOutputAttribute(gh_std_out, att, width, pos, &written_num);
	gi_sel_sub_menu = num;  /*�޸�ѡ���Ӳ˵����*/
	return;
}

/**
* ��������: LocSubMenu
* ��������: ���㵯���Ӳ˵��������ϽǺ����½ǵ�λ��.
* �������: num ѡ�е����˵����
* �������: rc �������λ����Ϣ�ĵ�ַ
* �� �� ֵ: ��
* ����˵��:
*/
void LocSubMenu(int num, SMALL_RECT* rc)
{
	int i, len, loc = 0;

	rc->Top = 1; /*������ϱ߶��ڵ�2�У��к�Ϊ1*/
	rc->Left = 1;
	for (i = 1; i<num; i++)
	{   /*����������߽�λ��, ͬʱ�����һ���Ӳ˵������Ӳ˵��ַ��������е�λ��*/
		rc->Left += strlen(ga_main_menu[i - 1]) + 8;
		loc += ga_sub_menu_count[i - 1];
	}
	rc->Right = strlen(ga_sub_menu[loc]);/*��ʱ��ŵ�һ���Ӳ˵����ַ�������*/
	for (i = 1; i<ga_sub_menu_count[num - 1]; i++)
	{   /*������Ӳ˵��ַ��������䳤�ȴ����rc->Right*/
		len = strlen(ga_sub_menu[loc + i]);
		if (rc->Right < len)
		{
			rc->Right = len;
		}
	}
	rc->Right += rc->Left + 3;  /*����������ұ߽�*/
	rc->Bottom = rc->Top + ga_sub_menu_count[num - 1] + 1;/*���������±ߵ��к�*/
	if (rc->Right >= SCR_COL)  /*�ұ߽�Խ��Ĵ���*/
	{
		len = rc->Right - SCR_COL + 1;
		rc->Left -= len;
		rc->Right = SCR_COL - 1;
	}
	return;
}

/**
* ��������: RunSys
* ��������: ����ϵͳ, ��ϵͳ�������������û���ѡ��Ĺ���ģ��.
* �������: ��
* �������: phead ����ͷָ��ĵ�ַ
* �� �� ֵ: ��
* ����˵��:
*/
void RunSys(SchoolNode **phead)
{
	INPUT_RECORD inRec;
	DWORD res;
	COORD pos = { 0, 0 };
	BOOL bRet = TRUE;
	int i, loc, num;
	int cNo, cAtt;      /*cNo:�ַ���Ԫ���, cAtt:�ַ���Ԫ����*/
	char vkc, asc;      /*vkc:���������, asc:�ַ���ASCII��ֵ*/

	while (bRet)
	{   /*�ӿ���̨���뻺�����ж�һ����¼*/
		ReadConsoleInput(gh_std_in, &inRec, 1, &res);

		if (inRec.EventType == MOUSE_EVENT) /*�����¼������¼�����*/
		{
			pos = inRec.Event.MouseEvent.dwMousePosition;  /*��ȡ�������λ��*/
			cNo = gp_scr_att[pos.Y * SCR_COL + pos.X] & 3; /*ȡ��λ�õĲ��*/
			cAtt = gp_scr_att[pos.Y * SCR_COL + pos.X] >> 2;/*ȡ���ַ���Ԫ����*/
			if (cNo == 0) /*���Ϊ0��������λ��δ�������Ӳ˵�����*/
			{

				/* cAtt > 0 ������λ�ô�������(���˵����ַ���Ԫ)*/
				/* cAtt != gi_sel_menu ������λ�õ����˵���δ��ѡ��*/
				/* gp_top_layer->LayerNo > 0 ������ǰ���Ӳ˵�����*/
				if (cAtt > 0 && cAtt != gi_sel_menu && gp_top_layer->LayerNo > 0)
				{
					PopOff();            /*�رյ������Ӳ˵�*/
					gi_sel_sub_menu = 0; /*��ѡ���Ӳ˵���������Ϊ0*/
					PopMenu(cAtt);       /*��������������˵����Ӧ���Ӳ˵�*/
				}
			}
			else if (cAtt > 0) /*�������λ��Ϊ�����Ӳ˵��Ĳ˵����ַ���Ԫ*/
			{
				TagSubMenu(cAtt); /*�ڸ��Ӳ˵�������ѡ�б��*/
			}

			if (inRec.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) /*������������ߵ�һ��*/
			{
				if (cNo == 0) /*���Ϊ0��������λ��δ�������Ӳ˵�����*/
				{
					if (cAtt > 0) /*�����λ�ô�������(���˵����ַ���Ԫ)*/
					{
						PopMenu(cAtt);   /*��������������˵����Ӧ���Ӳ˵�*/
					}

					/*�����λ�ò��������˵����ַ���Ԫ�������Ӳ˵�����*/
					else if (gp_top_layer->LayerNo > 0)
					{
						PopOff();            /*�رյ������Ӳ˵�*/
						gi_sel_sub_menu = 0; /*��ѡ���Ӳ˵���������Ϊ0*/
					}
				}
				else /*��Ų�Ϊ0��������λ�ñ������Ӳ˵�����*/
				{
					if (cAtt > 0) /*�����λ�ô�������(�Ӳ˵����ַ���Ԫ)*/
					{
						PopOff(); /*�رյ������Ӳ˵�*/
						gi_sel_sub_menu = 0; /*��ѡ���Ӳ˵���������Ϊ0*/

											 /*ִ�ж�Ӧ���ܺ���:gi_sel_menu���˵����,cAtt�Ӳ˵����*/
						bRet = ExeFunction(gi_sel_menu, cAtt);
					}
				}
			}
			else if (inRec.Event.MouseEvent.dwButtonState == RIGHTMOST_BUTTON_PRESSED) /*�����������Ҽ�*/
			{
				if (cNo == 0) /*���Ϊ0��������λ��δ�������Ӳ˵�����*/
				{
					PopOff();            /*�رյ������Ӳ˵�*/
					gi_sel_sub_menu = 0; /*��ѡ���Ӳ˵���������Ϊ0*/
				}
			}
		}
		else if (inRec.EventType == KEY_EVENT && inRec.Event.KeyEvent.bKeyDown) /*�����¼�ɰ�������,�Ҽ�������*/
		{
			vkc = inRec.Event.KeyEvent.wVirtualKeyCode;      /*��ȡ�������������*/
			asc = inRec.Event.KeyEvent.uChar.AsciiChar;      /*��ȡ������ASCII��*/

			/*ϵͳ��ݼ��Ĵ���*/
			if (vkc == 112) /*�������F1��*/
			{
				if (gp_top_layer->LayerNo != 0) /*�����ǰ���Ӳ˵�����*/
				{
					PopOff();            /*�رյ������Ӳ˵�*/
					gi_sel_sub_menu = 0; /*��ѡ���Ӳ˵���������Ϊ0*/
				}
				bRet = ExeFunction(5, 1);  /*���а������⹦�ܺ���*/
			}
			else if (inRec.Event.KeyEvent.dwControlKeyState & (LEFT_ALT_PRESSED | RIGHT_ALT_PRESSED))
			{ /*������������Alt��*/
				switch (vkc)  /*�ж���ϼ�Alt+��ĸ*/
				{
				case 88:  /*Alt+X �˳�*/
					if (gp_top_layer->LayerNo != 0)
					{
						PopOff();
						gi_sel_sub_menu = 0;
					}
					bRet = ExeFunction(1, 5);
					break;
				case 70:  /*Alt+F*/
					PopMenu(1);
					break;
				case 77: /*Alt+M*/
					PopMenu(2);
					break;
				case 81: /*Alt+Q*/
					PopMenu(3);
					break;
				case 83: /*Alt+S*/
					PopMenu(4);
					break;
				case 72: /*Alt+H*/
					PopMenu(5);
					break;
				}
			}
			else if (asc == 0) /*�������Ƽ��Ĵ���*/
			{
				if (gp_top_layer->LayerNo == 0) /*���δ�����Ӳ˵�*/
				{
					switch (vkc) /*�������(���ҡ���)������Ӧ�������Ƽ�*/
					{
					case 37:
						gi_sel_menu--;
						if (gi_sel_menu == 0)
						{
							gi_sel_menu = 5;
						}
						TagMainMenu(gi_sel_menu);
						break;
					case 39:
						gi_sel_menu++;
						if (gi_sel_menu == 6)
						{
							gi_sel_menu = 1;
						}
						TagMainMenu(gi_sel_menu);
						break;
					case 40:
						PopMenu(gi_sel_menu);
						TagSubMenu(1);
						break;
					}
				}
				else  /*�ѵ����Ӳ˵�ʱ*/
				{
					for (loc = 0, i = 1; i < gi_sel_menu; i++)
					{
						loc += ga_sub_menu_count[i - 1];
					}  /*������Ӳ˵��еĵ�һ�����Ӳ˵��ַ��������е�λ��(�±�)*/
					switch (vkc) /*�����(���ҡ��ϡ���)�Ĵ���*/
					{
					case 37:
						gi_sel_menu--;
						if (gi_sel_menu < 1)
						{
							gi_sel_menu = 5;
						}
						TagMainMenu(gi_sel_menu);
						PopOff();
						PopMenu(gi_sel_menu);
						TagSubMenu(1);
						break;
					case 38:
						num = gi_sel_sub_menu - 1;
						if (num < 1)
						{
							num = ga_sub_menu_count[gi_sel_menu - 1];
						}
						if (strlen(ga_sub_menu[loc + num - 1]) == 0)
						{
							num--;
						}
						TagSubMenu(num);
						break;
					case 39:
						gi_sel_menu++;
						if (gi_sel_menu > 5)
						{
							gi_sel_menu = 1;
						}
						TagMainMenu(gi_sel_menu);
						PopOff();
						PopMenu(gi_sel_menu);
						TagSubMenu(1);
						break;
					case 40:
						num = gi_sel_sub_menu + 1;
						if (num > ga_sub_menu_count[gi_sel_menu - 1])
						{
							num = 1;
						}
						if (strlen(ga_sub_menu[loc + num - 1]) == 0)
						{
							num++;
						}
						TagSubMenu(num);
						break;
					}
				}
			}
			else if ((asc - vkc == 0) || (asc - vkc == 32))
			{  /*������ͨ��*/
				if (gp_top_layer->LayerNo == 0)  /*���δ�����Ӳ˵�*/
				{
					switch (vkc)
					{
					case 70: /*f��F*/
						PopMenu(1);
						break;
					case 77: /*m��M*/
						PopMenu(2);
						break;
					case 81: /*q��Q*/
						PopMenu(3);
						break;
					case 83: /*s��S*/
						PopMenu(4);
						break;
					case 72: /*h��H*/
						PopMenu(5);
						break;
					case 13: /*�س�*/
						PopMenu(gi_sel_menu);
						TagSubMenu(1);
						break;
					}
				}
				else /*�ѵ����Ӳ˵�ʱ�ļ������봦��*/
				{
					if (vkc == 27) /*�������ESC��*/
					{
						PopOff();
						gi_sel_sub_menu = 0;
					}
					else if (vkc == 13) /*������»س���*/
					{
						num = gi_sel_sub_menu;
						PopOff();
						gi_sel_sub_menu = 0;
						bRet = ExeFunction(gi_sel_menu, num);
					}
					else /*������ͨ���Ĵ���*/
					{
						/*������Ӳ˵��еĵ�һ�����Ӳ˵��ַ��������е�λ��(�±�)*/
						for (loc = 0, i = 1; i < gi_sel_menu; i++)
						{
							loc += ga_sub_menu_count[i - 1];
						}

						/*�����뵱ǰ�Ӳ˵���ÿһ��Ĵ����ַ����бȽ�*/
						for (i = loc; i < loc + ga_sub_menu_count[gi_sel_menu - 1]; i++)
						{
							if (strlen(ga_sub_menu[i]) > 0 && vkc == ga_sub_menu[i][1])
							{ /*���ƥ��ɹ�*/
								PopOff();
								gi_sel_sub_menu = 0;
								bRet = ExeFunction(gi_sel_menu, i - loc + 1);
							}
						}
					}
				}
			}
		}
		
	}
}

/**
* ��������: CloseSys
* �������� : �ر�ϵͳ.
* ������� : hd ����ͷָ��
* ������� : ��
* �� �� ֵ : ��
* ����˵�� :
*/
void CloseSys(SchoolNode *hd)
{
	SchoolNode *pSchNode1 = hd, *pSchNode2;
	MajorNode *pMajorNode1, *pMajorNode2;
	RecruitNode *pEnrolNode1, *pEnrolNode2;

	while (pSchNode1 != NULL) /*�ͷ�ʮ�ֽ�������Ķ�̬�洢��*/
	{
		pSchNode2 = pSchNode1->next;
		pMajorNode1 = pSchNode1->m_next;
		while (pMajorNode1 != NULL) /*�ͷ�רҵ��Ϣ֧���Ķ�̬�洢��*/
		{
			pMajorNode2 = pMajorNode1->next;
			pEnrolNode1 = pMajorNode1->r_next;
			while (pEnrolNode1 != NULL) /*�ͷ�������Ϣ֧���Ķ�̬�洢��*/
			{
				pEnrolNode2 = pEnrolNode1->next;
				free(pEnrolNode1);
				pEnrolNode1 = pEnrolNode2;
			}
			free(pMajorNode1);
			pMajorNode1 = pMajorNode2;
		}
		free(pSchNode1);  /*�ͷ��������Ķ�̬�洢��*/
		pSchNode1 = pSchNode2;
	}

	CONSOLE_SCREEN_BUFFER_INFO bInfo;
	COORD home = { 0, 0 };
	unsigned long size;
	GetConsoleScreenBufferInfo(gh_std_out, &bInfo);/*ȡ��Ļ��������Ϣ*/
	size = bInfo.dwSize.X * bInfo.dwSize.Y; /*������Ļ�������ַ���Ԫ��*/
											/*����Ļ���������е�Ԫ���ַ���������Ϊ��ǰ��Ļ�������ַ�����*/
	FillConsoleOutputAttribute(gh_std_out, bInfo.wAttributes, size, home, &written_num);
	/*����Ļ���������е�Ԫ���Ϊ�ո��ַ�*/
	FillConsoleOutputCharacter(gh_std_out, ' ', size, home, &written_num);
	goto_xy(0, 1);
	printf("��лʹ�ø�ϵͳ��\n");

	/*�ͷŴ�Ų˵�����״̬������Ϣ��̬�洢��*/
	free(gp_buff_menubar_info);
	free(gp_buff_stateBar_info);

	/*�رձ�׼���������豸���*/
	CloseHandle(gh_std_out);
	CloseHandle(gh_std_in);

	/*�����ڱ�������Ϊ���н���*/
	SetConsoleTitle(TEXT("���н���"));
	return;
}

/**
* ��������: ExeFunction
* ��������: ִ�������˵��ź��Ӳ˵���ȷ���Ĺ��ܺ���.
* �������: m ���˵����,s �Ӳ˵����
* �������: ��
* �� �� ֵ: BOOL����, TRUE �� FALSE
* ����˵��: ����ִ�к���ExitSysʱ, �ſ��ܷ���FALSE, ������������Ƿ���TRUE
*/
BOOL ExeFunction(int m, int s)
{
	BOOL bRet = TRUE;
	/*����ָ�����飬����������й��ܺ�������ڵ�ַ*/
	BOOL(*pFunction[19])(void);
	int i=0, loc=0;

	/*�����ܺ�����ڵ�ַ�����빦�ܺ����������˵��ź��Ӳ˵��Ŷ�Ӧ�±������Ԫ��*/
	pFunction[0] = SaveData;
	pFunction[1] = BackupData;
	pFunction[2] = RestoreData;
	pFunction[3] = NULL;
	pFunction[4] = ExitSys;

	pFunction[5] = MaintainSchInfo;
	pFunction[6] = MaintainMajorInfo;
	pFunction[7] = MaintainRecInfo;

	pFunction[8] = QuerySchInfo;
	pFunction[9] = QueryMajorInfo;
	pFunction[10] = QueryRecInfo;
	pFunction[11] = NULL;
	pFunction[12] = QueryMajorInfoM;

	pFunction[13] = StatSchNum;
	pFunction[14] = NULL;
	pFunction[15] = StatRecNum;

	pFunction[16] = HelpTopic;
	pFunction[17] = NULL;
	pFunction[18] = AboutDorm;

	for (i = 1, loc = 0; i<m; i++)  /*�������˵��ź��Ӳ˵��ż����Ӧ�±�*/
	{
		loc += ga_sub_menu_count[i - 1];
	}
	loc += s - 1;
	if (pFunction[loc] != NULL)
	{
		bRet = (*pFunction[loc])();  /*�ú���ָ�������ָ��Ĺ��ܺ���*/
	}
	return bRet;

}

/*���Ҹ�У��Ϣ���*/
SchoolNode *SeekSchNode(SchoolNode *hd, char *sch)
{
	SchoolNode *psch_node;
	psch_node = hd;
	int flag = 0;
	while (psch_node != NULL)
	{
		if ((strcmp(psch_node->schoolId, sch) == 0) || (strcmp(psch_node->schoolName, sch)==0))
		{
			flag = 1;
			break;
		}
		psch_node = psch_node->next;
	}
	if (flag)
	{
		return psch_node;
	}
	else
	{
		return NULL;
	}
}

/*����רҵ������Ϣ���*/
MajorNode *SeekMajorNode(SchoolNode *hd, char *major)
{
	SchoolNode *psch_node;
	MajorNode *pmaj_node;
	psch_node = hd;
	int flag = 0;
	while (psch_node != NULL)
	{
		pmaj_node = psch_node->m_next;
		while (pmaj_node != NULL)
		{
			if (strcmp(pmaj_node->majorId, major) == 0)
			{
				flag = 1;
				goto loop;
			}
			pmaj_node = pmaj_node->next;
		}
		psch_node = psch_node->next;
	}
	loop:if (flag)
	{
		return pmaj_node;
	}
	else
	{
		return NULL;
	}
}

/*����������Ϣ���*/
RecruitNode *SeekRecNode(char *sch, char *maj, int year)
{
	SchoolNode *psch_node;
	psch_node = SeekSchNode(phead, sch);
	MajorNode *pmaj_node;
	pmaj_node = SeekMajorNode(phead, maj);
	RecruitNode *prec_node;
	int flag = 0;
	while (pmaj_node != NULL)
	{
	    prec_node = pmaj_node->r_next;
		while (prec_node!=NULL)
		{
			if (prec_node->year == year)
			{
				flag = 1;
				goto lp;
			}
			prec_node = prec_node->next;
		}
		pmaj_node = pmaj_node->next;
	}
	lp:if (flag)
	{
		return prec_node;
	}
	else
	{
		return NULL;
	}
}

/*�����У��Ϣ���*/
BOOL InsertSchNode(SchoolNode **hd, SchoolNode *psch_node)
{
	SchoolNode *p = *hd;
	while (p != NULL && strcmp(p->schoolId,psch_node->schoolId) != 0)
	{
		p = p->next;
	}
	if (p == NULL)
	{
		psch_node->next = *hd;
		*hd = psch_node;
		printf("�����У��Ϣ���ɹ���\n");
	}
	else  printf("�˸�У�Ѵ��ڣ�����ʧ�ܣ�\n");
	return TRUE;
}

/*ɾ����У��Ϣ���*/
int DelSchNode(SchoolNode **hd, char *sch)
{
	SchoolNode *ps_prev,*ps_current ;
	RecruitNode *pr_prev, *pr_current;
	int bRet = 0;
	for (ps_current = (*hd), ps_prev = NULL; ps_current != NULL; ps_prev = ps_current, ps_current = ps_current->next)
	{
		if (strcmp(ps_current->schoolId, sch) == 0||strcmp(ps_current->schoolName,sch))
		{
			bRet = 1;
			break;
		}
	}
	if (ps_current != NULL)
	{
		if (ps_prev == NULL) /*����ͷŵ��Ǹ�У������ͷ���*/
		{
			(*hd) = ps_current->next;
		}
		else
		{
			ps_prev->next = ps_current->next;
		}
		free(ps_current); /*�ͷ�Ҫɾ���ĸ�У���*/
	}
	return bRet;
}

/*����רҵ��Ϣ���*/
BOOL InsertMajorNode(SchoolNode *hd, MajorNode *pmajor_node)
{
	SchoolNode *psch_node;
	psch_node = SeekSchNode(hd, pmajor_node->m_schoolId);
	if (SeekMajorNode(hd, pmajor_node->majorId) == NULL)
	{
		if (psch_node != NULL)
		{
			pmajor_node->next = psch_node->m_next;
			pmajor_node->r_next = NULL;
			psch_node->m_next = pmajor_node;
		}
	}
	else
	{
		printf("��רҵ��Ϣ�Ѵ��ڣ�\n");
	}
	return TRUE;
}

/*ɾ��רҵ��Ϣ���*/
int DelMajorNode(SchoolNode *hd, char *major_id)
{
	int bRet = 0;
	MajorNode *pm_prev , *pm_current;
	SchoolNode *psch_node = hd;
	while (psch_node != NULL)
	{
		pm_current = psch_node->m_next;
		pm_prev = NULL;
		while (pm_current != NULL&&strcmp(pm_current->majorId,major_id))
		{
			pm_prev = pm_current;
			pm_current = pm_current->next;
		}
		if (pm_current != NULL)
		{
			bRet = 1;
			break;
		}
		psch_node = psch_node->next;
	}
	if (psch_node != NULL)
	{
		if (pm_prev == NULL)/*ɾ������ͷ*/
		{
			psch_node->m_next = pm_current->next;
		}
		else
		{
			pm_prev->next = pm_current->next;
		}
		free(pm_current);
	}
	return bRet;
}

/*����������Ϣ���*/
BOOL InsertRecNode(SchoolNode *hd, RecruitNode *prec_node)
{
	MajorNode *pmaj_node;
	SchoolNode *psch_node;
	pmaj_node = SeekMajorNode(phead, prec_node->r_majorId);
	psch_node = SeekSchNode(phead, prec_node->r_schoolId);
	if (SeekRecNode(psch_node->schoolId,pmaj_node->majorId, prec_node->year) == NULL)
	{
		if (pmaj_node != NULL)
		{
			prec_node->next = pmaj_node->r_next;
			pmaj_node->r_next = prec_node;
		}
	}
	else
	{
		printf("��������Ϣ�Ѵ��ڣ�\n");
	}
	return TRUE;
}

/*ɾ��������Ϣ���*/
int DelRecNode( char *sch,char *maj,int year)
{
	SchoolNode *psch_node;
	MajorNode *pmaj_node;
	psch_node = SeekSchNode(phead, sch);
	pmaj_node = SeekMajorNode(phead, maj);
	RecruitNode *prec_node;
	RecruitNode *begin = NULL;
	prec_node = SeekRecNode(psch_node->schoolId, pmaj_node->majorId, year);
	if (prec_node != NULL)
	{
		pmaj_node = SeekMajorNode(phead, prec_node->r_majorId);
		begin = pmaj_node->r_next;
		if (prec_node == begin)
		{
			pmaj_node->r_next = prec_node->next;
		}
		else
		{
			while (begin->next != prec_node)
			{
				begin = begin->next;
			}
			begin->next = prec_node->next;
		}
		free(prec_node);
		printf("��ɾ����Ӧ��������Ϣ��\n");
	}
	else
	{
		printf("�����ڸ�������Ϣ��\n");
		return 0;
	}
	return 1;
}

/*ά����У��Ϣ*/
BOOL MaintainSchInfo(void)
{
	ClearScreen();
	int i = 0;
	SchoolNode *psch_node=NULL;
	char sch[20];
	char schoolId[10];
	char schoolName[30];
	char schoolAddress[30];
	char schoolPhone[15];
	printf("������Ҫ���еĲ˵�ѡ�\n");
	printf("ѡ��1*****************�����У��Ϣ\n");
	printf("ѡ��2*****************�޸ĸ�У��Ϣ\n");
	printf("ѡ��3*****************ɾ����У��Ϣ\n");
	scanf("%d", &i);
	if (i == 1)
	{
		psch_node = (SchoolNode*)malloc(sizeof(SchoolNode));
		psch_node->m_next = NULL;
		printf("�������У���:\n");
		scanf("%s",psch_node->schoolId);
		printf("�������У����:\n");
		scanf("%s", psch_node->schoolName);
		printf("�������У��ַ\n");
		scanf("%s", psch_node->schoolAddress);
		printf("��������ϵ�绰:\n");
		scanf("%s", psch_node->schoolPhone);
		InsertSchNode(&phead, psch_node);
		SaveList(phead, FileName);
		printf("���ݱ���ɹ���\n");
	}
	else if (i == 2)
	{
		ClearScreen();
		int j = 0;
		printf("������Ҫ�޸ĵ�ѧУ��Ż����ƣ�\n");
		scanf("%s", sch);
		psch_node = SeekSchNode(phead, sch);
		if (psch_node != NULL)
		{
			printf("ѡ����Ҫ�޸ĵ�ѡ�\n");
			printf("1***��У���\n2***��У����\n3***��У��ַ\n4***��У�绰\n");
			while (scanf("%d", &j) == 1)
			{
				if (j == 1)
				{
					printf("������Ҫ�޸ĵĸ�У���:\n");
					scanf("%s", psch_node->schoolId);
				}
				else if (j == 2)
				{
					printf("�������޸ĺ�ĸ�У����:\n");
					scanf("%s", psch_node->schoolName);
				}
				else if (j == 3)
				{
					printf("�������޸ĺ�ĸ�У��ַ:\n");
					scanf("%s", psch_node->schoolAddress);
				}
				else if (j == 4)
				{
					printf("�������޸ĺ����ϵ�绰:\n");
					scanf("%s", psch_node->schoolPhone);
				}
				printf("���ݱ���ɹ���\n");
				printf("�����ѡ�������q�˳���\n");
			}
			getchar();
			SaveList(phead, FileName);
		}
		else
		{
			printf( "�����ڸ���Ϣ��\n" );
		}
	}
	else if (i == 3)
	{
		printf("������Ҫɾ���ĸ�У��Ż�����:\n");
		scanf("%s", sch);
		if(DelSchNode(&phead, schoolId) == 1) printf("ɾ���ɹ���\n");
		else printf("�޴���Ϣ��\n");
		SaveList(phead, FileName);
	}
	return TRUE;
}

/*ά��רҵ��Ϣ*/
BOOL MaintainMajorInfo(void)
{
	ClearScreen();
	int i = 0;
	MajorNode *pmaj_node;
	char m_schoolId[10];
	char majorId[10];
	char majorName[30];
	printf("������Ҫ���еĲ˵�ѡ�\n");
	printf("ѡ��1*****************����רҵ��Ϣ\n");
	printf("ѡ��2*****************�޸�רҵ��Ϣ\n");
	printf("ѡ��3*****************ɾ��רҵ��Ϣ\n");
	scanf("%d", &i);
	if (i == 1)
	{
		pmaj_node = (MajorNode*)malloc(sizeof(MajorNode));
		pmaj_node->r_next = NULL;
		printf("������רҵ������У���:\n");
		scanf("%s", pmaj_node->m_schoolId);
		printf("������רҵ���:\n");
		scanf("%s", pmaj_node->majorId);
		printf("������רҵ���ƣ�\n");
		scanf("%s", pmaj_node->majorName);
		InsertMajorNode(phead, pmaj_node);
		SaveList(phead, FileName);
		printf("���ݱ���ɹ���\n");
	}
	else if (i == 2)
	{
		ClearScreen();
		printf("������Ҫ�޸�רҵ�ı�ţ�\n");
		scanf("%s", majorId);
		pmaj_node = SeekMajorNode(phead, majorId);
		int j = 0;
		if (pmaj_node != NULL)
		{
			printf("ѡ����Ҫ�޸ĵ�ѡ�\n");
			printf("1***��У���\n2***רҵ����\n3***רҵ���\n");
			while (scanf("%d", &j) == 1)
			{
				if (j == 1)
				{
					printf("������Ҫ�޸ĵ�רҵ������У���:\n");
					scanf("%s", pmaj_node->m_schoolId);
				}
				else if (j == 2)
				{
					printf("������Ҫ�޸ĵ�רҵ���ƣ�\n");
					scanf("%s", pmaj_node->majorName);
				}
				else if (j == 3)
				{
					printf("������Ҫ�޸ĵ�רҵ���:\n");
					scanf("%s", pmaj_node->majorId);
				}
				printf("���ݱ���ɹ���\n");
				printf("�����ѡ�������q�˳���\n");
			}
			getchar();
			//ModifMajorNode(phead, majorId, m_schoolId, majorName);
			SaveList(phead, FileName);
			printf("���ݱ���ɹ���\n");
		}
	}
	else if (i == 3)
	{
		printf("������Ҫɾ����רҵ���:\n");
		scanf("%s", majorId);
		if (DelMajorNode(phead, majorId) == 1)printf("ɾ���ɹ���\n");
		else printf("�޴���Ϣ��\n");
		SaveList(phead, FileName);
	}
	return TRUE;
}

/*ά��������Ϣ*/
BOOL MaintainRecInfo(void)
{
	ClearScreen();
	int i = 0;
	RecruitNode *prec_node;
	SchoolNode *psch_node;
	MajorNode *pmaj_node;
	char r_schoolId[10];
	char r_majorId[10];
	char recId[10];
	int year, PlanNum, ActualNum, MaxScore, MinScore;
	printf("������Ҫ���еĲ˵�ѡ�\n");
	printf("ѡ��1*****************����������Ϣ\n");
	printf("ѡ��2*****************�޸�������Ϣ\n");
	printf("ѡ��3*****************ɾ��������Ϣ\n");
	scanf("%d", &i);
	if (i == 1)
	{
		prec_node = (RecruitNode*)malloc(sizeof(RecruitNode));
		printf("������רҵ������Ϣ������У���:\n");
		scanf("%s", prec_node->r_schoolId);
		printf("��������������רҵ���:\n");
		scanf("%s", prec_node->r_majorId);
		printf("������������ݣ�\n");
		scanf("%d", &prec_node->year);
		printf("���������ݸ�רҵ�����ƻ�������\n");
		scanf("%d", &prec_node->PlanNum);
		printf("���������ݸ�רҵʵ������������\n");
		scanf("%d", &prec_node->ActualNum);
		printf("���������ݸ�רҵ���¼ȡ������\n");
		scanf("%d", &prec_node->MaxScore);
		printf("���������ݸ�רҵ���¼ȡ������\n");
		scanf("%d", &prec_node->MinScore);
		InsertRecNode(phead, prec_node);
		SaveList(phead, FileName);
		printf("���ݱ���ɹ���\n");
	}
	else if (i == 2)
	{
		ClearScreen();
		printf("����������Ҫ�޸ĵ�������Ϣ�ĸ�У��ţ�\n");
		scanf("%s", r_schoolId);
		printf("����������Ҫ�޸ĵ�������Ϣ��רҵ��ţ�\n");
		scanf("%s", r_majorId);
		printf("����������Ҫ�޸ĵ�������Ϣ����ݣ�\n");
		scanf("%d", &year);
		psch_node = SeekSchNode(phead, r_schoolId);
		pmaj_node = SeekMajorNode(phead, r_majorId);
		prec_node = SeekRecNode(r_schoolId, r_majorId, year);
		int j = 0;
		if (prec_node != NULL)
		{
			printf("��������Ҫ�޸ĵ�ѡ�\n");
			printf("1***��У���\n2***רҵ���\n3***�������\n4***�����ƻ�����\n5***ʵ����������\n6***��߷�\n7***��ͷ�\n");
			while (scanf("%d", &j) == 1)
			{
				if (j == 1)
				{
					printf("������Ҫ�޸ĵ�������Ϣ��������У���:\n");
					scanf("%s", prec_node->r_schoolId);
				}
				else if (j == 2)
				{
					printf("������Ҫ�޸ĵ�������Ϣ������רҵ���:\n");
					scanf("%s", prec_node->r_majorId);
				}
				else if (j == 3)
				{
					printf("������������ݣ�\n");
					scanf("%d", &prec_node->year);
				}
				else if (j == 4)
				{
					printf("���������ݸ�רҵ�����ƻ�������\n");
					scanf("%d", &prec_node->PlanNum);
				}
				else if (j == 5)
				{
					printf("���������ݸ�רҵʵ������������\n");
					scanf("%d", &prec_node->ActualNum);
				}
				else if (j == 6)
				{
					printf("���������ݸ�רҵ���¼ȡ������\n");
					scanf("%d", &prec_node->MaxScore);
				}
				else if (j == 7)
				{
					printf("���������ݸ�רҵ���¼ȡ������\n");
					scanf("%d", &prec_node->MinScore);
				}
				printf("���ݱ���ɹ���\n");
				printf("�����ѡ�������q�˳���\n");
			}
			getchar();
			//ModifRecNode(r_schoolId, r_majorId, year, PlanNum, ActualNum, MaxScore, MinScore);
			SaveList(phead, FileName);
			printf("���ݱ���ɹ���\n");
		}
		else
		{
			printf("����Ϣ�����ڣ�\n");
		}
	}
	else if (i == 3)
	{
		printf("����������Ҫɾ����������Ϣ�ĸ�У��ţ�\n");
		scanf("%s", r_schoolId);
		printf("����������Ҫɾ����������Ϣ��רҵ��ţ�\n");
		scanf("%s", r_majorId);
		printf("����������Ҫɾ����������Ϣ����ݣ�\n");
		scanf("%d", &year);
		if (DelRecNode(r_schoolId,r_majorId,year) == 1)printf("ɾ���ɹ���\n");
		else
		{
			printf("����Ϣ�����ڣ�\n");
		}
		SaveList(phead, FileName);
	}
	return TRUE;
}

/*��ѯ��У��Ϣ*/
BOOL QuerySchInfo(void)
{
	ClearScreen();
	SchoolNode *psch_node;
	char school[20];
	printf("������ѧУ��Ż��߸�У���ƣ�\n");
	scanf("%s", school);
	psch_node = SeekSchNode(phead, school);
	ClearScreen();
	if (psch_node != NULL)
	{
		printf("��У���ƣ�%s\n��У��ţ�%s\n��У��ַ��%s\n��У��ϵ�绰��%s\n", psch_node->schoolName, psch_node->schoolId,
			psch_node->schoolAddress, psch_node->schoolPhone);	
	}
	else
	{
		printf("��ѧУ��Ϣ�����ڣ�\n");
	}
	return TRUE;
}

/*��ѯרҵ��Ϣ*/
BOOL QueryMajorInfo(void)
{
	ClearScreen();
	SchoolNode *psc_node;
	MajorNode *pmaj_node;
	char major[20];
	int flag = 0;
	printf("�������������ѯ��רҵ�����ƣ�\n");
	scanf("%s", major);
	printf("\n\n");
	printf("%-18s%-18s%-18s%-18s\n","רҵ����","רҵ���","��У���","��У����");
	for ( psc_node = phead; psc_node != NULL; psc_node=psc_node->next)
	{
		for (pmaj_node = psc_node->m_next; pmaj_node != NULL; pmaj_node = pmaj_node->next)
		{
			if (strcmp(pmaj_node->majorName, major) == 0)
			{
				printf("%-18s%-18s", pmaj_node->majorName, pmaj_node->majorId);
				printf("%-18s%-18s\n", pmaj_node->m_schoolId, psc_node->schoolName);
				flag += 1;
			}
		}
	}
	if (flag == 0)
	{
		printf("��רҵ��Ϣ�����ڣ�\n");
	}
	else
		printf("��רҵ��Ϣ����%d����\n",flag);
	return TRUE;
}

/*��ѯ������Ϣ*/
BOOL QueryRecInfo(void)
{
	ClearScreen();
	RecruitNode *prec_node;
	MajorNode *pmaj_node;
	SchoolNode *psch_node;
	char school[20];
	char major[20];
	int year;
	printf("������Ҫ���ҵ�������Ϣ�����ĸ�У���ƻ��ţ�\n");
	scanf("%s", school);
	printf("������Ҫ���ҵ�������Ϣ����רҵ��ţ�\n");
	scanf("%s", major);
	printf("��������ݣ�\n");
	scanf("%d", &year);
	ClearScreen();
	psch_node = SeekSchNode(phead, school);
	pmaj_node = SeekMajorNode(phead, major);
	prec_node = SeekRecNode(school, major, year);
	if (prec_node != NULL)
	{
		printf("����������У��ţ�%s\n��������רҵ��ţ�%s\n\n", prec_node->r_schoolId,prec_node->r_majorId);
		printf("��У��%s\nרҵ��%s\n", psch_node->schoolName, pmaj_node->majorName);
		printf("������� : %d\n�����ƻ����� : %d\n����ʵ������ : %d\n���¼ȡ���� : %d\n���¼ȡ���� : %d\n",
			prec_node->year, prec_node->PlanNum, prec_node->ActualNum, prec_node->MaxScore, prec_node->MinScore);
	}
	else
	{
		printf("��������Ϣ�����ڣ�\n");
	}
	return TRUE;
}

/*�ۺϲ�ѯ��Ӧ��רҵ*/
BOOL QueryMajorInfoM(void)
{
	ClearScreen();
	SchoolNode *psch_node;
	MajorNode *pmaj_node;
	RecruitNode *prec_node;
	int year;
	int i;
	printf("��������ݣ�");
	scanf("%d", &year);
	printf("�������������������ʵ�ʷ�����");
	scanf("%d", &i);
	printf("\n\n������Щרҵ��ӭ��ѡ�񱨿���\n\n\n");

	for (psch_node = phead; psch_node != NULL; psch_node = psch_node->next)
	{
		for (pmaj_node = psch_node->m_next; pmaj_node != NULL; pmaj_node = pmaj_node->next)
		{
			for (prec_node = pmaj_node->r_next; prec_node != NULL; prec_node = prec_node->next)
			{
				if ((year == prec_node->year) && (i >= prec_node->MinScore))
				{
					printf("%-15s %-18s רҵ��ţ�%-8s ��ͷ֣�%-5d ��߷֣�%-5d\n", psch_node->schoolName, pmaj_node->majorName,
						pmaj_node->majorId, prec_node->MinScore, prec_node->MaxScore);
				}
			}

		}
	}
	printf("\n�޸�����Ϣ��\n");
	return TRUE;
}

/*ͳ��¼����Ϣ����*/
BOOL StatSchNum(void)
{
	ClearScreen();
	SchoolNode *psch_node;
	MajorNode *pmaj_node;
	RecruitNode *prec_node;
	int sch_num = 0,maj_num=0,rec_num=0;
	for (psch_node = phead; psch_node != NULL; psch_node = psch_node->next)
	{
		sch_num++;
		for (pmaj_node = psch_node->m_next; pmaj_node != NULL; pmaj_node = pmaj_node->next)
		{
			maj_num++;
			for (prec_node = pmaj_node->r_next; prec_node != NULL; prec_node = prec_node->next)
			{
				rec_num++;
			}
		}
	}
	printf("��¼���У��Ϣ����Ϊ%d\n", sch_num);
	printf("��¼��רҵ��Ϣ����Ϊ%d\n", maj_num);
	printf("��¼��������Ϣ����Ϊ%d\n\n", rec_num);

	printf("����Ϊ��У������룺\n\n\n");
	printf("\t��У����\t��У����\n\n");
	for (psch_node = phead; psch_node != NULL; psch_node = psch_node->next)
	{
		printf("\t%s\t%s\n", psch_node->schoolName, psch_node->schoolId);
	}
	return TRUE;
}

/*��У¼ȡ��ͳ��*/
BOOL StatSchRate(void)
{
	ClearScreen();
	double rate;
	SchoolNode *psch_node;
	MajorNode *pmaj_node;
	RecruitNode *prec_node;
	double actual=0, plan=0;
	int  year;
	for (psch_node = phead,year=2014; psch_node != NULL,year<2017; psch_node = psch_node->next, year++)
	{
		for (pmaj_node = psch_node->m_next; pmaj_node != NULL; pmaj_node = pmaj_node->next)
		{
			for (prec_node = pmaj_node->r_next; prec_node != NULL; prec_node = prec_node->next)
			{
				if (year == prec_node->year)
				{
					actual += prec_node->ActualNum;
					plan += prec_node->PlanNum;
				}
			}
		}
		rate = actual / plan;
		printf("%s��%d��¼ȡ��Ϊ%.2lf%%\n", psch_node->schoolName,year,100 * rate);
	}
	return TRUE;
}

/*�����������ͳ��*/
BOOL StatRecNum(void)
{
	ClearScreen();
	SchoolNode *psch_node;
	MajorNode *pmaj_node;
	RecruitNode *prec_node;
	int rec_num,year;
	for (rec_num = 0, psch_node = phead; psch_node != NULL; psch_node = psch_node->next)
	{
		printf("%s:\n\n", psch_node->schoolName);
		for (year = 2014; year <= 2016; year++)
		{
			for (pmaj_node = psch_node->m_next; pmaj_node != NULL; pmaj_node = pmaj_node->next)
			{
				for (prec_node = pmaj_node->r_next; prec_node != NULL; prec_node = prec_node->next)
				{
					if (prec_node->year == year)
					{
						rec_num += prec_node->ActualNum;
					}
				}
			}
			printf("\t%d�꣺\t%d��\n", year, rec_num);
		}
	}
	return TRUE;
}

/*��������*/
BOOL HelpTopic(void)
{
	ClearScreen();
	BOOL bRet = TRUE;
	printf("\n\n��ϵͳ֧���������̲�������֧�ֿ�ݼ���ʹ�ã������ݼ�Ϊ��Alt��+�˵��������е�Ӣ����ĸ����\n");
	printf("��Alt��+X����ֱ���˳�ϵͳ��\n");
	return bRet;
}

/*����ϵͳ*/
BOOL AboutDorm(void)
{
	BOOL bRet = TRUE;
	ClearScreen();
	printf("\n\n    ��������ѯϵͳC���Կγ�����ɱ��˶�����ɡ����ڱ�д�������������������,");
	printf("��л�ڴ˹�������˽�����ҵ���ʦ��ͬѧ����ӭ���ָ����\n");
	return bRet;
}

/*��������*/
BOOL SaveData(void)
{
	BOOL bRet = TRUE;
	ClearScreen();
	LoadList(&phead, FileName);
	SaveList(phead, FileName);
	return bRet;
}

/**< ����������ݴ���*/
void SaveList(SchoolNode *phead, char *fn[3])
{
	FILE *fout1, *fout2, *fout3;
	SchoolNode *p1 = phead;
	MajorNode *p2;
	RecruitNode *p3;
	if ((fout1 = fopen(fn[0], "wb")) == NULL)
		exit(-1);
	if ((fout2 = fopen(fn[1], "wb")) == NULL)
		exit(-1);
	if ((fout3 = fopen(fn[2], "wb")) == NULL)
		exit(-1);
	while (p1 != NULL)
	{
		fwrite(p1, sizeof(SchoolNode), 1, fout1);
		p2 = p1->m_next;
		while (p2 != NULL)
		{
			fwrite(p2, sizeof(MajorNode), 1, fout2);
			p3 = p2->r_next;
			while (p3 != NULL)
			{
				fwrite(p3, sizeof(RecruitNode), 1, fout3);
				p3 = p3->next;
			}
			p2 = p2->next;
		}
		p1 = p1->next;
	}
	fclose(fout1);
	fclose(fout2);
	fclose(fout3);
	printf("[��������ѱ���!]\n");
}

/**< �����ݼ���*/
BOOL LoadData(void)
{
	BOOL bRet = TRUE;
	ClearScreen();
	LoadList(&phead, FileName);   /*����������Ϣ*/
	printf("���سɹ���\n");
	return bRet;
}

/**< �����ļ��е���Ϣ��*/
void LoadList(SchoolNode  **phead, char *fn[3])
{
	SchoolNode *hd = NULL, *p1;
	MajorNode *p2;
	RecruitNode  *p3;
	FILE *in1, *in2, *in3;
	int  find = 0;
	if ((in1 = fopen(fn[0], "rb")) == NULL)
	{
		printf("��У�ļ�����ʧ��,ϵͳ�Զ��˳�!\n"); Sleep(1000);  exit(-1);
	}
	if ((in2 = fopen(fn[1], "rb")) == NULL)
	{
		printf("רҵ�ļ�����ʧ��,ϵͳ�Զ��˳�!\n"); Sleep(1000);  exit(-1);
	}
	if ((in3 = fopen(fn[2], "rb")) == NULL)
	{
		printf("�����ļ�����ʧ��,ϵͳ�Զ��˳�!\n"); Sleep(1000);  exit(-1);
	}
	while (!feof(in1))
	{
		p1 = (SchoolNode *)malloc(sizeof(SchoolNode));
		fread(p1, sizeof(SchoolNode), 1, in1);
		if (!feof(in1))
		{
			p1->m_next = NULL;
			p1->next = hd;
			hd = p1;
		}
	}
	(*phead) = hd;
	while (!feof(in2))
	{
		p2 = (MajorNode *)malloc(sizeof(MajorNode));
		fread(p2, sizeof(MajorNode), 1, in2);
		if (!feof(in2))
		{
			p1 = (*phead);
			p2->r_next = NULL;
			while (p1 != NULL)
			{
				if (!strcmp(p1->schoolId, p2->m_schoolId))
				{
					p2->next = p1->m_next;
					p1->m_next = p2;
					break;
				}
				else p1 = p1->next;
			}
		}
	}
	while (!feof(in3))
	{
		p3 = (RecruitNode *)malloc(sizeof(RecruitNode));
		fread(p3, sizeof(RecruitNode), 1, in3);
		//fscanf(in3, "%s %s %d %d %d %d %d", p3->r_schoolId, p3->r_majorId, &p3->year, &p3->PlanNum, &p3->ActualNum,
		//	&p3->MaxScore, &p3->MinScore);
		find = 0;
		if (!feof(in3))
		{
			p1 = (*phead);
			while (p1 != NULL && find == 0)
			{
				p2 = p1->m_next;
				while (p2 != NULL && find == 0)
				{
					if (!strcmp(p2->majorId, p3->r_majorId))
					{
						find = 1;
						break;
					}
					p2 = p2->next;
				}
				p1 = p1->next;
			}
		}
		if (find)
		{
			p3->next = p2->r_next;
			p2->r_next = p3;
		}
		else free(p3);
	}
	fclose(in1);
	fclose(in2);
	fclose(in3);
}

/**< �������ݱ���*/
BOOL BackupData(void)
{
	BOOL bRet = TRUE;
	ClearScreen();
	LoadList(&phead, FileName);
	SaveList(phead, BeiFen_FileName);
	printf("���ݱ��ݳɹ�![�������ݴ洢�����������ļ���]\n");
	return bRet;
}

/**< �ָ���������*/
BOOL RestoreData(void)
{
	BOOL bRet = TRUE;
	char ch;
	ClearScreen();
	printf("ע��:����ָ��������ݵ�ǰ�����ݽ�������,��ȷ�������ָ���?ȷ��(Y) ȡ��(N):\n");
	ch = getchar(); getchar();
	if (ch != 'Y' && ch != 'y')    return bRet;
	LoadList(&phead, BeiFen_FileName);
	SaveList(phead, FileName);
	printf("�������ݻָ��ɹ�!\n");
	return bRet;
}

/*�˳�ϵͳ*/
BOOL ExitSys(void)
{
	LABEL_BUNDLE labels;
	HOT_AREA areas;
	BOOL bRet = TRUE;
	SMALL_RECT rcPop;
	COORD pos;
	WORD att;
	char *pCh[] = { "ȷ���˳�ϵͳ��", "ȷ��    ȡ��" };
	int iHot = 1;

	ClearScreen();
	pos.X = strlen(pCh[0]) + 6;
	pos.Y = 7;
	rcPop.Left = (SCR_COL - pos.X) / 2;
	rcPop.Right = rcPop.Left + pos.X - 1;
	rcPop.Top = (SCR_ROW - pos.Y) / 2;
	rcPop.Bottom = rcPop.Top + pos.Y - 1;

	att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*�׵׺���*/
	labels.num = 2;
	labels.ppLabel = pCh;
	COORD aLoc[] = { { rcPop.Left + 3, rcPop.Top + 2 },
	{ rcPop.Left + 5, rcPop.Top + 5 } };
	labels.pLoc = aLoc;

	areas.num = 2;
	SMALL_RECT aArea[] = { { rcPop.Left + 5, rcPop.Top + 5,
		rcPop.Left + 8, rcPop.Top + 5 },
		{ rcPop.Left + 13, rcPop.Top + 5,
		rcPop.Left + 16, rcPop.Top + 5 } };
	char aSort[] = { 0, 0 };
	char aTag[] = { 1, 2 };
	areas.pArea = aArea;
	areas.pSort = aSort;
	areas.pTag = aTag;
	PopUp(&rcPop, att, &labels, &areas);

	pos.X = rcPop.Left + 1;
	pos.Y = rcPop.Top + 4;
	FillConsoleOutputCharacter(gh_std_out, '-', rcPop.Right - rcPop.Left - 1, pos, &written_num);

	if (DealInput(&areas, &iHot) == 13 && iHot == 1)
	{
		bRet = FALSE;
	}
	else
	{
		bRet = TRUE;
	}
	PopOff();

	return bRet;
}

/*��������*/
void SetHotPoint(HOT_AREA *pHotArea, int iHot)
{
	CONSOLE_CURSOR_INFO lpCur;
	COORD pos = { 0, 0 };
	WORD att1, att2;
	int i, width;

	att1 = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;  /*�ڵװ���*/
	att2 = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*�׵׺���*/
	for (i = 0; i < pHotArea->num; i++)
	{  /*����ť��������Ϊ�׵׺���*/
		pos.X = pHotArea->pArea[i].Left;
		pos.Y = pHotArea->pArea[i].Top;
		width = pHotArea->pArea[i].Right - pHotArea->pArea[i].Left + 1;
		if (pHotArea->pSort[i] == 0)
		{  /*�����ǰ�ť��*/
			FillConsoleOutputAttribute(gh_std_out, att2, width, pos, &written_num);
		}
	}

	pos.X = pHotArea->pArea[iHot - 1].Left;
	pos.Y = pHotArea->pArea[iHot - 1].Top;
	width = pHotArea->pArea[iHot - 1].Right - pHotArea->pArea[iHot - 1].Left + 1;
	if (pHotArea->pSort[iHot - 1] == 0)
	{  /*�����������ǰ�ť��*/
		FillConsoleOutputAttribute(gh_std_out, att1, width, pos, &written_num);
	}
	else if (pHotArea->pSort[iHot - 1] == 1)
	{  /*�������������ı�����*/
		SetConsoleCursorPosition(gh_std_out, pos);
		GetConsoleCursorInfo(gh_std_out, &lpCur);
		lpCur.bVisible = TRUE;
		SetConsoleCursorInfo(gh_std_out, &lpCur);
	}
}

/*����̨���봦��*/
int DealInput(HOT_AREA *pHotArea, int *piHot)
{
	INPUT_RECORD inRec;
	DWORD res;
	COORD pos = { 0, 0 };
	int num, arrow, iRet = 0;
	int cNo, cTag, cSort;/*cNo:���, cTag:�������, cSort: ��������*/
	char vkc, asc;       /*vkc:���������, asc:�ַ���ASCII��ֵ*/

	SetHotPoint(pHotArea, *piHot);
	while (TRUE)
	{    /*ѭ��*/
		ReadConsoleInput(gh_std_in, &inRec, 1, &res);
		if ((inRec.EventType == MOUSE_EVENT) &&
			(inRec.Event.MouseEvent.dwButtonState
				== FROM_LEFT_1ST_BUTTON_PRESSED))
		{
			pos = inRec.Event.MouseEvent.dwMousePosition;
			cNo = gp_scr_att[pos.Y * SCR_COL + pos.X] & 3;
			cTag = (gp_scr_att[pos.Y * SCR_COL + pos.X] >> 2) & 15;
			cSort = (gp_scr_att[pos.Y * SCR_COL + pos.X] >> 6) & 3;

			if ((cNo == gp_top_layer->LayerNo) && cTag > 0)
			{
				*piHot = cTag;
				SetHotPoint(pHotArea, *piHot);
				if (cSort == 0)
				{
					iRet = 13;
					break;
				}
			}
		}
		else if (inRec.EventType == KEY_EVENT && inRec.Event.KeyEvent.bKeyDown)
		{
			vkc = inRec.Event.KeyEvent.wVirtualKeyCode;
			asc = inRec.Event.KeyEvent.uChar.AsciiChar;;
			if (asc == 0)
			{
				arrow = 0;
				switch (vkc)
				{  /*�����(���ϡ��ҡ���)�Ĵ���*/
				case 37: arrow = 1; break;
				case 38: arrow = 2; break;
				case 39: arrow = 3; break;
				case 40: arrow = 4; break;
				}
				if (arrow > 0)
				{
					num = *piHot;
					while (TRUE)
					{
						if (arrow < 3)
						{
							num--;
						}
						else
						{
							num++;
						}
						if ((num < 1) || (num > pHotArea->num) ||
							((arrow % 2) && (pHotArea->pArea[num - 1].Top
								== pHotArea->pArea[*piHot - 1].Top)) || ((!(arrow % 2))
									&& (pHotArea->pArea[num - 1].Top
										!= pHotArea->pArea[*piHot - 1].Top)))
						{
							break;
						}
					}
					if (num > 0 && num <= pHotArea->num)
					{
						*piHot = num;
						SetHotPoint(pHotArea, *piHot);
					}
				}
			}
			else if (vkc == 27)
			{  /*ESC��*/
				iRet = 27;
				break;
			}
			else if (vkc == 13 || vkc == 32)
			{  /*�س�����ո��ʾ���µ�ǰ��ť*/
				iRet = 13;
				break;
			}
		}
	}
	return iRet;
}

/*��char*����ת��Ϊwchar*����*/
wchar_t* AnsiToUnicode( const char* szStr)
{
	int nLen = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szStr, -1, NULL, 0);
	if (nLen == 0)
	{
		return NULL;
	}
	wchar_t* pResult = new wchar_t[nLen];
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szStr, -1, pResult, nLen);
	return pResult;
}

/*��λ���*/
void goto_xy(int x, int y)
{
	HANDLE hOut;
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos = { x, y };
	SetConsoleCursorPosition(hOut, pos);
}

/**< ��ʼ����ϵͳ��ӭ����.*/
void Start(void)
{
	CONSOLE_CURSOR_INFO lpCur;
	goto_xy(17, 7);
	printf("*  *  * "); Delay(); printf(" *  * "); Delay(); printf(" *  * "); Delay(); printf(" *  * "); Delay();
	printf(" *  * "); Delay(); printf(" *  * "); Delay(); printf(" *  *  *\n\n"); Delay(); printf("%14s��%11s", " ", " ");
	printf("  �� "); Delay(); printf(" �� "); Delay(); printf(" ӭ "); Delay(); printf(" �� "); Delay();
	printf(" �� "); Delay(); printf(" �� %12s��\n\n"," "); Delay(); printf("%14s��", " ");
	printf("%13s�� "," "); Delay(); printf(" �� "); Delay();printf(" �� "); Delay();
	printf(" ѯ "); Delay(); printf(" ϵ "); Delay();printf(" ͳ%13s��\n\n"," "); Delay(); printf("%14s��", " ");
	printf("   ��"); Delay(); printf("Ȩ"); Delay(); printf("��"); Delay(); printf("��"); Delay();
	printf(" (C)"); Delay(); printf(" 2017/09   "); Delay(); printf("����"); Delay(); printf(""); Delay();
	printf("    "); Delay(); printf("U201614792%4s��\n\n"," "); 
	goto_xy(17, 15);
	printf("*  *  * "); Delay(); printf(" *  * "); Delay(); printf(" *  * "); Delay(); printf(" *  * "); Delay();
	printf(" *  * "); Delay(); printf(" *  * "); Delay(); printf(" *  *  *\n\n"); Delay();	Sleep(1000);
	printf("\n\n\n\n%32s�����������ϵͳ...", " ", " ");
	getch();
	ScreenFresh();
	return;
}

/**< ˢ������*/
inline void ScreenFresh(void)
{
	unsigned long size;
	int i;
	goto_xy(0,1);
	for (i = 0; i < SCR_ROW; i++)
	{
		printf("%80s", " ");
		Sleep(35);
	}
	goto_xy(0,1);
}

/**< ʱ������0.05s.*/
inline void Delay(void)
{
	int goal;
	goal = 50 + clock();
	while (goal > clock());
}
