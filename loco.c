#include "api_robot2.h"

//cabecalho das funcoes
void print_int(int a);
void sort_friends_order (int *vetor, int n, int *friends_order);
void turn_to_head_direction(int direction, Vector3 angles);
void turn_absolute_y(int angle, Vector3 angles);
void dodge_enemy(int index, int current_axis);
void wait(int waiting_time);
int verify_enemies(int current_axis);
void print_int_with_new_line(int a);
void verify_position(int *increaseX, int *increaseZ, Vector3 pos, int x, int z);
void go_to_objective(int x, int z, Vector3 pos, Vector3 angles);
int verify_obstacles();
void me_and_the_boys(int *friends_order);
int verify_inclination(Vector3 angles);


int main() {
	set_head_servo(1, 78);
    Vector3 pos;
	Vector3 angles;
	get_gyro_angles(&angles);
	get_current_GPS_position(&pos);
	print_int_with_new_line(pos.x);
	print_int_with_new_line(pos.z);

	int num_friends = sizeof(friends_locations)/sizeof(friends_locations[0]);
	int order[num_friends];
	me_and_the_boys(&order[0]);

	for (int i = 0; i < num_friends; i++){
		go_to_objective(friends_locations[order[i]].x, friends_locations[order[i]].z, pos, angles);
		puts("cheguei\n");
		wait(500);
		print_int_with_new_line(get_time());
	}

	puts("terminei");
	set_torque(0,0);
	
	
	
	
    while(1) {
	//Nada
    }
    return 0;
}


/*
 *Função responsavel por determinar se o Uoli precisa aumentar seu X ou Z para chegar na posicao do amigo
 *ie. andar no sentido positivo do eixo X ou no sentido negativo
 *
 *Parametros:
 *		int *increaseX: ponteiro para um inteiro que indica se devemos aumentar, diminuir ou manter X
 *		int *increaseZ: ponteiro para um inteiro que indica se devemos aumentar, diminuir ou manter Z
 * 		Vector3 pos: Vector para armazenar a posicao atual do Uoli
 * 		int x: posicao x do amigo
 * 		int z: posicao z do amigo
 * 
 *Retorno:
 *		void
 */
void verify_position(int *increaseX, int *increaseZ, Vector3 pos, int x, int z){
	get_current_GPS_position(&pos);
	

	if (pos.x >= x - 4 && pos.x <= x + 4)
		*increaseX = 2;

	else if (pos.x < x)
		*increaseX = 1;

	else if (pos.x > x)
		*increaseX = 0;


	if (pos.z >= z - 4 && pos.z <= z + 4)
		*increaseZ = 2;

	else if (pos.z < z)
		*increaseZ = 1;

	else if (pos.z > z)
		*increaseZ = 0;
}


/*
 *Funcao para verificar a inclinacao do robo com base nos angulos de Euler
 *
 *Parametros:
 *		Vector3 angles: Vector para armazenar os angulos atuais do Uoli 
 *
 *Retorno:
 *		int: 1 se a inclinacao é aceitavel
 * 			 0 se a inclinacao é muito grande
 */
int verify_inclination(Vector3 angles){
	get_gyro_angles(&angles);
	if(!(angles.x > 353 || angles.x < 7) || !(angles.z > 353 || angles.z < 7)){
		return 0;
	}
	return 1;
}

/*
 * Função responsavel por guiar o Uoli ate os amigos e desviar de obstaculos como barris e montanhas
 * O algoritimo se baseia em verificarmos se as coordenadas X e Z do robo são maiores ou menores que as 
 * coordenadas do amigo. Apos isso andamos sobre um dos eixos (X ou Z) em direcao ao amigo. Se encontramos 
 * um obstaculo no meio do caminho mudamos o eixo sobre o qual estamos andando e assim em diante. 
 * 
 * Parametros:
 * 		int x: posicao x do amigo
 * 		int z: posicao z do amigo
 * 		Vector3 pos: Vector para armazenar a posicao atual do Uoli
 * 		Vector3 angles: Vector para armazenar os angulos atuais do Uoli 
 * 
 * Retorno:
 * 		void  
 */
void go_to_objective(int x, int z, Vector3 pos, Vector3 angles){

	int increaseX;
	int increaseZ;
	
	verify_position(&increaseX, &increaseZ, pos, x, z);
	
	int angle;
	int inimigo;
	
	while(increaseX != 2 || increaseZ != 2){
		puts("increaseX: ");
		print_int_with_new_line(increaseX);
		puts("increaseZ: ");
		print_int_with_new_line(increaseZ);

		if (increaseX != 2){
			puts("andando no X\n");
			int increaseX_inicial =  increaseX;
		
			if (increaseX)
				angle = 90;
			else if (!increaseX)
				angle = 270; 

			turn_absolute_y(angle, angles);
			
			set_torque(7,7);
			
			while(!verify_obstacles() && increaseX_inicial == increaseX && verify_inclination(angles)){
				verify_position(&increaseX, &increaseZ, pos, x, z);
				get_gyro_angles(&angles);
				if ((angles.y-angle)<-30) {
					if((angles.y-angle)>30) {
						puts("lala");
						break;
					}
				}
			}
			puts("vai bater\n");
			
			
			
			set_torque(0,0);
			//wait(3);
			
		}

		if(increaseZ != 2){
			puts("andando no Z\n");
			int increaseZ_inicial = increaseZ;

			if (increaseZ)
				angle = 0;
			else if (!increaseZ)
				angle = 180;
			
			
			turn_absolute_y(angle, angles);
			
			set_torque(7,7);
			
			while (!verify_obstacles() && increaseZ_inicial == increaseZ && verify_inclination(angles)){
				verify_position(&increaseX, &increaseZ, pos, x, z);
				get_gyro_angles(&angles);
				if ((angles.y-angle)<-30) {
					if((angles.y-angle)>30) {
						puts("lala");
						break;
					}
				}
			}
			puts("vai bater\n");
			
			
			set_torque(0,0);
			//wait(3);
			

		}

		verify_position(&increaseX, &increaseZ, pos, x, z);

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
		if(angles.y >= angle - 5 && angles.y <= angle + 5)
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

int verify_enemies(int current_axis) {
    int num_enemies = sizeof(dangerous_locations)/sizeof(dangerous_locations[0]);
    Vector3 pos;
    for(int i=0; i<num_enemies; i++) {
		get_current_GPS_position(&pos);
		int dist = (pos.x-dangerous_locations[i].x)*(pos.x-dangerous_locations[i].x);
		dist = dist + (pos.z-dangerous_locations[i].z)*(pos.z-dangerous_locations[i].z);
		if (dist < 100) {
			return 1;
			// if (current_axis == 0) {
			// 	if (pos.z<(dangerous_locations[i].z) && pos.z>(dangerous_locations[i].z-25)) {
			// 		return 1;
			// 	}
			// 	else if (pos.z>(dangerous_locations[i].z) && pos.z<(dangerous_locations[i].z+25)) {
			// 		return 1;
			// 	}
			// }
			// else {
			// 	if (pos.x<(dangerous_locations[i].x) && pos.x>(dangerous_locations[i].x-25)) {
			// 		return 1;
			// 	}
			// 	else if (pos.x>(dangerous_locations[i].x) && pos.x<(dangerous_locations[i].x+25)) {
			// 		return 1;
			// 	}
			// }
		}
    }
	return 0;
}



void wait(int waiting_time) {
    
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




