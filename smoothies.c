#include "smoothies.h"

static void enqueue(Smoothies* smoothieQueue, Customer* customer);
static Customer* dequeue(Smoothies* smoothieQueue);
static int empty(Smoothies* smoothieQueue);
static Customer* peek(Smoothies* smoothieQueue);

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

Customer* createCustomer() {
    Customer* customer = malloc(sizeof(Customer));
    char tempName[16];
    scanf("%d %d %s %d", &customer->time, &customer->line, tempName, &customer->smoothies);
    customer->name = malloc(sizeof(char) * (strlen(tempName) + 1));
    strcpy(customer->name, tempName);
    return customer;
}

void freeSmoothies(Smoothies* smoothieQueue) {
    while (!smoothieQueue->empty(smoothieQueue)) {
        Customer* customer = smoothieQueue->dequeue(smoothieQueue);
        freeCustomer(customer);
    }
    free(smoothieQueue);
}

void freeCustomer(Customer* customer) {
    free(customer->name);
    free(customer);
}

static void enqueue(Smoothies* smoothieQueue, Customer* customer) {
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

static Customer* dequeue(Smoothies* smoothieQueue) {
    if (smoothieQueue->empty(smoothieQueue)) {
        return NULL;
    }
    node* temp = smoothieQueue->front;
    Customer* customer = temp->customer;
    smoothieQueue->front = smoothieQueue->front->next;
    free(temp);
    return customer;
}

static int empty(Smoothies* smoothieQueue) {
    return smoothieQueue->front == NULL;
}

static Customer* peek(Smoothies* smoothieQueue) {
    if (smoothieQueue->empty(smoothieQueue)) {
        return NULL;
    }
    return smoothieQueue->front->customer;
}

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

void calcTime(int* time, Customer* customer) {
    *time += (30 + (customer->smoothies * 5));
}

void printTransaction(int time, Customer* customer) {
    printf("At time %d, %s left the counter from line %d.", time, customer->name, customer->line);
}
