#ifndef SMOOTHIES_H
#define SMOOTHIES_H

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

Smoothies* createSmoothies();
Customer* createCustomer();
void freeSmoothies(Smoothies* smoothieQueue);
void freeCustomer(Customer* customer);
Smoothies* findNextCustomer(Smoothies** smoothieQueues, int* time);
Smoothies* findLowestTime(Smoothies** smoothieQueues);
void calcTime(int* time, Customer* customer);
void printTransaction(int time, Customer* customer);

#endif // SMOOTHIES_H
