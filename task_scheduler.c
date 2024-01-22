#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_INPUT 50
#define DEBUG 0

// Enumeration for task states
enum TaskState {
    RUNNING,
    READY,
    WAITING
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
    while (current != NULL && (current->task_pri < newTask->task_pri)) {
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

// Function to read tasks from a text file and create tasks
void readTasksFromFile(const char* filename, struct Task** waitingQueue, struct Task** readyQueue, struct Task** runningQueue) {
    FILE* file = fopen(filename, "r");
    
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }
    struct Task** queue = NULL;
    char line[MAX_INPUT];  // Adjust the buffer size as needed


    // Read tasks from the file
    //while (!feof(file)) {
    while(fgets(line, sizeof(line), file) != NULL){
        
        struct Task* newTask = (struct Task*)malloc(sizeof(struct Task));
        if (newTask == NULL) {
            perror("Error allocating memory for new task");
            exit(EXIT_FAILURE);
        }

        
        char task_state_str[20];
        // Read task fields from the file
        if (sscanf(line, "%d,%d,%d,%s", &newTask->task_id, &newTask->task_pri, &newTask->event_id,task_state_str) ) {
            newTask->ptr_next_task = NULL;
        
        if (strcmp(task_state_str, "RUNNING") == 0) {
            newTask->task_state = RUNNING;
            queue=runningQueue;
        } else if (strcmp(task_state_str, "WAITING") == 0) {
            newTask->task_state = WAITING;
            queue=waitingQueue;
        } else if (strcmp(task_state_str, "READY") == 0) {
            newTask->task_state = READY;
            queue=readyQueue;
        } else {
            fprintf(stderr, "Invalid task state: %s\n", task_state_str);
            continue;
        }

            // Add the task to the queue
           
            addTaskToQueue(queue, newTask);
            
        } else {
            
            free(newTask); // Discard the task if reading fails
        }
    }

    fclose(file);
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
            return 0;
    } 
    else if(previous == NULL){
        *queue = current->ptr_next_task;
    }
    else {
        
        previous->ptr_next_task = current->ptr_next_task;
    }
    
    return 1;
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

// Function to move a task from one queue to another
void switchQueue(struct Task** queue_from, struct Task** queue_to,int task_id, int event_id, enum TaskState state){
    struct Task* current = *queue_from;
    int status;
    while (current != NULL && (current->task_id != task_id)){
        current = current->ptr_next_task;
    }
    
    if(current == NULL){
        fprintf(stderr,"Cannot move task with Task_ID: %d , Task does not exist\n",task_id);
    } else {
         current->event_id = event_id;
         current->task_state = state;
         status = deleteTaskFromQueue(queue_from, task_id);  
         addTaskToQueue(queue_to, current);
    }


}

// Function to move tasks from waiting to ready queue on the occurence of an event
void Triggerevent(struct Task** waitingQueue, struct Task** readyQueue, int event_id){
        struct Task* current = *waitingQueue;
        int task_id;
        while (current != NULL){//move all tasks which are triggered by event to ready queue
                if(current->event_id == event_id){
                        task_id=current->task_id;
                        current = current->ptr_next_task;
                        switchQueue(waitingQueue,readyQueue,task_id,0,READY);
                       
                        
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
        
        if(readyQueue != NULL){
            if(running == NULL){
                switchQueue(readyQueue,runningQueue,current->task_id,0,RUNNING);
            }
            else{
                switchQueue(runningQueue,readyQueue,running->task_id,0,READY);
                switchQueue(readyQueue,runningQueue,current->task_id,0,RUNNING);
            }
        }
        else{
            printf("Ready Queue is empty");
        }
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
    
    
    // Specify the path init_tasks.txt
    const char* filename = "init_tasks.txt";
   
    // Read tasks from the text file and create tasks
    readTasksFromFile(filename, &waitingQueue,&readyQueue,&runningQueue);

    while(1){
    
    printf("\nn <task_id>            - create a new task");
    printf("\nd <task_id>            - delete task from ready/waiting queue");
    printf("\nw <task_id> <event_id> - Move task from Ready to Waiting queue");
    printf("\ne <event_id>           - Trigger the event of event_id");
    printf("\ns <event_id>           - Suspend running task with event_id");
    if (DEBUG) 
    printf("\np                      - Display all Queues");
    printf("\nexit                   - Exit\n");
    // defining buffer
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
        switchQueue(&readyQueue, &waitingQueue,*id,*(id+1),WAITING);
    }

    else if(*token == 'e'){
        token = strtok(NULL, " ");
       
        Triggerevent(&waitingQueue, &readyQueue, atoi(token));
        runningState(&runningQueue, &readyQueue);
    }

    else if(*token == 's'){
        token = strtok(NULL, " ");
        *id = atoi(token);
         switchQueue(&runningQueue,&waitingQueue,runningQueue->task_id,*id,WAITING);
         runningState(&runningQueue, &readyQueue);
    }

    else if(*token == 'p'){
        if(DEBUG){
         displayQueue("WaitingQueue",waitingQueue);
         displayQueue("ReadyQueue",readyQueue);
         displayQueue("RunningQueue",runningQueue);
         *inputLine = '\n';
        }
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
           
