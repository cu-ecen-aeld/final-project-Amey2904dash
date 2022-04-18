/*
 * @file_name   :   multi_thread_safe.c
 *
 * @brief       :   Sample implementation of multi_thread_safe
 *                  reentrant functions using mutex lock
 *                  & unlock
 *
 * @references  :   1. Linux man pages
 *                  2. http://mercury.pr.erau.edu/~siewerts/cec450/code/rt_simplethread/pthread.c
 */

#define _GNU_SOURCE
/*-------------------------------------------------------------------------------*/
/*                                 LIBRARY FILES                                 */
/*-------------------------------------------------------------------------------*/
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <sched.h>
#include <time.h>
#include <sys/sysinfo.h>
#include <sys/types.h>
#include <unistd.h>
#include <syslog.h>
#include <math.h>
#include <time.h>
#include <errno.h>
/*-------------------------------------------------------------------------------*/
/*                                 MACROS                                        */
/*-------------------------------------------------------------------------------*/
#define ERROR               (-1)
#define SUCCESS             (0)
#define DEBUG               (1) //set this to 1 to enable printfs in error debug
#define TOTAL_THREADS       (2)
#define ONE_SECOND          (1000000)

/*-------------------------------------------------------------------------------*/
/*                           GLOBAL VARIABLES                                    */
/*-------------------------------------------------------------------------------*/
pthread_t           threads[TOTAL_THREADS];             //array of threads
pthread_attr_t      rt_sched_attr[TOTAL_THREADS];       //array for attributes of threads
int                 rt_max_priority,                    //maximum priority of scheduler
                    rt_min_priority;                    //maximum priority of scheduler
struct sched_param  rt_param[TOTAL_THREADS];            //array of threads for scheduler parameters
struct sched_param  main_param;                         //main process scheduler parameters
pthread_attr_t      main_attr;                          //main process attributes
pid_t               mainpid;                            //main process pid
int fib, fib0,fib1, idx = 0, jdx = 0;

pthread_mutex_t mutex_lock = PTHREAD_MUTEX_INITIALIZER; //to initialize mutex lock and unlock


#define FIB_TEST(seqCnt, iterCnt)      \
   for(idx=0; idx < iterCnt; idx++)    \
   {                                   \
      fib = fib0 + fib1;               \
      while(jdx < seqCnt)              \
      {                                \
         fib0 = fib1;                  \
         fib1 = fib;                   \
         fib = fib0 + fib1;            \
         jdx++;                        \
      } 				\
      jdx = 0;                         \
   }                                   \


/*-------------------------------------------------------------------------------*/
/* @brief       :   send_data_to_lcd
 *
 * @parameters  :   threadp     :   pointer to the data structure
 *
 * @returns     :   None
 */
/*-------------------------------------------------------------------------------*/

void* send_data_to_lcd(void *threadp)
{
    struct timespec time = {0, 0};  //for timestamp
    int status; //error check variable
    int i = 0;
    /*-------------------------------------------------------------------------------*/
    while (i++ < 10)
    {
        status = clock_gettime(CLOCK_REALTIME, &time); //get timestamp
        if (status == ERROR)
        {
            syslog(LOG_ERR, "Error!! clock_gettime() failed\n");
            #if DEBUG
                printf("Error!! clock_gettime() failed\n");
            #endif
            exit(EXIT_FAILURE);
        }
        /*-------------------------------------------------------------------------------*/


        FIB_TEST(47,1000);

        /*-------------------------------------------------------------------------------*/
       
        /*-------------------------------------------------------------------------------*/
        usleep(ONE_SECOND);
    }
    pthread_exit(0);
}

/*-------------------------------------------------------------------------------*/
/* @brief       :   socket
 *
 * @parameters  :   threadp     :   pointer to the data structure
 *
 * @returns     :   None
 */
