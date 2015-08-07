#include "stdafx.h"

int main()
{	
	// As of now the redis server is hard coded
	int conn_timeout = 10, rw_timeout = 0;
	// Change this ip address/port to whatever the redis server is
	acl::string addr("135.44.219.210:6379");

	acl::acl_cpp_init();
	acl::log::stdout_open(true);
	acl::redis_client client(addr.c_str(), conn_timeout, rw_timeout);
	acl::redis_pubsub redis(&client);

	redis.clear();

	// Change this channel to whatever channel you want to read from
	acl::string channel = "channel";
	redis.subscribe(channel, NULL);

	acl::string msg;

	channel.clear();
	msg.clear();
	redis.clear();
	// When a message is published to the channel, grab it and print it to the console with the channel
	while(true){
		if ((redis.get_message(channel, msg)) == false)
		{
			printf("get_message error(%s)\r\n",
				redis.result_error());
			return false;
		}
		else 
			printf("message: %s, channel: %s\r\n",
				msg.c_str(), channel.c_str());
	}
#ifdef WIN32
	printf("enter any key to exit\r\n");
	getchar();
#endif
	return 0;
}

