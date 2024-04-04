#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <Windows.h>
#include <time.h>
#include <conio.h>
#define Width 25								//定义地图大小（可改），默认值25，最小15，最大50
int hX = 1, hY = 1, len = 4;					//定义蛇头坐标，蛇长
int* map = NULL;								//记录地图方块，食物(-1)，墙(-2)，自己(>=1)，空地(0)
int map_temp[Width * Width] = { 0 };			//记录需要更改的游戏画面坐标
int c = 'd', cl = 'd', i = 0;					//方向，方向缓存，循环变量
char n_1, n_2 = '4', n_3, j = 0;				//n_1选择模式，n_2选择难度,n_3为迷宫选择,j为字符缓存
char deaw[Width * Width * 2 + 1] = { 0 };		//绘制缓存

void Menu();
void Create_Map();
void Sleep_time();
void Move();
void Food();
void Print_Map();
void Hide_Move_Cursor(int x, int y);
BOOL SetConsoleColor(unsigned int wAttributes);
int  GameOver();
int main(void)
{
	sprintf(deaw, "mode con: cols=%d lines=%d", Width * 2, Width);
	system(deaw);						//修改控制台窗口大小
	srand((unsigned)time(0));			//生成随机数
	map = (int*)malloc((Width * Width) * sizeof(int));
	if (map == NULL)
	{
		printf("No enough memory!\n");
		exit(1);
	}
	do {								//随机生成界面颜色
		i = rand() % 6;
	} while (i == 3);
	sprintf(deaw, "color F%d", i);
	system(deaw);
	Menu();								//初始化菜单
	system("cls");
	sprintf(deaw, "color 0%d", i);
	system(deaw);
	do {
		hX = 1, hY = 1, len = 4;		//初始化蛇图形数据
		c = 'd', cl = 'd', i = 0;
		for (i = 0; i < Width * Width; i++)				//初始化地图围墙
		{
			map[i] = 0;
			map_temp[i] = 1;
		}
		Create_Map();									//生成地图
		Hide_Move_Cursor(0, 0);							//将光标移至指定位置并隐藏
		Print_Map();									//打印画面
		if (n_1 == '1' || n_1 == '2' || (n_1 == '3' && n_3 == '1'))
		{
			hX = 1; hY = 1;
		}
		else if (n_1 == '3' && n_3 == '2')
		{
			hX = Width / 5;
			hY = (2 * Width / 5 + 2);
		}
		for (i = 0; i < Width * Width; i++)				//初始化记录数组
			map_temp[i] = 0;
		do {
			i = rand() % (Width * Width);
		} while (map[i] != 0);							//防止食物位置生成在蛇身或墙上
		map[i] = -1;									//生成食物
		map_temp[i] = 1;								//是否生成：1是、0否
		for (system("title 得分:0"); 1; Sleep_time())	//初始化计分
		{
			Move();										//控制蛇移动
			if (map[hY * Width + hX] > 1 ||				//判断是否吃到自己
				map[hY * Width + hX] == -2) break;		//判断是否撞墙
			Food();										//吃食物判定，生成食物
			map[hY * Width + hX] = len;					//蛇头赋值
			map_temp[hY * Width + hX] = 1;
			Print_Map();								//打印画面
			for (i = 0; i < Width * Width; i++)			//将记录数组初始化
				map_temp[i] = 0;
		}
		sprintf(deaw, "title GameOver!得分:%d 按空格键重新开始", len - 4);
		system(deaw);				//结束提醒
	} while (GameOver());			//输入空格重新游戏，输入Esc退出
	Hide_Move_Cursor(Width * Width, Width);				//结束时将光标移至地图下方
	free(map);
	return 0;
}