/*-------------------------------------------------------------------------------*/
void* socket(void *threadp)
{
    int i = 0;
    while(i++ < 10)
    {
        //lock the global data structure
       


        FIB_TEST(47,1000);

        //unlock the global data structure
       
        usleep(ONE_SECOND);
    }
    pthread_exit(0);
}
/*-------------------------------------------------------------------------------*/
/* @brief       :   Main function. Application entry point                                      */
/*-------------------------------------------------------------------------------*/
int main()
{
    //open log for error logging
    openlog("thread_safe_reentrant", LOG_PID, LOG_USER);

    int status, idx;

    mainpid = getpid();

    rt_max_priority = sched_get_priority_max(SCHED_FIFO);
    rt_min_priority = sched_get_priority_min(SCHED_FIFO);

    status=sched_getparam(mainpid, &main_param);
    if(status == ERROR)
    {
        syslog(LOG_ERR, "Error!! sched_setscheduler() failed\n");
        #if DEBUG
            printf("Error!! sched_setscheduler() failed\n");
        #endif
        exit(EXIT_FAILURE);
    }
    main_param.sched_priority=rt_max_priority;

    /*-------------------------------------------------------------------------------*/
    status = sched_setscheduler(getpid(), SCHED_FIFO, &main_param);
    if(status == ERROR)
    {
        if (status == EINVAL)
        {
         	printf("EINVAL\n");
        }
        if (status == EPERM)
        {
         	printf("EPERM\n");
        }
        if (status == ESRCH)
        {
         	printf("ESRCH\n");
        }
        
        
        exit(EXIT_FAILURE);
    }
    /*-------------------------------------------------------------------------------*/
    for(idx=0; idx < TOTAL_THREADS; idx++)
    {
       status=pthread_attr_init(&rt_sched_attr[idx]);
       if (status != SUCCESS)
        {
            syslog(LOG_ERR, "Error!! pthread_attr_init() failed with error code %d\n", status);
            #if DEBUG
                printf("Error!! pthread_attr_init() failed with error code %d\n", status);
            #endif
            exit(EXIT_FAILURE);
        }
        /*-------------------------------------------------------------------------------*/
        status=pthread_attr_setinheritsched(&rt_sched_attr[idx], PTHREAD_EXPLICIT_SCHED);
        if (status != SUCCESS)
        {
            syslog(LOG_ERR, "Error!! pthread_attr_setinheritsched() failed with error code %d\n", status);
            #if DEBUG
                printf("Error!! pthread_attr_setinheritsched() failed with error code %d\n", status);
            #endif
            exit(EXIT_FAILURE);
        }
        /*-------------------------------------------------------------------------------*/
        status=pthread_attr_setschedpolicy(&rt_sched_attr[idx], SCHED_FIFO);
        if (status != SUCCESS)
        {
            syslog(LOG_ERR, "Error!! pthread_attr_setschedpolicy() failed with error code %d\n", status);
            #if DEBUG
                printf("Error!! pthread_attr_setschedpolicy() failed with error code %d\n", status);
            #endif
            exit(EXIT_FAILURE);
        }
        /*-------------------------------------------------------------------------------*/
        rt_param[idx].sched_priority=rt_max_priority-idx-1;
        pthread_attr_setschedparam(&rt_sched_attr[idx], &rt_param[idx]);
        if (status != SUCCESS)
        {
            syslog(LOG_ERR, "Error!! pthread_attr_setschedparam() failed with error code %d\n", status);
            #if DEBUG
                printf("Error!! pthread_attr_setschedparam() failed with error code %d\n", status);
            #endif
            exit(EXIT_FAILURE);
        }
        /*-------------------------------------------------------------------------------*/

        if (idx == 0)
        {
            status = pthread_create(&threads[idx],      // pointer to thread descriptor
                    &rt_sched_attr[idx],                // use SPECIFIC SECHED_FIFO attributes
                    socket,                  // thread function entry point
                    (void *)0            // parameters to pass in
                    );
            if (status != SUCCESS)
            {
                syslog(LOG_ERR, "Error!! pthread_create() failed with error code %d\n", status);
                #if DEBUG
                    printf("Error!! pthread_create() failed with error code %d\n", status);
                #endif
                exit(EXIT_FAILURE);
            }
        }

        if (idx == 1)
        {
            status = pthread_create(&threads[idx],      // pointer to thread descriptor
                    &rt_sched_attr[idx],                // use SPECIFIC SECHED_FIFO attributes
                    send_data_to_lcd,                   // thread function entry point
                    (void *)0             // parameters to pass in
                    );
            if (status != SUCCESS)
            {
                syslog(LOG_ERR, "Error!! pthread_create() failed with error code %d\n", status);
                #if DEBUG
                    printf("Error!! pthread_create() failed with error code %d\n", status);
                #endif
                exit(EXIT_FAILURE);
            }
        }

    }
    /*-------------------------------------------------------------------------------*/
    for(idx=0;idx<TOTAL_THREADS;idx++)
    {
       pthread_join(threads[idx], NULL);
       if (status != SUCCESS)
       {
            syslog(LOG_ERR, "Error!! pthread_create() failed with error code %d\n", status);
            #if DEBUG
                printf("Error!! pthread_create() failed with error code %d\n", status);
            #endif
            exit(EXIT_FAILURE);
       }
    }
    /*-------------------------------------------------------------------------------*/
    closelog();
    return SUCCESS;
}
