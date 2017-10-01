#include "stdafx.h"
/**
* 函数名称: InitInterface
* 函数功能: 初始化界面.
* 输入参数: 无
* 输出参数: 无
* 返 回 值: 无
* 调用说明:
*/
void InitInterface(void)
{
	WORD att = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY | BACKGROUND_BLUE;

	SetConsoleTextAttribute(gh_std_out, att);          /*设置黄色前景和蓝色背景*/
	ClearScreen();                                     /*清屏*/
	/*创建弹出窗口信息堆栈，将初始化后的屏幕当作第一层弹出窗口*/
	gp_scr_att = (char*)calloc(SCR_COL*SCR_ROW, sizeof(char));/*屏幕字符属性*/
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
	gp_top_layer->LayerNo = 0;                                /*弹出窗口的层号为0*/
	gp_top_layer->rcArea.Left = 0;                            /*弹出窗口的区域为整个屏幕窗口*/
	gp_top_layer->rcArea.Top = 0;
	gp_top_layer->rcArea.Right = SCR_COL - 1;
	gp_top_layer->rcArea.Bottom = SCR_ROW - 1;
	gp_top_layer->pContent = NULL;
	gp_top_layer->pScrAtt = gp_scr_att;
	gp_top_layer->next = NULL;
	ShowMenu();                                        /*显示系统主菜单*/


}

/**
* 函数名称: ShowMenu
* 函数功能: 在屏幕上显示主菜单, 并设置热区, 在主菜单第一项上置选中标记.
* 输入参数: 无
* 输出参数: 无
* 返 回 值: 无
* 调用说明:
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
	for (i = 0; i < 5; i++) /*在窗口第一行第一列处输出主菜单项*/
	{
		printf("    %s    ", ga_main_menu[i]);
	}
	GetConsoleCursorInfo(gh_std_out, &lpCur);
	lpCur.bVisible = FALSE;
	SetConsoleCursorInfo(gh_std_out, &lpCur);  /*隐藏光标*/
	/*申请动态存储区作为存放菜单条屏幕区字符信息的缓冲区*/
	gp_buff_menubar_info = (CHAR_INFO *)malloc(size.X * size.Y * sizeof(CHAR_INFO));
	SMALL_RECT rcMenu = { 0, 0, size.X - 1, 0 };
	/*将窗口第一行的内容读入到存放菜单条屏幕区字符信息的缓冲区中*/
	ReadConsoleOutput(gh_std_out, gp_buff_menubar_info, size, pos, &rcMenu);
	/*将这一行中英文字母置为红色，其他字符单元置为白底黑字*/
	for (i = 0; i<size.X; i++)
	{
		(gp_buff_menubar_info + i)->Attributes = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;
		ch = (char)((gp_buff_menubar_info + i)->Char.AsciiChar);
	    (gp_buff_menubar_info + i)->Attributes |= FOREGROUND_RED;
	}
	/*修改后的菜单条字符信息回写到窗口的第一行*/
	WriteConsoleOutput(gh_std_out, gp_buff_menubar_info, size, pos, &rcMenu);
	TagMainMenu(gi_sel_menu);  /*在选中主菜单项上做标记，gi_sel_menu初值为1*/
	return;
}

/**
* 函数名称: TagMainMenu
* 函数功能: 在指定主菜单项上置选中标志.
* 输入参数: num 选中的主菜单项号
* 输出参数: 无
* 返 回 值: 无
* 调用说明:
*/
void TagMainMenu(int num)
{
	CONSOLE_SCREEN_BUFFER_INFO bInfo;
	COORD size;
	COORD pos = { 0, 0 };
	int PosA = 2, PosB;
	char ch;
	int i;
	if (num == 0) /*num为0时，将会去除主菜单项选中标记*/
	{
		PosA = 0;
		PosB = 0;
	}
	else  /*否则，定位选中主菜单项的起止位置: PosA为起始位置, PosB为截止位置*/
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
	/*去除选中菜单项前面的菜单项选中标记*/
	for (i = 0; i<PosA; i++)
	{
		(gp_buff_menubar_info + i)->Attributes = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;
		ch = (gp_buff_menubar_info + i)->Char.AsciiChar;
		if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'))
		{
			(gp_buff_menubar_info + i)->Attributes |= FOREGROUND_RED;
		}
	}
	/*在选中菜单项上做标记，黑底白字*/
	for (i = PosA; i<PosB+2; i++)
	{
		(gp_buff_menubar_info + i)->Attributes = FOREGROUND_BLUE | FOREGROUND_GREEN
			| FOREGROUND_RED;
	}
	/*去除选中菜单项后面的菜单项选中标记*/
	for (i = PosB+2; i<bInfo.dwSize.X; i++)
	{
		(gp_buff_menubar_info + i)->Attributes = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;
		ch = (char)((gp_buff_menubar_info + i)->Char.AsciiChar);
		if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'))
		{
			(gp_buff_menubar_info + i)->Attributes |= FOREGROUND_RED;
		}
	}
	/*将做好标记的菜单条信息写到窗口第一行*/
	SMALL_RECT rcMenu = { 0, 0, size.X - 1, 0 };
	WriteConsoleOutput(gh_std_out, gp_buff_menubar_info, size, pos, &rcMenu);
	return;
}

/**
* 函数名称: ClearScreen
* 函数功能: 清除屏幕信息.
* 输入参数: 无
* 输出参数: 无
* 返 回 值: 无
* 调用说明:
*/
void ClearScreen(void)
{
	CONSOLE_SCREEN_BUFFER_INFO bInfo;
	COORD home = { 0, 1 };
	unsigned long size;
	GetConsoleScreenBufferInfo(gh_std_out, &bInfo);/*取屏幕缓冲区信息*/
	size = bInfo.dwSize.X * bInfo.dwSize.Y; /*计算屏幕缓冲区字符单元数*/
	/*将屏幕缓冲区所有单元的字符属性设置为当前屏幕缓冲区字符属性*/
	FillConsoleOutputAttribute(gh_std_out, bInfo.wAttributes, size, home, &written_num);
	/*将屏幕缓冲区所有单元填充为空格字符*/
	FillConsoleOutputCharacter(gh_std_out, ' ', size, home, &written_num);
	goto_xy(0, 1);

	return;
}

