#!/usr/bin/env bash
# This script prints number from 0, 1, 2, ... until the script is stopped.

# Infinite loop
i=0
while true
do
  # Print the number
  echo $((i++))
  # Sleep for 0.1 milliseconds
    sleep 0.0001
done
