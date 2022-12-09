#include <stdlib.h>

#include "console_buf.h"
#include "simulator_lab2.h"
#include "simulator.h"
#include "scheduler.h"
#include "jrb.h"
#include "kt.h"
#include "dllist.h"
#include "syscall.h"

kt_sem writeok;
kt_sem writers;
kt_sem readers;


void initialize_sema(){
	writeok = make_kt_sem(0);
    writers = make_kt_sem(1);
    readers = make_kt_sem(1);
}

void syscall_return(struct PCB *pcb, int value)
{
    // printf("PCReg is %d\n", pcb->my_registers[PCReg]);
    // printf("NextPCReg is %d\n", pcb->my_registers[NextPCReg]);
    // printf("Reg2 is %d\n", pcb->my_registers[2]);
    pcb->my_registers[PCReg] = pcb->my_registers[NextPCReg];
    //pcb->my_registers[NextPCReg] =  pcb->my_registers[NextPCReg] +4;
    pcb->my_registers[2] = value;
    dll_append(readyq, new_jval_v((void *)pcb));
    kt_exit();
}

void *do_write(void *arg)
{
    struct PCB *pcb = (struct PCB *)arg;
    //check if address is valid
    if(ValidAddress(arg) == FALSE){
        syscall_return(pcb, -EFAULT);
    }

    int file_d_num = pcb->my_registers[5];
    //if(file_d_num<0 || file_d_num>=64){
    //    syscall_return(pcb, -EBADF); 
    //}
    if(pcb->fd[file_d_num]->open == FALSE){
        syscall_return(pcb, -EBADF); 
    }
    if(pcb->fd[file_d_num]->isread == TRUE){
        syscall_return(pcb, -EBADF); 
    }

    if(pcb->fd[file_d_num]->console == TRUE){
        int arg1 = pcb->my_registers[5];
    
        if (arg1 != 1 && arg1 != 2){   
            syscall_return(pcb, -EBADF);
        }
    
        int arg2 = pcb->my_registers[6];
        if (arg2 < 0) {
            syscall_return(pcb, -EFAULT);
        }

        if((arg2+pcb->my_registers[7]) > MemorySize){
            syscall_return(pcb,-EFBIG);
        }

        if(pcb->my_registers[7] < 0){
            syscall_return(pcb, -EINVAL);
        }

        P_kt_sem(writers);
        //pcb->my_registers[6]
        int my_local_reg6 = (int)(pcb->my_registers[6] + main_memory + pcb->base); // convert the second arg into system address
        
        int write_count = 0;
        char *chars = (char *)(my_local_reg6);
        char *j = chars;
        
        
        for(int i=0; i < pcb->my_registers[7]; i++){
            //printf("I write this %c", *j);
            console_write(*j);
            P_kt_sem(writeok);
            j++;
            write_count++;
        }

        

        V_kt_sem(writers);
        syscall_return(pcb, write_count);
    
    }else{

        int arg2 = pcb->my_registers[6];
        if (arg2 < 0) {
            syscall_return(pcb, -EFAULT);
        }

        if((arg2+pcb->my_registers[7]) > MemorySize){
            syscall_return(pcb,-EFBIG);
        }

        if(pcb->my_registers[7] < 0){
            syscall_return(pcb, -EINVAL);
        }

        //if(pcb->fd[file_d_num]->my_pipe->read_count==0){
            //no more readers
       //    syscall_return(pcb, -EBADF);
       // }
	P_kt_sem(writers);
        P_kt_sem(pcb->fd[file_d_num]->my_pipe->write);
        int my_local_reg6 = (int)(pcb->my_registers[6] + main_memory + pcb->base); // convert the second arg into system address

        
        int write_count = 0;
        char *chars = (char *)(my_local_reg6);
        char *j = chars;
        
        int start_point = pcb->fd[file_d_num]->my_pipe->write_head;

        for(int i=0; i < pcb->my_registers[7]; i++){

            //printf("I write this %c\n", *j);
            pcb->fd[file_d_num]->my_pipe->buffer[start_point] = *j;
            j++;
            write_count++;
            
            pcb->fd[file_d_num]->my_pipe->write_head = (pcb->fd[file_d_num]->my_pipe->write_head+1) % (8192);
            start_point += 1;
            P_kt_sem(pcb->fd[file_d_num]->my_pipe->space_available);

            if(pcb->fd[file_d_num]->my_pipe->read_count ==0){
                //no more readers
                V_kt_sem(pcb->fd[file_d_num]->my_pipe->write);
                //broken pipe error
                syscall_return(pcb, -EPIPE);
            }
            V_kt_sem(pcb->fd[file_d_num]->my_pipe->nelement);

            //pcb->fd[file_d_num]->my_pipe->writer_in_use += 1;
        }
        //printf("The write count is %d", write_count);
        pcb->fd[file_d_num]->my_pipe->writer_in_use += write_count;

        V_kt_sem(pcb->fd[file_d_num]->my_pipe->write);
        V_kt_sem(writers);

        syscall_return(pcb, write_count);
    }
}

