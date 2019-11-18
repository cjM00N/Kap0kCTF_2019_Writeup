// gcc dark.c -o dark -z now -s -fno-stack-protector
// 16.04
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

void welcome(){
    setvbuf(stdin,0,2,0);
    setvbuf(stdout,0,2,0);
    setvbuf(stderr,0,2,0);
    alarm(0x20);    
    puts("  ,                  __   _       ___  ______ ______   __  __  , __ ");
    puts(" /|   /             /  \\ | |     / (_)(_) |  (_) |    /  )/  \\/|/  |");
    puts("  |__/   __,    _  |    || |    |         |     _|_     /|    ||\\_/|");
    puts("  | \\   /  |  |/ \\_|    ||/_)   |       _ |    / | |   / |    ||   |");
    puts("  |  \\_/\\_/|_/|__/  \\__/ | \\_/   \\___/ (_/    (_/     /___\\__/ |   |");
    puts("             /|                                                     ");
    puts("             \\|                "          );                           
}

int main(){
    welcome();
    __asm__(
        "jmp here;"
        "syscall;"
        "ret;"
        "here:"
        "nop;"
    );
    puts("\nNight is comming~~~");
    puts("We didn’t even say goodbye!");
    close(1);
    char buf[0x20]={0};
    read(0,buf,0x108);
    return 0;
}