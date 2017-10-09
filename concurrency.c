/* 
 * Group 34
 * Xiaoyi Yang, Jiaxu Li
 * project 1: conccurrency: the producer-consumer
 * CS444
 * reference: Stack Overflow
 * */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include "mt19937ar.c"
#include "stdbool.h"
#include <unistd.h>

#define BUFFER_SIZE 32


/*Functions prototype*/
void *producer(void *dummy);
void *consumer(void *dummy);
bool is_empty(int index);

/*Struct buffer-builder, first value is a simple 
 *number, second value is random time*/
struct Buffer_builder {
   int value;
   int time;
};

/*global value: mutual exclusion mutex 
 * and pthread condition for prod and cons*/
pthread_mutex_t mutex;
pthread_cond_t producer_condition, consumer_condition;

struct Buffer_builder buffer[BUFFER_SIZE];

int main(int argc, char **argv)
{
   printf("Program start.");
   pthread_t prod, cons;
   init_genrand(1);

   for (int i = 0; i < 32; i++){
      buffer[i].value = 0;
      buffer[i].time = 0;
   }

   /*pthread init*/
   pthread_mutex_init(&mutex, NULL);
   pthread_cond_init(&producer_condition, NULL);
   pthread_cond_init(&consumer_condition, NULL);

   pthread_create(&prod, NULL, producer, NULL);
   pthread_create(&cons, NULL, consumer, NULL);

   pthread_join(prod, NULL);
   pthread_join(cons, NULL);

   pthread_mutex_destroy(&mutex);
   pthread_cond_destroy(&producer_condition);
   pthread_cond_destroy(&consumer_condition);


}

bool is_empty(int i)
{
   if(buffer[i].time == 0 || buffer[i].value == 0)
      return true;
   return false;

}

void *producer(void *dummy)
{
   /*Value define before get into loop*/
   int index = 0;

   while (true){
      /*Wait time between 3-7, then put them into sleep.*/
      int P_wait_time = genrand_int32() % 5 + 3;
      printf("Sleep for %i seconds before producing.\n", P_wait_time);
      sleep(P_wait_time);
      
      /*generating random number*/
      int value = genrand_int32() % 100 + 1;
      int wait = genrand_int32() % 8 + 2;
      pthread_mutex_lock(&mutex);

      while (buffer[index].value != 0)
	 pthread_cond_wait(&producer_condition, &mutex);

      pthread_mutex_unlock(&mutex);

      /*to make sure this buffer does not contain anything*/
      if (is_empty(index)){
	 pthread_mutex_lock(&mutex);

	 buffer[index].value = value;
	 buffer[index].time = wait;

	 if (index < 32)
	    index += 1;
	 else
	    index = 0;
      }
   }

   pthread_cond_signal(&consumer_condition);
   pthread_mutex_unlock(&mutex);

}

void *consumer(void *dummy)
{
   /*Value define before get into loop*/
   int index = 0;
   while (true){
      /*Make sure this buffer is not empty*/
      if (!is_empty(index)){
	 pthread_mutex_lock(&mutex);

	 while (buffer[index].value == 0)
	    pthread_cond_wait(&consumer_condition, &mutex);

	 pthread_mutex_unlock(&mutex);
	 sleep(buffer[index].time);
	 printf("Comsumer consumed %i and slept for %i seconds\n", 
	       buffer[index].value, buffer[index].time);
	 
	 pthread_mutex_lock(&mutex);
	 buffer[index].value = 0;
	 buffer[index].time = 0;

	 if (index < 32)
	    index += 1;
	 else
	    index = 0;

	 pthread_cond_signal(&producer_condition);
	 pthread_mutex_unlock(&mutex);
      }
   
   }

}
