#include "smoothies.h"

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
