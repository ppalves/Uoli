#include "api_robot2.h"

void print_int(int a);


int main() {
    unsigned int t;
    t = get_time();
    while(1) {
	if (get_time()-t>=1000) {
	    print_int(get_time());
	    puts("\n");
	    t = get_time();
	}
    }
    //loop_infinito
    while(1) {
	//Nada
    }
    return 0;
}


void print_int(int a) {
    if (a/10 == 0) {
	int rest = a%10;
	rest = rest+48;
	char s = (char)rest;
	char *teste = "";
	*teste = s;
	puts(teste);
    }
    else {
	print_int(a/10);
	int rest = a%10;
	rest = rest+48;
	char s = (char)rest;
	char *teste = "";
	*teste = s;
	puts(teste);
	}
}


