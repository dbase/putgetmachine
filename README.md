PUTGETMACHINE - /*(c) 2014 lancelion*/

-----

OVERVIEW

putgetmachine is a lightweight http client for HTTP PUT and HTTP GET requests, written in C and based on sockets libraries. Program is capable to send HTTP PUT and HTTP GET requests to specified host and port, and able to retrieve response to the query as an output file. 

Detailed information about code can be found in source code as comments. 

$ ./putgetmachine REMOTEHOST REMOTEFILE
f.e.
$ ./putgetmachine www.foo.com /index.html

KEY FUNCTIONS AND OPERATION LOGIC

BUILD INSTRUCTIONS

Entering command 'make' in extraction folder is sufficient for compiling the program.

USER INSTRUCTIONS

Possible functions can be revealed via raw executable, like below;

$./putgetmachine

$How-to-use: ./putgetmachine [-4|-6 IP VERSION] [-p PUT] [-d DESTPORT] [-o OUTPUT] REMOTEHOST REMOTEFILE [LOCAL FILE]
$Example-GET : ./putgetmachine -o bar.html www.foo.com  /bar.html
$Example-PUT : ./putgetmachine -p www.foo.com /bar.txt local.txt

[-4|-6 IP VERSION] : Parameter for specifying IP Version. Default is IPv4.
[-p PUT] : Parameter for HTTP PUT request.
[-d DESTPORT] : Parameter for specifying destination port. Default is 80.
[-o filename OUTPUT] : Parameter for specifying output file. Needs filename argument.
REMOTEHOST : Address for target host. e.g. www.foo.com
REMOTEFILE : Address of target file. e.g. /index.html
[LOCAL FILE] : Address for local file in HTTP PUT request. Default is same folder with executable. e.g. plain.txt 


TESTING AND KNOWN LIMITATIONS

It is tested that program can retrieve with GET and send with PUT methods files sized between 1 MB and 2 MB. It is tried between those numbers since most of online context are between those limits.

DETAILS

Program is not dependant on address families. It can be chosen via parameter if it is IPv6 or IPv4. Default is IPv4.

Program can handle different DNS answers and chooses first reply.

If remote host does not exist on network, returns error of 'getaddrinfo: No such file or directory'. The call 'getaddrinfo' returns this error since it could not reach target file properly.

If remote host does not have HTTP server, program returns error of 'connect: Connection refused'. It means target server is not listening specified port -by default it is 80-. User can change default port by using -d parameter if target host is listening on another port. 

If server or host suddenly disconnect, program ceases operation and returns to the terminal.

