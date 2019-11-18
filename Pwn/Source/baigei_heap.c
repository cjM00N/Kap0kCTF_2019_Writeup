// gcc ./baigei_heap.c -o baigei_heap 
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<stdint.h>
#include<sys/mman.h>

uint64_t read_num(){
    char buf[0x20]={0};
    fgets(buf,0x20,stdin);
    return strtoull(buf,(char**)buf+0x20,10);
}

int read_buff(char *buff,int len){
    char end_ch='\n';
    int i;
    int retval;
    char a;
    for (i=0;i<len-1;i++){
        retval=read(0,&a,1);
        if(retval<=0){
            exit(-1);
        }
        if(a==end_ch)
            break;
        buff[i]=a;
    }
    // buff[i]='\0';
    return i;
}

void *PTR;
size_t SIZE;
void add(){
    puts("size?");
    size_t size=read_num();
    PTR=malloc(size);
    SIZE=size;
    puts("content?");
    read_buff(PTR,size);
    printf("[DEBUG] In: ");
    puts(PTR);
}

void delete(){
    free(PTR);
    puts("[DEBUG] FREE");
}

void show(){
    puts("Lewis Lewis.gif");

}

void edit(){
    puts("content?");
    read_buff(PTR,SIZE);
}

void init(){
    setvbuf(stdin,0,2,0);
    setvbuf(stdout,0,2,0);
    alarm(0x20);
}

int main(){
    init();
    puts("======= Welcome to baigei_heap ========");
    while (1)
    {
        puts("1. add");
        puts("2. edit");
        puts("3. delete");
        puts("4. show");
        printf(">> ");
        int idx = read_num();
        switch(idx){
            case 1:
                add();
                break;
            case 2:
                edit();
                break;
            case 3:
                delete();
                break;
            case 4:
                show();
                break;
            case 5:
                exit(0);
                break;
            default:
                puts("bad choice!\n");
        }
    }
    return 0;
}

