#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "binary_tree.h"

#define SIZE(x) (sizeof(x) / sizeof(x[0]))

int main() {
	{
		printf("Testing addition - ");

		struct Tree* tree = NULL;

		add(&tree, 5);

		add(&tree, 7);

		assert(tree != NULL);

		destroyTree(tree);

		printf("\t Done\n\n");
	}

	{
		printf("Testing empty tree - ");

		struct Tree* empty_tree = NULL;

		assert(getHeight(empty_tree) == 0);

		assert(countNodes(empty_tree) == 0);

		assert(find(empty_tree, 5) == NULL);

		destroyTree(empty_tree);

		printf("\t Done\n\n");
	}

	{
		printf("Testing single node tree - ");

		int data = 5;

		struct Tree* single_node_tree = NULL;

		add(&single_node_tree, data);

		assert(getHeight(single_node_tree) == 1);

		assert(countNodes(single_node_tree) == 1);

		assert(find(single_node_tree, data) == single_node_tree);

		assert(find(single_node_tree, 42) == NULL);

		destroyTree(single_node_tree);

		printf("\t Done\n\n");
	}

	{
		printf("Testing tree resembling a list - ");

		int data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

		struct Tree* degenerate_tree = NULL;

		for (size_t i = 0; i < SIZE(data); i++) {
			add(&degenerate_tree, data[i]);
		}

		assert(getHeight(degenerate_tree) == 10);

		assert(countNodes(degenerate_tree) == 10);

		assert(find(degenerate_tree, 5) != NULL);

		assert(find(degenerate_tree, 42) == NULL);

		destroyTree(degenerate_tree);

		printf("\t Done\n\n");

	}

	{
		printf("Testing balanced tree - ");

		int data[] = { 16, 8, 4, 12, 24, 20, 28 };

		struct Tree* balanced_tree = NULL;

		for (size_t i = 0; i < SIZE(data); i++) {
			add(&balanced_tree, data[i]);
		}

		assert(getHeight(balanced_tree) == 3);

		assert(countNodes(balanced_tree) == 7);

		assert(find(balanced_tree, 28) != NULL);

		assert(find(balanced_tree, 42) == NULL);

		destroyTree(balanced_tree);

		printf("\t Done\n\n");

	}
}
