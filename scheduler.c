#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "dllist.h"
#include "simulator.h"
#include "scheduler.h"
#include "kt.h"
#include "memory.h"
struct PCB *curr;
Dllist readyq;
bool is_noop;
int curpid;
JRB rbtree;
struct PCB* init;
void initialize_scheduler()
{
    readyq = new_dllist();
    is_noop = TRUE;
    
    create_memory();;
    //printf("the value is %d", User_Limit);
    if (readyq == NULL)
    {
        exit(1);
    }
}

int perform_execve(struct PCB* pcb, char* filename, char** pcb_argv){
    for (int i=0; i<NumTotalRegs; ++i){
        pcb->my_registers[i]=0;
    }
    pcb->my_registers[PCReg] = 0;
    pcb->my_registers[NextPCReg] = 4;
    pcb->base = User_Base;
    pcb->limit = User_Limit;
    pcb->waiters_sem=make_kt_sem(0);
    pcb->waiters=new_dllist();
    int sbrk_ptr=load_user_program(filename);
    if (sbrk_ptr < 0) {
        fprintf(stderr,"Can't load program.\n");
        exit(1);
    }    
    pcb->sbrk_ptr=(void*)sbrk_ptr;

    int size = 0;
    while(pcb_argv[size]){
        size++;
    }
    int tos, argv, k;
    int argvptr[256];
    
    tos = User_Limit- 12-1024;
    
    int j;
    for(j = 0; j < size; j++){
        tos -= (strlen(pcb_argv[j]) + 1);
        while(tos%4 != 0){
            tos--;
       }
        argvptr[j] = tos;
        strcpy(main_memory+tos+pcb->base, pcb_argv[j]);
    }
    

    while (tos % 4) tos--;

    tos -= 4;
    k = 0;
    memcpy(main_memory+tos+pcb->base, &k, 4);

    for(int i = size - 1; i >= 0; i--){
        tos -= 4;
        memcpy(main_memory+tos+pcb->base, &argvptr[i], 4);
    }



    argv = tos;

    //envp
    tos -= 4;
    k = 0;
    memcpy(main_memory+tos+pcb->base, &k, 4);

    //&argv
    tos -= 4;
    memcpy(main_memory+tos+pcb->base, &argv, 4);

    //argc
    tos -= 4;
    k = size;
    memcpy(main_memory+tos+pcb->base, &k, 4);	

    tos -= 12;
    memset(main_memory+tos+pcb->base, 0, 12);

    /* need to back off from top of memory */
    /* 12 for argc, argv, envp */
    /* 12 for stack frame */
    //tos
    pcb->my_registers[StackReg] = tos;

    return 0;
}

void *initialize_user_process(void *arg)
{
    char **my_argv = (char **)arg;
    bzero(main_memory, MemorySize);
    User_Base = 0;
    struct PCB *my_pcb = (struct PCB *)malloc(sizeof(struct PCB));
    init=(struct PCB*)malloc(sizeof(struct PCB));
    int i;
    for (i = 0; i < NumTotalRegs; i++)
        my_pcb->my_registers[i] = 0;
    init->pid=get_new_pid();
    init->children = make_jrb();
    init->waiters_sem = make_kt_sem(0);
    init->waiters = new_dllist();
    memory_chunk[0]=1;
    my_pcb->mem_int=1;
    my_pcb->parent=init;
    my_pcb->children = make_jrb();
    int wanted_pid = get_new_pid();
    my_pcb->pid = (unsigned short)wanted_pid;


    jrb_insert_int(init->children, my_pcb->pid, new_jval_v((void*)my_pcb));
    int execv = perform_execve(my_pcb, my_argv[0], my_argv);
    InitUserArgs(my_pcb->my_registers, my_argv, User_Base+1024);
    if(execv == 0){
        dll_append(readyq,new_jval_v((void*)my_pcb));
        kt_exit();
    }else{
        exit(1);
    }
}

void scheduler()
{
    kt_joinall();
    JRB ptr = jrb_first(init->children);
    if(ptr == jrb_nil(init->children)){
        SYSHalt();
    }
    if (dll_empty(readyq))
    {
        curr = NULL;
        is_noop = TRUE;
        noop();
    }
    else
    {
        struct PCB *top = (struct PCB *)((dll_val(dll_first(readyq))).v);
        curr = top;
        dll_delete_node(dll_first(readyq));
        is_noop = FALSE;
        User_Base=curr->base;
        User_Limit=curr->limit;
        start_timer(10);
        run_user_code(curr->my_registers);
    }
}

int get_new_pid(){
    curpid =0;
    while(jrb_find_int(rbtree, curpid)){
        curpid++;
    }
    jrb_insert_int(rbtree, curpid, JNULL);
    return curpid;
}

void destroy_pid(int pid){
    JRB node = jrb_find_int(rbtree, pid);
    if(!node){
        return;
    }
    jrb_delete_node(node);
}
