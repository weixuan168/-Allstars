#include "stdafx.h"
#include <stdio.h>
//#include <sys/time.h>
//#include <sys/resource.h>


#include "timer.h"


//static struct rusage res;
//static struct timeval tp;
//static double virtual_time, real_time;



void start_timers(void)
/*    
      FUNCTION:       virtual and real time of day are computed and stored to 
                      allow at later time the computation of the elapsed time 
		      (virtual or real) 
      INPUT:          none
      OUTPUT:         none
      (SIDE)EFFECTS:  virtual and real time are computed   
*/
{
	/* 
    getrusage( RUSAGE_SELF, &res );
    virtual_time = (double) res.ru_utime.tv_sec +
		   (double) res.ru_stime.tv_sec +
		   (double) res.ru_utime.tv_usec / 1000000.0 +
		   (double) res.ru_stime.tv_usec / 1000000.0;

    gettimeofday( &tp, NULL );
    real_time =    (double) tp.tv_sec +
		   (double) tp.tv_usec / 1000000.0;
		  */ 
}



double elapsed_time(TIMER_TYPE type)
/*    
      FUNCTION:       return the time used in seconds (virtual or real, depending on type) 
      INPUT:          TIMER_TYPE (virtual or real time)
      OUTPUT:         seconds since last call to start_timers (virtual or real)
      (SIDE)EFFECTS:  none
*/
{	
	/* 
    if (type == REAL) {
        gettimeofday( &tp, NULL );
        return( (double) tp.tv_sec +
		(double) tp.tv_usec / 1000000.0
		- real_time );
    }
    else {
        getrusage( RUSAGE_SELF, &res );
        return( (double) res.ru_utime.tv_sec +
		(double) res.ru_stime.tv_sec +
		(double) res.ru_utime.tv_usec / 1000000.0 +
		(double) res.ru_stime.tv_usec / 1000000.0
		- virtual_time );
    }
	/*/
	return 0;
}