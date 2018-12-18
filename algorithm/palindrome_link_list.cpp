#include <stdio.h>
#include <stdlib.h>
/************************************************************************/
/* 双向链表检测回文串，两种思路：
   1. 从中间结点开始遍历
   2. 从前后两端开始遍历*/
   /************************************************************************/

typedef struct _LinkNode
{
	int value;
	struct _LinkNode *prev;
	struct _LinkNode *next;
}LinkNode, *LinkNodePtr;

//方法一：从中间结点开始遍历
//int createLinkList(char *pstr, LinkNodePtr head)
//{
//	LinkNodePtr cur = NULL, prev = NULL;
//	int count = 0;
//	//不能在函数内赋值，否则就相当于指向另一个地址，不会影响到原来的地址
//	//head = (LinkNodePtr)malloc(sizeof(LinkNode));
//	head->value = *pstr;
//	head->prev = NULL;
//	prev = head;
//	count++;
//
//	while (*(++pstr))
//	{
//		//先赋值value and prev，next在下次循环中赋值
//		cur = (LinkNodePtr)malloc(sizeof(LinkNode));
//		cur->value = *pstr;
//		cur->prev = prev;
//		prev->next = cur;
//		prev = cur;
//		count++;
//	}
//
//	prev->next = NULL; //尾结点next指向NULL
//	return count;
//}
//
//
//int main()
//{
//	char pstr[30];
//	printf("Input the str:");
//	scanf_s("%s", pstr, 30); //使用安全的scanf_s读取stdin
//	if (!(*pstr))
//	{
//		printf("Invalid Input!!!\n");
//		return 0;
//	}
//	LinkNodePtr ptr_head = (LinkNodePtr)malloc(sizeof(LinkNode));
//	int count = createLinkList(pstr, ptr_head);
//	////使用快慢指针找到中间结点
//	//LinkNodePtr fast_ptr = ptr_head, slow_ptr = ptr_head;
//	//LinkNodePtr pre_ptr, next_ptr;
//	////区分奇偶
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
//	//直接使用count找到中间结点
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
//	//从中间结点反向遍历
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


//方法二：从前后两端开始遍历
//传入head，传出tail
LinkNodePtr createLinkList(char *pstr, LinkNodePtr head)
{
	LinkNodePtr cur = NULL, prev = NULL;
	//int count = 0;
	//不能在函数内赋值，否则就相当于指向另一个地址，不会影响到原来的地址
	//head = (LinkNodePtr)malloc(sizeof(LinkNode));
	head->value = *pstr;
	head->prev = NULL;
	prev = head;
	//count++;

	while (*(++pstr))
	{
		//先赋值value and prev，next在下次循环中赋值
		cur = (LinkNodePtr)malloc(sizeof(LinkNode));
		cur->value = *pstr;
		cur->prev = prev;
		prev->next = cur;
		prev = cur;
		//count++;
	}

	prev->next = NULL; //尾结点next指向NULL
	return prev;
}


int main()
{
	char pstr[30];
	printf("Input the str:");
	scanf_s("%s", pstr, 30); //使用安全的scanf_s读取stdin
	if (!(*pstr))
	{
		printf("Invalid Input!!!\n");
		return 0;
	}
	LinkNodePtr ptr_head = (LinkNodePtr)malloc(sizeof(LinkNode));
	LinkNodePtr ptr_tail = createLinkList(pstr, ptr_head);

	//从两边开始遍历
	bool border; //奇偶分别对应一种边界情况
	do{
		printf("%c compare %c\n", ptr_head->value, ptr_tail->value);
		if (ptr_head->value != ptr_tail->value)
		{
			break;
		}
		border = (ptr_head->next == ptr_tail); //偶数个结点的边界情况
		border = (ptr_head == ptr_tail) || border; //奇数个结点的边界情况
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