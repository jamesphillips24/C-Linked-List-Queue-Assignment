#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 12

typedef struct Customer {
    int time;
    int line;
    char* name;
    int smoothies;
} Customer;

typedef struct node {
    Customer* customer;
    struct node* next;
} node;

typedef struct Smoothies {
    node* front;
    node* back;
    void (*enqueue)(struct Smoothies*, Customer*);
    Customer* (*dequeue)(struct Smoothies*);
    int (*empty)(struct Smoothies*);
    Customer* (*peek)(struct Smoothies*);
} Smoothies;

// Function prototypes
Smoothies* createSmoothies();
Customer* createCustomer();
void freeSmoothies(Smoothies* smoothieQueue);
void freeCustomer(Customer* customer);
void enqueue(Smoothies* smoothieQueue, Customer* customer);
Customer* dequeue(Smoothies* smoothieQueue);
int empty(Smoothies* smoothieQueue);
Customer* peek(Smoothies* smoothieQueue);
Smoothies* findNextCustomer(Smoothies** smoothieQueues, int* time);
Smoothies* findLowestTime(Smoothies** smoothieQueues);
void calcTime(int* time, Customer* customer);
void printTransaction(int time, Customer* customer);

// Main function
int main(void) {
    int numSimulations, numCustomers, time;
    Customer* customer;
    Smoothies* smoothieQueue;
    Smoothies** smoothieQueues;

    scanf("%d", &numSimulations);
    for (int i = 0; i < numSimulations; i++) {
        time = 0;
        smoothieQueues = malloc(sizeof(Smoothies*) * SIZE);
        for (int j = 0; j < SIZE; j++) {
            smoothieQueues[j] = createSmoothies();
        }

        scanf("%d", &numCustomers);
        for (int j = 0; j < numCustomers; j++) {
            customer = createCustomer();
            smoothieQueues[customer->line - 1]->enqueue(smoothieQueues[customer->line - 1], customer);
        }

        for (int j = 0; j < numCustomers; j++) {
            smoothieQueue = findNextCustomer(smoothieQueues, &time);
            customer = smoothieQueue->dequeue(smoothieQueue);
            calcTime(&time, customer);
            printTransaction(time, customer);
            printf("\n");
            freeCustomer(customer);
        }

        for (int j = 0; j < SIZE; j++) {
            freeSmoothies(smoothieQueues[j]);
        }
        free(smoothieQueues);
    }
    return 0;
}

// Create a new Smoothies queue
Smoothies* createSmoothies() {
    Smoothies* smoothieQueue = malloc(sizeof(Smoothies));
    smoothieQueue->front = NULL;
    smoothieQueue->back = NULL;
    smoothieQueue->enqueue = enqueue;
    smoothieQueue->dequeue = dequeue;
    smoothieQueue->empty = empty;
    smoothieQueue->peek = peek;
    return smoothieQueue;
}

// Create a new Customer
Customer* createCustomer() {
    Customer* customer = malloc(sizeof(Customer));
    char tempName[16];
    scanf("%d %d %s %d", &customer->time, &customer->line, tempName, &customer->smoothies);
    customer->name = malloc(sizeof(char) * (strlen(tempName) + 1));
    strcpy(customer->name, tempName);
    return customer;
}

// Free Smoothies queue
void freeSmoothies(Smoothies* smoothieQueue) {
    while (!smoothieQueue->empty(smoothieQueue)) {
        Customer* customer = smoothieQueue->dequeue(smoothieQueue);
        freeCustomer(customer);
    }
    free(smoothieQueue);
}

// Free Customer
void freeCustomer(Customer* customer) {
    free(customer->name);
    free(customer);
}

// Enqueue operation
void enqueue(Smoothies* smoothieQueue, Customer* customer) {
    node* temp = malloc(sizeof(node));
    temp->customer = customer;
    temp->next = NULL;
    if (smoothieQueue->empty(smoothieQueue)) {
        smoothieQueue->front = temp;
        smoothieQueue->back = temp;
    } else {
        smoothieQueue->back->next = temp;
        smoothieQueue->back = temp;
    }
}

// Dequeue operation
Customer* dequeue(Smoothies* smoothieQueue) {
    if (smoothieQueue->empty(smoothieQueue)) {
        return NULL;
    }
    node* temp = smoothieQueue->front;
    Customer* customer = temp->customer;
    smoothieQueue->front = smoothieQueue->front->next;
    free(temp);
    return customer;
}

// Check if queue is empty
int empty(Smoothies* smoothieQueue) {
    return smoothieQueue->front == NULL;
}

// Peek operation
Customer* peek(Smoothies* smoothieQueue) {
    if (smoothieQueue->empty(smoothieQueue)) {
        return NULL;
    }
    return smoothieQueue->front->customer;
}

// Find the next customer to be processed
Smoothies* findNextCustomer(Smoothies** smoothieQueues, int* time) {
    Smoothies* lowestQueue = findLowestTime(smoothieQueues);
    Smoothies* nextQueue = lowestQueue;

    if (*time < lowestQueue->peek(lowestQueue)->time) {
        *time = lowestQueue->peek(lowestQueue)->time;
        return lowestQueue;
    }

    for (int i = 0; i < SIZE; i++) {
        Smoothies* smoothieQueue = smoothieQueues[i];
        if (!smoothieQueue->empty(smoothieQueue) && 
            smoothieQueue->peek(smoothieQueue)->time <= *time && 
            smoothieQueue->peek(smoothieQueue)->smoothies < nextQueue->peek(nextQueue)->smoothies) {
            nextQueue = smoothieQueue;
        }
    }

    return nextQueue;
}

// Find the queue with the lowest arrival time
Smoothies* findLowestTime(Smoothies** smoothieQueues) {
    Smoothies* firstQueue = NULL;
    int min = -1;

    for (int i = 0; i < SIZE; i++) {
        Smoothies* smoothieQueue = smoothieQueues[i];
        if (!smoothieQueue->empty(smoothieQueue)) {
            if (min == -1 || smoothieQueue->peek(smoothieQueue)->time < min) {
                firstQueue = smoothieQueue;
                min = smoothieQueue->peek(smoothieQueue)->time;
            }
        }
    }

    return firstQueue;
}

// Calculate processing time
void calcTime(int* time, Customer* customer) {
    *time += (30 + (customer->smoothies * 5));
}

// Print transaction details
void printTransaction(int time, Customer* customer) {
    printf("At time %d, %s left the counter from line %d.", time, customer->name, customer->line);
}
