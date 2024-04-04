#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <Windows.h>
#include <time.h>
#include <conio.h>
#define Width 25								//�����ͼ��С���ɸģ���Ĭ��ֵ25����С15�����50
int hX = 1, hY = 1, len = 4;					//������ͷ���꣬�߳�
int* map = NULL;								//��¼��ͼ���飬ʳ��(-1)��ǽ(-2)���Լ�(>=1)���յ�(0)
int map_temp[Width * Width] = { 0 };			//��¼��Ҫ���ĵ���Ϸ��������
int c = 'd', cl = 'd', i = 0;					//���򣬷��򻺴棬ѭ������
char n_1, n_2 = '4', n_3, j = 0;				//n_1ѡ��ģʽ��n_2ѡ���Ѷ�,n_3Ϊ�Թ�ѡ��,jΪ�ַ�����
char deaw[Width * Width * 2 + 1] = { 0 };		//���ƻ���

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
	system(deaw);						//�޸Ŀ���̨���ڴ�С
	srand((unsigned)time(0));			//���������
	map = (int*)malloc((Width * Width) * sizeof(int));
	if (map == NULL)
	{
		printf("No enough memory!\n");
		exit(1);
	}
	do {								//������ɽ�����ɫ
		i = rand() % 6;
	} while (i == 3);
	sprintf(deaw, "color F%d", i);
	system(deaw);
	Menu();								//��ʼ���˵�
	system("cls");
	sprintf(deaw, "color 0%d", i);
	system(deaw);
	do {
		hX = 1, hY = 1, len = 4;		//��ʼ����ͼ������
		c = 'd', cl = 'd', i = 0;
		for (i = 0; i < Width * Width; i++)				//��ʼ����ͼΧǽ
		{
			map[i] = 0;
			map_temp[i] = 1;
		}
		Create_Map();									//���ɵ�ͼ
		Hide_Move_Cursor(0, 0);							//���������ָ��λ�ò�����
		Print_Map();									//��ӡ����
		if (n_1 == '1' || n_1 == '2' || (n_1 == '3' && n_3 == '1'))
		{
			hX = 1; hY = 1;
		}
		else if (n_1 == '3' && n_3 == '2')
		{
			hX = Width / 5;
			hY = (2 * Width / 5 + 2);
		}
		for (i = 0; i < Width * Width; i++)				//��ʼ����¼����
			map_temp[i] = 0;
		do {
			i = rand() % (Width * Width);
		} while (map[i] != 0);							//��ֹʳ��λ�������������ǽ��
		map[i] = -1;									//����ʳ��
		map_temp[i] = 1;								//�Ƿ����ɣ�1�ǡ�0��
		for (system("title �÷�:0"); 1; Sleep_time())	//��ʼ���Ʒ�
		{
			Move();										//�������ƶ�
			if (map[hY * Width + hX] > 1 ||				//�ж��Ƿ�Ե��Լ�
				map[hY * Width + hX] == -2) break;		//�ж��Ƿ�ײǽ
			Food();										//��ʳ���ж�������ʳ��
			map[hY * Width + hX] = len;					//��ͷ��ֵ
			map_temp[hY * Width + hX] = 1;
			Print_Map();								//��ӡ����
			for (i = 0; i < Width * Width; i++)			//����¼�����ʼ��
				map_temp[i] = 0;
		}
		sprintf(deaw, "title GameOver!�÷�:%d ���ո�����¿�ʼ", len - 4);
		system(deaw);				//��������
	} while (GameOver());			//����ո�������Ϸ������Esc�˳�
	Hide_Move_Cursor(Width * Width, Width);				//����ʱ�����������ͼ�·�
	free(map);
	return 0;
}

