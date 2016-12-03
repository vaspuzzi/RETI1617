#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>  
#include <iostream>
#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <string>

int main(int argc, char** argv){

	char* host = "127.0.0.1";
	int port = 259990;
	int sockfd;
	int len;
	char ack[3];
	std::string msg;
	sockaddr_in addr;

	//option retrieving
	int option = 0;
	while((option = getopt(argc, argv, "-:,f:,r:,w:,p:"))!=-1){
		switch(option){
			case '\1':host = optarg;					break;
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

		std::cout<<"write here the messagge"<<std::endl;
		std::getline(std::cin, msg, '\n');

		sockfd = socket(AF_INET, SOCK_DGRAM, 0);
		const char* mssg = msg.c_str();
		sendto(sockfd, mssg, sizeof(mssg), 0, (sockaddr*)&addr, sizeof(addr));

		std::cout<<"waiting for an ack"<<std::endl;
		recvfrom(sockfd, ack, 3, 0, (sockaddr*)&addr,  NULL);
		if(std::string(ack) == "ack")std::cout<<"ack received"<<std::endl;
	}
	return 0;
}





