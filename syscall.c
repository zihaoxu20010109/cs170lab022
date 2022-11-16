#include <stdlib.h>

#include "console_buf.h"
#include "simulator.h"
#include "scheduler.h"
#include "kt.h"
#include "dllist.h"
#include "syscall.h"
#include "memory.h"
kt_sem writeok;
kt_sem writers;
kt_sem readers;

void initialize_sema()
{
    writeok = make_kt_sem(0);
    writers = make_kt_sem(1);
    readers = make_kt_sem(1);
}

void syscall_return(struct PCB *pcb, int value)
{
    pcb->my_registers[PCReg] = pcb->my_registers[NextPCReg];
    pcb->my_registers[NextPCReg] = pcb->my_registers[NextPCReg] + 4;
    pcb->my_registers[2] = value;
    dll_append(readyq, new_jval_v((void *)pcb));
    kt_exit();
}

void *do_write(void *arg)
{
    struct PCB *pcb = (struct PCB *)arg;
    int arg1 = pcb->my_registers[5];
    if(ValidAddress(arg) == FALSE){
        syscall_return(pcb, -EFAULT);
    }
    if (arg1 != 1 && arg1 != 2)
    {
        syscall_return(pcb, -EBADF);
    }

    int arg2 = pcb->my_registers[6];
    if (arg2 < 0)
    {
        syscall_return(pcb, -EFAULT);
    }

    if ((arg2 + pcb->my_registers[7]) > MemorySize)
    {
        syscall_return(pcb, -EFBIG);
    }

    if (pcb->my_registers[7] < 0)
    {
        syscall_return(pcb, -EINVAL);
    }

    P_kt_sem(writers);
    // pcb->my_registers[6]
    int my_local_reg6 = (int)(pcb->my_registers[6] + main_memory+pcb->base); // convert the second arg into system address

    int write_count = 0;
    char *chars = (char *)(my_local_reg6);
    char *j = chars;
    for (int i = 0; i < pcb->my_registers[7]; i++)
    {
        console_write(*j);
        P_kt_sem(writeok);
        j++;
        write_count++;
    }

    // for (char *i = chars; *i != '\0'; i++)
    // {
    //     console_write(*i);
    //     P_kt_sem(writeok);
    //     write_count++;
    // }
    V_kt_sem(writers);
    syscall_return(pcb, write_count);
}

void *do_read(void *arg)
{
    struct PCB *pcb = (struct PCB *)arg;
    int arg1 = pcb->my_registers[5];
    if(ValidAddress(arg) == FALSE){
        syscall_return(pcb, -EFAULT);
    }
    if (arg1 != 0)
    {
        syscall_return(pcb, -EBADF);
    }
    int arg2 = pcb->my_registers[6];
    if (arg2 < 0)
    {
        syscall_return(pcb, -EFAULT);
    }
    if (pcb->my_registers[7] < 0)
    {
        syscall_return(pcb, -EINVAL);
    }

    P_kt_sem(readers);
    // int min = MIN(pcb->my_registers[7], buffer->size);
    int min = pcb->my_registers[7];
    int count = 0;
    for (int i = 0; i < min; i++)
    {
        P_kt_sem(nelem);
        char toRead = (char)(buffer->buff[buffer->head]);

        if (toRead == -1)
        {
            V_kt_sem(nslots);
            V_kt_sem(readers);
            syscall_return(pcb, count);
        }

        ((char *)(pcb->my_registers[6] + main_memory))[i] = toRead;

        buffer->head = (buffer->head + 1) % (buffer->size);
        V_kt_sem(nslots);
        count++;
    }
    // printf("what are you doing!!!");
    V_kt_sem(readers);
    // printf("Return statement!!!");
    syscall_return(pcb, count);
}

void ioctl(void *arg){
    struct PCB *pcb=(struct PCB*)arg;
    if(ValidAddress(arg) == FALSE){
        syscall_return(pcb, -EFAULT);
    }
    if (pcb->my_registers[5]!=1 || pcb->my_registers[6]!=JOS_TCGETP){
        syscall_return(pcb,-EINVAL);
    }
    struct JOStermios *input= (struct JOStermios *)&main_memory[pcb->my_registers[7]+ pcb->base];
    ioctl_console_fill(input);
    syscall_return(pcb,0);
}

