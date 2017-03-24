/***************************************************************************
 *            robot.h
 *
 *  Fri Aug  3 09:47:07 2007
 *  Copyright  2007  User Ricardo de Sousa Britto
 *  Email nephlinvocals@yahoo.com.br
 ****************************************************************************/

#ifndef _ROBOT_H
#define _ROBOT_H

#include <sys/errno.h>
#include <sys/types.h>
#include <iostream>
#include "ssocketU.h"
#include "dados.h"
#include "semaphore.h"

using namespace::std;
	
class robot{

  private:
	unsigned char tipo_operacao;
	tcpSocketU s;
  	semaphore sem;
  	int n;
      
  public:
	robot(const char* ip, int key); 
  	~robot(void);
  	void close(void);
    void onEngines(VELOCIDADES refs);
	void onEngine_L(float ref);
	void onEngine_R(float ref);
	struct ENCODERS readEncoders(void);
	float readEncoder_L(void);
	float readEncoder_R(void);
};


#endif /* _ROBOT_H */
