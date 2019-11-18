#include "api_robot2.h"

void print_int(int a);
void sort_friends_order (int *vetor, int n, int *friends_order);
void turn_to_head_direction(int direction, Vector3 angles);
void turn_absolute_y(int angle, Vector3 angles);
void dodge_enemy(int index, int current_axis);
void wait(int waiting_time);
void verify_enemies(int current_axis);
int verify_obstacles();

int main() {
    wait(2);
    print_int(verify_obstacles());
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

void turn_to_head_direction(int direction, Vector3 angles){
	get_gyro_angles(&angles);
	int absolute_angle = (direction - 78) + angles.y;
	print_int(absolute_angle);
	turn_absolute_y(absolute_angle, angles);
	return;
}

void turn_absolute_y(int angle, Vector3 angles){
	set_torque(20, -20);
	while(1){
		get_gyro_angles(&angles);
		if(angles.y >= angle - 4 && angles.y <= angle + 4)
			break;
	}
	set_torque(0,0);
	return;
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

void verify_enemies(int current_axis) {
    int num_enemies = sizeof(dangerous_locations)/sizeof(dangerous_locations[0]);
    Vector3 pos;
    for(int i=0; i<num_enemies; i++) {
	get_current_GPS_position(&pos);
	int dist = (pos.x-dangerous_locations[i].x)*(pos.x-dangerous_locations[i].x);
	dist = dist + (pos.z-dangerous_locations[i].z)*(pos.z-dangerous_locations[i].z);
	if (dist < 150) {
	    //Current_axis = 0 eh o eixo X
	    if (current_axis == 0) {
		if (pos.z<(dangerous_locations[i].z) && pos.z>(dangerous_locations[i].z-11)) {
		    dodge_enemy(i, current_axis);
		}
		else if (pos.z>(dangerous_locations[i].z) && pos.z<(dangerous_locations[i].z+11)) {
		    dodge_enemy(i, current_axis);
		}
	    }
	    else {
		if (pos.x<(dangerous_locations[i].x) && pos.x>(dangerous_locations[i].x-11)) {
		    dodge_enemy(i, current_axis);
		}
		else if (pos.x>(dangerous_locations[i].x) && pos.x<(dangerous_locations[i].x+11)) {
		    dodge_enemy(i, current_axis);
		}
	    }
	}
    }
}

void dodge_enemy(int index, int current_axis) {

}

void wait(int waiting_time) {
    waiting_time = waiting_time*1000;
    int initial_time = get_time();
    while((get_time()-initial_time)<waiting_time) {
	//Nothing
    }
}

int verify_obstacles() {
    int distance = get_us_distance();
    if (distance == 65535) {
	return 0; //Nenhum obstaculo a frente.
    }
    else {
	return 1; //Obstaculo a frente.
    }
}




