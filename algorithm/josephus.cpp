#include <iostream>

using namespace std;

typedef struct _LinkNode
{
	int value;
	struct _LinkNode *next;
}LinkNode, *LinkNodePtr;

LinkNodePtr createCycle(int total)
{
	int index = 1;
	LinkNodePtr head = NULL, curr = NULL, prev = NULL;
	head = (LinkNodePtr)malloc(sizeof(LinkNode));
	head->value = index;
	prev = head;

	while (--total > 0)
	{
		curr = (LinkNodePtr)malloc(sizeof(LinkNode));
		curr->value = ++index;
		//在链表尾部添加结点
		prev->next = curr;
		prev = curr;
	}

	//形成环路
	curr->next = head;
	return head;
}

void run(int total, int tag)
{
	LinkNodePtr node = createCycle(total);
	LinkNodePtr prev = NULL;

	int start = 1;
	int index = start;

	while (node && node->next)
	{
		if (index == tag)
		{
			printf("%d\n", node->value);
			if (tag == start)
			{
				//tag == 1 的特殊情况
				//prev还未赋值，直接给prev赋值下一个结点
				prev = node->next;
				//下面这句会导致循环链表断裂，但是因为相当于逐个删除，所以不影响
				node->next = NULL;
				node = prev;
			}
			else
			{
				//tag != 1 的一般情况
				//prev已经被赋值，用将prev->next指向当前结点的下一个节点，，相当于跳过当前结点
				prev->next = node->next;
				//已经完成跳过一个结点，下面这句可有可无
				node->next = NULL; 
				node = prev->next;
			}
			index = start; //重新开始计数
		}
		else
		{
			prev = node;
			node = node->next;
			index++;
		}
	}
}

int main() 
{
	run(5, 3);
	return 0;
}



