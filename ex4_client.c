// Gal Giladi 312491616
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>

// open the file "to_client_xxxx.txt" that have been created in the server, which contains the result
void openingHandler(int sig) {
    printf("in openingHandler\n");
    char nameOfFile[32] = {'\0'};
    strcat(nameOfFile, "to_client_");
    // pid of the client
    pid_t pid = getpid();
    printf("this is the pid of the client: %d\n", pid);
    char mypid[16] = {'\0'};
    sprintf(mypid, "%d", pid);
    strcat(nameOfFile, mypid);
    strcat(nameOfFile, ".txt");
    printf("open file named: %s\n", nameOfFile);
    int fd = open(nameOfFile, O_RDWR);
    if (fd == -1) {
        printf("error occured in opening %s\n", nameOfFile);
        exit(-1);
    }

    char buff[32] = {'\0'};
    if (read(fd, buff, 32) == -1) {
        printf("error occured while reading from to_client_xxx.txt\n");
        exit(-1);
    }
    printf("this is the result we print to screen: %s\n", buff);

    // delete the file to_client_xxxx
    close(fd);
    unlink(nameOfFile);
}

// closes the client proccess, if there is no response back from the server, about the result we need
void closeHandler(int sig) {
    printf("Client closed because no response was recieved from the server for 30 seconds\n");
    exit(-1);
}

int main (int argc, char* argv[]) {
    // declare signals
    signal(SIGUSR2, openingHandler);
    signal(SIGALRM, closeHandler);

    srand(time(NULL));
    int counter = 0;
    // as long as "to_srv.txt" exists, client should randomly wait, untill its good to use again
    while(access("to_srv.txt", F_OK) != -1) {
        printf("file exists oh yes\n");
        // rand a number between 1 to 5
        int random = rand() % 5 + 1;
        printf("waiting for %d seconds\n", random);
        sleep(random);
        counter++;
        if(counter == 10) {
            printf("client isn't gonna wait anymore.\n");
            // exit the client proccess
            exit(-1);
        }
    }

    // create a file "to_srv.txt" if its not already exists
    int fd;
    fd = open("to_srv.txt", O_RDWR | O_CREAT, 0777);
    if (fd == -1) {
        printf("error occured in opening to_srv.txt\n");
        exit(-1);
    }
    printf("opened srv.txt\n");

    // initalize the file "to_srv.txt" with all the 4 arguments, that we send to the server
    // first, write the pid of the client
    pid_t pid = getpid();
    char mypid[16] = {'\0'};
    sprintf(mypid, "%d", pid);
    write(fd, mypid, strlen(mypid));
    write(fd, "\n", 1);
    // then, write the second argument - the first number
    char* secondArg = argv[2];
    write(fd, secondArg, strlen(secondArg));
    write(fd, "\n", 1);
    // then, write the third argument - the operator number
    char* thirdArg = argv[3];
    write(fd, thirdArg, strlen(thirdArg));
    write(fd, "\n", 1);
    // then, write the fourth argument - the second number
    char* fourthArg = argv[4];
    write(fd, fourthArg, strlen(fourthArg));

    // sending a signal from the client to the server after creating the file "to_srv.txt"
    pid_t serverPid = atoi(argv[1]);
    printf("this is the server pid: %d\n", serverPid);
    kill(serverPid, SIGUSR1);
    printf("sent a first kill to server, after created to_srv.txt\n");
    // client wait for the result from the server for 30 seconds
    alarm(30);
    // if we got a signal back from the server, the alarm is cancelled.
    pause();
    printf("after pause, now we go to open the client file and print the result to screen\n");
}