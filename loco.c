#include "api_robot2.h"

//cabecalho das funcoes
void print_int(int a);
void sort_friends_order (int *vetor, int n, int *friends_order);
//void turn_to_head_direction(int direction, Vector3 angles);
void turn_absolute_y(int angle, Vector3 angles);
void wait(int waiting_time);
//int verify_enemies(int current_axis);
void print_int_with_new_line(int a);
void verify_position(int *increaseX, int *increaseZ, Vector3 pos, int x, int z);
void go_to_objective(int x, int z, Vector3 pos, Vector3 angles);
int verify_obstacles();
void me_and_the_boys(int *friends_order);
int verify_inclination(Vector3 angles);


int main() {
	set_head_servo(1, 78);  //ajusta a cabeça levemente pra baixo para ver melhor montanhas
    Vector3 pos; //inicia vetor usado pra guardar a posicao do GPS
	Vector3 angles; //inicia vetor usado pra guardar os angulos do giroscopio
	get_gyro_angles(&angles);
	get_current_GPS_position(&pos);

	int num_friends = sizeof(friends_locations)/sizeof(friends_locations[0]);   //numero de amigos
	int order[num_friends];  //vetor com a ordem dos amigos
	me_and_the_boys(&order[0]);  //calcula distancia e ordena o vetor

	for (int i = 0; i < num_friends; i++){   //loop para interar sobre os amigos ordenados e ir ate cada um deles
		go_to_objective(friends_locations[order[i]].x, friends_locations[order[i]].z, pos, angles);
		wait(500);
		puts("Amigo encontrado!\n");
		puts("Ola amigo!\n");
	}

	set_torque(0,0);
	puts("Todos os amigos encontrados, missao cumprida!\n");
	
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
	
	verify_position(&increaseX, &increaseZ, pos, x, z); //verifica em qual eixo e sobre qual direcao precisa andar
	
	int angle;
	int inimigo;
	
	while(increaseX != 2 || increaseZ != 2){  //enquanto nao estiver perto do amigo verifica sobre qual eixo e em qual direcao precisa andar

		if (increaseX != 2){
			int increaseX_inicial =  increaseX;
		
			if (increaseX)
				angle = 90;
			else if (!increaseX)
				angle = 270; 

			turn_absolute_y(angle, angles);
			
			set_torque(7,7);
			
			//while que executa enquanto o Uoli anda sobre o eixo, quebra o loop caso encontre algum obstaculo/inclinacao ou caso ele ja tenha passado da coordenada do amigo
			while(!verify_obstacles() && increaseX_inicial == increaseX && verify_inclination(angles)){
				verify_position(&increaseX, &increaseZ, pos, x, z); //verifica toda hora se ele ja nao passou do amigo e portanto precisa andar no outro eixo
				get_gyro_angles(&angles);
				if ((angles.y-angle)<-30) {  //caso esteja indo em uma direcao muito errada da break
					if((angles.y-angle)>30) {
						break;
					}
				}
			}			
			
			set_torque(0,0);	
		}

		if(increaseZ != 2){
			int increaseZ_inicial = increaseZ;

			if (increaseZ)
				angle = 0;
			else if (!increaseZ)
				angle = 180;
			
			turn_absolute_y(angle, angles);
			
			set_torque(7,7);

			//while que executa enquanto o Uoli anda sobre o eixo, quebra o loop caso encontre algum obstaculo/inclinacao ou caso ele ja tenha passado da coordenada do amigo
			while (!verify_obstacles() && increaseZ_inicial == increaseZ && verify_inclination(angles)){
				verify_position(&increaseX, &increaseZ, pos, x, z); //verifica toda hora se ele ja nao passou do amigo e portanto precisa andar no outro eixo
				get_gyro_angles(&angles);
				if ((angles.y-angle)<-30) { //caso esteja indo em uma direcao muito errada da break
					if((angles.y-angle)>30) {
						break;
					}
				}
			}			
			
			set_torque(0,0);
		}

		verify_position(&increaseX, &increaseZ, pos, x, z);  //verifica em qual eixo e sobre qual direcao precisa andar
	}
}

/*
 *Função para printar quebra de linha apos printar um numero inteiro
 *
 * Parametros:
 * 		int a: inteiro a ser printado
 * 
 * Retorno:
 * 		void
 */
