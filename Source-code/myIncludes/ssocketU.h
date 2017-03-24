#ifndef _SOCKETU_H_
#define _SOCKET$_H_

#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/select.h>

// Os tipos de erro

typedef enum
{
  SOCKET_OK=0,
  SOCKET_JA_EM_USO=-9,
  SOCKET_NAO_CRIADO=-10,
  SOCKET_NAO_FECHADO=-11,
  SOCKET_HOST_INVALIDO=-12,
  SOCKET_NAO_ACEITANDO=-13,
  SOCKET_NAO_CONECTADO=-14,
  SOCKET_ERRO_BIND=-15,
  SOCKET_ERRO_LISTEN=-16,
  SOCKET_ERRO_ACCEPT=-17,
  SOCKET_ERRO_CONEXAO=-18,
  SOCKET_ERRO_ENVIO=-19,
  SOCKET_ERRO_RECEPCAO=-20,
  SOCKET_EM_USO=-21,
  SOCKET_FILA_VAZIA=-22,
  SOCKET_ERRO_SELECT=-24,
  SOCKET_DADO_VAZIO=-25,
  SOCKET_TIMEOUT=1
} SOCKET_STATUS;

// Os tipos de sockets

typedef enum
{
  SOCKET_TCP=1,
  SOCKET_TCP_SERV=2,
  SOCKET_UDP=3,
  SOCKET_UDP_SERV=4,
  SOCKET_DESCONHECIDO=-1
} SOCKET_TIPO;

typedef enum
{
  SOCKET_IDLE=0,
  SOCKET_ACCEPTING=1,
  SOCKET_CONNECTED=2
} SOCKET_STATE;

// Variáveis globais

extern void (*socketU_error)(SOCKET_STATUS err, const char *msg);

// Predefinição das classes
//class fsocketU;
class tcpSocketU;
class tcpSocketUServidor;
//class udpSocketU;

/* #############################################################
   ##  A classe base dos sockets                              ##
   ############################################################# */

class ssocketU
{
 private:
  int id;
  SOCKET_STATE state;
 public:
  // Construtor e destrutor por default
  ssocketU();
  inline virtual ~ssocketU() {close();}
  virtual SOCKET_TIPO type() const = 0;
  // Fecha (caso esteja aberto) um socket
  SOCKET_STATUS close();
  // Testa se um socket é "virgem" ou foi fechado
  inline bool closed() const {return (id==-1 && state==SOCKET_IDLE);}
  // Testa se um socket está aberto (aceitando conexões)
  inline bool accepting() const {return (id>=0 && state==SOCKET_ACCEPTING);}
  // Testa se um socket está conectado (pronto para ler e escrever)
  inline bool connected() const {return (id>=0 && state==SOCKET_CONNECTED);}
  // Imprime um socket
  friend std::ostream& operator<<(std::ostream& os, const ssocketU &);

  friend class tcpSocketUServidor;
  friend class tcpSocketU;
  //friend class udpSocketU;
  //friend class fsocketU;
};

/* #############################################################
   ##  As classes dos sockets orientados a conexão (TCP)      ##
   ############################################################# */

class tcpSocketUServidor: public ssocketU
{
 public:
  // O tipo de socket
  inline SOCKET_TIPO type() const {return SOCKET_TCP_SERV;}
  // Abre um novo socket para esperar conexões
  // Só pode ser usado em sockets "virgens" ou explicitamente fechados
  SOCKET_STATUS listen(const char *name, int nconex=1);
  // Aceita uma conexão que chegou em um socket aberto
  // Retorna um socket conectado (não-conectado em caso de erro)
  SOCKET_STATUS accept(tcpSocketU&) const;
};

class tcpSocketU: public ssocketU
{
 public:
  // O tipo de socket
  inline SOCKET_TIPO type() const {return SOCKET_TCP;}
  // Se conecta a um socket aberto
  // Só pode ser usado em sockets "virgens" ou explicitamente fechados
  SOCKET_STATUS connect(const char *name);
  // Escreve em um socket conectado
  SOCKET_STATUS write(const void *, size_t) const;
  // Lê de um socket conectado
  SOCKET_STATUS read(void*, size_t) const;
};

/* #############################################################
   ##  A classe dos sockets baseados em datagramas (UDP)    ##
   ############################################################# */
/*
#define MSG_INITIAL "UDP_First"

class udpSocketU: public ssocketU
{
 public:
  // O tipo de socket
  inline SOCKET_TIPO type() const {return SOCKET_UDP;}
  // Abre um novo socket para esperar conexões
  // Só pode ser usado em sockets "virgens" ou explicitamente fechados
  SOCKET_STATUS listen(uint16_t port);
  // Aceita uma conexão que chegou em um socket aberto
  // Muda o estado do socket se a conexão foi bem sucedida
  SOCKET_STATUS accept();
  // Se conecta a um socket aberto
  // Só pode ser usado em sockets "virgens" ou explicitamente fechados
  SOCKET_STATUS connect(const char *name, uint16_t port);
  // Escreve em um socket conectado
  SOCKET_STATUS write(const void *, size_t) const;
  // Lê de um socket conectado
  SOCKET_STATUS read(void*, size_t) const;
};*/

/* #############################################################
   ##  A fila de sockets                                      ##
   ############################################################# */
/*
class fsocketU
{
 private:
  fd_set set;
  int nmax;
 public:
  inline fsocketU() {clean();}
  ~fsocketU();
  void clean();
  // Adiciona um socket a uma fila de sockets
  SOCKET_STATUS include(const ssocketU &a);
  // Retira um socket de uma fila de sockets
  SOCKET_STATUS exclude(const ssocketU &a);
  // Bloqueia até haver alguma atividade de leitura em socket da fila
  SOCKET_STATUS wait_read(long milisec=-1);
  // Bloqueia até haver alguma atividade de conexão em socket da fila
  inline SOCKET_STATUS wait_connect(long milisec=-1) {
    return wait_read(milisec);}
  // Bloqueia até haver alguma atividade de escrita em socket da fila
  SOCKET_STATUS wait_write(long milisec=-1);
  // Testa se houve atividade em um socket específico da fila
  bool had_activity(const ssocketU &a);
};
*/
#endif