//�������ܣ����ɳ�ʼ�˵�
void Menu()
{
	while (1)
	{
		system("cls");							//����
		HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
		COORD pos[7] = { 0 };
		for (i = 0; i < 7; i++)					//��������
		{
			pos[i].X = (Width - 7);
			pos[i].Y = i * 2 + Width / 5;
		}
		SetConsoleCursorPosition(hOut, pos[0]);	//���ɳ�ʼ��Ļ
		printf("�ҵ�̰����");
		SetConsoleCursorPosition(hOut, pos[1]);
		printf("1.����ģʽ");
		SetConsoleCursorPosition(hOut, pos[2]);
		printf("2.����ģʽ");
		SetConsoleCursorPosition(hOut, pos[3]);
		printf("3.�Թ�ģʽ");
		SetConsoleCursorPosition(hOut, pos[4]);
		printf("4.��Ϸ˵��");
		SetConsoleCursorPosition(hOut, pos[5]);
		printf("5.�˳���Ϸ");
		SetConsoleCursorPosition(hOut, pos[6]);
		n_1 = getchar();						//����ѡ��
		while (getchar() != '\n');				//���߻������ַ�
		if (n_1 == '1' || n_1 == '2')break;
		else if (n_1 == '3')
		{
			system("cls");						//����
			SetConsoleCursorPosition(hOut, pos[0]);
			printf(" �Թ�ģʽ");
			SetConsoleCursorPosition(hOut, pos[1]);
			printf(" 1.�Թ�һ");
			SetConsoleCursorPosition(hOut, pos[2]);
			printf(" 2.�Թ���");
			SetConsoleCursorPosition(hOut, pos[3]);
			printf(" ");
			n_3 = getchar();					//����ѡ��
			while (getchar() != '\n');			//���߻������ַ�
			if (n_3 == '1' || n_3 == '2')break;
		}
		else if (n_1 == '4')
		{
			system("cls");						//����
			printf("\n");
			printf("  wasd(��WSAD)��������� �����ƶ�\n");
			printf("  �ո�   ������ͣ��Ϸ\n");
			printf("  Esc    �����˳���Ϸ\n");
			printf("  1234560��������\n");

			printf("  �밴�س������ؿ�ʼ�˵�!\n");
			while (getchar() != '\n');			//���߻������ַ�
		}
		else if (n_1 == '5') exit(0);
	}
}