/**
* 函数名称: PopMenu
* 函数功能: 弹出指定主菜单项对应的子菜单
* 输入参数: num 指定的主菜单项号
* 输出参数: 无
* 返 回 值: 无
* 调用说明:
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
	if (num != gi_sel_menu)                  /*如果指定主菜单不是已选中菜单*/
	{
		if (gp_top_layer->LayerNo != 0)      /*如果此前已有子菜单弹出*/
		{
			PopOff();
			gi_sel_sub_menu = 0;
		}
	}
	else if (gp_top_layer->LayerNo != 0)     /*若已弹出该子菜单，则返回*/
	{
		return;
	}


	gi_sel_menu = num;                       /*将选中主菜单项置为指定的主菜单项*/
	TagMainMenu(gi_sel_menu);                /*在选中的主菜单项上做标记*/
	LocSubMenu(gi_sel_menu, &rcPop);         /*计算弹出子菜单的区域位置, 存放在rcPop中*/

	/*计算该子菜单中的第一项在子菜单字符串数组中的位置(下标)*/
	for (i = 1; i<gi_sel_menu; i++)
	{
		loc += ga_sub_menu_count[i - 1];
	}

	/*将该组子菜单项项名存入标签束结构变量*/
	labels.ppLabel = ga_sub_menu + loc;      /*标签束第一个标签字符串的地址*/
	labels.num = ga_sub_menu_count[gi_sel_menu - 1]; /*标签束中标签字符串的个数*/
    /*定义一个坐标数组，存放每个标签字符串输出位置的坐标*/
	COORD *aLoc;
	aLoc = (COORD*)calloc(labels.num, sizeof(COORD));
	
	for (i = 0; i<labels.num; i++)           /*确定标签字符串的输出位置，存放在坐标数组中*/
	{
		aLoc[i].X = rcPop.Left + 2;
		aLoc[i].Y = rcPop.Top + i + 1;
	}
	labels.pLoc = aLoc;                      /*使标签束结构变量labels的成员pLoc指向坐标数组的首元素*/
	/*设置热区信息*/
	areas.num = labels.num;                  /*热区的个数，等于标签的个数，即子菜单的项数*/
    /*定义数组存放所有热区位置*/
	SMALL_RECT *aArea;
	aArea = (SMALL_RECT*)calloc(areas.num, sizeof(SMALL_RECT));
    /*定义数组存放所有热区对应类别*/
	char *aSort;               
	aSort = (char*)calloc(areas.num, sizeof(char));
    /*定义数组存放每个热区的编号*/
	char *aTag;                    
	aTag= (char*)calloc(areas.num, sizeof(char));
	for (i = 0; i<areas.num; i++)
	{
		aArea[i].Left = rcPop.Left + 2;      /*热区定位*/
		aArea[i].Top = rcPop.Top + i + 1;
		aArea[i].Right = rcPop.Right - 2;
		aArea[i].Bottom = aArea[i].Top;
		aSort[i] = 0;                        /*热区类别都为0(按钮型)*/
		aTag[i] = i + 1;                     /*热区按顺序编号*/
	}
	areas.pArea = aArea;       /*使热区结构变量areas的成员pArea指向热区位置数组首元素*/
	areas.pSort = aSort;       /*使热区结构变量areas的成员pSort指向热区类别数组首元素*/
	areas.pTag = aTag;         /*使热区结构变量areas的成员pTag指向热区编号数组首元素*/
    /*白底黑字*/
	att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  
	PopUp(&rcPop, att, &labels, &areas);
	DrawBox(&rcPop);  /*给弹出窗口画边框*/
	pos.X = rcPop.Left + 2;
	for (pos.Y = rcPop.Top + 1; pos.Y<rcPop.Bottom; pos.Y++)
	{ /*此循环用来在空串子菜单项位置画线形成分隔，并取消此菜单项的热区属性*/
		pCh = ga_sub_menu[loc + pos.Y - rcPop.Top - 1];
		if (strlen(pCh) == 0) /*串长为0，表明为空串*/
		{   /*首先画横线*/
			FillConsoleOutputCharacter(gh_std_out, '-', rcPop.Right - rcPop.Left - 3, pos, &written_num);
			for (j = rcPop.Left + 2; j<rcPop.Right - 1; j++)
			{   /*取消该区域字符单元的热区属性*/
				gp_scr_att[pos.Y*SCR_COL + j] &= 3; /*按位与的结果保留了低两位*/
			}
		}
	}

	/*将子菜单项的功能键设为白底红字*/
	pos.X = rcPop.Left + 3;
	att = FOREGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;
	for (pos.Y = rcPop.Top + 1; pos.Y<rcPop.Bottom; pos.Y++)
	{
		if (strlen(ga_sub_menu[loc + pos.Y - rcPop.Top - 1]) == 0)
		{
			continue;  /*跳过空串*/
		}
		FillConsoleOutputAttribute(gh_std_out, att, 1, pos, &written_num);
	}
	return;
}

/**
* 函数名称: DrawBox
* 函数功能 : 在指定区域画边框.
* 输入参数 : pRc 存放区域位置信息的地址
* 输出参数 : 无
* 返 回 值 : 无
* 调用说明 :
*/
void DrawBox(SMALL_RECT* pRc)
{
	wchar_t chBox[] = { '+','-','|' };  /*画框用的字符*/
	COORD pos = { pRc->Left, pRc->Top };  /*定位在区域的左上角*/

    /*画边框左上角*/
	WriteConsoleOutputCharacter(gh_std_out, &chBox[0], 1, pos, &written_num);
	for (pos.X = pRc->Left + 1; pos.X < pRc->Right; pos.X++)
	{   /*此循环画上边框横线*/
		WriteConsoleOutputCharacter(gh_std_out, &chBox[1], 1, pos, &written_num);
	}
	pos.X = pRc->Right;

    /*画边框右上角*/
	WriteConsoleOutputCharacter(gh_std_out, &chBox[0], 1, pos, &written_num);
	for (pos.Y = pRc->Top + 1; pos.Y < pRc->Bottom; pos.Y++)
	{   /*此循环画边框左边线和右边线*/
		pos.X = pRc->Left;
		WriteConsoleOutputCharacter(gh_std_out, &chBox[2], 1, pos, &written_num);
		pos.X = pRc->Right;
		WriteConsoleOutputCharacter(gh_std_out, &chBox[2], 1, pos, &written_num);
	}
	pos.X = pRc->Left;
	pos.Y = pRc->Bottom;

    /*画边框左下角*/
	WriteConsoleOutputCharacter(gh_std_out, &chBox[0], 1, pos, &written_num);
	for (pos.X = pRc->Left + 1; pos.X < pRc->Right; pos.X++)
	{   /*画下边框横线*/
		WriteConsoleOutputCharacter(gh_std_out, &chBox[1], 1, pos, &written_num);
	}
	pos.X = pRc->Right;

    /*画边框右下角*/
	WriteConsoleOutputCharacter(gh_std_out, &chBox[0], 1, pos, &written_num);
	return;
}

