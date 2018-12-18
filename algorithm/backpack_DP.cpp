#include <stdio.h>

typedef struct
{
	int weight;
	int value;
} Object;

Object* array; //��Ʒ�б�
int num; //��Ʒ����
int container; //��������
int **dynamic_table; //��̬�滮��
bool *used_table; //ѡ���־��

void print_dynamic_table() {
	printf("dynamic table:\n");
	for (int i = 1; i <= num; i++)
	{
		for (int j = 1; j <= container; j++)
		{
			printf("%d ", dynamic_table[i][j]);
		}
		printf("\n");
	}
}

void print_array() {
	for (int i = 1; i <= num; i++)
	{
		printf("��%d����Ʒ�������ͼ�ֵ�ǣ�%d %d\n", i, array[i].weight, array[i].value);
	}
}

void print_used_object() {
	printf("��ʹ�õ���Ʒ������ʾ��\n");
	for (int i = 1; i <= num; i++)
	{
		if (used_table[i] == 1)
		{
			printf("%d - %d\n", array[i].weight, array[i].value);
		}
	}
}

void init_problem() {
	printf("Backpack capacity:\n");
	scanf("%d", &container);
	printf("Number of goods in Backpack:\n");
	scanf("%d", &num);
	array = new Object[num + 1]; //��1��Ԫ��Ϊ��Ԫ��
	printf("Input good:\n");
	for (int i = 1; i <= num; i++)
	{
		char c;
		scanf("%d%c%d", &array[i].weight, &c, &array[i].value);
	}
	print_array();
}

//ʹ���𼶵��Ƶķ����Ƴ�����������Ǵ洢��������
void trace_back() {
	int weight = container;
	used_table = new bool[num + 1];
	//initial the used_table to be non-used
	for (int i = 0; i <= num; i++)
	{
		used_table[i] = 0;
	}
	//����һ������б仯˵����ǰ��Ʒ��������ͬʱ�����Ϊ�ӱ�������
	for (int j = num; j > 0; j--)
	{
		if ((weight > 0) && (dynamic_table[j][weight] != dynamic_table[j - 1][weight]))
		{
			weight -= array[j].weight;
			used_table[j] = 1;
		}
	}
}

void dynamic_programming() {
	//�����ڴ�,�������ա���dummy head���������ա�����Ϊ[k - temp_weight]����Ϊ0
	//table�Ĵ�СΪ(num + 1) * (container + 1)
	dynamic_table = new int*[num + 1];
	for (int k = 0; k <= num; k++)
	{
		dynamic_table[k] = new int[container + 1];
	}
	//����ʼ��Ϊ0
	for (int m = 0; m <= num; m++)
	{
		for (int n = 0; n <= container; n++)
		{
			dynamic_table[m][n] = 0;
		}
	}
	//�����
	int temp_weight;
	int temp_value;
	for (int j = 1; j <= num; j++)
	{
		temp_weight = array[j].weight;
		temp_value = array[j].value;
		for (int k = 1; k <= container; k++)
		{
			if (k >= temp_weight && dynamic_table[j - 1][k] < dynamic_table[j - 1][k - temp_weight] + temp_value)
			{
				dynamic_table[j][k] = dynamic_table[j - 1][k - temp_weight] + temp_value;
			}
			else
			{
				dynamic_table[j][k] = dynamic_table[j - 1][k];
			}
		}
		print_dynamic_table();
	}
}

int main() {
	init_problem();
	dynamic_programming();
	trace_back();
	print_used_object();
}