//�������ܣ����ɵ�ͼ
void Create_Map()
{
	if (n_1 == '1')									//����ģʽ��ͼ
	{
		for (i = 0; i < Width; i++)map[i] = -2;		//��ռ����"()",���԰�2����Ϊһ��
		for (i = Width; i < Width * (Width - 1); i += Width)
			map[i] = map[i + Width - 1] = -2;		//ÿ�е�һ�������һ����Width-1
		for (i = 0; i < Width; i++)map[i + Width * (Width - 1)] = -2;
	}
	else if (n_1 == '3' && n_3 == '1')
	{
		for (i = 0; i < (Width - 1) / 2; i++)		//����ʮ���ǵ�ͼ
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
		if (Width % 2 == 0)							//ż����Ҫ�ټ�һ��һ�к�һ����
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
	else if (n_1 == '3' && n_3 == '2')				//�����Թ���
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

//�������ܣ������Ѷ�(�ӳ�ʱ��)
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

//�������ܣ��������ƶ�
void Move()
{
	if (_kbhit() && (cl = _getch()))					//�ж��Ƿ�����
		switch (cl)
		{
		case 'a':case 'K':case'A':if (c != 'd')c = 'a'; break;//�ж���ԭ�����Ƿ��ͻ
		case 'd':case 'M':case'D': if (c != 'a')c = 'd'; break;
		case 's':case 'P':case'S': if (c != 'w')c = 's'; break;
		case 'w':case 'H':case'W': if (c != 's')c = 'w'; break;
		case ' ':cl = _getch(); break;					//�ո���ͣ
		case 27:exit(0); break;							//Esc�˳�
		case '1':n_2 = '1'; break;
		case '2':n_2 = '2'; break;
		case '3':n_2 = '3'; break;
		case '4':n_2 = '4'; break;
		case '5':n_2 = '5'; break;
		case '6':n_2 = '6'; break;
		case '0':n_2 = '0'; break;

		}
	switch (c)						//�ƶ�����ͷ����ı仯
	{
	case 'a':
	{
		if (hX > 0) {
			hX -= 1;
			break;
		}
		else {
			hX = hX + Width - 1;	//��ǽʱ���겹��
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
			hX = hX - Width + 1;	//��ǽʱ���겹��
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
			hY = hY - Width + 1;	//��ǽʱ���겹��
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
			hY = hY + Width - 1;	//��ǽʱ���겹��
			break;
		}
	}
	}
}

//�������ܣ���ʳ���ж����������ʳ��
void Food()
{
	if (map[hY * Width + hX] == -1)			//�ж��Ƿ�Ե�ʳ��
	{
		len++;								//�߳�+1
		do {
			i = rand() % (Width * Width);
		} while (map[i] != 0);				//��ֹʳ��λ�������������ǽ��
		sprintf(deaw, "title �÷�:%d", len - 4);
		system(deaw);						//����+1
		map[i] = -1;
		map_temp[i] = 1;
	}
	else for (i = 0; i < Width * Width; i++)//û�Ե�ʳ�ȫ������ֵ-1
		if (map[i] > 0)
		{
			map[i] -= 1;
			map_temp[i] = 1;
		}
}

//�������ܣ���ӡ����
void Print_Map()
{
	for (i = 0; i < Width * Width * 2; i++)							//���»��ƻ���
	{
		if (map_temp[i / 2] == 1 && map[i / 2] == 0)				//��ӡ�յ�
		{
			Hide_Move_Cursor(i % (Width * 2), i / (Width * 2));
			SetConsoleColor(240);
			printf(" ");
		}
		else if (map_temp[i / 2] == 1 && map[i / 2] > 0)			//��ӡ����
		{
			Hide_Move_Cursor(i % (Width * 2), i / (Width * 2));
			SetConsoleColor(192);
			printf(" ");
		}
		else if (map_temp[i / 2] == 1 && map[i / 2] == -2)			//��ӡǽ
		{
			Hide_Move_Cursor(i % (Width * 2), i / (Width * 2));
			SetConsoleColor(130);
			printf(" ");
		}
		else if (map_temp[i / 2] == 1 && map[i / 2] == -1)			//��ӡʳ��
		{
			Hide_Move_Cursor(i % (Width * 2), i / (Width * 2));
			SetConsoleColor(160);
			printf(" ");
		}
	}
}

//�������ܣ����������ָ��λ�ò�����
void Hide_Move_Cursor(int x, int y)
{
	CONSOLE_CURSOR_INFO cursorInfo = { 25 };		//���ؿ���̨���
	COORD pos = { 0 };								//������Ƶ���ʼ��
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);	//��ȡ��׼����豸���
	pos.X = x; pos.Y = y;
	SetConsoleCursorInfo(hOut, &cursorInfo);		//���ÿ���̨�����Ϣ
	SetConsoleCursorPosition(hOut, pos);			//��λ���λ��
}

//�������ܣ�������ɫ
BOOL SetConsoleColor(unsigned int wAttributes)
{
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);		//��ȡ��׼����豸���
	if (hOut == INVALID_HANDLE_VALUE)					//�ж��Ƿ��ȡ����׼����豸
	{
		return FALSE;
	}
	return SetConsoleTextAttribute(hOut, wAttributes);	//���ع����ɫ����
}

//�������ܣ���Ϸ����ʱ��ֹ�󴥿���������
int  GameOver()
{
	for (; 1;)
	{
		j = _getch();
		if (j == ' ' || j == '0')return 1;				//����ո��0���¿�ʼ
		else if (j == 27)return 0;						//����Esc����ֵ27���˳�
		else return GameOver();
	}
}
