# Smoothie Shop Simulation

## Overview

This program simulates a smoothie shop with 12 lines and one clerk. It processes customers in the ideal order based on their arrival time and the number of smoothies being ordered. The simulation accounts for the running time and the time customers arrive.

## Author

James Phillips

## Date

6/15/24

## Project

PA2

## Features

- Simulates 12 customer lines
- Processes customers based on arrival time and order size
- Calculates and tracks time for each transaction
- Handles multiple simulation runs
- Implements a custom queue data structure

## How to Compile

To compile the program, use a C compiler such as gcc:

```
gcc -o smoothie_simulation main.c
```

## How to Run

After compiling, run the program:

```
./smoothie_simulation < sampleinput.txt
```

The program reads input from stdin, so you can either input data manually or use input redirection as shown above.

## Input Format

The input should be structured as follows:

1. Number of simulations
2. For each simulation:
   - Number of customers
   - For each customer: arrival time, line number, name, number of smoothies

Example input (from sampleinput.txt):

```
2
5
10 1 IMRAN 12
12 6 ADAM 8
13 1 MEHMED 40
22 6 CHRISTOPHER 39
100000 12 ORHAN 53
6
100 1 A 100
200 2 B 99
300 3 C 98
400 4 D 97
500 5 E 96
600 6 F 95
```

This example runs 2 simulations, the first with 5 customers and the second with 6 customers.

## Output Format

The program will output the time each customer leaves the counter, in the format:

```
At time <departure_time>, <customer_name> left the counter from line <line_number>.
```

## Implementation Details

- Uses a custom queue data structure (`struct Q`) to manage customers in each line
- Implements a linked list approach for the queue with `struct node`
- Customer information is stored in `struct Customer`
- Main processing loop:
  1. Read input and enqueue customers
  2. Process each customer in the optimal order
  3. Calculate processing time and print transaction details
- Processing time calculation: 30 + (5 * number_of_smoothies) seconds
- Customer selection criteria:
  1. Choose the customer arriving closest after the current time
  2. If multiple customers are present, choose the one ordering the least smoothies
  3. In case of a tie, choose the one in the lowest numbered line

## Memory Management

The program implements custom memory management:
- Dynamically allocates memory for customer names
- Frees memory for processed customers and queue structures

## Note

This program is designed for educational purposes and may not reflect real-world smoothie shop operations. The simulation can handle large time values and varying numbers of customers.
