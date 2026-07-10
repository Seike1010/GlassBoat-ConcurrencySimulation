#include <stdio.h> 
#include <stdlib.h> 
#include <pthread.h> 
#include <semaphore.h> 
#include <unistd.h> 
#include <time.h>  

// Shared resources 
sem_t boat_sem; 
pthread_mutex_t logbook_mutex; 

// Shared Memory 
const char* namelist[3]; 
int list_index = 0; // Name on Logbook 
int ticket_counter = 1; // Ticketing system 

// Tourist Action 
void* tourist_action(void* arg) { 
    const char* tourist_name = (const char*)arg; 

    // Simulate random walking time to the reception (0 to 2 seconds)
    // To break the order of thread creation
    int walk_time = rand() % 3;  
    sleep(walk_time);  
    printf("[%s] Arrives at the reception after walking for %d minutes.\n", tourist_name, walk_time);

    // 1. Queue, sign, and take a ticket
    pthread_mutex_lock(&logbook_mutex); 
    printf("   -> [%s] Locks the reception logbook.\n", tourist_name); 

    // Assign ticket and update the shared counter inside the mutex 
    int my_ticket = ticket_counter; 
    ticket_counter++; 
    printf("   -> [%s] signs the logbook and receives Ticket #%d...\n", tourist_name, my_ticket); 

    // Write to shared memory logbook
    namelist[list_index] = tourist_name; 
    list_index++;
    printf("   -> [%s] Finished signing, unlocks the logbook and walks to the dock.\n", tourist_name); 
    pthread_mutex_unlock(&logbook_mutex); 

    // 2. Wait for the glass boat at the dock 
    printf("[%s] is waiting at the dock for the glass boat (Holding Ticket #%d)...\n\n", tourist_name, my_ticket); 
    int boat_status;
    sem_getvalue(&boat_sem, &boat_status);
    if (boat_status == 0) {
        printf(" -> [Status Check] %s sees the boat is OCCUPIED. Must wait in line.\n\n", tourist_name);
    }
    sem_wait(&boat_sem);  

    // 3. Board and enjoy the ride
    printf("===============================================================================\n\n"); 
    printf("[%s] Ticket #%d checked successfully, boarding the glass boat!\n", tourist_name, my_ticket);  
    printf("[%s] is taking photos and enjoying the glass boat ride for 30 minutes\n", tourist_name); 
    sleep(3);  

    // Return the boat to the system 
    printf("[%s] Ride finished, steps off the glass boat.\n", tourist_name);
    printf(" -> [Staff] is cleaning and inspecting the boat for 1 minute...\n");
    sleep(1);
    printf(" -> [Staff] Cleaning done! Waking up the next person in queue.\n\n"); 
    sem_post(&boat_sem); 

    return NULL;

} 

 

int main() { 
    // Initialize random seed based on current time 
    srand(time(NULL));  

    printf("Group Members ID: CST2309677, CST2309261, CST2309176\n"); 
    printf("===============================================================================\n\n"); 
    pthread_t tourists[3];
    const char* tourist_names[3] = {"Xin Yue", "Jessica", "Wanen"}; 

    // Initialize semaphore: Boat is limited to 1 person, currently occupied
    sem_init(&boat_sem, 0, 0); 

    // Initialize mutex 
    pthread_mutex_init(&logbook_mutex, NULL); 
    printf("=== Start of Operation Day: The glass boat is currently occupied by an early bird tourist. ===\n\n"); 

    // Create 3 tourist threads 
    for (int i = 0; i < 3; i++) { 
        pthread_create(&tourists[i], NULL, tourist_action, (void*)tourist_names[i]); 
    } 

    // Early bird occupying the boat 
    printf("[Early Bird] is currently using the glass boat (sleep 3s). Other tourists must wait...\n"); 
    sleep(3); 

    // Early bird returns the boat 
    printf("\n[Early Bird] Ride finished, returns the glass boat, and officially opens the queue!\n\n"); 
    sem_post(&boat_sem);  

    // Main thread waits at the exit until all tourist threads safely finish 
    for (int i = 0; i < 3; i++) { 
        pthread_join(tourists[i], NULL); 
    } 

    // Print boarding log 
    printf("=========================================================\n"); 
    printf("=== Final Boarding Log for Today ===\n"); 
    for (int i = 0; i < 3; i++) { 
        printf("Boarding Ticket #%d: Tourist %s\n", i + 1, namelist[i]); 
    }
    printf("=========================================================\n\n"); 

    // Clean up resources 
    sem_destroy(&boat_sem); 
    pthread_mutex_destroy(&logbook_mutex); 
    printf("=== All tourists have safely left. The glass boat attraction is closed for today. ===\n");     

    return 0; 

} 