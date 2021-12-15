#include <iostream>
#include <string>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <csignal>
#include <sstream>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <map>


using namespace std;

struct timespec delta = {0 /*secs*/, 300000000 /*nanosecs*/}; //0.3 sec

ofstream file;
int unnamedPipe;

void termHendler(int sig){
    file << "Watchdog is terminating gracefully" ;
    file.close();
    while(wait(NULL)>0);
    close(unnamedPipe);
    exit(sig);
}

void intHendler(int sig){
    
    file << "Watchdog is terminating gracefully" ;

    file.close();



    close(unnamedPipe);

    exit(sig);
}

int main(int argc, char* argv[]){
    
    signal(SIGTERM, termHendler);
    signal(SIGINT, intHendler);

    int n = atoi(argv[1]);
    string watchdogPath = argv[3];
    
    int pidList[n+1];



    pid_t pid;
    pid_t head;
    file.open(watchdogPath);

    char * myfifo = (char*) "/tmp/myfifo";
    string s = "P0 " + to_string((int)getpid());
    char temp[30];
    strcpy(temp, s.c_str());
    unnamedPipe = open(myfifo, O_WRONLY);

    write(unnamedPipe, temp, 30);

    for(int i = 1; i <= n; i++){
        if(!(pid = fork())){
            s = "P" + to_string(i);
            strcpy(temp, s.c_str());
            execl("./process","process", argv[2], temp ,NULL);
            break;
        }
        else{
            file << "P" << i << " is started and it has a pid of " << pid << "\n" ;
            pidList[i] = pid;
            s = "P" + to_string(i) + " " + to_string(pid);
            strcpy(temp, s.c_str());
            write(unnamedPipe, temp, 30);
            if(i==1){
                head = pid;
            }
        }

    }



    while(1){
        int w = wait(NULL);
        if(w==head){
            file << "P1 is killed, all processes must be killed" << "\n" ;
            
            for(int i=2; i<=n; i++){
                kill(pidList[i], SIGTERM);
            }
            
            file << "Restarting all processes" << "\n";
            for(int i = 1; i <= n; i++){
                if(!(pid = fork())){
                    s = "P" + to_string(i);
                    strcpy(temp, s.c_str());
                    execl("./process","process", argv[2], temp ,NULL);
                    break;
                }
                else{
                    file << "P" << i << " is started and it has a pid of " << pid << "\n" ;
                    pidList[i] = pid;
                    s = "P" + to_string(i) + " " + to_string(pid);
                    strcpy(temp, s.c_str());
                    write(unnamedPipe, temp, 30);
                    if(i==1){
                        head = pid;
                    }
                }

            }
        }else{
            for(int i=1; i<=n; i++){
                if(pidList[i]==w){
                    if(!(pid = fork())){
                        s = "P" + to_string(i);
                        strcpy(temp, s.c_str());
                        execl("./process","process", argv[2], temp ,NULL);
                        break;
                    }
                    else{
                        file << "P" << i << " is killed\nRestarting P" << i << "\n";
                        file << "P" << i << " is started and it has a pid of " << pid << "\n" ;
                        pidList[i] = pid;
                        s = "P" + to_string(i) + " " + to_string(pid);
                        strcpy(temp, s.c_str());
                        write(unnamedPipe, temp, 30);
                        if(i==1){
                            head = pid;
                        }
                    }
                }
            }
        }
    }



    close(unnamedPipe);

    return 0;
}