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


#define TIMEOUT 3

bool resend_flag=false; 	// flag impostata true nel signalhandler

void sighandler (int signal) {
	resend_flag =false;
}

struct msg {
	int num; 						// numerazione del pacchetto
	std::string messaggio;				// messaggio
};

struct ack {
		char msg[4];
		int num;
	};


int main(int argc, char** argv){

	char* host = "127.0.0.1";
	int port = 259990;
	int sockfd;
	
	//std::string msg;
	
	sockaddr_in addr;
	
	// flag -a
	bool arq=false;
	
	msg msg_var;
	ack ack_var;
	
	//stringhe di supporto
	char* nummsg;
	
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
		std::string m;
		std::cout<<"write here the message"<<std::endl;
		
		std::getline(std::cin, m, '\n');

		msg_var.messaggio = m;
		msg_var.num=i;
		

		sockfd = socket(AF_INET, SOCK_DGRAM, 0);
		
		signal(SIGALRM,sighandler);
		
//		const char* mssg = msg.messaggio.c_str();
		sendto(sockfd, static_cast<void*>(&msg_var), sizeof(msg), 0, (sockaddr*)&addr, sizeof(addr));

		std::cout<<"waiting for an ack"<<std::endl;
		
		
		if(arq) {
			
			while(true)	{
			//	signal(SIGALRM,sighandler);
				alarm(TIMEOUT);
				recvfrom(sockfd, static_cast<void*>(&ack_var), sizeof(ack), 0, (sockaddr*)&addr,  NULL);	
				
						
				std::string ack("ack");
				ack += std::to_string(ack_var.num);
				if(resend_flag ==false && ack_var.msg == ack ) { // ack ricevuto in tempo e con giusti valori
					std::cout<<"ack"<<ack_var.num<<" received"<<std::endl;
					
		
					i++; // tutto ok passa al prossimo messaggio
					
					break;
				}
				else 
				// rimanda il messaggio	
					sendto(sockfd, static_cast<void*>(&msg_var), sizeof(msg), 0, (sockaddr*)&addr, sizeof(addr));
			}
							
		}
		else{
		//messaggio ricevuto
			std::string ack1("ack");
			ack1 += std::to_string(ack_var.num);
			if(ack_var.msg == ack1 == 0) {
				std::cout<<"ack"<<ack_var.num<<" received"<<std::endl;
				i++; // ok passo al prossimo;
			}
		}
		
	}
	return 0;
}





