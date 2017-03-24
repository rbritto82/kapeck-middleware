#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include "semaphore.h"

#if defined(__GNU_LIBRARY__) && !defined(_SEM_SEMUN_UNDEFINED)
/* union semun is defined by including <sys/sem.h> */
#else
/* according to X/OPEN we have to define it ourselves */
typedef union {
  int val;                  /* value for SETVAL */
  struct semid_ds *buf;     /* buffer for IPC_STAT, IPC_SET */
  unsigned short *array;    /* array for GETALL, SETALL */
  /* Linux specific part: */
  struct seminfo *__buf;    /* buffer for IPC_INFO */
} semun;
#endif

bool semaphore::stop_on_errors=true;

semaphore::semaphore(void)
{
  id = 0;
}

void semaphore::set_stop_on_errors(void)
{
  stop_on_errors=true;
}

void semaphore::unset_stop_on_errors(void)
{
  stop_on_errors=false;
}

int semaphore::create(int key, int flags1, int flags2)
{
  id = semget(key, 1, (flags1 & 0777)|flags2);
  if (id<0)
    {
      if (stop_on_errors)
	{
	  perror("semaphore create");
	  exit(errno);
	}
      else return(errno);
    }
  return(0);
}

int semaphore::create_only(int key, int flags, int val)
{
  int result = create(key,flags,IPC_CREAT|IPC_EXCL);
  if (result) return(result);
  return(set_value(val));
}

int semaphore::connect_only(int key)
{
  return(create(key,0666,0));
}

int semaphore::connect_or_create(int key, int flags, int val)
{
  // Tenta só conectar (sem criar)
  // Não pode chamar a connect_only existente porque ela interromperia
  // em caso de erro, não dando chance de tentar criar.
  id = semget(key, 1, (flags & 0777));
  if (id>=0) return(0);
  // Tenta criar (sem conectar)
  return(create_only(key,flags,val));
}

int semaphore::set_value(int val)
{
  semun arg;

  arg.val = val;
  if (semctl(id, 0, SETVAL, arg)<0)
    {
      if (stop_on_errors)
	{
	  perror("semaphore set value");
	  exit(errno);
	}
      else return(errno);
    }
  return(0);
}

int semaphore::remove(void)
{
  if (semctl(id, 0, IPC_RMID)<0)
    {
      if (stop_on_errors)
	{
	  perror("semaphore remove");
	  exit(errno);
	}
      else return(errno);
    }
  id = 0;
  return(0);
}

int semaphore::changevalue(int val, bool block, const char *msg)
{
  if (val!=0)
    {
      sembuf op[1];

      op[0].sem_num = 0;
      op[0].sem_op = val;
      op[0].sem_flg = (block ? 0 : IPC_NOWAIT);
      if (semop(id, op, 1)<0)
	{
	  if (stop_on_errors)
	    {
	      perror(msg);
	      exit(errno);
	    }
	  else return(errno);
	}
    }
  return(0);
}

int semaphore::increment(int val, bool block)
{
  return(changevalue(abs(val),block,"semaphore increment"));
}

int semaphore::decrement(int val, bool block)
{
  return(changevalue(-abs(val),block,"semaphore decrement"));
}

int semaphore::test_zero(bool block)
{
  return(changevalue(0,block,"semaphore test zero"));
}
