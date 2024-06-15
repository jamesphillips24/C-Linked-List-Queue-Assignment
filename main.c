#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// James Phillips
// 6/15/24
// PA2
// Program to simulate 12 lines for a smoothie shop with one clerk,
// accounting for the running time and the time the customers arrive
// to process customers in ideal order based on arrival time and #
// of smoothies being ordered.


#define SIZE 12

typedef struct Customer{
  int time;
  int line;
  char* name;
  int smoothies;
} Customer;

typedef struct node{
  Customer* customer;
  struct node* next;
} node;

typedef struct Q{
  node* front;
  node* back;
} Q;

Customer* init_customer();
Q** init_Q_array();
void enqueue(Customer* customer, Q** queues);
Customer* dequeue(Q* queue);
int empty(Q* queue);
Customer* peek(Q* queue);
Q* find_next_customer(Q** queues, int* time);
Q* find_lowest_time(Q** queues);
void calc_time(int* time, Customer* customer);
void print_transaction(int time, Customer* customer);
void free_queues(Q** queues);
void free_customer(Customer* customer);

int main(void) {
  int numSimulations, numCustomers, time;
  Customer* customer;
  Q* queue;
  Q** queues;

  // Loop for each simulation.
  scanf("%d", &numSimulations);
  for(int i = 0; i < numSimulations; i++){
    time = 0;
    queues = init_Q_array();

    // Place every customer into the queue.
    scanf("%d", &numCustomers);
    for(int j = 0; j < numCustomers; j++){
      customer = init_customer();
      enqueue(customer, queues);
    }

    // Then process each customer in order, update the time,
    // and print out the order processed. Also free memory.
    for(int j = 0; j < numCustomers; j++){
      customer = dequeue(find_next_customer(queues, &time));
      calc_time(&time, customer);
      print_transaction(time, customer);
      printf("\n");

      free_customer(customer);
    }
    free_queues(queues);
  }
}

// Declare memory for customer, take input, use temp name variable
// to declare memory for customer name, and then strcpy the name in.
Customer* init_customer(){
  Customer* customer = malloc(sizeof(Customer));
  char tempName[16];

  scanf("%d %d %s %d", &customer->time, &customer->line, tempName, &customer->smoothies);
  customer->name = malloc(sizeof(char) * (strlen(tempName) + 1));
  strcpy(customer->name, tempName);

  return customer;
}

// Declare memory for arrays of queues. Also declare
// each individual queue and init front and back.
Q** init_Q_array(){
  Q** queues = malloc(sizeof(Q*) * SIZE);

  for(int i = 0; i < SIZE; i++){
    queues[i] = malloc(sizeof(Q));
    queues[i]->front = NULL;
    queues[i]->back = NULL;
  }

  return queues;
}

// Add customer to the back of the line (add to queue).
void enqueue(Customer* customer, Q** queues){
  // Get customer line and corresponding queue.
  int line = customer->line;
  Q* queue = queues[line-1];

  // Create node
  node* temp = malloc(sizeof(node));
  temp->customer = customer;
  temp->next = NULL;

  // If queue is empty, make it the front and back, other wise
  // attach it to the end of the LL and set it to be the back.
  if(empty(queue)){
    queue->front = temp;
    queue->back = temp;
  }
  else{
    queue->back->next = temp;
    queue->back = temp;
  }
  return;
}

// Process first in line (remove from queue).
Customer* dequeue(Q* queue){
  // Temp customer to retain info after freeing node.
  Customer *c;

  // Get front and then move the front back one.
  node* temp = queue->front;
  queue->front = queue->front->next;

  // Free node but return customer.
  c = temp->customer;
  free(temp);
  return c;
}

// Check if a queue is empty.
int empty(Q* queue){
  if(queue->front == NULL)
    return 1;
  else
    return 0;
}

// Return first customer in line.
Customer* peek(Q* queue){
  return queue->front->customer;
}

// Find the next customer to be processed.
// Rules: choose the customer arriving closest after the 
// current time. If some are there before the current time,
// choose the one ordering the least smoothies. If there
// is a tie, choose the one in the lowest numbered line.
Q* find_next_customer(Q** queues, int* time){
  Q* queue;
  Q* lowestQueue = find_lowest_time(queues);
  Q* nextQueue = lowestQueue;

  // If no customers are there before the current time.
  // This also processes the very first customer.
  if(*time < peek(lowestQueue)->time){
    *time = peek(lowestQueue)->time;
    return lowestQueue;
  }

  for(int i = 0; i < SIZE; i++){ 
    queue = queues[i];
    if(!empty(queue) && peek(queue)->time <= *time && peek(queue)->smoothies < peek(nextQueue)->smoothies){ 
      nextQueue = queue;
    }
  }

  return nextQueue;
}

// Finds the front of line customer that got there earliest. Loops through
// all queues and checks customer times, keeping the lowest found.
Q* find_lowest_time(Q** queues){
  Q* queue;
  Q* firstQueue;

  int min = -1;
  for(int i = 0; i < SIZE; i++){
    queue = queues[i];

    if(!empty(queue)){
      // If this is the first non-empty queue found
      if(min == -1){
        firstQueue = queue;
        min = peek(firstQueue)->time;
      }
      else if(peek(queue)->time < min){
        firstQueue = queue;
        min = firstQueue->front->customer->time;
      }
    }
  }
  return firstQueue;
}

// Calculate and adjust time. Shorthand function for convinience.
// Process time = 30 + (5 * # of smoothies)
void calc_time(int* time, Customer* customer){
  *time += (30 + (customer->smoothies * 5));
  return;
}

// Shorthand output print function for convinience.
void print_transaction(int time, Customer* customer){
  printf("At time %d, %s left the counter from line %d.", time, customer->name, customer->line);
}

// Free customer memory.
void free_customer(Customer* customer){
  free(customer->name);
  free(customer);
}

// Free queue memory.
void free_queues(Q** queues){
  for(int i = 0; i < SIZE; i++)
    free(queues[i]);
  free(queues);
}
