Bonus part of the Project

Instructions Added:

->Comp R1 R2
Updates the value of R1 to the 1's complement of R2.

->Floor R1 R2
Updates the value of R1 to the floor of half the value of R2.

->Ceil R1 R2
Updates the value of R1 to the ceil of half the value of R2.

->Min R1 R2 R3
Updates the value of R1 to the minimum of R2 and R3.

->Max R1 R2 R3
Updaes the value of R1 to the maximum of R2 and R3.



Semantics for the added Instructions:

comp: 10100
opcode(5 bits) + filler(5 bits) + reg1(3 bits) +reg2(3 bits)

ceil: 10101
opcode(5 bits) + filler(5 bits) + reg1(3 bits) +reg2(3 bits)

floor: 10110
opcode(5 bits) + filler(5 bits) + reg1(3 bits) +reg2(3 bits)

max: 10111
opcode(5 bits) + filler(2 bit) +reg1(3 bits) + reg2(3 bits) + reg3(3 bits)

min: 11000
opcode(5 bits) + filler(2 bit) +reg1(3 bits) + reg2(3 bits) + reg3(3 bits)


Working with the project:
In order to run the Assembler first make a file "input.txt" in the current folder having the required instructions in assembly as per the instruction set. 

Then run the command "python3 'Assembler Updated for Bonus.py'" in your terminal in current directory and the output generated by the assembler will be written in a new file named "output.txt". Please not if such a file already exist its contents will be overwritten.

Similarly in order to run the simulator Make a file "sim_test.txt" and write the binary content produced by the assembler in it and then run the command on your terminal "python3 'Simulator Updated for Bonus.py'". Here the values of various flags will be printed on the terminal itself along with the memory dump in the end.

Please make sure the file you provide as input to the simulator contains valid binary expressions.
