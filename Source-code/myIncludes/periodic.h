/***************************************************************************
 *            periodic.h
 *
 *  Tue Aug  7 14:52:01 2007
 *  Copyright  2007  User Ricardo de Sousa Britto
 *  Email nephlinvocals@yahoo.com.br
 ****************************************************************************/

#ifndef _PERIODIC_H
#define _PERIODIC_H

#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#define DEZNA6 1000000
//A unidade de tempo utilizado é "milisegundos"
class periodic{
	
	private:
		
		long sleept,tp;
		int j, g;
       	//long t0;
       	struct timespec ts;
       	struct timespec tsr;
       	struct timeval tv;
    	struct timezone tz;
		
		
	public:
		
		periodic(void);
		~periodic(void);
		long setStart(void);
		long setSleep(long time, long t0);
		long agora(long t);
};

#endif /* _PERIODIC_H */
