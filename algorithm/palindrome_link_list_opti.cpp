#include <stdio.h>
#include <stdlib.h>
/************************************************************************/
/* 
������������Ĵ���
�����������ָ��,��ָ���ߵ��յ�ʱ��ָ���ߵ��м�
��ָ�����ǰ�У�����벿������ת
����ͷ�������Ա�ǰ�벿�ͺ�벿����
ע����ż������
*/
/************************************************************************/

typedef struct _LinkNode
{
	int value;
	struct _LinkNode *next;
}LinkNode, *LinkNodePtr;

LinkNodePtr createLinkList(char *pstr)
{
	LinkNodePtr head = NULL, cur = NULL, prev = NULL;
	//int count = 0;
	head = (LinkNodePtr)malloc(sizeof(LinkNode));
	head->value = *pstr;
	prev = head;
	//count++;

	while (*(++pstr))
	{
		//�ȸ�ֵvalue and prev��next���´�ѭ���и�ֵ
		cur = (LinkNodePtr)malloc(sizeof(LinkNode));
		cur->value = *pstr;
		prev->next = cur;
		prev = cur;
		//count++;
	}

	prev->next = NULL; //β���nextָ��NULL
	return head;
}

void printLinkList(LinkNodePtr head)
{
	while (head)
	{
		printf("[%c]\t", head->value);
		head = head->next;
	}
	printf("\n");
}


bool isPalindrome(LinkNodePtr head)
{
	LinkNodePtr fast = head, slow = head;
	while (fast && fast->next)
	{
		fast = fast->next->next;
		slow = slow->next;
	}
	LinkNodePtr prev = NULL, tmp = NULL;
	while (slow)
	{
		tmp = slow->next;
		slow->next = prev;
		prev = slow;
		slow = tmp;
	}
	bool rst = true;
	LinkNodePtr helper = prev;
	while (helper)
	{
		printf("%c compare %c\n", head->value, helper->value);
		if (head->value != helper->value)
		{
			rst = false;
			break;
		}
		head = head->next;
		helper = helper->next;
	}
	//�ָ�����
	slow = prev;
	prev = NULL;
	while (slow)
	{
		tmp = slow->next;
		slow->next = prev;
		prev = slow;
		slow = tmp;
	}
	return rst;
}


int main()
{
	char pstr[30];
	printf("Input the str:");
	scanf_s("%s", pstr, 30); //ʹ�ð�ȫ��scanf_s��ȡstdin
	if (!(*pstr))
	{
		printf("Invalid Input!!!\n");
		return 0;
	}
	LinkNodePtr ptr_head = createLinkList(pstr);
	printLinkList(ptr_head);
	bool result = isPalindrome(ptr_head);
	if (result)
	{
		printf("%s is palindrome!!!\n", pstr);
	}
	else
	{
		printf("%s is not palindrome!!!\n", pstr);
	}
	printLinkList(ptr_head);

	return 0;
}