/**
* 函数名称: PopOff
* 函数功能: 关闭顶层弹出窗口, 恢复覆盖区域原外观和字符单元原属性.
* 输入参数: 无
* 输出参数: 无
* 返 回 值: 无
* 调用说明:
*/
void PopOff(void)
{
	LAYER_NODE *nextLayer;
	COORD size;
	COORD pos = { 0, 0 };
	char *pCh;
	int i, j;
	if ((gp_top_layer->next == NULL) || (gp_top_layer->pContent == NULL))
	{   /*栈底存放的主界面屏幕信息，不用关闭*/
		return;
	}
	nextLayer = gp_top_layer->next;

	/*恢复弹出窗口区域原外观*/
	size.X = gp_top_layer->rcArea.Right - gp_top_layer->rcArea.Left + 1;
	size.Y = gp_top_layer->rcArea.Bottom - gp_top_layer->rcArea.Top + 1;
	WriteConsoleOutput(gh_std_out, gp_top_layer->pContent, size, pos, &(gp_top_layer->rcArea));
	
	/*恢复字符单元原属性*/
	pCh = gp_top_layer->pScrAtt;
	for (i = gp_top_layer->rcArea.Top; i <= gp_top_layer->rcArea.Bottom; i++)
	{
		for (j = gp_top_layer->rcArea.Left; j <= gp_top_layer->rcArea.Right; j++)
		{
			gp_scr_att[i*SCR_COL + j] = *pCh;
			pCh++;
		}
	}
	free(gp_top_layer->pContent);    /*释放动态存储区*/
	free(gp_top_layer->pScrAtt);
	free(gp_top_layer);
	gp_top_layer = nextLayer;
	gi_sel_sub_menu = 0;
	return;
}

