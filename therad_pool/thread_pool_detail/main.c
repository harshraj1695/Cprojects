#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <signal.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>



// macros

#define PORT 2000
#define MAX_CLIENTS 15
#define BACKLOG MAX_CLIENTS*2
#define BUF 256





// global variables

struct sockaddr_in server_addr;
int server_fd;
pthread_cond_t cond;
pthread_mutex_t mutex;




// sigint handler

void sigint_handler(int signum){
	if(signum==SIGINT){
		close(server_fd);
		pthread_cond_destroy(&cond);
		pthread_mutex_destroy(&mutex);
		printf("ctrl + c detected\nterminating\n");
		exit(0);
	}

}






// queue data structure to store client fds

int queue[MAX_CLIENTS];
int rear=0;
int front=0;
int qs=0;		// no of elements in queue





// enqueue function
// enqueue will enqueue a client socket 
// will also notify a waiting thread

void enqueue(int client_fd){
	pthread_mutex_lock(&mutex);
	
	if(qs<MAX_CLIENTS){
		queue[rear]=client_fd;
		rear=(rear+1) % MAX_CLIENTS;
		qs++;
		pthread_cond_signal(&cond);
	}
	else{
		printf("queue is full, closing the client\n");
		close(client_fd);
	}
	
	pthread_mutex_unlock(&mutex);
}




// dequeue
// dequeue will dequeue a client socket and return it to the calling function
// will also use cond variable till queue has atleast one client, cond_wait

int dequeue(){
	
	pthread_mutex_lock(&mutex);
	
	while(qs==0){
		pthread_cond_wait(&cond, &mutex);
	}
	
	int client_fd = queue[front];
	front = (front + 1) % MAX_CLIENTS;
	qs--;
	
	pthread_mutex_unlock(&mutex);
	return client_fd;

}






// pool of thread function dequeue a client and talk

void* pool_of_threads(void* arg){

	while(1){
		int client_fd = dequeue();
		
		if(client_fd == -1)continue;
		
		printf("thread %d handling a client \n", (int)pthread_self());
		
		char buf[BUF];
		while(1){
			
			memset(buf, 0, BUF);
			int bytes_read = recv(client_fd, buf, BUF, 0);
			
			if(bytes_read == -1){
				printf("some error receiving this message...continuing...\n");
				continue;
			}
			
			else if(bytes_read == 0){
				printf("client called an unconditional termination\n");
				break;
			}
			
			buf[bytes_read]='\0';
			
			printf("msg received: %s\n",buf);
			
			
			if(strcmp(buf, "close")==0){
				printf("close detected\nclosing...\n");
				break;
			}
		}
		
		close(client_fd);
		printf("client closed\n");
	}
	
	return NULL;
}




// acceptor() function will accept clients and enqueue them


void acceptor(){
	
	while(1){
		struct sockaddr_in client_addr;
		socklen_t client_len = sizeof(client_addr);
		int client_fd = accept(server_fd , (struct sockaddr*)&client_addr, &client_len);
		
		if(client_fd == -1){
			printf("error accepting client...continuing\n");
			continue;
		}
		char ip[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &client_addr.sin_addr, ip, sizeof(ip));
		printf("client : %s at port %d accepted\n",ip,ntohs(client_addr.sin_port));
		
		enqueue(client_fd);
	}
		
	
}



int main(){

	// ctrl+c handler, for graceful termination
	signal(SIGINT, sigint_handler);
	
	
	
	// initialising mutex and conditional varibales
	pthread_cond_init(&cond, NULL);
	pthread_mutex_init(&mutex, NULL);
	
	
	// setting up the server address
	memset(&server_addr,0, sizeof(server_addr));
	server_addr.sin_family=AF_INET;
	server_addr.sin_port=htons(PORT);
	server_addr.sin_addr.s_addr= INADDR_ANY;
	
	
	// making the socket
	server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(server_fd==-1){
		printf("error creating server socket\n");
		return 1;
	}
	
	
	// setting up port reuse
	int opt=1;
	int sock_opt_ret = setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	if(sock_opt_ret==-1){
		printf("setting port reuse for server failed\ncontinuing anyway\n");
	}
	
	
	
	
	// binding the address to the file descriptor
	int bind_ret = bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
	if(bind_ret==-1){
		printf("error in bind.. returning..\n");
		close(server_fd);
		return 2;	
	}
	
	printf("server bound to port : %d\n", ntohs(server_addr.sin_port));
	
	
	
	
	// listening for incoming client requests
	int listen_ret = listen(server_fd, BACKLOG);
	if(listen_ret==-1){
		printf("error in lsitening for clients\nexiting\n");
		close(server_fd);
		return 3;
	}
	
	
	printf("listening\n");
	
	
	
	
	
	
	pthread_t thread[10];
	
	for(int i=0; i<10; i++){
		
		int thread_create_ret=pthread_create(&thread[i], NULL, &pool_of_threads, NULL);
		if(thread_create_ret==-1){
			printf("error in creatign thread no %d\ncontinuing..\n", i);
		}
		
	}
	
	
	acceptor();
	
	
	// destroying mutexes and server_fd
	pthread_cond_destroy(&cond);
	pthread_mutex_destroy(&mutex);
	close(server_fd);

	return 0;
}








