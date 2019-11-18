#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main(){
	alarm(0x10);
	setbuf(stdin,0);
	setbuf(stdout,0);
	setbuf(stderr,0);

	char str[1000];
	printf("shell> ");
	read(0,str,1000);
	printf("END\n");
	sleep(1);
	close(1);
	// close(2);
	system(str);
}