void fstat(void *arg)
{
    struct PCB *curr = (struct PCB *)arg;
    int fd = curr->my_registers[5];
    if(ValidAddress(arg) == FALSE){
        syscall_return(curr, -EFAULT);
    }
    struct KOSstat *kosstat = (struct KOSstat *)&main_memory[curr->my_registers[6] + curr->base];
    
    if (fd == 0)
    {
        stat_buf_fill(kosstat, 1);
    }
    if (fd == 2 || fd == 1)
    {
        stat_buf_fill(kosstat, 256);
    }

    syscall_return(curr, 0);
}
void getpagesize(void *arg)
{
    struct PCB *curr = (struct PCB *)arg;
    syscall_return(curr, PageSize);
}
void getdtablesize(void *arg)
{
    struct PCB *curr = (struct PCB *)arg;
    syscall_return(curr, 64);
    
}
void do_sbrk(void *arg){
    struct PCB *curr = (struct PCB *)arg;
    if (ValidAddress(arg) == FALSE)
    {
        syscall_return(curr, -EFAULT);
    }

    int add = curr->my_registers[5];

    if (curr->sbrk_ptr == NULL)
    {
        curr->sbrk_ptr == 0;
    }
    int oldsbrk = (int)curr->sbrk_ptr;

    if (oldsbrk + add > User_Limit)
    {
        syscall_return(curr, -ENOMEM);
    }
    curr->sbrk_ptr += add;
    syscall_return(curr, oldsbrk);
}
bool ValidAddress(void *arg){
    struct PCB *pcb = (struct PCB*)arg;

    if (pcb->my_registers[RetAddrReg]>=0 && pcb->my_registers[RetAddrReg]<=pcb->limit){
        return TRUE;
    }
    return FALSE;
}

void getpid(void *arg){
   struct PCB *pcb=(struct PCB*)arg;
   syscall_return(pcb, pcb->pid); 
}


void do_execve(void *arg){
    struct PCB *pcb=(struct PCB*)arg;

    int arg_size = 0;
    while(main_memory[pcb->my_registers[5]+pcb->base+arg_size]){
        arg_size++;
    }
    char* filename = (char *)malloc((arg_size+1)*sizeof(char));
    for(int i = 0; i < arg_size; i++){
        strncpy(filename+i, main_memory+pcb->my_registers[5]+pcb->base+i, 1);
    }
    filename[arg_size] = '\0';

    int num_str=0;
    int ptr=pcb->my_registers[6]+pcb->base;
    while (TRUE){
        int add= main_memory[ptr] +(main_memory[ptr+1]<<8)+ (main_memory[ptr+2]<<16)+ (main_memory[ptr+3]<<24);

        if (add == 0 || add > pcb->base+User_Limit){
            break;
        }
        num_str++;
        ptr+=4;
    }
    char** argv = (char**)malloc((num_str+1) * sizeof(char*));
    ptr=pcb->my_registers[6]+pcb->base;
    
    for (int i=0; i<num_str; ++i){
        int size=0;
        int add= main_memory[ptr] +
        (main_memory[ptr+1]<<8)+
        (main_memory[ptr+2]<<16)+
        (main_memory[ptr+3]<<24);

        char* c_ptr = main_memory+add+pcb->base;
        size = strlen(c_ptr);
        
   

        char * temp= (char*)malloc((size+1)*sizeof(char));
        for (int j=0; j<size; ++j){
            
            memcpy(temp+j,main_memory+pcb->base+add+j,1);
        }
        temp[size]='\0';
        argv[i] = (char*)malloc((size+1)*sizeof(char));
        memcpy(argv[i],temp, size+1);
        ptr+=4;
        free(temp);
    }
    
    argv[num_str]=NULL;
    
    int return_value = perform_execve(pcb, filename, argv);
    pcb->my_registers[2]=return_value;
    dll_append(readyq,new_jval_v((void *)pcb));
    for(int i = 0; i< num_str; i++){
        free(argv[i]);    
    }
    free(argv);
    free(filename);
    kt_exit();
}

