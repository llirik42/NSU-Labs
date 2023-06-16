#include "singly_linked_list.h"

#define N 10

int main() {
	struct SinglyLinkedList* list = createNewList();

	for (int i = 0; i < N; i++) {
		append(list, i);
	}

	printList(list);

	freeList(list);
}
