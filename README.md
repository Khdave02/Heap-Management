# Heap-Management
Heap Management using first fit, best fit and buddy system and Implementation of reference counting and Mark &amp; Sweep Garbage Collector using C/C++ 

## First Fit && Best Fit

heap is made from char array of 1000 bytes 
both allocation method are almost similar only difference is 
1. In first first sucifient free block that is founded while traversing in allocated
2. In best whole block whichever block is sufficient for allocation among them smallest block is used for allocation
3. In program a slight change is there in bestMalloc and firstMalloc for finding that free block for allocation
![](https://i.imgur.com/nZRptww.png)



### Meta-data
- start (index) -> 4 bytes
- free (free = 1 block is free else occupied) -> 4bytes
- size (size_t) -> 8 bytes
- next pointer -> 8 bytes

### Overhead Memory -> 24 bytes

## Overview 
- free list is initialize -> void initialize_first() | initialize_best()
- Malloc finds the sufficient size for allocation according to first and best fit 
    - if the size found is exact that is required for allocation then whole block is used for allocation
     - if free block size is more than required than block is split into 2 fragments from which one is used for allocation and other is treated as free block -> split_block function
- free is implemented according to need of user need 
- this malloc allocation can be used in 2 ways i.e
    1. creating a malloc header file writing all malloc free function and user will include that header file and use these function for allocation of different data types according to their need
        * this requires declaration of that data so that it can be type casted to required type and a void pointer pointing to start address of memory allocated block is returned by malloc this void pointer is typecasted to required type. free takes a void pointer address of that variable that is allocated and is then deacllocated.
            
            * ![](https://i.imgur.com/2i9Sc7J.png)
    2. in this interface provided is simpler user only needs to give size of block to be allocated and and can be freed by its start index which is displyed in list printing
> void merge(..)  is used to merge adjacent free blocks together it checks whther next block is free by next_ptr and if found free it merges by adding their sizes 
and pointing next pointer to its next block 

![](https://i.imgur.com/YM9E8pW.png)
![](https://i.imgur.com/RPvZ71O.png)



## Buddy System 
- hashtable of linked list data structure is used for implementing free list
- linked list is allocatd memory dynamically instead of using array 
- available memory in free list 1024 bytes ie {2^10} so array of linked list is used as freelist of max index 10 
as 2^10 is memory
- allocList is a linked list pointing to all allocated blocks
- heap is used for allocating address to block by using starting address of heap and start index of each block
- address helps in colaescing free blocks together 

![](https://i.imgur.com/i9rutZ9.png)


## Overview 
- list is initialized which 1024 sized free block.
- `int getIndex(int size)` returns correct index at which size of free block will be pointed to in hashtable .
- `struct list_node *getAllocBlock(int index)` gives a required sized block or may be greter size block for allocation 
- `void *updateFreeList` if the halfed free block is larger than or equal to required size than it is splitted to 2 equal pieces 
    * update has 2 parts and is rcursive function 
        1. splits block to 2 parts and recursively goes on finding sufficient sized block required by splitting while one is used for splitting and other is appended in hashtable according to size and index 
        2. size is sufficient and correct for allocation and is appended in alloclist
- `void *buddyMalloc(int allocSize)` is used for allocation 
index allocblock is finded and is updated in freelist using above functions
- free is implemented in 2 ways 
    1. by starting address of block allocated user can include header buddyMalloc function file and use malloc functin to allocate a particular data type acc to to its size and can free by using address of that varible.
        * ![](https://i.imgur.com/3NSrBMU.png)

    2. start index of block allocated simpler interface for user 
- according to size of deallocated block index is finded and is inserted into free list 
- ![](https://i.imgur.com/hIv2uo9.png)

- merging free blocks is done by colaescebuddies function which finds buddy of its similar size by start index and size and merges with it to form a new larger node recursively more such nodes are found by function buddy node is founded by find buddy which takes in starting address and index as a particular size is stored/pointed to a particular size block so in that index of free list by startingindex buddy is founded and merged back. 


![](https://i.imgur.com/HhsNQEU.png)


###  Reference Counting Garbage Collector:-
![](https://i.imgur.com/DwNKUbg.png)
### Mark & Sweep Garbage Collector:-
![](https://i.imgur.com/lMoHxLi.png)

### Garbage Collection:-
![](https://i.imgur.com/CkQ5dUM.png)


