My design for the solution:

Begin(free_list head or main node 1)->sub_nodes(processes and holes)
    |
Main node 2->sub_nodes(processes and holes)
    |
Main node 3->sub_nodes(processes and holes)
    |
    |
    |
and so on...

Each main node have a size and permanent-address(the virtual address returned by the mmap).


The virtual address can be visualized as the the virtual address of the main node + some offset(to find the subnode).

virtual address of node n can be written as virtual address of node (n-1)+size of node (n-1) and virtual address of node 1 is set 1000 as given in the assignment.

The memory required for storing the values of the parameters in the struct "main_node" and "node" and for other things (such as the subchain length array) is allocated using mmap and is done in multiples of the page_size.
