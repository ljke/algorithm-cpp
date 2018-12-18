#include <stdio.h>

typedef struct
{
	int weight;
	int value;
} Object;

Object* array; //物品列表
int num; //物品数量
int container; //背包容量
int **dynamic_table; //动态规划表
bool *used_table; //选择标志表

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
		printf("第%d个物品的重量和价值是：%d %d\n", i, array[i].weight, array[i].value);
	}
}

void print_used_object() {
	printf("所使用的物品如下所示：\n");
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
	array = new Object[num + 1]; //第1个元素为空元素
	printf("Input good:\n");
	for (int i = 1; i <= num; i++)
	{
		char c;
		scanf("%d%c%d", &array[i].weight, &c, &array[i].value);
	}
	print_array();
}

//使用逐级倒推的方法推出结果，而不是存储决策数组
void trace_back() {
	int weight = container;
	used_table = new bool[num + 1];
	//initial the used_table to be non-used
	for (int i = 0; i <= num; i++)
	{
		used_table[i] = 0;
	}
	//与上一行相比有变化说明当前物品被包含，同时问题变为子背包问题
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
	//分配内存,首行留空——dummy head，首列留空——因为[k - temp_weight]可能为0
	//table的大小为(num + 1) * (container + 1)
	dynamic_table = new int*[num + 1];
	for (int k = 0; k <= num; k++)
	{
		dynamic_table[k] = new int[container + 1];
	}
	//表格初始化为0
	for (int m = 0; m <= num; m++)
	{
		for (int n = 0; n <= container; n++)
		{
			dynamic_table[m][n] = 0;
		}
	}
	//填充表格
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