void print_int_with_new_line(int a){
	print_int(a);
	puts("\n");
}


/*
 *Função recursiva para passar inteiros para char para printar
 *
 * Parametros:
 * 		int a: inteiro a ser printado
 * 
 * Retorno:
 * 		void
 */
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

/*
 *Função para ordenar os amigos em ordem de proximidade. Calcula as distancias e chama outra funcao para ordenar 
 *
 * Parametros:
 * 		int *friends_order: Ponteiro para o vetor que sera usado para ordenar os index dos amigos
 * 
 * Retorno:
 * 		void
 */
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


/*
 *Função para ordenar o vetor de distancias dos amigos
 *
 * Parametros:
 * 		int *vetor: vetor com as distancias
 * 		int n: numero de amigos
 * 		int *friends_order: vetor para salvar a ordem com base nos index dos amigos
 * 
 * Retorno:
 * 		void
 */
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

/*
 *Função para girar o Uoli para um angulo absoluto
 *
 * Parametros:
 * 		int angle: angulo a ser girado
 * 		Vector3 angles: vetor com os angulos de euler do Uoli
 * 
 * Retorno:
 * 		void
 */
void turn_absolute_y(int angle, Vector3 angles){
	set_torque(20, -20);
	while(1){    //loop ate o Uoli atingir aproximadamente o angulo desejado
		get_gyro_angles(&angles);
		if(angles.y >= angle - 5 && angles.y <= angle + 5)
			break;
	}
	set_torque(0,0);
	return;
}


/*
 *Função que utiliza do GPT para esperar um tempo
 *
 * Parametros:
 * 		int waiting_time: tempo a ser esperado em milisegundos
 * 
 * Retorno:
 * 		void
 */
void wait(int waiting_time) {
    
    int initial_time = get_time();
    while((get_time()-initial_time)<waiting_time) {
		//Nothing
    }
}

/*
 *Função para verificar a presença de obstaculos a frente
 *
 * Parametros:
 * 		none
 * 
 * Retorno:
 * 		int: 0 nenhum obstaculo
 * 			 1 obstaculo a frente
 */
int verify_obstacles() {
    int distance = get_us_distance();
    if (distance == 65535) {
		return 0; //Nenhum obstaculo a frente.
    }
    else {
		return 1; //Obstaculo a frente.
    }
}


/*
 *Função para virar o Uoli para onde o sensor ultrasonico estiver apontando.
 *Nao utilizamos a funcao pois nao foi necessaria no codigo final
 */

/*
void turn_to_head_direction(int direction, Vector3 angles){
	get_gyro_angles(&angles);
	int absolute_angle = (direction - 78) + angles.y;
	print_int(absolute_angle);
	turn_absolute_y(absolute_angle, angles);
	return;
}
*/


/*Função não utilizada. A ideia da função era verificar se era possivel passar por um inimigo, caso contrario desviar mudando de eixo
 *mesma forma usada pra desviar dos outros obstaculos
 *Não conseguimos implementar a função de um jeito satisfatorio e muitas vezes o Uoli ficava preso girando no lugar, sendo assim a funcao nao foi utilizada
 */
/*
int verify_enemies(int current_axis) {
    int num_enemies = sizeof(dangerous_locations)/sizeof(dangerous_locations[0]);
    Vector3 pos;
    for(int i=0; i<num_enemies; i++) {
		get_current_GPS_position(&pos);
		int dist = (pos.x-dangerous_locations[i].x)*(pos.x-dangerous_locations[i].x);
		dist = dist + (pos.z-dangerous_locations[i].z)*(pos.z-dangerous_locations[i].z);
		if (dist < 100) {
			return 1;
			if (current_axis == 0) {
				if (pos.z<(dangerous_locations[i].z) && pos.z>(dangerous_locations[i].z-25)) {
					return 1;
				}
				else if (pos.z>(dangerous_locations[i].z) && pos.z<(dangerous_locations[i].z+25)) {
					return 1;
				}
			}
			else {
				if (pos.x<(dangerous_locations[i].x) && pos.x>(dangerous_locations[i].x-25)) {
					return 1;
				}
				else if (pos.x>(dangerous_locations[i].x) && pos.x<(dangerous_locations[i].x+25)) {
					return 1;
				}
			}
		}
    }
	return 0;
}
*/
