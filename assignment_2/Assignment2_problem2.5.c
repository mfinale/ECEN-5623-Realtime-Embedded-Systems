

#include <fcntl.h> 
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <unistd.h>
const char *semName = "my_semaphore";
void parent(void){
    sem_t *sem_id = sem_open(semName, O_CREAT, 0600, 0); //open reference to semaphore
   

   //check if open to semaphore failed
   if (sem_id == SEM_FAILED){
        perror("Parent  : [sem_open] Failed\n"); return;
    }
	//print statement from parent
    printf("Parent  : Wait for Child to Print\n");
    //wait for semaphore to be set by child process.	
    sem_wait(sem_id);
    printf("Parent  : Child Printed! \n");
    sem_close(sem_id);
    sem_unlink(semName);

}

void child(void)
{
    sem_t *sem_id = sem_open(semName, O_CREAT, 0600, 0);
    if (sem_id == SEM_FAILED){
        perror("Child   : [sem_open] Failed\n"); return;        
    }
    sleep(12);
    printf("Child   : I am done! Release Semaphore\n");
    if (sem_post(sem_id) < 0)
        printf("Child   : [sem_post] Failed \n");
}
int main(int argc, char *argv[])
{
    pid_t pid; //create a pid_t datatype 
    pid = fork();
    if (pid < 0){
        perror("fork");
        printf("error on fork\n");
    }
    if (!pid){
        child();
        printf("Child   : Done with sem_open \n");
    }
    else{
        int status;
        parent();
        wait(&status);
        printf("Parent  : Done with sem_open \n");
    }
    return 0;

}
