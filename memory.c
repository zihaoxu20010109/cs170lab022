#include "memory.h"
int memory_chunk[8];

void create_memory(){
    //fill it with 0
    for (int i = 0; i < 8; i++){
        memory_chunk[i] = 0;
    }
    User_Limit = MemorySize / 8;
}
