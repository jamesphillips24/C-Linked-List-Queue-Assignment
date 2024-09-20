#!/bin/sh

gcc -o smoothie_simulation main.c smoothies.c && ./smoothie_simulation < input.txt
