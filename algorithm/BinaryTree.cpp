#include <iostream>
#include <assert.h>
#include <stdlib.h>

using namespace std;

struct BinaryTreeNode
{
	int m_nValue;
	struct BinaryTreeNode *m_pLeft;
	struct BinaryTreeNode *m_pRight;
	struct BinaryTreeNode *m_pParent;
};

BinaryTreeNode *ConstructCore(BinaryTreeNode *parent, int *startPreorder, int *endPreorder, int *startInorder, int *endInorder)
{
	//first node of preorder traversal is root node
	int rootValue = startPreorder[0];
	BinaryTreeNode *root = new BinaryTreeNode();
	root->m_nValue = rootValue;
	root->m_pParent = parent;
	root->m_pLeft = root->m_pRight = NULL;

	if (startPreorder == endPreorder) //preorder traversal has finished
	{
		//remain only one node, inorder traversal should also finished, and node value should be same
		//check this assertion
		if (startInorder == endInorder && *startPreorder == *startInorder)
		{
			return root;
		}
		else
		{
			cout << "Invalid input" << endl;
			exit(-1);
		}
	}
	//find the root node in inorder traversal
	int *rootInorder = startInorder; //pointer to root node in inorder traversal
	while (rootInorder <= endInorder && *rootInorder != rootValue)
	{
		++rootInorder;
	}
	if (rootInorder > endInorder) //not found node that equal to root node
	{
		cout << "Invalid input" << endl;
		exit(-1);
	}

	int leftLength = rootInorder - startInorder; //boundary
	int *leftPreorderEnd = startPreorder + leftLength;

	if (leftLength > 0)
	{
		root->m_pLeft = ConstructCore(root, startPreorder + 1, leftPreorderEnd, startInorder, rootInorder - 1);
	}
	if (leftLength < (endPreorder - startPreorder))
	{
		root->m_pRight = ConstructCore(root, leftPreorderEnd + 1, endPreorder, rootInorder + 1, endInorder);
	}

	return root;
}

BinaryTreeNode *Construct(int *preorder, int *inorder, int length)
{
	if (preorder == NULL || inorder == NULL || length <= 0) {
		return NULL;
	}
	return ConstructCore(NULL, preorder, preorder + length - 1, inorder, inorder + length - 1);
}

BinaryTreeNode *leftMostChild(BinaryTreeNode *current)
{
	if (current == NULL)
	{
		return NULL;
	}
	while (current->m_pLeft != NULL)
	{
		current = current->m_pLeft;
	}
	return current;
}

BinaryTreeNode *Successor(BinaryTreeNode *current)
{
	assert(current != NULL);
	BinaryTreeNode *p;
	// CASE 1: have right child
	if (current->m_pParent == NULL || current->m_pRight != NULL)
	{
		p = leftMostChild(current->m_pRight);
	}
	else
	{
		// CASE 2: does not have right child, find lowest parent node that has left child
		while ((p = current->m_pParent) != NULL)
		{
			if (p->m_pLeft == current)
			{
				break;
			}
			current = p;
		}
	}
	return p;
}


int main()
{
	int preOrder[] = { 5, 4, 8, 9, 6, 3, 18, 19, 2 };
	int inOrder[] = { 9, 8, 6, 3, 4, 5, 19, 18, 2 };
	BinaryTreeNode* result;


	BinaryTreeNode *parent = Construct(preOrder, inOrder, sizeof(inOrder) / sizeof(inOrder[0]));
	cout << "the successor of root node is:\r\n";

	cout << (Successor(parent))->m_nValue << endl;
	cout << (Successor(parent->m_pLeft))->m_nValue << endl;
	cout << (Successor(parent->m_pLeft->m_pLeft))->m_nValue << endl;
	cout << (Successor(parent->m_pRight))->m_nValue << endl;
	result = Successor(parent->m_pRight->m_pRight);
	if (result == NULL)
	{
		cout << "This is the end of inorder in the tree, it haven't successor" << endl;
	}
	else
	{
		cout << result->m_nValue << endl;
	}
	return 0;
}