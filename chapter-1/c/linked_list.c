#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char* value;
    //link to the previous node in list
    struct Node* previous;
    //link to the next node in the list
    struct Node* next;
} Node;

typedef struct {
    //first node
    Node* head;
    //last node
    Node* tail;
} List;

//set up an empty list with no nodes
void initializeList(List* list) {
    list->head = NULL;
    list->tail = NULL;
}

//make a heap copy of a C string
char* copyString(const char* source) {
    size_t length = strlen(source);
    char* copy = malloc(length + 1);

    if (copy == NULL) {
        return NULL;
    }

    memcpy(copy, source, length + 1);
    return copy;
}

//add a new string to the end of the list
bool insert(List* list, const char* value) {
    Node* node = malloc(sizeof(Node));

    if (node == NULL) {
        return false;
    }

    node->value = copyString(value);

    if (node->value == NULL) {
        free(node);
        return false;
    }

    node->previous = list->tail;
    node->next = NULL;

    if (list->tail == NULL) {
        list->head = node;
    } else {
        list->tail->next = node;
    }

    list->tail = node;
    return true;
}

//find the first node whose value matches the target string
Node* find(List* list, const char* value) {
    for (Node* current = list->head; current != NULL; current = current->next) {
        if (strcmp(current->value, value) == 0) {
            return current;
        }
    }

    return NULL;
}

//remove the first node with a matching value
bool deleteValue(List* list, const char* value) {
    Node* node = find(list, value);

    if (node == NULL) {
        return false;
    }

    if (node->previous == NULL) {
        list->head = node->next;
    } else {
        node->previous->next = node->next;
    }

    if (node->next == NULL) {
        list->tail = node->previous;
    } else {
        node->next->previous = node->previous;
    }

    free(node->value);
    free(node);
    return true;
}

void printForward(const List* list) {
    printf("Forward:");

    for (const Node* current = list->head; current != NULL; current = current->next) {
        printf(" %s", current->value);

        if (current->next != NULL) {
            printf(" <->");
        }
    }

    printf("\n");
}

void printBackward(const List* list) {
    printf("Backward:");

    for (const Node* current = list->tail; current != NULL; current = current->previous) {
        printf(" %s", current->value);

        if (current->previous != NULL) {
            printf(" <->");
        }
    }

    printf("\n");
}

//release every string and node in the list.
void freeList(List* list) {
    Node* current = list->head;

    while (current != NULL) {
        Node* next = current->next;
        free(current->value);
        free(current);
        current = next;
    }

    list->head = NULL;
    list->tail = NULL;
}

int main(void) {
    //build a sample list and exercise each operation.
    List list;
    initializeList(&list);

    if (!insert(&list, "Java") || !insert(&list, "Lox") || !insert(&list, "C") || !insert(&list, "Python")) {
        fprintf(stderr, "Memory allocation failed.\n");
        freeList(&list);
        return 1;
    }

    // Show list in both directions.
    printForward(&list);
    printBackward(&list);

    // Test a successful find and missing value.
    Node* found = find(&list, "Lox");
    if (found != NULL) {
        printf("Found existing value: %s\n", found->value);
    } else {
        printf("Lox was not found.\n");
    }

    if (find(&list, "Ruby") == NULL) {
        printf("Ruby was not found.\n");
    }

    deleteValue(&list, "Java");
    printForward(&list);
    printBackward(&list);

    deleteValue(&list, "Python");
    printForward(&list);
    printBackward(&list);

    deleteValue(&list, "Lox");
    printForward(&list);
    printBackward(&list);

    //try deleting a value that is not in the list.
    if (!deleteValue(&list, "Ruby")) {
        printf("Ruby could not be deleted because it was not found.\n");
    }

    //free the remaining nodes before exiting.
    freeList(&list);
    printForward(&list);

    return 0;
}
