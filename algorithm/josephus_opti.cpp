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
		//������β����ӽ��
		prev->next = curr;
		prev = curr;
	}

	//�γɻ�·
	curr->next = head;
	return curr; //changed:����β���
}

void run(int total, int tag)
{
	//ԭ����֮������Ҫ���������Ϊprev����û�и�ֵ
	//�Ľ���ʽΪ�ȶ�prev��ֵ�����ҵ�head��ǰ����㣬����ʹ��˫������
	//����ǵ����������޸��¹��컷�Ĺ��̣�ʹ֮����β���
	LinkNodePtr prev = createCycle(total); //changed:β�����Ϊprev
	LinkNodePtr node = prev->next; //ͷ���Ϊβ����next

	int start = 1;
	int index = start;

	while (node->next != node) //changed: �������һ�����
	{
		if (index == tag)
		{
			printf("%d\n", node->value);
			//changed:������Ҫ�����
			//prev�Ѿ�����ֵ���ý�prev->nextָ��ǰ������һ���ڵ㣬���൱��������ǰ���
			prev->next = node->next;
			//�Ѿ��������һ����㣬���������п���
			node->next = NULL;
			node = prev->next;
			index = start; //���¿�ʼ����
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