void *do_read(void *arg)
{
    struct PCB *pcb = (struct PCB *)arg;

    
    //check if address is valid
    if(ValidAddress(arg) == FALSE){
        syscall_return(pcb, -EFAULT);
    }

    int file_d_num = pcb->my_registers[5];
    if(file_d_num<0 || file_d_num>=64){
        syscall_return(pcb, -EBADF); 
    }
    
    // printf("I'm batman\n");
    // printf("My pid is %d\n", pcb->pid);
    // printf("The read count is %d\n", pcb->fd[file_d_num]->my_pipe->read_count);
    // printf("The write count is %d\n", pcb->fd[file_d_num]->my_pipe->write_count);
    // printf("The status is %d\n", pcb->fd[file_d_num]->open);

    if(pcb->fd[file_d_num]->open == FALSE){
        syscall_return(pcb, -EBADF); 
    }
    if(pcb->fd[file_d_num]->isread == FALSE){
        syscall_return(pcb, -EBADF); 
    }

    if(pcb->fd[file_d_num]->console == TRUE){
        int arg1 = pcb->my_registers[5];
        if (arg1 != 0)
        {
            syscall_return(pcb, -EBADF);
        }
        int arg2 = pcb->my_registers[6];
        if (arg2 < 0)
        {
            syscall_return(pcb, -EFAULT);
        }
        //the third argument
        if(pcb->my_registers[7] < 0){
            syscall_return(pcb, -EINVAL);
        }


        P_kt_sem(readers);
        //int min = MIN(pcb->my_registers[7], buffer->size);
        int min = pcb->my_registers[7];
        int count = 0;
        for (int i = 0; i < min; i++){
            P_kt_sem(nelem);
            char toRead = (char)(buffer->buff[buffer->head]);

            if (toRead == -1){
                V_kt_sem(nslots); 
                V_kt_sem(readers);
                syscall_return(pcb, count);
            }

            ((char *)(pcb->my_registers[6] + main_memory + pcb->base))[i] = toRead;

            buffer->head = (buffer->head + 1) % (buffer->size);
            V_kt_sem(nslots); 
            count++;
        }
        //printf("what are you doing!!!");
        V_kt_sem(readers);
        //printf("Return statement!!!");
        syscall_return(pcb, count);
    }else{
        int arg2 = pcb->my_registers[6];
        if (arg2 < 0)
        {
            syscall_return(pcb, -EFAULT);
        }
        //the third argument
        if(pcb->my_registers[7] < 0){
            syscall_return(pcb, -EINVAL);
        }

        //printf("I'm batman\n");

        P_kt_sem(pcb->fd[file_d_num]->my_pipe->read);
        
        int starter_int = pcb->fd[file_d_num]->my_pipe->read_head;

        int min = pcb->my_registers[7];
        int count = 0;
        for (int i = 0; i < min; i++){
            if(pcb->fd[file_d_num]->my_pipe->write_count==0){
                //no more writers
                //V_kt_sem(pcb->fd[file_d_num]->my_pipe->space_available); 
                V_kt_sem(pcb->fd[file_d_num]->my_pipe->read);
                syscall_return(pcb, count);
            }

            if(count == pcb->fd[file_d_num]->my_pipe->writer_in_use){
                //printf("I'm batman\n");
                //printf("The number is %d\n", count);
                //no more writer, exit
                pcb->fd[file_d_num]->my_pipe->writer_in_use -= count;
                //V_kt_sem(pcb->fd[file_d_num]->my_pipe->space_available); 
                V_kt_sem(pcb->fd[file_d_num]->my_pipe->read);
                syscall_return(pcb, count);
            }

            P_kt_sem(pcb->fd[file_d_num]->my_pipe->nelement);
            char toRead = (char)(pcb->fd[file_d_num]->my_pipe->buffer[starter_int]);
            //printf("My starter_int is  %d\n", starter_int);
            //printf("I read this %c\n", pcb->fd[file_d_num]->my_pipe->buffer[starter_int]);
      
            if (toRead == -1){
                V_kt_sem(pcb->fd[file_d_num]->my_pipe->space_available); 
                V_kt_sem(pcb->fd[file_d_num]->my_pipe->read);
                syscall_return(pcb, count);
            }



            ((char *)(pcb->my_registers[6] + main_memory + pcb->base))[i] = toRead;

            pcb->fd[file_d_num]->my_pipe->read_head = (pcb->fd[file_d_num]->my_pipe->read_head+1)%(8192);
            starter_int += 1;

            V_kt_sem(pcb->fd[file_d_num]->my_pipe->space_available); 
            
            count++;
        }
        //printf("what are you doing!!!");
        V_kt_sem(pcb->fd[file_d_num]->my_pipe->read);
        //printf("Return statement!!!");
        syscall_return(pcb, count);
    }
}