//函数功能：生成初始菜单
void Menu()
{
	while (1)
	{
		system("cls");							//清屏
		HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
		COORD pos[7] = { 0 };
		for (i = 0; i < 7; i++)					//定义坐标
		{
			pos[i].X = (Width - 7);
			pos[i].Y = i * 2 + Width / 5;
		}
		SetConsoleCursorPosition(hOut, pos[0]);	//生成初始字幕
		printf("我的贪吃蛇");
		SetConsoleCursorPosition(hOut, pos[1]);
		printf("1.经典模式");
		SetConsoleCursorPosition(hOut, pos[2]);
		printf("2.无限模式");
		SetConsoleCursorPosition(hOut, pos[3]);
		printf("3.迷宫模式");
		SetConsoleCursorPosition(hOut, pos[4]);
		printf("4.游戏说明");
		SetConsoleCursorPosition(hOut, pos[5]);
		printf("5.退出游戏");
		SetConsoleCursorPosition(hOut, pos[6]);
		n_1 = getchar();						//输入选择
		while (getchar() != '\n');				//读走缓冲区字符
		if (n_1 == '1' || n_1 == '2')break;
		else if (n_1 == '3')
		{
			system("cls");						//清屏
			SetConsoleCursorPosition(hOut, pos[0]);
			printf(" 迷宫模式");
			SetConsoleCursorPosition(hOut, pos[1]);
			printf(" 1.迷宫一");
			SetConsoleCursorPosition(hOut, pos[2]);
			printf(" 2.迷宫二");
			SetConsoleCursorPosition(hOut, pos[3]);
			printf(" ");
			n_3 = getchar();					//输入选择
			while (getchar() != '\n');			//读走缓冲区字符
			if (n_3 == '1' || n_3 == '2')break;
		}
		else if (n_1 == '4')
		{
			system("cls");						//清屏
			printf("\n");
			printf("  wasd(或WSAD)或↑↓←→ ——移动\n");
			printf("  空格   ——暂停游戏\n");
			printf("  Esc    ——退出游戏\n");
			printf("  1234560——调速\n");

			printf("  请按回车键返回开始菜单!\n");
			while (getchar() != '\n');			//读走缓冲区字符
		}
		else if (n_1 == '5') exit(0);
	}
}

//函数功能：生成地图
void Create_Map()
{
	if (n_1 == '1')									//经典模式地图
	{
		for (i = 0; i < Width; i++)map[i] = -2;		//蛇占两空"()",所以把2空作为一格
		for (i = Width; i < Width * (Width - 1); i += Width)
			map[i] = map[i + Width - 1] = -2;		//每行第一格与最后一个差Width-1
		for (i = 0; i < Width; i++)map[i + Width * (Width - 1)] = -2;
	}
	else if (n_1 == '3' && n_3 == '1')
	{
		for (i = 0; i < (Width - 1) / 2; i++)		//生成十字星地图
		{
			map[i * Width + (Width - 1) / 2] = -2;
		}
		for (i = 0; i < Width; i++)
		{
			map[(Width - 1) / 2 * Width + i] = -2;
		}
		for (i = 0; i < (Width - 1) / 2; i++)
		{
			map[(i + (Width + 1) / 2) * Width + (Width - 1) / 2] = -2;
		}
		if (Width % 2 == 0)							//偶数需要再加一行一列和一个数
		{
			for (i = 0; i < Width; i++)
			{
				map[(Width + 1) / 2 * Width + i] = -2;
			}
			for (i = 0; i < Width; i++)
			{
				map[Width / 2 + (i * Width)] = -2;
			}
			map[(Width - 1) * Width + (Width / 2) - 1] = -2;
		}
	}
	else if (n_1 == '3' && n_3 == '2')				//生成迷宫二
	{
		for (i = (2 * Width / 5 * Width);
			i < (2 * Width * Width / 5 + 2 * Width / 5); i++)map[i] = -2;
		for (i = (2 * Width * Width + 3 * Width) / 5;
			i < (2 * Width * Width / 5 + Width); i++)map[i] = -2;
		for (i = (3 * Width * Width / 5);
			i < (Width + 3 * Width * Width / 5); i++)map[i] = -2;
		for (i = (Width * Width - Width + 3 * Width / 5);
			i < (Width * Width); i++)map[i] = -2;
		for (i = Width * 2 / 5 - 1;
			i < (2 * Width * Width / 5 + 2 * Width / 5 - 1); i += Width)map[i] = -2;
		for (i = 2 * Width / 5 - 1;
			i < 4 * Width / 5 - 1; i++)map[i] = -2;
		for (i = (3 * Width * Width / 5 + 3 * Width / 5);
			i < (Width * Width - Width + 3 * Width / 5); i += Width)map[i] = -2;
	}
}

//函数功能：设置难度(延迟时间)
void Sleep_time()
{
	switch (n_2)
	{
	case '6':Sleep(200); break;
	case '5':Sleep(100); break;
	case '4':Sleep(50); break;
	case '3':Sleep(25); break;
	case '2':Sleep(5); break;
	case '1':Sleep(1); break;
	case '0':break;
	}
}