/**
* 函数名称: PopUp
* 函数功能: 在指定区域输出弹出窗口信息, 同时设置热区, 将弹出窗口位置信息入栈.
* 输入参数: pRc 弹出窗口位置数据存放的地址,att 弹出窗口区域字符属性
*          pLabel 弹出窗口中标签束信息存放的地址
*          pHotArea 弹出窗口中热区信息存放的地址
* 输出参数: 无
* 返 回 值: 无
* 调用说明:
*/
void PopUp(SMALL_RECT* pRc, WORD att, LABEL_BUNDLE* pLabel, HOT_AREA* pHotArea)
{
	LAYER_NODE *nextLayer;
	COORD size;
	COORD pos = { 0, 0 };
	char *pCh;
	int i, j, row;
	/*弹出窗口所在位置字符单元信息入栈*/
	size.X = pRc->Right - pRc->Left + 1;    /*弹出窗口的宽度*/
	size.Y = pRc->Bottom - pRc->Top + 1;    /*弹出窗口的高度*/

	/*申请存放弹出窗口相关信息的动态存储区*/
	nextLayer = (LAYER_NODE *)malloc(sizeof(LAYER_NODE));
	nextLayer->next = gp_top_layer;
	nextLayer->LayerNo = gp_top_layer->LayerNo + 1;
	nextLayer->rcArea = *pRc;
	nextLayer->pContent = (CHAR_INFO *)malloc(size.X*size.Y * sizeof(CHAR_INFO));
	nextLayer->pScrAtt = (char *)malloc(size.X*size.Y * sizeof(char));
	pCh = nextLayer->pScrAtt;

	/*将弹出窗口覆盖区域的字符信息保存，用于在关闭弹出窗口时恢复原样*/
	ReadConsoleOutput(gh_std_out, nextLayer->pContent, size, pos, pRc);
	for (i = pRc->Top; i <= pRc->Bottom; i++)
	{   /*此二重循环将所覆盖字符单元的原先属性值存入动态存储区，便于以后恢复*/
		for (j = pRc->Left; j<= pRc->Right; j++)
		{
			*pCh = gp_scr_att[i*SCR_COL + j];
			pCh++;
		}
	}
	gp_top_layer = nextLayer;  /*完成弹出窗口相关信息入栈操作*/

	/*设置弹出窗口区域字符的新属性*/
	pos.X = pRc->Left;
	pos.Y = pRc->Top;
	for (i = pRc->Top; i <= pRc->Bottom; i++)
	{
		FillConsoleOutputAttribute(gh_std_out, att, size.X, pos, &written_num);
		pos.Y++;
	}

	/*将标签束中的标签字符串在设定的位置输出*/
	for (i = 0; i<pLabel->num; i++)
	{
		pCh = pLabel->ppLabel[i];
		wchar_t*a = AnsiToUnicode(pCh);
		if (wcslen(a) != 0)
		{
	
			WriteConsoleOutputCharacter(gh_std_out,a, wcslen(a),pLabel->pLoc[i], &written_num);
		}
	}

	/*设置弹出窗口区域字符单元的新属性*/
	for (i = pRc->Top; i <= pRc->Bottom; i++)
	{   /*此二重循环设置字符单元的层号*/
		for (j = pRc->Left; j <= pRc->Right; j++)
		{
			gp_scr_att[i*SCR_COL + j] = gp_top_layer->LayerNo;
		}
	}

	for (i = 0; i<pHotArea->num; i++)
	{   /*此二重循环设置所有热区中字符单元的热区类型和热区编号*/
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
* 函数名称: TagSubMenu
* 函数功能: 在指定子菜单项上做选中标记.
* 输入参数: num 选中的子菜单项号
* 输出参数: 无
* 返 回 值: 无
* 调用说明:
*/
void TagSubMenu(int num)
{
	SMALL_RECT rcPop;
	COORD pos;
	WORD att;
	int width;

    /*计算弹出子菜单的区域位置, 存放在rcPop中*/
	LocSubMenu(gi_sel_menu, &rcPop);  
	if ((num<1) || (num == gi_sel_sub_menu) || (num>rcPop.Bottom - rcPop.Top - 1))
	{   /*如果子菜单项号越界，或该项子菜单已被选中，则返回*/
		return;
	}
	pos.X = rcPop.Left + 2;
	width = rcPop.Right - rcPop.Left - 3;
	if (gi_sel_sub_menu != 0) /*首先取消原选中子菜单项上的标记*/
	{
		pos.Y = rcPop.Top + gi_sel_sub_menu;
		att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/
		FillConsoleOutputAttribute(gh_std_out, att, width, pos, &written_num);
		pos.X += 1;
		att |= FOREGROUND_RED;/*白底红字*/
		FillConsoleOutputAttribute(gh_std_out, att, 1, pos, &written_num);
	}

	/*在制定子菜单项上做选中标记*/
	pos.X = rcPop.Left + 2;
	pos.Y = rcPop.Top + num;
	att = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;  /*黑底白字*/
	FillConsoleOutputAttribute(gh_std_out, att, width, pos, &written_num);
	gi_sel_sub_menu = num;  /*修改选中子菜单项号*/
	return;
}

/**
* 函数名称: LocSubMenu
* 函数功能: 计算弹出子菜单区域左上角和右下角的位置.
* 输入参数: num 选中的主菜单项号
* 输出参数: rc 存放区域位置信息的地址
* 返 回 值: 无
* 调用说明:
*/
void LocSubMenu(int num, SMALL_RECT* rc)
{
	int i, len, loc = 0;

	rc->Top = 1; /*区域的上边定在第2行，行号为1*/
	rc->Left = 1;
	for (i = 1; i<num; i++)
	{   /*计算区域左边界位置, 同时计算第一个子菜单项在子菜单字符串数组中的位置*/
		rc->Left += strlen(ga_main_menu[i - 1]) + 8;
		loc += ga_sub_menu_count[i - 1];
	}
	rc->Right = strlen(ga_sub_menu[loc]);/*暂时存放第一个子菜单项字符串长度*/
	for (i = 1; i<ga_sub_menu_count[num - 1]; i++)
	{   /*查找最长子菜单字符串，将其长度存放在rc->Right*/
		len = strlen(ga_sub_menu[loc + i]);
		if (rc->Right < len)
		{
			rc->Right = len;
		}
	}
	rc->Right += rc->Left + 3;  /*计算区域的右边界*/
	rc->Bottom = rc->Top + ga_sub_menu_count[num - 1] + 1;/*计算区域下边的行号*/
	if (rc->Right >= SCR_COL)  /*右边界越界的处理*/
	{
		len = rc->Right - SCR_COL + 1;
		rc->Left -= len;
		rc->Right = SCR_COL - 1;
	}
	return;
}

/**
* 函数名称: RunSys
* 函数功能: 运行系统, 在系统主界面下运行用户所选择的功能模块.
* 输入参数: 无
* 输出参数: phead 主链头指针的地址
* 返 回 值: 无
* 调用说明:
*/
void RunSys(SchoolNode **phead)
{
	INPUT_RECORD inRec;
	DWORD res;
	COORD pos = { 0, 0 };
	BOOL bRet = TRUE;
	int i, loc, num;
	int cNo, cAtt;      /*cNo:字符单元层号, cAtt:字符单元属性*/
	char vkc, asc;      /*vkc:虚拟键代码, asc:字符的ASCII码值*/

	while (bRet)
	{   /*从控制台输入缓冲区中读一条记录*/
		ReadConsoleInput(gh_std_in, &inRec, 1, &res);

		if (inRec.EventType == MOUSE_EVENT) /*如果记录由鼠标事件产生*/
		{
			pos = inRec.Event.MouseEvent.dwMousePosition;  /*获取鼠标坐标位置*/
			cNo = gp_scr_att[pos.Y * SCR_COL + pos.X] & 3; /*取该位置的层号*/
			cAtt = gp_scr_att[pos.Y * SCR_COL + pos.X] >> 2;/*取该字符单元属性*/
			if (cNo == 0) /*层号为0，表明该位置未被弹出子菜单覆盖*/
			{

				/* cAtt > 0 表明该位置处于热区(主菜单项字符单元)*/
				/* cAtt != gi_sel_menu 表明该位置的主菜单项未被选中*/
				/* gp_top_layer->LayerNo > 0 表明当前有子菜单弹出*/
				if (cAtt > 0 && cAtt != gi_sel_menu && gp_top_layer->LayerNo > 0)
				{
					PopOff();            /*关闭弹出的子菜单*/
					gi_sel_sub_menu = 0; /*将选中子菜单项的项号置为0*/
					PopMenu(cAtt);       /*弹出鼠标所在主菜单项对应的子菜单*/
				}
			}
			else if (cAtt > 0) /*鼠标所在位置为弹出子菜单的菜单项字符单元*/
			{
				TagSubMenu(cAtt); /*在该子菜单项上做选中标记*/
			}

			if (inRec.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) /*如果按下鼠标左边第一键*/
			{
				if (cNo == 0) /*层号为0，表明该位置未被弹出子菜单覆盖*/
				{
					if (cAtt > 0) /*如果该位置处于热区(主菜单项字符单元)*/
					{
						PopMenu(cAtt);   /*弹出鼠标所在主菜单项对应的子菜单*/
					}

					/*如果该位置不属于主菜单项字符单元，且有子菜单弹出*/
					else if (gp_top_layer->LayerNo > 0)
					{
						PopOff();            /*关闭弹出的子菜单*/
						gi_sel_sub_menu = 0; /*将选中子菜单项的项号置为0*/
					}
				}
				else /*层号不为0，表明该位置被弹出子菜单覆盖*/
				{
					if (cAtt > 0) /*如果该位置处于热区(子菜单项字符单元)*/
					{
						PopOff(); /*关闭弹出的子菜单*/
						gi_sel_sub_menu = 0; /*将选中子菜单项的项号置为0*/

											 /*执行对应功能函数:gi_sel_menu主菜单项号,cAtt子菜单项号*/
						bRet = ExeFunction(gi_sel_menu, cAtt);
					}
				}
			}
			else if (inRec.Event.MouseEvent.dwButtonState == RIGHTMOST_BUTTON_PRESSED) /*如果按下鼠标右键*/
			{
				if (cNo == 0) /*层号为0，表明该位置未被弹出子菜单覆盖*/
				{
					PopOff();            /*关闭弹出的子菜单*/
					gi_sel_sub_menu = 0; /*将选中子菜单项的项号置为0*/
				}
			}
		}
		else if (inRec.EventType == KEY_EVENT && inRec.Event.KeyEvent.bKeyDown) /*如果记录由按键产生,且键被按下*/
		{
			vkc = inRec.Event.KeyEvent.wVirtualKeyCode;      /*获取按键的虚拟键码*/
			asc = inRec.Event.KeyEvent.uChar.AsciiChar;      /*获取按键的ASCII码*/

			/*系统快捷键的处理*/
			if (vkc == 112) /*如果按下F1键*/
			{
				if (gp_top_layer->LayerNo != 0) /*如果当前有子菜单弹出*/
				{
					PopOff();            /*关闭弹出的子菜单*/
					gi_sel_sub_menu = 0; /*将选中子菜单项的项号置为0*/
				}
				bRet = ExeFunction(5, 1);  /*运行帮助主题功能函数*/
			}
			else if (inRec.Event.KeyEvent.dwControlKeyState & (LEFT_ALT_PRESSED | RIGHT_ALT_PRESSED))
			{ /*如果按下左或右Alt键*/
				switch (vkc)  /*判断组合键Alt+字母*/
				{
				case 88:  /*Alt+X 退出*/
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
			else if (asc == 0) /*其他控制键的处理*/
			{
				if (gp_top_layer->LayerNo == 0) /*如果未弹出子菜单*/
				{
					switch (vkc) /*处理方向键(左、右、下)，不响应其他控制键*/
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
				else  /*已弹出子菜单时*/
				{
					for (loc = 0, i = 1; i < gi_sel_menu; i++)
					{
						loc += ga_sub_menu_count[i - 1];
					}  /*计算该子菜单中的第一项在子菜单字符串数组中的位置(下标)*/
					switch (vkc) /*方向键(左、右、上、下)的处理*/
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
			{  /*按下普通键*/
				if (gp_top_layer->LayerNo == 0)  /*如果未弹出子菜单*/
				{
					switch (vkc)
					{
					case 70: /*f或F*/
						PopMenu(1);
						break;
					case 77: /*m或M*/
						PopMenu(2);
						break;
					case 81: /*q或Q*/
						PopMenu(3);
						break;
					case 83: /*s或S*/
						PopMenu(4);
						break;
					case 72: /*h或H*/
						PopMenu(5);
						break;
					case 13: /*回车*/
						PopMenu(gi_sel_menu);
						TagSubMenu(1);
						break;
					}
				}
				else /*已弹出子菜单时的键盘输入处理*/
				{
					if (vkc == 27) /*如果按下ESC键*/
					{
						PopOff();
						gi_sel_sub_menu = 0;
					}
					else if (vkc == 13) /*如果按下回车键*/
					{
						num = gi_sel_sub_menu;
						PopOff();
						gi_sel_sub_menu = 0;
						bRet = ExeFunction(gi_sel_menu, num);
					}
					else /*其他普通键的处理*/
					{
						/*计算该子菜单中的第一项在子菜单字符串数组中的位置(下标)*/
						for (loc = 0, i = 1; i < gi_sel_menu; i++)
						{
							loc += ga_sub_menu_count[i - 1];
						}

						/*依次与当前子菜单中每一项的代表字符进行比较*/
						for (i = loc; i < loc + ga_sub_menu_count[gi_sel_menu - 1]; i++)
						{
							if (strlen(ga_sub_menu[i]) > 0 && vkc == ga_sub_menu[i][1])
							{ /*如果匹配成功*/
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
* 函数名称: CloseSys
* 函数功能 : 关闭系统.
* 输入参数 : hd 主链头指针
* 输出参数 : 无
* 返 回 值 : 无
* 调用说明 :
*/
void CloseSys(SchoolNode *hd)
{
	SchoolNode *pSchNode1 = hd, *pSchNode2;
	MajorNode *pMajorNode1, *pMajorNode2;
	RecruitNode *pEnrolNode1, *pEnrolNode2;

	while (pSchNode1 != NULL) /*释放十字交叉链表的动态存储区*/
	{
		pSchNode2 = pSchNode1->next;
		pMajorNode1 = pSchNode1->m_next;
		while (pMajorNode1 != NULL) /*释放专业信息支链的动态存储区*/
		{
			pMajorNode2 = pMajorNode1->next;
			pEnrolNode1 = pMajorNode1->r_next;
			while (pEnrolNode1 != NULL) /*释放招生信息支链的动态存储区*/
			{
				pEnrolNode2 = pEnrolNode1->next;
				free(pEnrolNode1);
				pEnrolNode1 = pEnrolNode2;
			}
			free(pMajorNode1);
			pMajorNode1 = pMajorNode2;
		}
		free(pSchNode1);  /*释放主链结点的动态存储区*/
		pSchNode1 = pSchNode2;
	}

	CONSOLE_SCREEN_BUFFER_INFO bInfo;
	COORD home = { 0, 0 };
	unsigned long size;
	GetConsoleScreenBufferInfo(gh_std_out, &bInfo);/*取屏幕缓冲区信息*/
	size = bInfo.dwSize.X * bInfo.dwSize.Y; /*计算屏幕缓冲区字符单元数*/
											/*将屏幕缓冲区所有单元的字符属性设置为当前屏幕缓冲区字符属性*/
	FillConsoleOutputAttribute(gh_std_out, bInfo.wAttributes, size, home, &written_num);
	/*将屏幕缓冲区所有单元填充为空格字符*/
	FillConsoleOutputCharacter(gh_std_out, ' ', size, home, &written_num);
	goto_xy(0, 1);
	printf("感谢使用该系统！\n");

	/*释放存放菜单条、状态条等信息动态存储区*/
	free(gp_buff_menubar_info);
	free(gp_buff_stateBar_info);

	/*关闭标准输入和输出设备句柄*/
	CloseHandle(gh_std_out);
	CloseHandle(gh_std_in);

	/*将窗口标题栏置为运行结束*/
	SetConsoleTitle(TEXT("运行结束"));
	return;
}

/**
* 函数名称: ExeFunction
* 函数功能: 执行由主菜单号和子菜单号确定的功能函数.
* 输入参数: m 主菜单项号,s 子菜单项号
* 输出参数: 无
* 返 回 值: BOOL类型, TRUE 或 FALSE
* 调用说明: 仅在执行函数ExitSys时, 才可能返回FALSE, 其他情况下总是返回TRUE
*/
BOOL ExeFunction(int m, int s)
{
	BOOL bRet = TRUE;
	/*函数指针数组，用来存放所有功能函数的入口地址*/
	BOOL(*pFunction[19])(void);
	int i=0, loc=0;

	/*将功能函数入口地址存入与功能函数所在主菜单号和子菜单号对应下标的数组元素*/
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

	for (i = 1, loc = 0; i<m; i++)  /*根据主菜单号和子菜单号计算对应下标*/
	{
		loc += ga_sub_menu_count[i - 1];
	}
	loc += s - 1;
	if (pFunction[loc] != NULL)
	{
		bRet = (*pFunction[loc])();  /*用函数指针调用所指向的功能函数*/
	}
	return bRet;

}

/*查找高校信息结点*/
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

/*查找专业基本信息结点*/
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

/*查找招生信息结点*/
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

/*插入高校信息结点*/
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
		printf("插入高校信息结点成功！\n");
	}
	else  printf("此高校已存在，插入失败！\n");
	return TRUE;
}

/*删除高校信息结点*/
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
		if (ps_prev == NULL) /*如果释放的是高校链的链头结点*/
		{
			(*hd) = ps_current->next;
		}
		else
		{
			ps_prev->next = ps_current->next;
		}
		free(ps_current); /*释放要删除的高校结点*/
	}
	return bRet;
}

/*插入专业信息结点*/
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
		printf("此专业信息已存在！\n");
	}
	return TRUE;
}

/*删除专业信息结点*/
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
		if (pm_prev == NULL)/*删的是链头*/
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

/*插入招生信息结点*/
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
		printf("此招生信息已存在！\n");
	}
	return TRUE;
}

/*删除招生信息结点*/
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
		printf("已删除相应的招生信息！\n");
	}
	else
	{
		printf("不存在该招生信息！\n");
		return 0;
	}
	return 1;
}

/*维护高校信息*/
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
	printf("请输入要进行的菜单选项：\n");
	printf("选择1*****************插入高校信息\n");
	printf("选择2*****************修改高校信息\n");
	printf("选择3*****************删除高校信息\n");
	scanf("%d", &i);
	if (i == 1)
	{
		psch_node = (SchoolNode*)malloc(sizeof(SchoolNode));
		psch_node->m_next = NULL;
		printf("请输入高校编号:\n");
		scanf("%s",psch_node->schoolId);
		printf("请输入高校名称:\n");
		scanf("%s", psch_node->schoolName);
		printf("请输入高校地址\n");
		scanf("%s", psch_node->schoolAddress);
		printf("请输入联系电话:\n");
		scanf("%s", psch_node->schoolPhone);
		InsertSchNode(&phead, psch_node);
		SaveList(phead, FileName);
		printf("数据保存成功！\n");
	}
	else if (i == 2)
	{
		ClearScreen();
		int j = 0;
		printf("请输入要修改的学校编号或名称：\n");
		scanf("%s", sch);
		psch_node = SeekSchNode(phead, sch);
		if (psch_node != NULL)
		{
			printf("选择需要修改的选项：\n");
			printf("1***高校编号\n2***高校名称\n3***高校地址\n4***高校电话\n");
			while (scanf("%d", &j) == 1)
			{
				if (j == 1)
				{
					printf("请输入要修改的高校编号:\n");
					scanf("%s", psch_node->schoolId);
				}
				else if (j == 2)
				{
					printf("请输入修改后的高校名称:\n");
					scanf("%s", psch_node->schoolName);
				}
				else if (j == 3)
				{
					printf("请输入修改后的高校地址:\n");
					scanf("%s", psch_node->schoolAddress);
				}
				else if (j == 4)
				{
					printf("请输入修改后的联系电话:\n");
					scanf("%s", psch_node->schoolPhone);
				}
				printf("数据保存成功！\n");
				printf("请继续选择或输入q退出！\n");
			}
			getchar();
			SaveList(phead, FileName);
		}
		else
		{
			printf( "不存在该信息！\n" );
		}
	}
	else if (i == 3)
	{
		printf("请输入要删除的高校编号或名字:\n");
		scanf("%s", sch);
		if(DelSchNode(&phead, schoolId) == 1) printf("删除成功！\n");
		else printf("无此信息！\n");
		SaveList(phead, FileName);
	}
	return TRUE;
}

/*维护专业信息*/
BOOL MaintainMajorInfo(void)
{
	ClearScreen();
	int i = 0;
	MajorNode *pmaj_node;
	char m_schoolId[10];
	char majorId[10];
	char majorName[30];
	printf("请输入要进行的菜单选项：\n");
	printf("选择1*****************插入专业信息\n");
	printf("选择2*****************修改专业信息\n");
	printf("选择3*****************删除专业信息\n");
	scanf("%d", &i);
	if (i == 1)
	{
		pmaj_node = (MajorNode*)malloc(sizeof(MajorNode));
		pmaj_node->r_next = NULL;
		printf("请输入专业所属高校编号:\n");
		scanf("%s", pmaj_node->m_schoolId);
		printf("请输入专业编号:\n");
		scanf("%s", pmaj_node->majorId);
		printf("请输入专业名称：\n");
		scanf("%s", pmaj_node->majorName);
		InsertMajorNode(phead, pmaj_node);
		SaveList(phead, FileName);
		printf("数据保存成功！\n");
	}
	else if (i == 2)
	{
		ClearScreen();
		printf("请输入要修改专业的编号：\n");
		scanf("%s", majorId);
		pmaj_node = SeekMajorNode(phead, majorId);
		int j = 0;
		if (pmaj_node != NULL)
		{
			printf("选择需要修改的选项：\n");
			printf("1***高校编号\n2***专业名称\n3***专业编号\n");
			while (scanf("%d", &j) == 1)
			{
				if (j == 1)
				{
					printf("请输入要修改的专业所属高校编号:\n");
					scanf("%s", pmaj_node->m_schoolId);
				}
				else if (j == 2)
				{
					printf("请输入要修改的专业名称：\n");
					scanf("%s", pmaj_node->majorName);
				}
				else if (j == 3)
				{
					printf("请输入要修改的专业编号:\n");
					scanf("%s", pmaj_node->majorId);
				}
				printf("数据保存成功！\n");
				printf("请继续选择或输入q退出！\n");
			}
			getchar();
			//ModifMajorNode(phead, majorId, m_schoolId, majorName);
			SaveList(phead, FileName);
			printf("数据保存成功！\n");
		}
	}
	else if (i == 3)
	{
		printf("请输入要删除的专业编号:\n");
		scanf("%s", majorId);
		if (DelMajorNode(phead, majorId) == 1)printf("删除成功！\n");
		else printf("无此信息！\n");
		SaveList(phead, FileName);
	}
	return TRUE;
}

/*维护招生信息*/
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
	printf("请输入要进行的菜单选项：\n");
	printf("选择1*****************插入招生信息\n");
	printf("选择2*****************修改招生信息\n");
	printf("选择3*****************删除招生信息\n");
	scanf("%d", &i);
	if (i == 1)
	{
		prec_node = (RecruitNode*)malloc(sizeof(RecruitNode));
		printf("请输入专业招生信息所属高校编号:\n");
		scanf("%s", prec_node->r_schoolId);
		printf("请输入招生所属专业编号:\n");
		scanf("%s", prec_node->r_majorId);
		printf("请输入招生年份：\n");
		scanf("%d", &prec_node->year);
		printf("请输入该年份该专业招生计划人数：\n");
		scanf("%d", &prec_node->PlanNum);
		printf("请输入该年份该专业实际招生人数：\n");
		scanf("%d", &prec_node->ActualNum);
		printf("请输入该年份该专业最高录取分数：\n");
		scanf("%d", &prec_node->MaxScore);
		printf("请输入该年份该专业最低录取分数：\n");
		scanf("%d", &prec_node->MinScore);
		InsertRecNode(phead, prec_node);
		SaveList(phead, FileName);
		printf("数据保存成功！\n");
	}
	else if (i == 2)
	{
		ClearScreen();
		printf("请输入您需要修改的招生信息的高校编号：\n");
		scanf("%s", r_schoolId);
		printf("请输入您需要修改的招生信息的专业编号：\n");
		scanf("%s", r_majorId);
		printf("请输入您需要修改的招生信息的年份：\n");
		scanf("%d", &year);
		psch_node = SeekSchNode(phead, r_schoolId);
		pmaj_node = SeekMajorNode(phead, r_majorId);
		prec_node = SeekRecNode(r_schoolId, r_majorId, year);
		int j = 0;
		if (prec_node != NULL)
		{
			printf("请输入您要修改的选项：\n");
			printf("1***高校编号\n2***专业编号\n3***招生年份\n4***招生计划人数\n5***实际招生人数\n6***最高分\n7***最低分\n");
			while (scanf("%d", &j) == 1)
			{
				if (j == 1)
				{
					printf("请输入要修改的招生信息的所属高校编号:\n");
					scanf("%s", prec_node->r_schoolId);
				}
				else if (j == 2)
				{
					printf("请输入要修改的招生信息的所属专业编号:\n");
					scanf("%s", prec_node->r_majorId);
				}
				else if (j == 3)
				{
					printf("请输入招生年份：\n");
					scanf("%d", &prec_node->year);
				}
				else if (j == 4)
				{
					printf("请输入该年份该专业招生计划人数：\n");
					scanf("%d", &prec_node->PlanNum);
				}
				else if (j == 5)
				{
					printf("请输入该年份该专业实际招生人数：\n");
					scanf("%d", &prec_node->ActualNum);
				}
				else if (j == 6)
				{
					printf("请输入该年份该专业最高录取分数：\n");
					scanf("%d", &prec_node->MaxScore);
				}
				else if (j == 7)
				{
					printf("请输入该年份该专业最低录取分数：\n");
					scanf("%d", &prec_node->MinScore);
				}
				printf("数据保存成功！\n");
				printf("请继续选择或输入q退出！\n");
			}
			getchar();
			//ModifRecNode(r_schoolId, r_majorId, year, PlanNum, ActualNum, MaxScore, MinScore);
			SaveList(phead, FileName);
			printf("数据保存成功！\n");
		}
		else
		{
			printf("该信息不存在！\n");
		}
	}
	else if (i == 3)
	{
		printf("请输入您需要删除的招生信息的高校编号：\n");
		scanf("%s", r_schoolId);
		printf("请输入您需要删除的招生信息的专业编号：\n");
		scanf("%s", r_majorId);
		printf("请输入您需要删除的招生信息的年份：\n");
		scanf("%d", &year);
		if (DelRecNode(r_schoolId,r_majorId,year) == 1)printf("删除成功！\n");
		else
		{
			printf("该信息不存在！\n");
		}
		SaveList(phead, FileName);
	}
	return TRUE;
}

/*查询高校信息*/
BOOL QuerySchInfo(void)
{
	ClearScreen();
	SchoolNode *psch_node;
	char school[20];
	printf("请输入学校编号或者高校名称！\n");
	scanf("%s", school);
	psch_node = SeekSchNode(phead, school);
	ClearScreen();
	if (psch_node != NULL)
	{
		printf("高校名称：%s\n高校编号：%s\n高校地址：%s\n高校联系电话：%s\n", psch_node->schoolName, psch_node->schoolId,
			psch_node->schoolAddress, psch_node->schoolPhone);	
	}
	else
	{
		printf("该学校信息不存在！\n");
	}
	return TRUE;
}

/*查询专业信息*/
BOOL QueryMajorInfo(void)
{
	ClearScreen();
	SchoolNode *psc_node;
	MajorNode *pmaj_node;
	char major[20];
	int flag = 0;
	printf("请输入您所想查询的专业的名称：\n");
	scanf("%s", major);
	printf("\n\n");
	printf("%-18s%-18s%-18s%-18s\n","专业名称","专业编号","高校编号","高校名称");
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
		printf("该专业信息不存在！\n");
	}
	else
		printf("该专业信息共有%d个！\n",flag);
	return TRUE;
}

/*查询招生信息*/
BOOL QueryRecInfo(void)
{
	ClearScreen();
	RecruitNode *prec_node;
	MajorNode *pmaj_node;
	SchoolNode *psch_node;
	char school[20];
	char major[20];
	int year;
	printf("请输入要查找的招生信息所属的高校名称或编号！\n");
	scanf("%s", school);
	printf("请输入要查找的招生信息所属专业编号！\n");
	scanf("%s", major);
	printf("请输入年份！\n");
	scanf("%d", &year);
	ClearScreen();
	psch_node = SeekSchNode(phead, school);
	pmaj_node = SeekMajorNode(phead, major);
	prec_node = SeekRecNode(school, major, year);
	if (prec_node != NULL)
	{
		printf("招生所属高校编号：%s\n招生所属专业编号：%s\n\n", prec_node->r_schoolId,prec_node->r_majorId);
		printf("高校：%s\n专业：%s\n", psch_node->schoolName, pmaj_node->majorName);
		printf("招生年份 : %d\n招生计划人数 : %d\n招生实际人数 : %d\n最高录取分数 : %d\n最低录取分数 : %d\n",
			prec_node->year, prec_node->PlanNum, prec_node->ActualNum, prec_node->MaxScore, prec_node->MinScore);
	}
	else
	{
		printf("该招生信息不存在！\n");
	}
	return TRUE;
}

/*综合查询相应的专业*/
BOOL QueryMajorInfoM(void)
{
	ClearScreen();
	SchoolNode *psch_node;
	MajorNode *pmaj_node;
	RecruitNode *prec_node;
	int year;
	int i;
	printf("请输入年份：");
	scanf("%d", &year);
	printf("请输入您该年份所考的实际分数：");
	scanf("%d", &i);
	printf("\n\n以下这些专业欢迎您选择报考！\n\n\n");

	for (psch_node = phead; psch_node != NULL; psch_node = psch_node->next)
	{
		for (pmaj_node = psch_node->m_next; pmaj_node != NULL; pmaj_node = pmaj_node->next)
		{
			for (prec_node = pmaj_node->r_next; prec_node != NULL; prec_node = prec_node->next)
			{
				if ((year == prec_node->year) && (i >= prec_node->MinScore))
				{
					printf("%-15s %-18s 专业编号：%-8s 最低分：%-5d 最高分：%-5d\n", psch_node->schoolName, pmaj_node->majorName,
						pmaj_node->majorId, prec_node->MinScore, prec_node->MaxScore);
				}
			}

		}
	}
	printf("\n无更多信息！\n");
	return TRUE;
}

/*统计录入信息总数*/
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
	printf("所录入高校信息总数为%d\n", sch_num);
	printf("所录入专业信息总数为%d\n", maj_num);
	printf("所录入招生信息总数为%d\n\n", rec_num);

	printf("下列为高校及其编码：\n\n\n");
	printf("\t高校名称\t高校编码\n\n");
	for (psch_node = phead; psch_node != NULL; psch_node = psch_node->next)
	{
		printf("\t%s\t%s\n", psch_node->schoolName, psch_node->schoolId);
	}
	return TRUE;
}

/*高校录取率统计*/
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
		printf("%s在%d年录取率为%.2lf%%\n", psch_node->schoolName,year,100 * rate);
	}
	return TRUE;
}

