First few lines of the program are for including required libraries for the proper functioning of the program.

Then next 3 lines describe the module it contains the description author and license of the kernel module. Here in the description i have used the value "MY MODULE", marked author as "ME" and the module license is "GPL" which is a widely used free software license.

Now the actual starts:-

First function of the program is count_task which serves the main function of the our kernel module within the function i have made a task_struct which is basically a process descriptor and sets it value to current which is the current process.Then i have used the list_for_each_entry macro to iterate through the task struct and see all the running process and increasing the value of count for the same.

Then there is the init function and the exit function as usual.