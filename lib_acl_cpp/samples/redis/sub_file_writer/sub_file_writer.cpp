#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <list>
#include <string>
#include <stdlib.h>
using namespace std;

int main(int argc, char** argv)
{
	// This is the address of the redis server
	if(argc != 2){
		printf("Usage: [ip:port] of redis server\n");
		exit(1);
	}

	int conn_timeout = 10, rw_timeout = 0;
	acl::string addr(argv[1]);

	acl::acl_cpp_init();
	acl::log::stdout_open(true);
	acl::redis_client client(addr.c_str(), conn_timeout, rw_timeout);
	acl::redis_pubsub redis(&client);

	// This subscribe file listens to the server/channel set and then writes to a local text document
	ofstream myfile;
	// This path will change for wherever you want to store the ip addresses
	myfile.open("/var/www/test.com/js/txt.txt");

	redis.clear();

	acl::string channel = "channel";
	redis.subscribe(channel, NULL);

	acl::string msg;

	channel.clear();
	msg.clear();
	redis.clear();

	// If a message is received then write to the text file. The local html reads the text file for printing the ip addresses
	while(true){
		if ((redis.get_message(channel, msg)) == false)
		{
			printf("get_message error(%s)\r\n",
				redis.result_error());
			return false;
		}
		else {
			if(myfile.is_open()){
				myfile << msg.c_str() << "\n";
				myfile.flush();
			}
			else printf("not open\n");
				
		}	
	}
	myfile.close();
#ifdef WIN32
	printf("enter any key to exit\r\n");
	getchar();
#endif
	return 0;
}

