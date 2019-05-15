/*	IDA*�㷨���FLOODit����
*/
#include <iostream>
#include <string.h>
#include <vector>
#include <random>
#include <time.h>
#include <ctime>
using namespace std;

#define SumColor 6//��Ϸ�з������ɫ����
#define Size 6//��Ϸ�еĳ��ȺͿ�ȡ�

void DFS(int px, int py, int color);
bool IDAStar(int HMax);
int GetH();
void CreateImg();		//�������ͼ��
int RandomColor();	 //�������һ����ɫ
bool expand(int color, bool update=false);

vector <int> BestColors;	//�����¼һ������·��

/*int img[Size][Size] = { 1,4,3,4,2 ,2 ,3,1, 3,1,
		4, 0, 3 ,2 ,2 ,1 ,4 ,4 ,1 ,3,
			1 ,3 ,2, 0 ,2 ,2 ,3, 3, 0 ,4,
4, 2 ,3 ,2 ,4 ,4 ,4 ,0, 3 ,0,
3 ,1 ,4 ,3 ,0 ,2 ,1, 1, 2, 0,
3, 2 ,4, 3, 1, 0 ,3 ,3, 1 ,0,
0 ,0 ,4 ,1 ,4, 4, 4 ,2 ,4, 3,
1 ,0 ,0, 4, 3, 3, 1 ,2 ,1, 3,
3 ,3 ,2, 4, 2 ,4, 1, 4, 3, 2,
2, 0 ,0 ,3, 3 ,3, 2 ,1 ,2, 3};*/

int img[Size][Size];
int States[Size][Size];	// 1���������Ͻ���ɫ��ͬ ��2 ������1���� �� 3����ʣ���
// ����
int DisX[4] = { 0,1,0,-1 };
int DisY[4] = { 1,0,-1,0 };


int main()
{

	time_t begin = time(0);

	srand((unsigned)time(0));
	CreateImg();
	/*��ʾ��Ϸ��ʼ״̬*/
	for (int i = 0; i < Size; i++)
	{
		for (int j = 0; j< Size; j++)
		{
			cout << img[i][j]<<" ";

		}
		cout << endl;
	}
	memset(States, 0,sizeof(States));
	// �Ե�ǰ��Ϸ״̬���м���
	DFS(0, 0, img[0][0]);
	//�õ���ʼʱ������������
	int HMax = GetH();
	BestColors.clear();
	while (!IDAStar(HMax))
	{
		cout << "�������Ϊ" << HMax <<"�����ҵ����н�"<<endl;
		HMax++;
		BestColors.clear();
	}
	cout << "���Žⲽ��Ϊ:" << HMax<<"��"<<endl;
	int length = BestColors.size();
	memset(States, 0, sizeof(States));
	DFS(0, 0, img[0][0]);
	//ģ�����̵ĸ���
	for (int i = length - 1; i >= 0; i--)
	{
		cout << "ѡ��:"<<BestColors[i] << endl;
		cout << "����" << endl;
		expand(BestColors[i], true);
		for (int i = 0; i < Size; i++)
		{
			for (int j = 0; j < Size; j++)
			{
				cout << img[i][j] << " ";

			}
			cout << endl;
		}
	}
	time_t end = time(0);
	cout << "����ʱΪ" << end - begin << "S" << endl;

	getchar();
	return 0;
}
void DFS(int x,int y,int color)
{
	States[x][y] = 1;
	for (int i = 0; i < 4; i++)
	{
		int px = x + DisX[i];
		int py = y + DisY[i];
		//�ж������Ƿ����
		if ((0 <= px &&px<Size) && (0 <= py && py < Size))
		{
			if (States[px][py]!= 1)
			{
				if (img[px][py]==color)
				{
					DFS(px, py, img[px][py]);
				}
				else
				{
					States[px][py] = 2;
				}
			}
		}
	}
}
// ͨ��һ���ݹ���ã�ʵ��IDA*�㷨
bool IDAStar(int HMax)
{

	if (HMax < GetH())
	{
		return false;
	}
	if (GetH() == 0)
	{
		return true;
	}
	for (int color = 0; color < SumColor; color++)
	{
		int memory[Size][Size];

		memcpy(memory, States, sizeof(States));

		//������µ���չ�ڵ㣬
		if (!expand(color))
		{
			continue;
		}
		if (IDAStar(HMax - 1))
		{
			BestColors.push_back(color);
			return true;
		}
		//������µ���չ�ڵ㣬�����մﵽ������ȴ�����ҵ��������,�ָ�ԭ��״̬
		memcpy(States, memory, sizeof(memory));

	}

	return false;

}
/*ģ�����һ�β���*/
bool expand(int color,bool update)
{
	bool back = false;
	for (int px = 0; px < Size; px++)
	{
		for (int py = 0; py < Size; py++)
		{	//��������ڽڵ��з�������չ��ɫ��ͬ����ı���״̬�����������DFS
			if ((States[px][py] == 2) && (img[px][py] == color))
			{
				DFS(px, py, color);
				back = true;
			}
		}
	}
	if (update)
	{
		for (int px = 0; px < Size; px++)
		{
			for (int py = 0; py < Size; py++)
			{
				if (States[px][py] == 1)
				{
					img[px][py] = color;
				}

			}
		}

	}
	//����img�е����ݽ��иı䣬�����ô���չ������·���ϵĽ⣬��Ҫ���ظ��ڵ�״̬�����ڴ˲���img�����޸ģ�
	return back;
}
/*�õ���ǰcost��ֵ*/
int GetH()
{	//H(n) = States״̬��Ϊ1��ʣ����ɫ������
	int MarkedColor[SumColor];
	memset(MarkedColor, 0, SumColor * sizeof(int));
	int num = 0;
	for (int i = 0; i < Size; i++)
	{
		for (int j = 0; j < Size; j++)
		{
			if (States[i][j] != 1)
			{
				if (MarkedColor[img[i][j]] == 0)
				{
					MarkedColor[img[i][j]] = 1;
					num++;
				}
				else
				{
					continue;
				}
			}
		}
	}

	return num;
}

void CreateImg()
{
	for (int i = 0; i < Size; i++)
	{
		for (int j = 0; j < Size; j++)
		{
			img[i][j] = RandomColor();
		}
	}
}

int RandomColor()
{
	int color = rand()%SumColor;
	return color;
}
