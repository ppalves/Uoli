#include "api_robot2.h"

int main() {
    unsigned int t;
    set_head_servo(2, 156);
    t = get_time();
    while(get_time()-t<3000){
	//Wait
    }
    set_torque(50, 50);

    char* s = "Teste";
    puts(s);
    //loop_infinito
    while(1) {
	//Nada
    }
    return 0;
}
