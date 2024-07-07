In this program to achieve our objective of comparing various scheduling policies i first performed the fork 3 times and in each fork i used the execvp command to run the "./ans.o" file that counts from 1 to 2^32. 

In order to calculate the runtime of the three processes i used the time library to get the timestamp before the fork and after the termination of each program.

Then i printed the result to terminal as well as saved it the result to a file named "output.txt" and then the python script generates the bar-graph.


According to my observations:-
    the time-taken to by different policies follows the order:-
    FIFO < RR < OTHER