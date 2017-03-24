BASEDIR = /home/rbritto/kapeck
CC = g++
CXX = g++
CPPFLAGS = -Wall -I ${BASEDIR}/prototipo9/myIncludes
CFLAGS = -O
LDFLAGS = -L ${BASEDIR}/prototipo9/myLibs
LDLIBS = -lpthread -lmatrix -lssocket -lftd2xx -lssocketU -lxfig
TARGETS = busArbiter posCon kapeck1 actionPl_pathPl visionSys kapeck2 bBoardServer

all: $(TARGETS) 

visionSys: visionSys.o semaphore.o periodic.o

busArbiter: busArbiter.o semaphore.o Motores.o usbCAN.o

posCon: posCon.o controle.o semaphore.o  robot.o periodic.o

kapeck1: kapeck1.o semaphore.o

kapeck2: kapeck2.o semaphore.o

actionPl_pathPl: actionPl_pathPl.o Ambiente.o Mapa.o Planejamento.o Porta.o semaphore.o

bBoardServer: bBoardServer.o semaphore.o 

#servidorSemaphore: servidorSemaphore.o semaphore.o

#teste_CAN: teste_CAN.o robot.o semaphore.o periodic.o

#kapeck2: kapeck2.o semaphore.o

#testePeriodico: testePeriodico.o 

#testePeriodico_alt: testePeriodico_alt.o periodic.o

#teste: teste.o Motores.o usbCAN.o

clean:
	rm -f *~ *.bak *.o

realclean: clean
	rm -f $(TARGETS)
