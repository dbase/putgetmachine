# include <stdio.h>
# include <stdlib.h>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <netinet/in.h>
# include <netdb.h>
# include <string.h>
# include <unistd.h>
# include <fcntl.h>
# include <errno.h>

/*(c) 2014 lancelion*/

//Usage string.
const char* help_string = "How-to-use: ./putgetmachine [-4|-6 IP VERSION] [-p PUT] [-d DESTPORT] [-o OUTPUT] REMOTEHOST REMOTEFILE [LOCAL FILE]\nExample-GET : ./putgetmachine www.foo.com /index.html\nExample-PUT : ./putgetmachine -p www.foo.com /index.html local.txt\n";

//Error function.
void errExit(const char* str, char p)
{
        if ( p != 0 )
        {
                perror(str);
        } else
        {
                fprintf(stderr,str);
        }
        exit(1);
}


//Main program function
int main (int argc, char** argv)
{	

        struct addrinfo *result, hints;
	// Defining variables for socket, error handling, output file and ai_family.
        int srvfd, rwerr = 42, outfile, ai_family = AF_UNSPEC;
	// Defining variables for request string, buffer, ports and .
	char *request, *requestput, putflag, buf[16], port[6],c;
	

	//Making 'port[6] = 000000
        memset(port,0,6);
	//Less than three arguments leads to failure.
        if ( argc < 3 )
                errExit(help_string,0);
        //If no port defined, default is 80.
        strncpy(port,"80",2);
	//Loop below is taken from github.com/dermesser under GNU license and edited for own purposes.
	//Wait for argument for if 'd' and 'o' if they are used. Other parameters does not need extra arguments.
        while ( (c = getopt(argc,argv,"d:po:46")) >= 0 )
        {
                switch (c)
                {
			//User gives a target port.
                        case 'd' :
                                strncpy(port,optarg,5);
                                break;
			//User asks for PUT.
                        case 'p' :
                                putflag++;
                                break;
			//User defines an output file.
                        case 'o' :
                                outfile = open(optarg,O_WRONLY|O_CREAT,0644);
                                close(1);
                                dup2(outfile,1);
                                break;
			//User initiates IPv4 mode.
                        case '4' :
                                ai_family = AF_INET;
				printf("IPV4 ON!\n");
                                break;
			//User initiates IPv6 mode.
                        case '6' :
                                ai_family = AF_INET6;
				printf("IPV6 ON!\n");
                                break;
                }
        }
	//Filling hints with 0's.
        memset(&hints,0,sizeof(struct addrinfo));
	//Choosing IP version.
        hints.ai_family = ai_family;
	//Socket type chosen as TCP
        hints.ai_socktype = SOCK_STREAM;
	//If getaddrinfo fails, throw its error.
        if ( 0 != getaddrinfo(argv[optind],port,&hints,&result))
                errExit("getaddrinfo",1);

        // Form a socket with IP version and TCP.
        srvfd = socket(result->ai_family,SOCK_STREAM,0);
	// If socket creation fails, throw error.
        if ( srvfd < 0 )
                errExit("socket()",1);
	// If connection with created socket to given address fails, throw error.
        if ( connect(srvfd,result->ai_addr,result->ai_addrlen) == -1)
                errExit("connect",1);
        
        
        // Connection Established ! -hopefully.
        printf("Connection Established !\n");
        // If-else for deciding if it is a GET or PUT query.
	if(putflag == 0){
	// Memory-allocating for request string, number is request dependant.
	request = malloc(75+strlen(argv[optind+1])+strlen(argv[optind]));
	sprintf(request,"GET %s HTTP/1.1\r\nHost: %s\r\nUser-agent: putgetmachine\r\nIam: eunlu1\r\n\r\n",argv[optind+1],argv[optind]);
	// Printing request string.
	printf("%s\n", request);
	// Writing request to socket.	
	write(srvfd,request,strlen(request));
	// Stop writing to socket.
	shutdown(srvfd,SHUT_WR);
	// Clearing request memory.
	free(request);
	} else {
	/*FILEREADER*/
   	 FILE* file = fopen(argv[optind+2],"r");
    	if(file == NULL)
    	{
	//Error handling for file.
        printf( "Could not open file\n" );
	return 0;
   	 }
	//Seeking for size of file.
   	 fseek(file, 0, SEEK_END);
   	 int size = ftell(file);
   	 rewind(file);
	//Memory allocation for file.
   	 char* content = calloc(size + 1, 1);
	//Reading file to content.
  	  fread(content,1,size,file);		
	/*FILEREADER*/
	// Memory-allocating for request string, number is request dependant.
        requestput = malloc(150+strlen(argv[optind+1])+size+strlen(argv[optind]));
	sprintf(requestput,"PUT %s HTTP/1.1\r\nHost: %s\r\nUser-agent: putgetmachine\r\nPassword: moo7IecooH\r\nContent-Type: text/plain\r\nContent-Length: %i\r\nIam: eunlu1\r\n\r\n%s",argv[optind+1],argv[optind],size,content);
	// Printing request string.
	printf("%s\n", requestput);
	// Writing request to socket.
	write(srvfd,requestput,strlen(requestput));
	// Stop writing to socket.
	shutdown(srvfd,SHUT_WR);
	// Clearing request memory.
	free(requestput);
	free(content);
	}
	
        
        
	// Loop for reading socket data.
        while ( rwerr > 0 )
        {	//Read socket data.
                rwerr = read(srvfd,buf,16);
                write(1,buf,rwerr);
        }
        //Connection shut.
        close(srvfd);
	//Return to home.
        return 0;

}
