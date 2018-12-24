#include <stdio.h>
#include <stdlib.h>

// used in dump display
enum child_dir
{
	left_child,
	right_child,
	root
};

struct tree_node {
	unsigned long data;
	struct tree_node *left;
	struct tree_node *right;
};

struct tree_root {
	struct tree_node *r;
};

//in-order traverse???
void dump(tree_node *node, int level, enum child_dir dir) {
	//if (!node)
	//{
	//	return;
	//}
	//dump(node->left, level + 1, left_child);
	//if (dir == left_child) {
	//	printf("%*s\n", level * 3, "|");
	//}
	//printf("%*s - %05lu\n", level * 3, " ", node->data);
	//if (dir == right_child)
	//{
	//	printf("%*s\n", level * 3, "|");
	//}
	//dump(node->right, level + 1, right_child);
	if (!node)
		return;

	dump(node->right, level + 1, right_child);

	if (dir == left_child)
		printf("%*s\n", level * 3, "|");

	printf("%*s - %05lu\n", level * 3, " ", node->data);

	if (dir == right_child)
		printf("%*s\n", level * 3, "|");

	dump(node->left, level + 1, left_child);
}

struct tree_node* find_node(tree_root *root, unsigned long data) {
	struct tree_node *n = root->r;
	while (n)
	{
		if (data == n->data)
		{
			return n;
		}
		else if (data < n->data)
		{
			n = n->left;
		}
		else {
			n = n->right;
		}
	}

	return NULL;
}

struct tree_node* new_node(unsigned long data) {
	struct tree_node *n;
	n = (struct tree_node *)malloc(sizeof(tree_node));
	n->data = data;
	n->left = n->right = NULL;
	return n;
}

void insert_node(tree_root *root, unsigned long data) {
	struct tree_node *n = new_node(data);
	//insert root node
	if (root->r == NULL)
	{
		root->r = n;
		return;
	}
	struct tree_node *cur = root->r;
	while (true)
	{
		if (cur->data == data) 		//do not insert duplicate data
		{
			break;
		}
		else if (cur->data > data) {
			if (cur->left == NULL)
			{
				cur->left = n;
				break;
			}
			else {
				cur = cur->left;
			}
		}
		else if (cur->data < data)
		{
			if (cur->right == NULL)
			{
				cur->right = n;
				break;
			}
			else {
				cur = cur->right;
			}
		}
	}
}

struct tree_node* delete_node(struct tree_root *root, unsigned long data) {
	struct tree_node *n = root->r, **p = &root->r;
	struct tree_node *child;

	//find suitable position to insert
	while (n)
	{
		if (data < n->data)
		{
			p = &n->left;
			n = n->left;
		}
		else if (data > n->data) {
			p = &n->right;
			n = n->right;
		}
		else {
			break;
		}
	}
	//no found, return NULL
	if (!n) {
		return NULL;
	}
	//delete node in three cases, no narrative
	if (n->left && n->right)
	{
		struct tree_node *rn = n->right, **rp = &n->right;
		while (rn->left)
		{
			rp = &rn->left;
			rn = rn->left;
		}
		n->data = rn->data;
		n = rn;
		p = rp;
	}
	child = n->left ? n->left : n->right;
	*p = child;
	return n;
}


void insert_test(tree_root *tree)
{
	insert_node(tree, (unsigned long)9);
	insert_node(tree, (unsigned long)2);
	insert_node(tree, (unsigned long)5);
	insert_node(tree, (unsigned long)8);
	insert_node(tree, (unsigned long)18);
	insert_node(tree, (unsigned long)13);
	insert_node(tree, (unsigned long)21);
	insert_node(tree, (unsigned long)20);

	dump(tree->r, 0, root);

	struct tree_node* n;
	n = find_node(tree, (unsigned long)18);
	if (n && n->data == 18)
		printf("Get 18\n");
}

void delete_test(tree_root *tree) {
	tree_node *del_node;
	del_node = delete_node(tree, 20);
	if (del_node)
	{
		printf("Delete 20\n");
		free(del_node); //free deleted node
	}
	dump(tree->r, 0, root);

	del_node = delete_node(tree, 9);
	if (del_node)
	{
		printf("Delete 9\n");
		free(del_node);
	}
	dump(tree->r, 0, root);
}

int main() {
	struct tree_root tree;
	tree.r = NULL;
	insert_test(&tree);
	delete_test(&tree);
	return 0;
}