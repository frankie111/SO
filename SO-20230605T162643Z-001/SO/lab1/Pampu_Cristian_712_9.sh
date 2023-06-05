#!/bin/bash

# Check if exactly 6 arguments are provided
if [ $# -ne 6 ]
  then echo "Error: Six arguments are required"
       exit 1
fi

# Store the arguments in an array
args=("$@")

# Calculate the scalar product
product=$((${args[0]} * ${args[3]} + ${args[1]} * ${args[4]} + ${args[2]} * ${args[5]}))

# Vectors are perpendicular when the scalar product is equal to 0
# Print Yes if the scalar product is equal to 0

if [ $product -eq 0 ]
  then echo "Yes"
else
  echo "No"
fi

# For values 10 10 2 2 -3 5 it should output Yes
