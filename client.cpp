#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <iostream>
#include <iomanip>

//define Port to recieve from AWS
#define PORT 25687

//Function to recieve Data from AWS, which is
void recieveFromAWS(int socket, int suppliedLink){
  double AWSData;
  if (recv(socket, &AWSData, sizeof(AWSData),0) < 0) {
    perror("Failed to receive from AWS");
  } else if(AWSData!=0) {
    std::cout << "The delay for link <" << suppliedLink << "> is <" << std::setprecision(4) <<  AWSData << ">ms" << std::endl;
  } else {
    std::cout << "Found no matches for link <" << suppliedLink << "> " << std::endl;
  }
}

int main(int argc, char* argv[]){
  //define constants
  printf("The client is up and running\n");
  int link = atoi(argv[1]);
  int size = atoi(argv[2]);
  int power = atoi(argv[3]);
  int Vals[3] = {link, size, power};
  double Data;

  //Create soceket
  int awsSoc = 0, valread;
  struct sockaddr_in aws;
  if ((awsSoc = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    printf("Socket creation error\n");
    return -1;
  }

  memset(&aws,'0',sizeof(aws));

  aws.sin_family = AF_INET;
  aws.sin_port = htons(PORT);
  aws.sin_addr.s_addr = inet_addr("127.0.0.1");

  //Connect to AWS
  if (connect(awsSoc, (struct sockaddr *)&aws, sizeof(aws)) < 0)
  {
    perror("Connection Failed");
    return -1;
  }
  // send data to AWS in an array of ints that client supplied
  if (send(awsSoc, (char*)Vals, 3*sizeof(int), 0) < 0){
    perror("failed to send\n");
    return -1;
  } else {
    printf("The client sent link=%d, size=%d, power=%d to AWS.\n", link, size, power);
  }

  //recieve data from AWS
  recieveFromAWS(awsSoc, link);
  
  close(awsSoc);
}
