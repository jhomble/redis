#include <iostream>
#include <string>
#include "stdafx.h"
#include "tcpacceptor.h"

int main(int argc, char** argv)
{
	int conn_timeout = 10, rw_timeout = 0;
	
	// As of now this is to choose the server to connect to
	if(argc != 3){
		printf("Usage: server <port> [<ip>]\n");
		exit(1);
	}

	TCPStream* stream = NULL;
    TCPAcceptor* acceptor = NULL;
       
    acceptor = new TCPAcceptor(atoi(argv[1]), argv[2]);

    // This is hard coded to the server thats running the redis server
	acl::string addr("135.44.219.148:6380");
	
	acl::acl_cpp_init();
	acl::log::stdout_open(true);
	acl::redis_client client(addr.c_str(), conn_timeout, rw_timeout);
	acl::redis_pubsub redis(&client);
	// Hard coded channel 
	acl::string channel = "channel", msg;

	redis.clear();

	// Have this program run continuously, and if an ip address is picked up, publish it to the redis server
    if (acceptor->start() == 0) {
        while (1) {   	
            stream = acceptor->accept();
            if (stream != NULL) {
		    	printf("IP %s\n", stream->getPeerIP().c_str());	
		    	msg.format("%s", stream->getPeerIP().c_str());
		    	redis.publish(channel, msg, msg.length());
			}
		}
	}


#ifdef WIN32
	printf("enter any key to exit\r\n");
	getchar();
#endif
	return 0;
}

