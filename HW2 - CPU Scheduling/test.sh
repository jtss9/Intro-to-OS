#!/bin/bash
# write a shell scritps to compare the output with the real answers

# Compile the program
g++ -std=c++11 -o hw2_110550071 hw2_110550071.cpp

# Run the program and compare the output
total=1000
correct=0
for i in {1..1000}; do
        echo "Test $i"
        ./hw2_110550071 < ./testcase/tc_$i.txt > output.txt
        res=$(diff output.txt ans/ans_$i.txt)
        if [ "$res" == "" ]; then
            echo "Correct"
            correct=$((correct+1))
        else
            echo "Wrong"
            echo "Your output:"
            cat output.txt
            echo "Correct answer:"
            cat ans/ans_$i.txt
        fi
done
rm output.txt
echo "Total: $correct/$total"