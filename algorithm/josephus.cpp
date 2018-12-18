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
				//tag == 1 ���������
				//prev��δ��ֵ��ֱ�Ӹ�prev��ֵ��һ�����
				prev = node->next;
				//�������ᵼ��ѭ��������ѣ�������Ϊ�൱�����ɾ�������Բ�Ӱ��
				node->next = NULL;
				node = prev;
			}
			else
			{
				//tag != 1 ��һ�����
				//prev�Ѿ�����ֵ���ý�prev->nextָ��ǰ������һ���ڵ㣬���൱��������ǰ���
				prev->next = node->next;
				//�Ѿ��������һ����㣬���������п���
				node->next = NULL; 
				node = prev->next;
			}
			index = start; //���¿�ʼ����
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