void ioctl(void *arg){
    struct PCB *pcb=(struct PCB*)arg;

    //check whether the address is valid
    if(ValidAddress(arg) == FALSE){
        syscall_return(pcb, -EFAULT);
    }

    if (pcb->my_registers[5]!=1 || pcb->my_registers[6]!=JOS_TCGETP){
        syscall_return(pcb,-EINVAL);
    }

    //address of the third argument
    struct JOStermios *input= (struct JOStermios *)&main_memory[pcb->my_registers[7]+ pcb->base];
    ioctl_console_fill(input);
    //return zero
    syscall_return(pcb,0);
}

void fstat(void *arg)
{
    struct PCB *curr = (struct PCB *)arg;
    int fd = curr->my_registers[5];
    if(ValidAddress(arg) == FALSE){
        syscall_return(curr, -EFAULT);
    }
    struct KOSstat *kosstat = (struct KOSstat *)&main_memory[curr->my_registers[6]+ curr->base];
    
    if (fd == 0){
        stat_buf_fill(kosstat, 1);
    }
    if (fd == 2 || fd == 1){
        stat_buf_fill(kosstat, 256);
    }

    syscall_return(curr, 0);
}

void getpagesize(void *arg)
{
    struct PCB *curr = (struct PCB *)arg;
    syscall_return(curr, PageSize);
}

void do_sbrk(void *arg){
    struct PCB *curr = (struct PCB *)arg;
    
    //check address
    if (ValidAddress(arg) == FALSE){
        syscall_return(curr, -EFAULT);
    }

    //in case sbrk is not set
    if (curr->sbrk_ptr == NULL){
        curr->sbrk_ptr == 0;
    }

    int oldsbrk = (int)curr->sbrk_ptr;
    //get increment
    int add = curr->my_registers[5];

    if (oldsbrk + add > User_Limit){
        syscall_return(curr, -ENOMEM);
    }
    curr->sbrk_ptr += add;
    syscall_return(curr, oldsbrk);
}

bool ValidAddress(void *arg){
    //check the arg's address is valid or not
    struct PCB *pcb = (struct PCB*)arg;

    if (pcb->my_registers[RetAddrReg]>=0 && pcb->my_registers[RetAddrReg]<=pcb->limit){
        return TRUE;
    }
    return FALSE;
}

