#include "api_robot2.h"

void print_int(int a);
sort_friends_order(dist_aux, num_friends, friends_order);

int main() {
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

void me_and_the_boys(int *friends_order) {
    int num_friends = sizeof(friends_locations)/sizeof(friends_locations[0]);
    int dist_aux[num_friends];
    for (int i=0; i<num_friends; i++) {
	friends_order[i] = i;
    }
    for (int i=0; i<num_friends; i++) {
	Vector3 pos;
	get_current_GPS_position(&pos);
	dist_aux[i] = (pos.x-friends_locations[i].x)*(pos.x-friends_locations[i].x);
	dist_aux[i] = dist_aux[i]+(pos.z-friends_locations[i].z)*(pos.z-friends_locations[i].z);
    }
    sort_friends_order(dist_aux, num_friends, friends_order);
}

void sort_friends_order (int *vetor, int n, int *friends_order) {
    int k, j, aux, aux_2;
    for (k = 1; k < n; k++) {
        for (j = 0; j < n - 1; j++) {
            if (vetor[j] > vetor[j + 1]) {
                aux = vetor[j];
		aux_2 = friends_order[j];
                vetor[j] = vetor[j + 1];
		friends_order[j] = friends_order[j+1];
                vetor[j+1] = aux;
		friends_order[j+1] = aux_2;
            }
        }
    }
}
