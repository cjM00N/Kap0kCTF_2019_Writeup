// gcc printf.c -o printf -s -fpie -no-pie
#include <stdio.h>
#include <unistd.h>
#include <string.h>

void init() {
	setvbuf(stdin, NULL, _IONBF, 0);
	setvbuf(stdout, NULL, _IONBF, 0);
}

void vuln() {
	char buf[0xA0];

	puts("How would you pwn me?");
	while(1) {
		puts("Come on!");
		read(0, buf, 0xA0);
		printf(buf);
	}
}

int main() {
	init();
	vuln();

	return 0;
}