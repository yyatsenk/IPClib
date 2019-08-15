#include <stdio.h>
#include <iostream>
#include <memory>
#include <thread>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <mutex>
#include <condition_variable>

#include <memory>

std::mutex mute;
std::condition_variable cond_var;

bool done = false;

class Server
{
	static Server *server;
	private:
		Server():opt(1),addrlen(sizeof(address)), hello(strdup("Hello from server"))
		{
			if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    		{ 
        		perror("Server: socket creation failed\n"); 
    		}
    		if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                                                  &opt, sizeof(opt))) 
    		{ 
        		perror("Server: setsockopt failed\n"); 
        		exit(EXIT_FAILURE); 
    		} 
    		address.sin_family = AF_INET; 
    		address.sin_addr.s_addr = INADDR_ANY; 
    		address.sin_port = htons( 8080 ); 
       
    // Forcefully attaching socket to the port 8080 
    if (bind(server_fd, (struct sockaddr *)&address,  
                                 sizeof(address))<0) 
    { 
        perror("Server: bind failed\n"); 
    } 
    if (listen(server_fd, 3) < 0) 
    { 
        perror("Server: listen failed\n"); 
    } 
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,  
                       (socklen_t*)&addrlen))<0) 
    { 
        perror("Server: accept failed\n"); 
    } 
		}
	public:
		void sendToClient()
		{
			std::unique_lock<std::mutex> lock(mute);
			for(int i = 0; i < 5; i++)
			{
				valread = read( new_socket , buffer, 1024); 
    			printf("%s\n",buffer ); 
    			send(new_socket , hello , strlen(hello) , 0 ); 
    			printf("Server: Hello message sent %d\n", i); 
    		}
    		done = true;
    		cond_var.notify_one();
		}
	~Server()
	{
		close(sockfd);
		delete server;
		delete hello;
	}
		static Server *createServerInstance()
		{
			if (!server)
				server = new Server();
			return server;
		}
	int server_fd;
	int sockfd;
	int new_socket;
	struct sockaddr_in address;
	int opt; 
    int addrlen; 
    char buffer[1024] = {0}; 
    char *hello;
    int valread;
};

Server *Server::server = 0;

class Client
{
public:
	Client():sock(-1), valread(-1), hello(strdup("Hello from client"))
	{
		if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    	{ 
        	printf("\nClient: Socket creation error \n"); 
    	} 
   
    	serv_addr.sin_family = AF_INET; 
    	serv_addr.sin_port = htons(8080); 
       
    // Convert IPv4 and IPv6 addresses from text to binary form 
   		 if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)  
    	{ 
        	printf("\nClient: Invalid address/ Address not supported \n"); 
    	} 
   
    	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    	{ 
        	printf("\n Client: Connection Failed \n"); 
    	} 
	}
	void reieveFromServer()
	{
		for(int i = 0; i < 5; i++)
		{
			send(sock , hello , strlen(hello) , 0 ); 
    		printf("Client: Hello message sent %d\n", i); 
    		valread = read( sock , buffer, 1024); 
    		printf("%s\n",buffer );
    	}
	}
	~Client()
	{
		close(sock);
		delete hello;
	}
private:
	int sock;
	int valread; 
    struct sockaddr_in serv_addr; 
    char *hello; 
    char buffer[1024] = {0}; 
};

//#include "SmartPtr.hpp"
#define DEBUG

#ifdef DEBUG
	#define sharedPtr std::shared_ptr
#else
	#include "SmartPtr.hpp"
#endif

void foo(sharedPtr<int> ptr)
{
	printf("5) use_count = %d\n\n", ptr.use_count());
}

int main()
{
	int *int_p = new int(100);
	int *int_p_2 = new int(200);

	sharedPtr<int> ptr(int_p);
	printf("1) ptr.use_count = %d\n\n", ptr.use_count());
	sharedPtr<int> ptr_2(ptr);
	printf("2) ptr.use_count = %d\n\n", ptr.use_count());
	{
		sharedPtr<int> ptr_3(ptr);
		printf("3) ptr.use_count = %d\n\n", ptr.use_count());
	}
	printf("4) ptr.use_count = %d\n\n", ptr.use_count());
	//sharedPtr<int> ptr(int_p);
	//sharedPtr<int> ptr_2(ptr);
	//ptr = int_p;
	
	//printf("ptr_2.use_count = %d\n\n", ptr_2.use_count());
	foo(ptr);
	printf("6) ptr.use_count = %d\n\n", ptr.use_count());

	sharedPtr<int> ptr_4(int_p_2);
	sharedPtr<int> ptr_5(ptr_4);
	ptr = ptr_4;
	printf("7) ptr.use_count = %d\n\n", ptr.use_count());
	printf("8) ptr_4.use_count = %d\n\n", ptr_4.use_count());
	printf("9) ptr.get = %d\n\n", *ptr.get());
	printf("10) ptr_4.get = %d\n\n", *ptr_4.get());
	printf("11) ptr_2.use_count = %d\n\n", ptr_2.use_count());
	/*std::thread waiting_thread([](){
		std::unique_lock<std::mutex> lock(mute);
		printf("Waiting_thread: i am waiting ...\n");
		while(!done)
			cond_var.wait(lock);
		printf("Waiting_thread: i am awake yeahh, all data recieved successfuly!\n");
	});
	std::thread server_thread([](){
		Server *serv = Server::createServerInstance();
		printf("Server: waiting for messages ...\n");
		serv->sendToClient();
	});
	std::thread client_thread([](){
		Client *client = new Client();
		sleep(3);
		printf("Client: here we go ...\n");
		client->reieveFromServer();
	});
	//Server *serv = Server::createServerInstance();
	//serv->sendToClient();
	if (waiting_thread.joinable())
		waiting_thread.join();

	if (server_thread.joinable())
		server_thread.join();

	if (client_thread.joinable())
		client_thread.join();
*/
	return (0);
}
