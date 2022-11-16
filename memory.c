#include "memory.h"
int memory_chunk[8];

void create_memory(){
    memset(memory_chunk,0,sizeof(memory_chunk));
    User_Limit=(MemorySize)/8;
}
