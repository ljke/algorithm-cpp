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
	return curr; //changed:返回尾结点
}

void run(int total, int tag)
{
	//原程序之所以需要分情况是因为prev可能没有赋值
	//改进方式为先对prev赋值，即找到head的前驱结点，可以使用双向链表
	//如果是单向链表，就修改下构造环的过程，使之返回尾结点
	LinkNodePtr prev = createCycle(total); //changed:尾结点作为prev
	LinkNodePtr node = prev->next; //头结点为尾结点的next

	int start = 1;
	int index = start;

	while (node->next != node) //changed: 留下最后一个结点
	{
		if (index == tag)
		{
			printf("%d\n", node->value);
			//changed:不再需要分情况
			//prev已经被赋值，用将prev->next指向当前结点的下一个节点，，相当于跳过当前结点
			prev->next = node->next;
			//已经完成跳过一个结点，下面这句可有可无
			node->next = NULL;
			node = prev->next;
			index = start; //重新开始计数
		}
		else
		{
			prev = node;
			node = node->next;
			index++;
		}
	}
	printf("left:%d\n", node->value);
}

int main()
{
	run(5, 2);
	return 0;
}



