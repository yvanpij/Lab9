#include <stdio.h>
#include <stdlib.h> // For malloc and free

// RecordType
struct RecordType {
    int id;
    char name;
    int order;
};

// Linked list node
struct ListNode {
    struct RecordType data;
    struct ListNode* next;
};

// HashType - array of linked lists
#define HASH_SIZE 10

struct HashType {
    struct ListNode* hashArray[HASH_SIZE];
};

// Compute the hash function
int hash(int x) {
    return x % HASH_SIZE;
}

// Insert a record into the hash table
void insertRecordIntoHash(struct HashType* pHashTable, struct RecordType record) {
    int index = hash(record.id);
    struct ListNode* newNode = (struct ListNode*)malloc(sizeof(struct ListNode));
    if (newNode == NULL) {
        printf("Cannot allocate memory\n");
        exit(-1);
    }

    newNode->data = record;
    newNode->next = pHashTable->hashArray[index];
    pHashTable->hashArray[index] = newNode;
}

// Parses input file to an integer array
int parseData(char* inputFileName, struct HashType* pHashTable) {
    FILE* inFile = fopen(inputFileName, "r");
    int dataSz = 0;
    int i, n;
    char c;
    struct RecordType* pRecord;

    if (inFile) {
        fscanf(inFile, "%d\n", &dataSz);

        for (i = 0; i < dataSz; ++i) {
            pRecord = (struct RecordType*)malloc(sizeof(struct RecordType));
            if (pRecord == NULL) {
                printf("Cannot allocate memory\n");
                exit(-1);
            }

            fscanf(inFile, "%d ", &n);
            pRecord->id = n;
            fscanf(inFile, "%c ", &c);
            pRecord->name = c;
            fscanf(inFile, "%d ", &n);
            pRecord->order = n;

            insertRecordIntoHash(pHashTable, *pRecord);
        }

        fclose(inFile);
    }

    return dataSz;
}

// Prints the records
void printRecords(struct RecordType pData[], int dataSz) {
    int i;
    printf("\nRecords:\n");
    for (i = 0; i < dataSz; ++i) {
        printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
    }
    printf("\n\n");
}

// Display records in the hash structure
void displayRecordsInHash(struct HashType* pHashTable, int hashSz) {
    int i;

    for (i = 0; i < hashSz; ++i) {
        struct ListNode* currentNode = pHashTable->hashArray[i];
        if (currentNode != NULL) {
            printf("Index %d ->", i);
            while (currentNode != NULL) {
                printf(" %d %c %d ->", currentNode->data.id, currentNode->data.name, currentNode->data.order);
                currentNode = currentNode->next;
            }
            printf("\n");
        }
    }
}

// Free memory used by the linked list
void freeLinkedList(struct ListNode* head) {
    struct ListNode* currentNode = head;
    while (currentNode != NULL) {
        struct ListNode* temp = currentNode;
        currentNode = currentNode->next;
        free(temp);
    }
}

// Free memory used by the hash table
void freeHashTable(struct HashType* pHashTable) {
    for (int i = 0; i < HASH_SIZE; i++) {
        freeLinkedList(pHashTable->hashArray[i]);
    }
}

int main(void) {
    struct HashType hashTable;
    for (int i = 0; i < HASH_SIZE; i++) {
        hashTable.hashArray[i] = NULL;
    }

    int recordSz = parseData("input.txt", &hashTable);
    displayRecordsInHash(&hashTable, HASH_SIZE);
    
    // Cleanup: Free allocated memory
    freeHashTable(&hashTable);

    return 0;
}
