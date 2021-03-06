#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>  
#include <iostream>
#include <stdio.h>
#include <getopt.h>
#include <string>
#include <stdlib.h>
#include <signal.h>

#define BUFFER_SIZE 50



int main(int argc, char** argv){
	
	int sockfd, n;
	int port = 259990;
	unsigned int len;
	char* from = NULL;	
//	char* ack = "ack"; 
	struct ack {
		std::string msg; // "ack1\0"
		int num;
	};
//	char buffer[BUFFER_SIZE];
	struct msg {
		std::string buffer;
		int num;
	};
	sockaddr_in addr;
	
	msg msg_var;
	ack ack_var;
	
	char* nummsg;
	

	//option retrieving
	int option = 0;
	while((option = getopt(argc, argv, "-:,h:,f:,r:,w:,p:"))!=-1){
		switch(option){
			case '\1':break;
			case 'h' :from = optarg;					break;
			case 'a' :break;
			case 'f' :break;
			case 'r' :break;
			case 'w' :break;
			case 'p' :port = strtol(optarg, NULL, 10);  break;
			default  :return 1;		 					break;
		}
	}
	
	//create socket
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sockfd < 0){
		std::cerr<<"socket creation error"<<std::endl;
		return 1;
	}

	//initialize address
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);




	//bind socket
	if(bind(sockfd,(sockaddr*)&addr, sizeof(addr)) < 0){
		std::cerr<<"bind error"<<std::endl;
		return 1;
	}

	//main loop
	while(1){

		// ricezione messaggio in msg_var
		n=recvfrom(sockfd, static_cast<void*>(&msg_var), sizeof(msg), 0, (sockaddr*)&addr, &len);
		if(n < 0){
			std::cerr<<("recvfrom error")<<std::endl;
			return 1;
		}

		msg mess = msg_var;

		std::cout<<"message: "<<mess.buffer<<std::endl;
		std::cout<<"from:    "<<inet_ntoa(addr.sin_addr)<<std::endl;
		std::cout<<"on port: "<<port<<std::endl;

		std::cout<<"press a key to send the ack"<<std::endl;
    	std::cin.get(); 
    	
    	sprintf(nummsg,"%d",msg_var.num);
		
		// inizializzo ack_var, da inviare come risposta al server
		std::string s=("ack");
		s += std::to_string(msg_var.num);
		ack_var.msg=s;  // inizializzo ack_var con stringa "ack1", "ack2" ecc...
		ack_var.num=msg_var.num;
		
		// invio
		n = sendto(sockfd, static_cast<void*>(&ack_var), sizeof(ack), 0, (sockaddr*)&addr,  sizeof(addr));
		if(n < 0){
			std::cerr<<("recvfrom error")<<std::endl;
			return 1;
		}

		std::cout<<"ack sent... "<<std::endl;
	}

	return 0;
}