/*年份招生总数统计*/
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
			printf("\t%d年：\t%d人\n", year, rec_num);
		}
	}
	return TRUE;
}

/*帮助主题*/
BOOL HelpTopic(void)
{
	ClearScreen();
	BOOL bRet = TRUE;
	printf("\n\n该系统支持鼠标与键盘操作，并支持快捷键的使用，具体快捷键为（Alt键+菜单里括号中的英文字母）。\n");
	printf("按Alt键+X键可直接退出系统！\n");
	return bRet;
}

/*关于系统*/
BOOL AboutDorm(void)
{
	BOOL bRet = TRUE;
	ClearScreen();
	printf("\n\n    该招生查询系统C语言课程设计由本人独立完成。我在编写程序中遇到了诸多困难,");
	printf("感谢在此过程中无私帮助我的老师和同学！欢迎大家指正！\n");
	return bRet;
}

/*保存数据*/
BOOL SaveData(void)
{
	BOOL bRet = TRUE;
	ClearScreen();
	LoadList(&phead, FileName);
	SaveList(phead, FileName);
	return bRet;
}

/**< 将链表的内容存盘*/
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
	printf("[相关数据已保存!]\n");
}

/**< 将数据加载*/
BOOL LoadData(void)
{
	BOOL bRet = TRUE;
	ClearScreen();
	LoadList(&phead, FileName);   /*加载链表信息*/
	printf("加载成功！\n");
	return bRet;
}