//函数功能：控制蛇移动
void Move()
{
	if (_kbhit() && (cl = _getch()))					//判断是否输入
		switch (cl)
		{
		case 'a':case 'K':case'A':if (c != 'd')c = 'a'; break;//判断与原方向是否冲突
		case 'd':case 'M':case'D': if (c != 'a')c = 'd'; break;
		case 's':case 'P':case'S': if (c != 'w')c = 's'; break;
		case 'w':case 'H':case'W': if (c != 's')c = 'w'; break;
		case ' ':cl = _getch(); break;					//空格暂停
		case 27:exit(0); break;							//Esc退出
		case '1':n_2 = '1'; break;
		case '2':n_2 = '2'; break;
		case '3':n_2 = '3'; break;
		case '4':n_2 = '4'; break;
		case '5':n_2 = '5'; break;
		case '6':n_2 = '6'; break;
		case '0':n_2 = '0'; break;

		}
	switch (c)						//移动后蛇头坐标的变化
	{
	case 'a':
	{
		if (hX > 0) {
			hX -= 1;
			break;
		}
		else {
			hX = hX + Width - 1;	//穿墙时坐标补偿
			break;
		}
	}
	case 'd':
	{
		if (hX < Width - 1) {
			hX += 1;
			break;
		}
		else {
			hX = hX - Width + 1;	//穿墙时坐标补偿
			break;
		}
	}
	case 's':
	{
		if (hY < Width - 1) {
			hY += 1;
			break;
		}
		else {
			hY = hY - Width + 1;	//穿墙时坐标补偿
			break;
		}
	}
	case 'w':
	{
		if (hY > 0) {
			hY -= 1;
			break;
		}
		else {
			hY = hY + Width - 1;	//穿墙时坐标补偿
			break;
		}
	}
	}
}

//函数功能：吃食物判定与随机生成食物
void Food()
{
	if (map[hY * Width + hX] == -1)			//判断是否吃到食物
	{
		len++;								//蛇长+1
		do {
			i = rand() % (Width * Width);
		} while (map[i] != 0);				//防止食物位置生成在蛇身或墙上
		sprintf(deaw, "title 得分:%d", len - 4);
		system(deaw);						//分数+1
		map[i] = -1;
		map_temp[i] = 1;
	}
	else for (i = 0; i < Width * Width; i++)//没吃到食物，全部蛇身值-1
		if (map[i] > 0)
		{
			map[i] -= 1;
			map_temp[i] = 1;
		}
}

//函数功能：打印画面
void Print_Map()
{
	for (i = 0; i < Width * Width * 2; i++)							//更新绘制缓存
	{
		if (map_temp[i / 2] == 1 && map[i / 2] == 0)				//打印空地
		{
			Hide_Move_Cursor(i % (Width * 2), i / (Width * 2));
			SetConsoleColor(240);
			printf(" ");
		}
		else if (map_temp[i / 2] == 1 && map[i / 2] > 0)			//打印蛇身
		{
			Hide_Move_Cursor(i % (Width * 2), i / (Width * 2));
			SetConsoleColor(192);
			printf(" ");
		}
		else if (map_temp[i / 2] == 1 && map[i / 2] == -2)			//打印墙
		{
			Hide_Move_Cursor(i % (Width * 2), i / (Width * 2));
			SetConsoleColor(130);
			printf(" ");
		}
		else if (map_temp[i / 2] == 1 && map[i / 2] == -1)			//打印食物
		{
			Hide_Move_Cursor(i % (Width * 2), i / (Width * 2));
			SetConsoleColor(160);
			printf(" ");
		}
	}
}

//函数功能：将光标移至指定位置并隐藏
void Hide_Move_Cursor(int x, int y)
{
	CONSOLE_CURSOR_INFO cursorInfo = { 25 };		//隐藏控制台光标
	COORD pos = { 0 };								//将光标移到初始处
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);	//获取标准输出设备句柄
	pos.X = x; pos.Y = y;
	SetConsoleCursorInfo(hOut, &cursorInfo);		//设置控制台光标信息
	SetConsoleCursorPosition(hOut, pos);			//定位光标位置
}

//函数功能：设置颜色
BOOL SetConsoleColor(unsigned int wAttributes)
{
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);		//获取标准输出设备句柄
	if (hOut == INVALID_HANDLE_VALUE)					//判断是否获取到标准输出设备
	{
		return FALSE;
	}
	return SetConsoleTextAttribute(hOut, wAttributes);	//返回光标颜色设置
}

//函数功能：游戏结束时防止误触看不到分数
int  GameOver()
{
	for (; 1;)
	{
		j = _getch();
		if (j == ' ' || j == '0')return 1;				//输入空格或0重新开始
		else if (j == 27)return 0;						//输入Esc（码值27）退出
		else return GameOver();
	}
}
