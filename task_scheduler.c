#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_INPUT 50

// Enumeration for task states
enum TaskState {
    RUNNING,
    WAITING,
    READY
};

// Task structure
struct Task {
    int task_id;
    int task_pri;
    void* ptr_context;
    enum TaskState task_state;
    int event_id;
    struct Task* ptr_next_task;
};

struct Task* createTask(int task_id, int task_pri, void* ptr_context, enum TaskState task_state, int event_id) {
    struct Task* newTask = (struct Task*)malloc(sizeof(struct Task));
    if (newTask != NULL) {
        newTask->task_id = task_id;
        newTask->task_pri = task_pri;
        newTask->ptr_context = ptr_context;
        newTask->task_state = task_state;
        newTask->event_id = event_id;
        newTask->ptr_next_task = NULL;
    }
    return newTask;
}

// Function to add a task to the priority queue based on task_pri
void addTaskToQueue(struct Task** queue, struct Task* newTask) {
    struct Task* current = *queue;
    struct Task* previous = NULL;

    // Find the correct position in the queue based on task_pri
    while (current != NULL && current->task_pri < newTask->task_pri) {
        previous = current;
        current = current->ptr_next_task;
    }

    // Insert at the correct position in the queue
    if (previous == NULL) {
        newTask->ptr_next_task = *queue;
        *queue = newTask;
    } else {
        previous->ptr_next_task = newTask;
        newTask->ptr_next_task = current;
    }
}

// Function to add a task to the priority queue based on task_pri
int deleteTaskFromQueue(struct Task** queue, int task_id) {
    struct Task* current = *queue;
    struct Task* previous= NULL;


    // Find the position in the queue based on task_pri
    while ((current != NULL) && (current->task_id != task_id )) {
        previous = current;
        current = current->ptr_next_task;
    }

    // delete task from the queue
    if (current == NULL) {
          //   printf("Task with Task_ID: %d does not exist\n",task_id);
             return 0;
    } else if((previous == NULL) && (current == *queue)){//queue with one Task
            *queue = NULL;
      
    }
    else {
        previous->ptr_next_task = current->ptr_next_task;
    }
    
    return 1;
}

// Function to move a task from one queue to another
void switchQueue(struct Task** queue_from, struct Task** queue_to,int task_id){
    struct Task* current = *queue_from;
    int status;
    while (current != NULL && (current->task_id != task_id)){
        current = current->ptr_next_task;
    }

    if(current == NULL){
        fprintf(stderr,"Cannot move task with Task_ID: %d , Task does not exist\n",task_id);
    } else {
         addTaskToQueue(queue_to, current);
         status = deleteTaskFromQueue(queue_from, task_id);

    }


}

// Function to move tasks from waiting to ready queue on the occurence of an event
void Triggerevent(struct Task** waitingQueue, struct Task** readyQueue, int event_id){
        struct Task* current = *waitingQueue;
        printf("check");
        while (current != NULL){//move all tasks which are triggered by event to ready queue
                if(current->event_id == event_id){
                        printf("event  matched");
                        switchQueue(waitingQueue,readyQueue,current->task_id);
                        current = current->ptr_next_task;
                }
                else{
                        current = current->ptr_next_task;
                }
        }
    printf("Done trigger");
}

//Function to move task to running state
void runningState(struct Task** runningQueue,struct Task** readyQueue ){
        struct Task* current = *readyQueue;
        struct Task* running = *runningQueue;
        int status;
        
        if(running == NULL || ((current->task_pri) < (running->task_pri)) ){//move highest priority task in ready queue to running
                switchQueue(readyQueue,runningQueue,current->task_id);
                status = deleteTaskFromQueue(readyQueue, current->task_id);
        }
}

// Function to display tasks in a queue
void displayQueue(const char* queueName, struct Task* queue) {
    printf("%s Queue:\n", queueName);
    while (queue != NULL) {
        printf("Task ID: %d, Priority: %d, State: %d, Event ID: %d\n", queue->task_id, queue->task_pri, queue->task_state, queue->event_id);
        queue = queue->ptr_next_task;
    }
    printf("\n");
}


int main()
{

    // Initialize priority queues
    struct Task* runningQueue = NULL;
    struct Task* waitingQueue = NULL;
    struct Task* readyQueue = NULL;
    int id[3];
    char inputLine[MAX_INPUT];
    char* token=NULL;
    int status;
 

    while(1){
    
    printf("\nn <task_id>            - create a new task");
    printf("\nd <task_id>            - delete task from ready/waiting queue");
    printf("\nw <task_id> <event_id> - Move task from Ready to Waiting queue");
    printf("\ne <event_id>           - Trigger the event of event_id");
    printf("\ns <event_id>           - Suspend running task with event_id");
    printf("\np                      - Display all Queues");
    printf("\nexit                   - Exit\n");
    
    // using fgets to take input from stdin
    printf("# ");

    // Read the line from the user
        
        if (fgets(inputLine, sizeof(inputLine), stdin) == NULL ) {
            // Handle end-of-file or error
            break;
        }
        if(*inputLine == '\n'){
        continue;
        }
        token = strtok(inputLine, " ");


    if(*token == 'n'){
      token = strtok(NULL, " ");

      if((token == NULL) || (*token == ' ')){
         fprintf(stderr,"No task id provided");
         break;
      }
        printf("Please provide event_id for new Task :");
        scanf("%d",id);
        printf("Please provide priority of the new Task :");
        scanf("%d",(id+1));
        addTaskToQueue(&waitingQueue, createTask(atoi(token),*(id+1),NULL,WAITING,*id));
        if(*fgets(inputLine, sizeof(inputLine), stdin) == '\n'){
        continue;
        }
    }

    else if(*token == 'd'){
        token = strtok(NULL, " ");
        status = deleteTaskFromQueue(&readyQueue,atoi(token));
        if(!status)
            status = deleteTaskFromQueue(&waitingQueue,atoi(token));
        if(!status)
            printf("Task with Task_ID: %d , Task does not exist\n",atoi(token));
    }

    else if(*token == 'w'){
        token = strtok(NULL, " ");
        *id = atoi(token);
        token = strtok(NULL, " ");
        *(id+1) = atoi(token);
        switchQueue(&readyQueue, &waitingQueue,*id);
    }

    else if(*token == 'e'){
        token = strtok(NULL, " ");
       
        Triggerevent(&waitingQueue, &readyQueue, atoi(token));
        runningState(&runningQueue, &readyQueue);
    }

    else if(*token == 's'){

         switchQueue(&runningQueue,&readyQueue,runningQueue->task_id);
         runningState(&runningQueue, &readyQueue);
    }

    else if(*token == 'p'){
         displayQueue("WaitingQueue",waitingQueue);
         displayQueue("ReadyQueue",readyQueue);
         displayQueue("RunningQueue",runningQueue);
         *inputLine = '\n';
    }
    else if(!(strcmp(token,"exit"))){
            printf("Exiting");
            exit(0);
     }
   
   }
   free(runningQueue);
   free(waitingQueue);
   free(readyQueue);


}
           