void do_execve(void *arg){
    struct PCB *pcb = (struct PCB*)arg;

    char **my_argv = (char **)(pcb->my_registers[6] + main_memory + pcb->base);
    //check how many arguments are there
    int numArg = 0;
    while (my_argv[numArg] != NULL){
        numArg += 1;
    }
    //include the null
    numArg += 1;

    //allock the memory for them
    char **array = malloc(numArg * sizeof(char *));
    char *filename = strdup(pcb->my_registers[5] + main_memory + pcb->base);

    int *offset;
    char *argArr;
    int j = 0;
    for (int i = 0; i < numArg - 1; i++){
        offset = (int *)(pcb->my_registers[6] + main_memory + pcb->base + j);
        argArr = (char *)(*offset+ main_memory + pcb->base);
        // + main_memory + pcb->base
        array[i] = strdup(argArr);
        j += 4;
    }
    array[numArg - 1] = '\0';

    int result = perform_execve(pcb, filename, array);

    free(filename);
    free(array);

    if(result == 0){
        //we work successfully
        //syscall_return(pcb, 0);
        execve_return(pcb, 0);
    }

    // pcb->my_registers[PCReg] = 0;
    // pcb->my_registers[NextPCReg] =  4;
    // pcb->my_registers[2] = 0;
    //syscall_return(pcb, -EINVAL);
    execve_return(pcb, -EINVAL);
}

void execve_return(struct PCB *pcb, int value)
{
    // printf("PCReg is %d\n", pcb->my_registers[PCReg]);
    // printf("NextPCReg is %d\n", pcb->my_registers[NextPCReg]);
    // printf("Reg2 is %d\n", pcb->my_registers[2]);
    pcb->my_registers[PCReg] = 0;
    pcb->my_registers[NextPCReg] =  4;
    pcb->my_registers[2] = value;
    dll_append(readyq, new_jval_v((void *)pcb));
    kt_exit();
}

void getpid(void *arg){
    struct PCB *pcb=(struct PCB*)arg;
    syscall_return(pcb, pcb->pid); 
}

void do_fork(void *arg){
    struct PCB *pcb=(struct PCB*)arg;

    struct PCB *newproc = (struct PCB*)malloc(sizeof(struct PCB));
    //write a for loop to check memory.
    bool Found = FALSE;
    for (int i = 0; i < 8; i++){
        if(memory_chunk[i]==0){
            
            //we get an empty chunk to use
            
            //setup memory
            memory_chunk[i] = 1;
            newproc->memory_num = i;
            newproc->base = i * User_Limit;
            newproc->limit = User_Limit;
            memcpy(main_memory+newproc->base,main_memory+pcb->base,User_Limit);


            //copy the registers
            for (int j=0; j < NumTotalRegs; j++){
                newproc->my_registers[j] = pcb->my_registers[j];
            }
            newproc->sbrk_ptr = pcb->sbrk_ptr;

            newproc->pid = get_new_pid();
            newproc->parent = pcb;
            newproc->waiters_sem = make_kt_sem(0);
            newproc->waiters = new_dllist();
            newproc->children = make_jrb();

            //insert child to parents' jrb
            jrb_insert_int(pcb->children, newproc->pid, new_jval_v((void*)newproc));

            
            for (int k = 0; k < 64; k++){
                newproc->fd[k] = (struct Fd*)malloc(sizeof(struct Fd));
                //printf("I'm batman\n");
                newproc->fd[k]->console = pcb->fd[k]->console;
                newproc->fd[k]->isread = pcb->fd[k]->isread;
                newproc->fd[k]->my_pipe = pcb->fd[k]->my_pipe;
                newproc->fd[k]->open = pcb->fd[k]->open;
                newproc->fd[k]->reference_count = pcb->fd[k]->reference_count;

                if(pcb->fd[k]->open == TRUE){
                    if(pcb->fd[k]->console==FALSE){
                        if(pcb->fd[k]->isread == TRUE){
                            pcb->fd[k]->my_pipe->read_count += 1;
                        }else{
                            pcb->fd[k]->my_pipe->write_count += 1;
                        }
                    }

                }
            }
            
            Found = TRUE;
            break;
        }
    }

    //printf("new pid is %d\n", newproc->pid);

    if(Found == FALSE){
        syscall_return(pcb, -EAGAIN);
    }else{
        kt_fork((void *)finish_fork, newproc);
        syscall_return(pcb, newproc->pid);
    }
}