/**< 加载文件中的信息。*/
void LoadList(SchoolNode  **phead, char *fn[3])
{
	SchoolNode *hd = NULL, *p1;
	MajorNode *p2;
	RecruitNode  *p3;
	FILE *in1, *in2, *in3;
	int  find = 0;
	if ((in1 = fopen(fn[0], "rb")) == NULL)
	{
		printf("高校文件加载失败,系统自动退出!\n"); Sleep(1000);  exit(-1);
	}
	if ((in2 = fopen(fn[1], "rb")) == NULL)
	{
		printf("专业文件加载失败,系统自动退出!\n"); Sleep(1000);  exit(-1);
	}
	if ((in3 = fopen(fn[2], "rb")) == NULL)
	{
		printf("招生文件加载失败,系统自动退出!\n"); Sleep(1000);  exit(-1);
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

/**< 基础数据备份*/
BOOL BackupData(void)
{
	BOOL bRet = TRUE;
	ClearScreen();
	LoadList(&phead, FileName);
	SaveList(phead, BeiFen_FileName);
	printf("数据备份成功![备份数据存储在三个备份文件中]\n");
	return bRet;
}

/**< 恢复备份数据*/
BOOL RestoreData(void)
{
	BOOL bRet = TRUE;
	char ch;
	ClearScreen();
	printf("注意:如果恢复备份数据当前的数据将被覆盖,你确定继续恢复吗?确定(Y) 取消(N):\n");
	ch = getchar(); getchar();
	if (ch != 'Y' && ch != 'y')    return bRet;
	LoadList(&phead, BeiFen_FileName);
	SaveList(phead, FileName);
	printf("备份数据恢复成功!\n");
	return bRet;
}

/*退出系统*/
BOOL ExitSys(void)
{
	LABEL_BUNDLE labels;
	HOT_AREA areas;
	BOOL bRet = TRUE;
	SMALL_RECT rcPop;
	COORD pos;
	WORD att;
	char *pCh[] = { "确认退出系统吗？", "确定    取消" };
	int iHot = 1;

	ClearScreen();
	pos.X = strlen(pCh[0]) + 6;
	pos.Y = 7;
	rcPop.Left = (SCR_COL - pos.X) / 2;
	rcPop.Right = rcPop.Left + pos.X - 1;
	rcPop.Top = (SCR_ROW - pos.Y) / 2;
	rcPop.Bottom = rcPop.Top + pos.Y - 1;

	att = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/
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

/*设置热区*/
void SetHotPoint(HOT_AREA *pHotArea, int iHot)
{
	CONSOLE_CURSOR_INFO lpCur;
	COORD pos = { 0, 0 };
	WORD att1, att2;
	int i, width;

	att1 = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;  /*黑底白字*/
	att2 = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;  /*白底黑字*/
	for (i = 0; i < pHotArea->num; i++)
	{  /*将按钮类热区置为白底黑字*/
		pos.X = pHotArea->pArea[i].Left;
		pos.Y = pHotArea->pArea[i].Top;
		width = pHotArea->pArea[i].Right - pHotArea->pArea[i].Left + 1;
		if (pHotArea->pSort[i] == 0)
		{  /*热区是按钮类*/
			FillConsoleOutputAttribute(gh_std_out, att2, width, pos, &written_num);
		}
	}

	pos.X = pHotArea->pArea[iHot - 1].Left;
	pos.Y = pHotArea->pArea[iHot - 1].Top;
	width = pHotArea->pArea[iHot - 1].Right - pHotArea->pArea[iHot - 1].Left + 1;
	if (pHotArea->pSort[iHot - 1] == 0)
	{  /*被激活热区是按钮类*/
		FillConsoleOutputAttribute(gh_std_out, att1, width, pos, &written_num);
	}
	else if (pHotArea->pSort[iHot - 1] == 1)
	{  /*被激活热区是文本框类*/
		SetConsoleCursorPosition(gh_std_out, pos);
		GetConsoleCursorInfo(gh_std_out, &lpCur);
		lpCur.bVisible = TRUE;
		SetConsoleCursorInfo(gh_std_out, &lpCur);
	}
}

/*控制台输入处理*/
int DealInput(HOT_AREA *pHotArea, int *piHot)
{
	INPUT_RECORD inRec;
	DWORD res;
	COORD pos = { 0, 0 };
	int num, arrow, iRet = 0;
	int cNo, cTag, cSort;/*cNo:层号, cTag:热区编号, cSort: 热区类型*/
	char vkc, asc;       /*vkc:虚拟键代码, asc:字符的ASCII码值*/

	SetHotPoint(pHotArea, *piHot);
	while (TRUE)
	{    /*循环*/
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
				{  /*方向键(左、上、右、下)的处理*/
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
			{  /*ESC键*/
				iRet = 27;
				break;
			}
			else if (vkc == 13 || vkc == 32)
			{  /*回车键或空格表示按下当前按钮*/
				iRet = 13;
				break;
			}
		}
	}
	return iRet;
}

/*将char*类型转换为wchar*类型*/
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

/*定位光标*/
void goto_xy(int x, int y)
{
	HANDLE hOut;
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos = { x, y };
	SetConsoleCursorPosition(hOut, pos);
}

/**< 开始进入系统欢迎界面.*/
void Start(void)
{
	CONSOLE_CURSOR_INFO lpCur;
	goto_xy(17, 7);
	printf("*  *  * "); Delay(); printf(" *  * "); Delay(); printf(" *  * "); Delay(); printf(" *  * "); Delay();
	printf(" *  * "); Delay(); printf(" *  * "); Delay(); printf(" *  *  *\n\n"); Delay(); printf("%14s│%11s", " ", " ");
	printf("  ★ "); Delay(); printf(" 欢 "); Delay(); printf(" 迎 "); Delay(); printf(" 进 "); Delay();
	printf(" 入 "); Delay(); printf(" ★ %12s│\n\n"," "); Delay(); printf("%14s│", " ");
	printf("%13s招 "," "); Delay(); printf(" 生 "); Delay();printf(" 查 "); Delay();
	printf(" 询 "); Delay(); printf(" 系 "); Delay();printf(" 统%13s│\n\n"," "); Delay(); printf("%14s│", " ");
	printf("   版"); Delay(); printf("权"); Delay(); printf("所"); Delay(); printf("有"); Delay();
	printf(" (C)"); Delay(); printf(" 2017/09   "); Delay(); printf("——"); Delay(); printf(""); Delay();
	printf("    "); Delay(); printf("U201614792%4s│\n\n"," "); 
	goto_xy(17, 15);
	printf("*  *  * "); Delay(); printf(" *  * "); Delay(); printf(" *  * "); Delay(); printf(" *  * "); Delay();
	printf(" *  * "); Delay(); printf(" *  * "); Delay(); printf(" *  *  *\n\n"); Delay();	Sleep(1000);
	printf("\n\n\n\n%32s按任意键进入系统...", " ", " ");
	getch();
	ScreenFresh();
	return;
}

/**< 刷屏函数*/
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

/**< 时间暂延0.05s.*/
inline void Delay(void)
{
	int goal;
	goal = 50 + clock();
	while (goal > clock());
}
