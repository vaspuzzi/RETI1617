#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>  
#include <iostream>
#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <signal.h>

#define TIMEOUT=3;

bool resend_flag=false; 	// flag impostata true nel signalhandler

void sighandler (int signal) {
	resend_flag =false;
}


int main(int argc, char** argv){

	char* host = "127.0.0.1";
	int port = 259990;
	int sockfd;
	int len;
	struct ack {
		char msg[4];
		int num;
	};
	//std::string msg;
	struct msg {
		int num; 				// numerazione del pacchetto
		char messaggio[50];		// messaggio
	};
	sockaddr_in addr;
	bool arq=false;
	
	msg msg_var;
	ack ack_var;
	
	
	
	int i=0;
	

	//option retrieving
	int option = 0;
	while((option = getopt(argc, argv, "-:,a:,f:,r:,w:,p:"))!=-1){
		switch(option){
			case '\1':host = optarg;					break;
			case 'a' :arq = true; break;  
			case 'f' :break;
			case 'r' :break;
			case 'w' :break;
			case 'p' :port = strtol(optarg, NULL, 10);  break;
			default  :return 1;		 					break;
		}
	}
	
	std::cout<<"sending to: "<<host<<std::endl;
	std::cout<<"on port:	"<<port<<std::endl;
	while(true){
		
		//initialize address
		addr.sin_family = AF_INET;
		addr.sin_port = htons(port);
		inet_aton(host, &addr.sin_addr);

		// inizializzo messaggio da inviare;
		std::cout<<"write here the message"<<std::endl;
		std::getline(std::cin, msg_var.messaggio, '\n');
		msg_var.num=i;
		

		sockfd = socket(AF_INET, SOCK_DGRAM, 0);
		
		signal(SIGALRM,sighandler);
		
//		const char* mssg = msg.messaggio.c_str();
		sendto(sockfd, msg_var, sizeof(msg), 0, (sockaddr*)&addr, sizeof(addr));

		std::cout<<"waiting for an ack"<<std::endl;
		
		
		
		if(arq) {
			
			while(true)	{
			//	signal(SIGALRM,sighandler);
				alarm(TIMEOUT);
				recvfrom(sockfd, ack_var, sizeof(ack), 0, (sockaddr*)&addr,  NULL);			
				if(resend_flag ==false && strcmp(ack_var.msg,(strcat("ack",ack_var.num)) == 0   ) { // ack ricevuto in tempo e con giusti valori
					std::cout<<"ack"<<ack_var.num<<" received"<<std::endl;
					i++; // tutto ok passa al prossimo messaggio
					break;
					}
				else 
				// rimanda il messaggio
					sendto(sockfd, msg_var, sizeof(msg), 0, (sockaddr*)&addr, sizeof(addr));
			}
							
		}
		else
		//messaggio ricevuto
			if(strcmp(ack_var.msg,(strcat("ack",ack_var.num)) == 0) {
				std::cout<<"ack"<<ack_var.num<<" received"<<std::endl;
				i++; // ok passo al prossimo;
		
		
	}
	return 0;
}