void finish_fork(void *arg){
    struct PCB *newjob=(struct PCB*)arg;
    syscall_return(newjob, 0);
}

void do_exit(void *arg){
    struct PCB *curr=(struct PCB*)arg;
    curr->exit_value = curr->my_registers[5];

    memory_chunk[curr->memory_num] = 0;
    memset(main_memory + curr->base, 0, curr->limit);

    //add to waiter list
    // V_kt_sem(curr->parent->waiters_sem);
    // dll_append(curr->parent->waiters, new_jval_v((void*)curr));
    jrb_delete_node(jrb_find_int(curr->parent->children, curr->pid));

    //switch children to parents
    struct PCB* tmp_pcb;
    Dllist tmp_zombie;
	dll_traverse(tmp_zombie, curr->waiters){
		tmp_pcb = (struct PCB*)jval_v(dll_val(tmp_zombie));
		destroy_pid(tmp_pcb->pid);
		free(tmp_pcb);
	}
    while (!jrb_empty(curr->children)){
        struct PCB *pcb = (struct PCB *)jval_v(jrb_val(jrb_first(curr->children)));
        jrb_delete_node(jrb_find_int(curr->children, pcb->pid));
        pcb->parent = init;
        jrb_insert_int(init->children, pcb->pid, new_jval_v((void *)pcb));
    }
    
    //clean up zombie
    while (!dll_empty(curr->waiters)){
        struct PCB *wait_child = (struct PCB *)jval_v(dll_val(dll_first(curr->waiters)));
        wait_child->parent = init;
        dll_append(init->waiters, dll_val(dll_first(curr->waiters)));
        dll_delete_node(dll_first(curr->waiters));
        V_kt_sem(init->waiters_sem);
    }

   //close related fd table
    for (int i = 0; i < 64; i++){
        if(curr->fd[i]->console==FALSE){
            if(curr->fd[i]->open==TRUE){
                if(curr->fd[i]->isread==TRUE){
                    curr->fd[i]->my_pipe->read_count -= 1;
                    //curr->fd[i]->reference_count -= 1;
                }else{
                    curr->fd[i]->my_pipe->write_count-= 1;
                    //curr->fd[i]->reference_count -= 1;
                }

                if(curr->fd[i]->my_pipe->read_count == 0 && curr->fd[i]->my_pipe->write_count==0){
                    free(curr->fd[i]->my_pipe);
                }

                curr->fd[i]->open = FALSE;
            }
        }
        //free(curr->fd[i]);
    }

    //clean up
    if(curr->parent->pid == 0){
        //    //close related fd table
        // for (int i = 0; i < 64; i++){
        //     if(curr->fd[i]->console==FALSE){
        //         if(curr->fd[i]->open==TRUE){
        //             if(curr->fd[i]->isread==TRUE){
        //                 curr->fd[i]->my_pipe->read_count -= 1;
        //                 //curr->fd[i]->reference_count -= 1;
        //             }else{
        //                 curr->fd[i]->my_pipe->write_count-= 1;
        //                 //curr->fd[i]->reference_count -= 1;
        //             }

        //             if(curr->fd[i]->my_pipe->read_count == 0 && curr->fd[i]->my_pipe->write_count==0){
        //                 free(curr->fd[i]->my_pipe);
        //             }

        //             curr->fd[i]->open = FALSE;
        //         }
        //     }
        //     //free(curr->fd[i]);
        // }

        //if parent is init
        for (int i = 0; i < NumTotalRegs; i++){
            curr->my_registers[i] = 0;
        }
        destroy_pid(curr->pid);
        jrb_free_tree(curr->children);
        free_dllist(curr->waiters);
        free(curr);
    }else{
        V_kt_sem(curr->parent->waiters_sem);
        dll_append(curr->parent->waiters, new_jval_v((void*)curr));
    }
    kt_exit();
    //SYSHalt();
}

