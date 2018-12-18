#include <iterator>
#include <iostream>
#include <vector>

using namespace std;
const int BUCKET_NUM = 10;

//每个桶内部用链表表示
struct ListNode
{
	//explicit指定构造函数为显式，不能使用隐式转换和复制初始化
	explicit ListNode(int i = 0) :mData(i), mNext(NULL) {}
	ListNode* mNext;
	int mData;
};

//入桶时插入排序
ListNode* insert(ListNode* head, int val) {
	ListNode dummyNode;
	ListNode *newNode = new ListNode(val);
	ListNode *pre, *curr;
	dummyNode.mNext = head;
	pre = &dummyNode;
	curr = head;
	//遍历到正确位置
	while (NULL != curr && curr->mData <= val)
	{
		pre = curr;
		curr = curr->mNext;
	}
	//插入
	newNode->mNext = curr;
	pre->mNext = newNode;
	return dummyNode.mNext;
}

//把各个桶中的数据合并
ListNode* Merge(ListNode *head1, ListNode *head2) {
	ListNode dummyNode;
	ListNode *dummy = &dummyNode;
	while (NULL != head1 && NULL != head2)
	{
		if (head1->mData <= head2->mData)
		{
			dummy->mNext = head1;
			head1 = head1->mNext;
		}
		else
		{
			dummy->mNext = head2;
			head2 = head2->mNext;
		}
		dummy = dummy->mNext;
	}
	if (NULL != head1)
	{
		dummy->mNext = head1;
	}
	if (NULL != head2)
	{
		dummy->mNext = head2;
	}
	return dummyNode.mNext;
}

void BucketSort(int n, int arr[]) {
	vector<ListNode*> buckets(BUCKET_NUM, (ListNode*)(0));
	for (int i = 0; i < n; i++)
	{
		int index = arr[i] / BUCKET_NUM;
		ListNode *head = buckets.at(index);
		buckets.at(index) = insert(head, arr[i]);
	}
	ListNode *head = buckets.at(0);
	for (int i = 1; i < BUCKET_NUM; ++i)
	{
		head = Merge(head, buckets.at(i));
	}
	for (int i = 0; i < n; ++i)
	{
		arr[i] = head->mData;
		head = head->mNext;
	}
}

void main() {
	printf("Initial:\n");
	int test[100] = { 0 };
	for (int i = 0; i < 100; i++) {
		test[i] = rand() % 100;
		printf("%d\t", test[i]);
		if ((i + 1) % 10 == 0) {
			printf("\n");
		}
	}
	BucketSort(100, test);
	printf("After:\n");
	for (int i = 0; i < 100; i++)
	{
		printf("%d\t", test[i]);
		if ((i + 1) % 10 == 0) {
			printf("\n");
		}
	}
	return;
}