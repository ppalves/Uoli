#include "api_robot2.h"

void print_int(int a);
void sort_friends_order (int *vetor, int n, int *friends_order);
void turn_to_head_direction(int direction, Vector3 angles);
void turn_absolute_y(int angle, Vector3 angles);
void dodge_enemy(int index, int current_axis);
void wait(int waiting_time);
void verify_enemies(int current_axis);
void print_int_with_new_line(int a);
void verify_position(int *aumentarX, int *aumentarZ, Vector3 pos, int x, int z);
void go_to_objective(int x, int z, Vector3 pos, Vector3 angles);
int verify_obstacles();
void me_and_the_boys(int *friends_order);
int verify_inclination(Vector3 angles);

int main() {
    Vector3 pos;
	Vector3 angles;
	get_gyro_angles(&angles);
	get_current_GPS_position(&pos);
	print_int_with_new_line(pos.x);
	print_int_with_new_line(pos.z);
	
	// print_int_with_new_line(angles.y);
	// turn_absolute_y(90, angles);
	// get_gyro_angles(&angles);
	// print_int_with_new_line(angles.y);
	// set_torque(10,10);
	// get_current_GPS_position(&pos);
	// print_int_with_new_line(pos.x);
	// print_int_with_new_line(pos.z);
	// wait(2);
	// get_current_GPS_position(&pos);
	// print_int_with_new_line(pos.x);
	// print_int_with_new_line(pos.z);
	// wait(2);
	// get_current_GPS_position(&pos);
	// print_int_with_new_line(pos.x);
	// print_int_with_new_line(pos.z);

	int num_friends = sizeof(friends_locations)/sizeof(friends_locations[0]);
	int order[num_friends];
	me_and_the_boys(&order[0]);

	for (int i = 0; i < num_friends; i++){
		go_to_objective(friends_locations[order[i]].x, friends_locations[order[i]].z, pos, angles);
		puts("cheguei");
		wait(1);
	}
	
	
	
    while(1) {
	//Nada
    }
    return 0;
}

void verify_position(int *aumentarX, int *aumentarZ, Vector3 pos, int x, int z){
	get_current_GPS_position(&pos);
	

	if (pos.x >= x - 4 && pos.x <= x + 4)
		*aumentarX = 2;

	else if (pos.x < x)
		*aumentarX = 1;

	else if (pos.x > x)
		*aumentarX = 0;


	if (pos.z >= z - 4 && pos.z <= z + 4)
		*aumentarZ = 2;

	else if (pos.z < z)
		*aumentarZ = 1;

	else if (pos.z > z)
		*aumentarZ = 0;
}

int verify_inclination(Vector3 angles){
	get_gyro_angles(&angles);
	if(!(angles.x > 355 || angles.x < 5) || !(angles.z > 355 || angles.z < 5)){
		return 0;
	}
	return 1;
}

void go_to_objective(int x, int z, Vector3 pos, Vector3 angles){

	int aumentarX;
	int aumentarZ;
	
	verify_position(&aumentarX, &aumentarZ, pos, x, z);
	
	int angle;
	
	while(aumentarX != 2 || aumentarZ != 2){
		puts("aumentarX: ");
		print_int_with_new_line(aumentarX);
		puts("aumentarZ: ");
		print_int_with_new_line(aumentarZ);

		if (aumentarX != 2){
			puts("andando no X\n");
			int aumentarX_inicial =  aumentarX;
		
			if (aumentarX)
				angle = 90;
			else if (!aumentarX)
				angle = 270; 

			turn_absolute_y(angle, angles);
			
			set_torque(7,7);
			while(!verify_obstacles() && aumentarX_inicial == aumentarX && verify_inclination(angles)){
				verify_position(&aumentarX, &aumentarZ, pos, x, z);
			}
			puts("vai bater\n");
			
			set_torque(0,0);
			wait(3);
			
		}

		if(aumentarZ != 2){
			puts("andando no Z\n");
			int aumentarZ_inicial = aumentarZ;

			if (aumentarZ)
				angle = 0;
			else if (!aumentarZ)
				angle = 180;
			
			
			turn_absolute_y(angle, angles);
			
			set_torque(7,7);
			while (!verify_obstacles() && aumentarZ_inicial == aumentarZ && verify_inclination(angles)){
				verify_position(&aumentarX, &aumentarZ, pos, x, z);
			}
			puts("vai bater\n");
			
			set_torque(0,0);
			wait(3);
			

		}

		verify_position(&aumentarX, &aumentarZ, pos, x, z);

	}
	
	
	
}

void print_int_with_new_line(int a){
	print_int(a);
	puts("\n");
}

void print_int(int a) {
	if (a < 0){
		a = a * (-1);
		puts("-");
	}
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
		if(angles.y >= angle - 7 && angles.y <= angle + 5)
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
				if (pos.z<(dangerous_locations[i].z) && pos.z>(dangerous_locations[i].z-110)) {
					dodge_enemy(i, current_axis);
				}
				else if (pos.z>(dangerous_locations[i].z) && pos.z<(dangerous_locations[i].z+110)) {
					dodge_enemy(i, current_axis);
				}
			}
			else {
				if (pos.x<(dangerous_locations[i].x) && pos.x>(dangerous_locations[i].x-110)) {
					dodge_enemy(i, current_axis);
				}
				else if (pos.x>(dangerous_locations[i].x) && pos.x<(dangerous_locations[i].x+110)) {
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