void getdtablesize(void *arg){
    struct PCB *curr = (struct PCB *)arg;
    syscall_return(curr, 64);
}

void do_close(void * arg){
    struct PCB *curr = (struct PCB *)arg;
    int fd_num = curr->my_registers[5];

    if(fd_num<0 || fd_num>=64){
        syscall_return(curr, -EBADF); 
    }

    if(curr->fd[fd_num]->open == FALSE){
        syscall_return(curr, -EBADF); 
    }

    //pay attention to consol!!!
    if(curr->fd[fd_num]->console == FALSE){
        if(curr->fd[fd_num]->isread == TRUE){
            curr->fd[fd_num]->my_pipe->read_count -= 1;
            curr->fd[fd_num]->open = FALSE;
            if(curr->fd[fd_num]->my_pipe->read_count == 0){
                //try a for loop to release all the writers
                for (int f = 0; f < curr->fd[fd_num]->my_pipe->write_count; f++){
                    V_kt_sem(curr->fd[fd_num]->my_pipe->space_available);
                }
                    
                //clean up
                if(curr->fd[fd_num]->my_pipe->write_count == 0){
                    free(curr->fd[fd_num]->my_pipe);
                }
            }
        }else{
            curr->fd[fd_num]->my_pipe->write_count -= 1;
            curr->fd[fd_num]->open = FALSE;
            if(curr->fd[fd_num]->my_pipe->write_count==0){
                if(curr->fd[fd_num]->my_pipe->read_count==0){
                    free(curr->fd[fd_num]->my_pipe);
                }
            }
        }
    }

    
    //syscall_return(curr, -EBADF);
    syscall_return(curr, 0);
}

void do_wait(void * arg){
    struct PCB *curr=(struct PCB*)arg;

    P_kt_sem(curr->waiters_sem);
    
    struct PCB *completed_child = (struct PCB *)(jval_v((dll_val(dll_first(curr->waiters)))));

    //clean up everything for child
    dll_delete_node(dll_first(curr->waiters));
    destroy_pid(completed_child->pid);
    for(int i=0; i<NumTotalRegs; i++){
        completed_child->my_registers[i] = 0;
    }
    free(completed_child->my_registers);
    jrb_free_tree(completed_child->children);
    free_dllist(completed_child->waiters);

    // for (int i = 0; i < 64; i++){
    //     if(completed_child->fd[i]->console==FALSE){
    //         if(completed_child->fd[i]->open==TRUE){
    //             if(completed_child->fd[i]->isread==TRUE){
    //                 completed_child->fd[i]->my_pipe->read_count -= 1;
    //             }else{
    //                 completed_child->fd[i]->my_pipe->write_count-= 1;
    //             }

    //             if(completed_child->fd[i]->my_pipe->read_count == 0 && completed_child->fd[i]->my_pipe->write_count==0){
    //                 free(completed_child->fd[i]->my_pipe);
    //             }

    //             completed_child->fd[i]->open = FALSE;
    //         }
    //     }
    //     //free(curr->fd[i]);
    // }

    int child_id = completed_child->pid;
    free(completed_child);


    syscall_return(curr, child_id);

    //SYSHalt();
}

void get_ppid(void * arg){
    struct PCB *curr=(struct PCB*)arg;
    syscall_return(curr, curr->parent->pid); 
}

void do_dup(void * arg){
    struct PCB *curr=(struct PCB*)arg;

    int old_fd = curr->my_registers[5];
    if(old_fd<0 || old_fd>=64){
        syscall_return(curr, -EBADF);
    }

    if(curr->fd[old_fd]->open==FALSE){
        syscall_return(curr, -EBADF);
    }

    //find lowest FD
    int new_fd = -1;
    int is_found = FALSE;
    for (int i = 0; i < 64; i++){
        if(curr->fd[i]->open==FALSE){
            new_fd = i;
            is_found = TRUE;
            break;
        }
    }
    if(is_found==FALSE){
        //return error
        syscall_return(curr, -EBADF);
    }

    curr->fd[old_fd]->reference_count += 1;
    curr->fd[new_fd] = curr->fd[old_fd];

    //increment pipe
    if(curr->fd[new_fd]->isread==TRUE){
        curr->fd[new_fd]->my_pipe->read_count += 1;
    }else{
        curr->fd[new_fd]->my_pipe->write_count += 1;
    }
    
    syscall_return(curr, new_fd);
}

