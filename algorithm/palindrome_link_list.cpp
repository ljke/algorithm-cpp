#include <stdio.h>
#include <stdlib.h>
/************************************************************************/
/* ˫����������Ĵ�������˼·��
   1. ���м��㿪ʼ����
   2. ��ǰ�����˿�ʼ����*/
   /************************************************************************/

typedef struct _LinkNode
{
	int value;
	struct _LinkNode *prev;
	struct _LinkNode *next;
}LinkNode, *LinkNodePtr;

//����һ�����м��㿪ʼ����
//int createLinkList(char *pstr, LinkNodePtr head)
//{
//	LinkNodePtr cur = NULL, prev = NULL;
//	int count = 0;
//	//�����ں����ڸ�ֵ��������൱��ָ����һ����ַ������Ӱ�쵽ԭ���ĵ�ַ
//	//head = (LinkNodePtr)malloc(sizeof(LinkNode));
//	head->value = *pstr;
//	head->prev = NULL;
//	prev = head;
//	count++;
//
//	while (*(++pstr))
//	{
//		//�ȸ�ֵvalue and prev��next���´�ѭ���и�ֵ
//		cur = (LinkNodePtr)malloc(sizeof(LinkNode));
//		cur->value = *pstr;
//		cur->prev = prev;
//		prev->next = cur;
//		prev = cur;
//		count++;
//	}
//
//	prev->next = NULL; //β���nextָ��NULL
//	return count;
//}
//
//
//int main()
//{
//	char pstr[30];
//	printf("Input the str:");
//	scanf_s("%s", pstr, 30); //ʹ�ð�ȫ��scanf_s��ȡstdin
//	if (!(*pstr))
//	{
//		printf("Invalid Input!!!\n");
//		return 0;
//	}
//	LinkNodePtr ptr_head = (LinkNodePtr)malloc(sizeof(LinkNode));
//	int count = createLinkList(pstr, ptr_head);
//	////ʹ�ÿ���ָ���ҵ��м���
//	//LinkNodePtr fast_ptr = ptr_head, slow_ptr = ptr_head;
//	//LinkNodePtr pre_ptr, next_ptr;
//	////������ż
//	//if (count % 2 == 1)
//	//{
//	//	while (fast_ptr->next)
//	//	{
//	//		slow_ptr = slow_ptr->next;
//	//		fast_ptr = fast_ptr->next->next;
//	//	}
//	//	pre_ptr = slow_ptr;
//	//	next_ptr = slow_ptr;
//
//	//}
//	//else
//	//{
//	//	while (fast_ptr->next->next)
//	//	{
//	//		slow_ptr = slow_ptr->next;
//	//		fast_ptr = fast_ptr->next->next;
//	//	}
//	//	pre_ptr = slow_ptr;
//	//	next_ptr = pre_ptr->next;
//
//	//}
//	//ֱ��ʹ��count�ҵ��м���
//	LinkNodePtr pre_ptr = ptr_head, next_ptr = ptr_head;
//	int i = count / 2;
//	if (count % 2 == 1)
//	{
//		for (int j = 0; j < i; j++)
//		{
//			pre_ptr = pre_ptr->next;
//			next_ptr = next_ptr->next;
//		}
//	}
//	else 
//	{
//		for (int j = 0; j < i - 1; j++)
//		{
//			pre_ptr = pre_ptr->next;;
//			next_ptr = next_ptr->next;
//		}
//		next_ptr = next_ptr->next;
//	}
//
//	//���м��㷴�����
//	while ((pre_ptr != NULL) && (next_ptr != NULL))
//	{
//		printf("%c compare %c\n", pre_ptr->value, next_ptr->value);
//		if (pre_ptr->value != next_ptr->value)
//		{
//			break;
//		}
//		pre_ptr = pre_ptr->prev;
//		next_ptr = next_ptr->next;
//	}
//	if ((pre_ptr == NULL) && (next_ptr == NULL))
//	{
//		printf("%s is palindrome!!!\n", pstr);
//	}
//	else
//	{
//		printf("%s is not palindrome!!!\n", pstr);
//	}
//
//	return 0;
//}


//����������ǰ�����˿�ʼ����
//����head������tail
LinkNodePtr createLinkList(char *pstr, LinkNodePtr head)
{
	LinkNodePtr cur = NULL, prev = NULL;
	//int count = 0;
	//�����ں����ڸ�ֵ��������൱��ָ����һ����ַ������Ӱ�쵽ԭ���ĵ�ַ
	//head = (LinkNodePtr)malloc(sizeof(LinkNode));
	head->value = *pstr;
	head->prev = NULL;
	prev = head;
	//count++;

	while (*(++pstr))
	{
		//�ȸ�ֵvalue and prev��next���´�ѭ���и�ֵ
		cur = (LinkNodePtr)malloc(sizeof(LinkNode));
		cur->value = *pstr;
		cur->prev = prev;
		prev->next = cur;
		prev = cur;
		//count++;
	}

	prev->next = NULL; //β���nextָ��NULL
	return prev;
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
	LinkNodePtr ptr_head = (LinkNodePtr)malloc(sizeof(LinkNode));
	LinkNodePtr ptr_tail = createLinkList(pstr, ptr_head);

	//�����߿�ʼ����
	bool border; //��ż�ֱ��Ӧһ�ֱ߽����
	do{
		printf("%c compare %c\n", ptr_head->value, ptr_tail->value);
		if (ptr_head->value != ptr_tail->value)
		{
			break;
		}
		border = (ptr_head->next == ptr_tail); //ż�������ı߽����
		border = (ptr_head == ptr_tail) || border; //���������ı߽����
		ptr_head = ptr_head->next;
		ptr_tail = ptr_tail->prev;
	} while (!border);
	if (border)
	{
		printf("%s is palindrome!!!\n", pstr);
	}
	else
	{
		printf("%s is not palindrome!!!\n", pstr);
	}

	return 0;
}