void mydo_fork(void *arg){
    struct PCB *pcb=(struct PCB*)arg;
    int mem_int=0;
    if (!empty_memory(&mem_int)){
        syscall_return(pcb,-EAGAIN);
    }
    struct PCB *job= (struct PCB*)malloc(sizeof(struct PCB));

    // initialize all fields of job.
    job ->pid=get_new_pid();
    job ->base=mem_int*User_Limit;
    job ->limit=User_Limit;
    job ->sbrk_ptr=pcb->sbrk_ptr;
    job ->mem_int=mem_int;
    //printf("mymeemorylockis %d ",job ->mem_int);
    memory_chunk[job ->mem_int]=1;
    job ->parent=pcb;
    job ->children = make_jrb();
    job->waiters_sem=make_kt_sem(0);
    job->waiters=new_dllist();
    jrb_insert_int(pcb->children, job ->pid, new_jval_v((void*)job));
    // copy the registers.
    for (int i=0; i<NumTotalRegs; ++i){
        job ->my_registers[i]=pcb->my_registers[i];
    }

    // copy the memory.
    memcpy(main_memory+job ->base,main_memory+pcb->base,User_Limit);

    // fork the main process
    kt_fork(finish_fork, (void *)job );
    syscall_return(pcb,job ->pid);
}

void *finish_fork(void *arg){
    struct PCB *pcb=(struct PCB*)arg;
    syscall_return(pcb,0);
}
bool empty_memory(int* ptr){
    for (int i=0; i<sizeof(memory_chunk)/sizeof(int); ++i){
        if (memory_chunk[i]==0){
            *ptr=i;
            return TRUE;
        }
    }
    return FALSE;
}


void myown_exit(void *arg)
{   
    struct PCB *pcb=(struct PCB*)arg;
    pcb->exit_value=pcb->my_registers[5];
    memory_chunk[pcb->mem_int]=0;
    memset(main_memory + pcb->base, 0, pcb->limit);

    if (curr->parent)
    {
        jrb_delete_node(jrb_find_int(pcb->parent->children, pcb->pid));
    }
    
  
    
    JRB ptr;
    jrb_traverse(ptr, pcb->children){
        jrb_insert_int(init->children, jval_i(ptr->key), ptr->val);
        struct PCB* temp = (struct PCB*)(ptr->val.v);
        temp->parent = init;
    }
    jrb_free_tree(pcb->children);

    if(!dll_empty(pcb->waiters)){
        Dllist ptr;
        dll_traverse(ptr, pcb->waiters){
            struct PCB* child=(struct PCB*)(ptr->val.v);
            dll_delete_node(ptr);
            child->parent = init;
            destroy_pid(child->pid);
            for (int i=0; i<NumTotalRegs; ++i){
                child->my_registers[i]=0;
            }
            free(child);
        }
    }
    
    if(pcb->parent == init){
        destroy_pid(pcb->pid);
        for (int i=0; i<NumTotalRegs; ++i){
            pcb->my_registers[i]=0;
        }
        free(pcb);
    }else{
        V_kt_sem(pcb->parent->waiters_sem);
        dll_append(pcb->parent->waiters,new_jval_v((void*)pcb)); 
    }
    kt_exit();
}



void do_close(void *arg){
    struct PCB *curr = (struct PCB *)arg;
    syscall_return(curr, -EBADF);
}

void get_ppid(void *arg){
    struct PCB *curr = (struct PCB *)arg;
    syscall_return(curr, curr->parent->pid);
}


void do_wait(void *arg){
    struct PCB *curr = (struct PCB *)arg;
    JRB ptr = jrb_first(curr->children);
    if(ptr == jrb_nil(curr->children) && dll_empty(curr->waiters)){
        syscall_return(curr, -1);
    }
    P_kt_sem(curr->waiters_sem);

    struct PCB *child = (struct PCB *)(jval_v((dll_val(dll_first(curr->waiters)))));
    memcpy(&(main_memory[curr->my_registers[5]+ curr->base]),&(child->exit_value),sizeof(int));
    
    dll_delete_node(dll_first(curr->waiters));
    destroy_pid(child->pid);
    for (int i=0; i<NumTotalRegs; ++i){
        child->my_registers[i]=0;
    }
    free(child);
    
    //SYSHalt();
    syscall_return(curr, child->pid);
}


