#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "jrb.h"
#include "dllist.h"
#include "simulator_lab2.h"
#include "simulator.h"
#include "scheduler.h"
#include "kt.h"

struct PCB *curr;
Dllist readyq;
bool is_noop;
int curpid;
JRB rbtree;
struct PCB *init;

void initialize_scheduler(){
    readyq = new_dllist();
    rbtree = make_jrb();
    create_memory();
    is_noop = TRUE;
    if(readyq == NULL){
        exit(1);
    }
}

void* initialize_user_process(void* arg){
    //first part: allocate new PCB and initialize things
    User_Base = 0 ;
    //User_Limit = MemorySize;

    //char **my_argv = (char **)arg;	
    bzero(main_memory, MemorySize);

    init = (struct PCB*)malloc(sizeof(struct PCB));
    int i;
	for (i=0; i < NumTotalRegs; i++){
        init->my_registers[i] = 0;
    }
    //which is 0
    init->pid = get_new_pid();
    init->waiters_sem = make_kt_sem(0);
    init->waiters = new_dllist();
    init->children = make_jrb();


    //allocate a new pcb
    struct PCB* my_pcb = (struct PCB*)malloc(sizeof(struct PCB));
	for (i=0; i < NumTotalRegs; i++)
		my_pcb->my_registers[i] = 0;
	/* set up the program counters and the stack register */
	my_pcb->my_registers[PCReg] = 0;
	my_pcb->my_registers[NextPCReg] = 4;

	/* need to back off from top of memory */
	/* 12 for argc, argv, envp */
	/* 12 for stack frame */
    //my_pcb->my_registers[StackReg] = MemorySize - 12;

    
    my_pcb->base = User_Base;
    my_pcb->limit = User_Limit;
    //occupy the first memory chunk
    memory_chunk[0] = 1;
    my_pcb->memory_num = 0;

    //set up pid
    my_pcb->pid = get_new_pid();
    my_pcb->parent = init;
    my_pcb->waiters_sem = make_kt_sem(0);
    my_pcb->waiters = new_dllist();
    my_pcb->children = make_jrb();
    //insert Process 1 into sentinal
    jrb_insert_int(init->children, my_pcb->pid, new_jval_v((void *)my_pcb));

    for (int i = 0; i < 64; i++){
        my_pcb->fd[i] = (struct Fd*)malloc(sizeof(struct Fd));
    }
    
    // standard in, out, err
    my_pcb->fd[0]->console = TRUE;
    my_pcb->fd[0]->open = TRUE;
    my_pcb->fd[0]->isread = TRUE;
    my_pcb->fd[0]->reference_count = 0;
    my_pcb->fd[0]->my_pipe = NULL;

    my_pcb->fd[1]->console = TRUE;
    my_pcb->fd[1]->open = TRUE;
    my_pcb->fd[1]->isread = FALSE;
    my_pcb->fd[1]->reference_count = 0;
    my_pcb->fd[1]->my_pipe = NULL;

    my_pcb->fd[2]->console = TRUE;
    my_pcb->fd[2]->open = TRUE;
    my_pcb->fd[2]->isread = FALSE;
    my_pcb->fd[2]->reference_count = 0;
    my_pcb->fd[2]->my_pipe = NULL;

    for (int i = 3; i < 64; i++){
        my_pcb->fd[i]->console = FALSE;
        my_pcb->fd[i]->open = FALSE;
        my_pcb->fd[i]->isread = FALSE;
        my_pcb->fd[i]->reference_count = 0;
        my_pcb->fd[i]->my_pipe = NULL;
    }

    // part2, call perform_execve
    char **my_argv = (char **)arg;
    int result = perform_execve(my_pcb, my_argv[0], my_argv);
    if(result == 0){
        //we are success
        InitUserArgs(my_pcb->my_registers,my_argv,User_Base);
        dll_append(readyq, new_jval_v((void*)my_pcb));
    }

    //my_pcb->sbrk_ptr = (void *)load_user_program(my_argv[0]);
    

    //push into d list. and kt_exit
    
    kt_exit();
}




void scheduler(){
    kt_joinall();

    if(dll_empty(readyq)== 1){
        //printf("I'm batman!!!\n");
	curr = NULL;
        if(jrb_empty(init->children)){
            //printf("I'm groot!!!\n");
            SYSHalt();
        }
    struct PCB *top = (struct PCB *)(jval_v(dll_val(dll_first(readyq))));
    if(curr->parent->pid ==0)
    {SYSHalt();}
	    
	 is_noop = TRUE;
        noop();
    }else{
        struct PCB *top = (struct PCB *)(jval_v(dll_val(dll_first(readyq))));
        curr = top;
        dll_delete_node(dll_first(readyq));
	start_timer(10);
        is_noop = FALSE;
        User_Base = curr->base;
        User_Limit = curr->limit;

        run_user_code(curr->my_registers);
    } 
}

int perform_execve(struct PCB* pcb, char* filename, char** pcb_argv){
    if (load_user_program(filename) < 0) {
		fprintf(stderr,"Can't load program.\n");
		exit(1);
	}

    pcb->my_registers[PCReg] = 0;
    pcb->my_registers[NextPCReg] = 4;
    pcb->my_registers[StackReg] = pcb->limit - 12;
    pcb->sbrk_ptr = (void *)load_user_program(filename);
    InitUserArgs(pcb->my_registers, pcb_argv, pcb->base);

    return 0;
}

int get_new_pid(){
    curpid = 0;
    while(jrb_find_int(rbtree, curpid)!= NULL){
        curpid++;
    }
    //JNULL new_jval_i(0)
    jrb_insert_int(rbtree, curpid, new_jval_i(0));
    return curpid;
}

void destroy_pid(int pid){
    JRB node = jrb_find_int(rbtree, pid);
    if(!node){
        return;
    }
    jrb_delete_node(node);
}
