#include "usbCAN.h"

bool usbCAN::status_open=false;

usbCAN::~usbCAN(){
	
	if (status_open == true){
			status_open = false;
			close_mod();
		}
	
}

usbCAN::usbCAN(){
		
	if (status_open == false){			
			status_open = true;
			open_mod();
		}
}

void usbCAN::open_mod(){
	DWORD EventMask;
	char * 	pcBufLD[MAX_DEVICES + 1];
	char 	cBufLD[MAX_DEVICES][64];	
	int	iNumDevs = 0;

	//inicializa as variáveis globais
	ftStatus=FT_OK; porta_aberta=FT_OK; escrita_feita=FT_OK;
	leitura_feita=FT_OK; baud_OK=FT_OK; dado_ok=FT_OK;
	
	//porta_aberta = FT_Open(0,&ftHandle);
	//porta_aberta = FT_Open(iport, &ftHandle);

	for(int i = 0; i < MAX_DEVICES; i++) {
		pcBufLD[i] = cBufLD[i];
	}

	porta_aberta = FT_ListDevices(pcBufLD, &iNumDevs, FT_LIST_ALL | FT_OPEN_BY_SERIAL_NUMBER);


	if (porta_aberta == FT_OK) {
			for(int i=0;( (i <MAX_DEVICES) && (i < iNumDevs) ); i++)
				porta_aberta = FT_OpenEx(cBufLD[i], FT_OPEN_BY_SERIAL_NUMBER, &ftHandle[i]);
	}
	if (porta_aberta == FT_OK) {
		printf("Porta aberta \n");
	}
	else {
		printf("Erro ao tentar abrir a porta \n");
		printf("1: Certifique-se que o driver esta ativado \n");
		printf("2: Certifique-se que o dispositivo esta conectado \n");
		printf("3: Certifique-se que o dispositivo esta alimentado \n");
		exit(1);
	}

	

	pthread_mutex_init(&eh.eMutex, NULL);
	pthread_cond_init(&eh.eCondVar, NULL);
	EventMask = FT_EVENT_RXCHAR;
	
	evento_OK = FT_SetEventNotification(ftHandle[0], EventMask, (PVOID)&eh);
	
	//determina o baud rate da transmissão (em bps)
	if((baud_OK = FT_SetBaudRate(ftHandle[0], 921600)) != FT_OK) {
		printf("ERRO: Baud nao setado\n");
		exit(1);
	}

}
void usbCAN::close_mod(){


	ftStatus = FT_Close(ftHandle[0]);

	if (ftStatus != FT_OK) {
	 //FT_Open failed
	printf("Erro ao tentar FECHAR a porta USB \n");
	exit(1);
	}
}
void usbCAN::read_mod(unsigned char* recepcao){

	DWORD RxBytes=0;
	DWORD BytesReceived;
	unsigned char RxBuffer[256];
	

	pthread_mutex_lock(&eh.eMutex);
	//Checa se a quantidade de bytes é compatível com o protocolo
	while ((RxBytes < 4) && (dado_ok == FT_OK)){
		pthread_cond_wait(&eh.eCondVar, &eh.eMutex);
		dado_ok = FT_GetQueueStatus(ftHandle[0], &RxBytes);
	}	
	
	pthread_mutex_unlock(&eh.eMutex);

	if(dado_ok != FT_OK){
		printf("Erro na leitura da USB\n");
		exit(1);
	}
	//Realiza a leitura do barramento usb
	leitura_feita = FT_Read(ftHandle[0],RxBuffer,RxBytes,&BytesReceived);
	
	if (leitura_feita != FT_OK) {
		// FT_Read Failed
		printf("Erro na leitura da USB\n");
		exit(1);
	}
	else{ //passa os valores lidos para a posição de memoria repassada
		for(int i=0; i<4; i++)
			recepcao[i] = RxBuffer[i];
	}

}
void usbCAN::write_mod(unsigned char word_length, unsigned char id, unsigned char* dado){
	
	DWORD BytesReceived;
	char TxBuffer[5];
	
//	TxBuffer[0] = word_length;
	TxBuffer[0] = id;
	for(int i=1; i<5; i++){
		TxBuffer[i] = dado[(i-1)];
	}

	if(porta_aberta==FT_OK) //realiza a escrita no barramento USB
		escrita_feita = FT_Write(ftHandle[0],TxBuffer,5,&BytesReceived);
	else{
		printf("ERRO: Impossível escrever na USB. Porta Fechada. \n");
		exit(1);
	}

	if (escrita_feita != FT_OK){
		// FT_Write ERRO
		printf("ERRO: Durante a escrita na USB. \n");
		exit(1);
	}
}