void do_dup2(void * arg){
    struct PCB *curr=(struct PCB*)arg;

    int old_fd = curr->my_registers[5];
    if(old_fd<0 || old_fd>=64){
        syscall_return(curr, -EBADF);
    }

    if(curr->fd[old_fd]->open==FALSE){
        syscall_return(curr, -EBADF);
    }

    int new_fd = curr->my_registers[6];
    if(new_fd<0 || new_fd>=64){
        syscall_return(curr, -EBADF);
    }

    if(curr->fd[new_fd]->open==TRUE){
        //close it before dup2
        curr->fd[new_fd]->console = FALSE;

        curr->fd[new_fd]->my_pipe->read_count -= 1;
        curr->fd[new_fd]->my_pipe->write_count -= 1;

        curr->fd[new_fd]->my_pipe = NULL;
        curr->fd[new_fd]->open = FALSE;
        curr->fd[new_fd]->reference_count = 0;

    }


    curr->fd[old_fd]->reference_count += 1;
    curr->fd[new_fd] = curr->fd[old_fd];

    if(curr->fd[new_fd]->isread==TRUE){
        curr->fd[new_fd]->my_pipe->read_count += 1;
    }else{
        curr->fd[new_fd]->my_pipe->write_count += 1; 
    }

    syscall_return(curr, new_fd);
}

void do_pipe(void *arg){
    struct PCB *curr=(struct PCB*)arg;
    struct Pipe *new_pipe = malloc(sizeof(struct Pipe));

    //find read & write FD for both
    int read_Fd = -1;
    bool isFound1 = FALSE;
    for (int i = 0; i < 64; i++){
        if(curr->fd[i]->open==FALSE){
            read_Fd = i;
            isFound1 = TRUE;
            break;
        }
    }
    if (isFound1 == FALSE){
        syscall_return(curr, -EMFILE);
    }
    curr->fd[read_Fd]->open = TRUE;


    int write_Fd = -1;
    bool isFound2 = FALSE;
    for (int i = 0; i < 64; i++){
        if(curr->fd[i]->open==FALSE){
            write_Fd = i;
            isFound2 = TRUE;
            break;
        }
    }
    if (isFound2 == FALSE){
        syscall_return(curr, -EMFILE);
    }
    curr->fd[write_Fd]->open=TRUE;

    //initialization and set ref count
    new_pipe->buffer = malloc(8192*sizeof(char));

    new_pipe->read_count = 1;
    new_pipe->write_count = 1;
    
    new_pipe->read = make_kt_sem(1);
    new_pipe->write = make_kt_sem(1);
    new_pipe->nelement = make_kt_sem(0);
    new_pipe->space_available = make_kt_sem(8192);
    
    new_pipe->write_head = 0;
    new_pipe->read_head = 0;

    new_pipe->writer_in_use = 0;

    curr->fd[read_Fd]->my_pipe = new_pipe;
    curr->fd[read_Fd]->isread = TRUE;
    //curr->fd[read_Fd]->open = TRUE;

    curr->fd[write_Fd]->my_pipe = new_pipe;
    curr->fd[write_Fd]->isread = FALSE;
    //curr->fd[write_Fd]->open = TRUE;

    
    //set pipe[0] and pipe[1]
    memcpy(curr->my_registers[5] + main_memory + curr->base, &read_Fd, 4);
    memcpy(curr->my_registers[5] + main_memory + curr->base + 4, &write_Fd, 4);

    
    syscall_return(curr, 0);
}
void handle_interrupt(struct PCB *pcb){
	//DEBUG('e', "Interrupt handler (separate thread)");
}
void program_ready(struct PCB *pcb){ //TODO: maybe check for duplicates?
	Jval val = new_jval_v(pcb);
	dll_append(readyq, val);
}
