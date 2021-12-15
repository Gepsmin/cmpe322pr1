#include <iostream>
#include <unistd.h>
#include <fstream>
#include <sys/types.h>
#include <sys/wait.h>
#include <string>

using namespace std;

string process;
string directory;
fstream file;

void intHandler(int sig){
    file.open(directory, std::ios_base::app);
    file << process << " recieved signal " << sig << "\n";
    file.close();
}

void hupHandler(int sig){
    file.open(directory, std::ios_base::app);
    file << process << " recieved signal " << sig << "\n";
    file.close();
}

void illHandler(int sig){
    file.open(directory, std::ios_base::app);
    file << process << " recieved signal " << sig << "\n";
    file.close();
}

void trapHandler(int sig){
    file.open(directory, std::ios_base::app);
    file << process << " recieved signal " << sig << "\n";
    file.close();
}

void fpeHandler(int sig){
    file.open(directory, std::ios_base::app);
    file << process << " recieved signal " << sig << "\n";
    file.close();
}

void segvHandler(int sig){
    file.open(directory, std::ios_base::app);
    file << process << " recieved signal " << sig << "\n";
    file.close();
}

void termHandler(int sig){
    file.open(directory, std::ios_base::app);
    file << process << " recieved signal " << sig <<", terminating gracefully" << "\n";
    file.close();
    exit(sig);
}

void xcpuHandler(int sig){
    file.open(directory, std::ios_base::app);
    file << process << " recieved signal " << sig << "\n";
    file.close();
}

int main(int argc, char* argv[]){
    process = argv[2];
    directory = argv[1];
    file.open(directory, std::ios_base::app);

    file << argv[2] << " is waiting for a signal" << "\n";
    file.close();

    signal(SIGINT, intHandler);
    signal(SIGHUP, hupHandler);
    signal(SIGILL, illHandler);
    signal(SIGTRAP, trapHandler);
    signal(SIGFPE, fpeHandler);
    signal(SIGSEGV, segvHandler);
    signal(SIGTERM, termHandler);
    signal(SIGXCPU, xcpuHandler);
    

    while(1){
        sleep(1);
    }



    return 0;
    
}