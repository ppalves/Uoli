.globl set_torque
.globl set_engine_torque
.globl set_head_servo
.globl get_us_distance
.globl get_current_GPS_position
.globl get_gyro_angles
.globl get_time
.globl set_time
.globl puts

# Seta o torque dos dois motores. O torque tem que ser um valor
#entre -100 e 100.
# Parametros:
#	a0: Engine 1 torque
#	a1: Engine 2 torque
# Retorno:
#	-1 caso o valor do torque esteja fora do range
#	0 caso o valor do torque esteja correto
set_torque:
    addi t0, a0, 0	#t0=a0
    addi t1, a1, 0	#t1=a1
    if_torque_value_invalid:
	li t2, 100
	li t3, -100
	blt t2, t0, invalid_torque_value
	blt t0, t3, invalid_torque_value
	blt t2, t1, invalid_torque_value
	blt t1, t3, invalid_torque_value
    #Chamada das syscalls
    #Primeiro motor:
    li a0, 0		#id do primeiro motor
    addi a1, t0, 0	#a1=t0 (torque do primeiro motor)
    li a7, 18
    ecall		#Chama a SysCall para o primeiro motor
    #Segundo motor:
    li a0, 1		#id do segundo motor
    addi a1, t1, 0	#a1=t1 (torque do segundo motor)
    li a7, 18
    ecall		#Chama a SysCall para o segundo motor
    li a0, 0		#Seta o valor de retorno
    j end_set_torque_1_2
    invalid_torque_value:
	li a0, -1	#Seta o valor de retorno para id invalido
    end_set_torque_1_2:
    ret

#Seta o torque de um motor. O torque deve ter um valor entre 100 e -100.
#Parametros:
#	a0: Engine ID
#	a1: Engine torque
#Retorno:
#	-1 caso o torque seja invalido
#	-2 caso o id seja invalido
#	0 caso esteja tudo certo.
set_engine_torque:
    if_torque_ok:
	li t2, 100
	li t3, -100
	blt a1, t3, invalid_torque	#If a1<-100 then invalid_torque
	blt t2, a1, invalid_torque	#If a1>100 then invalid_torque
	li a7, 18
	ecall		#Chamada da Syscall para setar o torque
    if_eng_id_ok:
	li t2, -1
	beq a0, t2, invalid_eng_id	#If a0=-1 then invalid_eng_id
	j end_set_eng_torq
    invalid_torque:
	li t2, 0
	li t3, 1
	beq a0, t2, id_ok		#If a0=0 then id_ok
	bne a0, t3, invalid_eng_id	#If a0!=1 then invalid_eng_id
	id_ok:
	    li a0, -1	#Valor de retorno para torque invalido
	    j end_set_eng_torq
    invalid_eng_id:
	li a0, -2	#Valor de retorno para ID invalido
    end_set_eng_torq:
    ret

#Seta o angulo dos servo motores (mid, top, base)
#Parametros:
#	a0: Servo ID
#	a1: Servo Angle
#Retorno:
#	-1 caso ID seja invalido
#	-2 caso o angulo seja invalido
#	 0 caso esteja tudo certo
set_head_servo:
    li a7, 17	#Codigo da SysCall
    ecall	#Chama a SysCall
    if_return_invalid_id:
	li t0, -2	#t0=-2
	bne a0, t0, else_if_invalid_angle	#If a0!=-2 then else_if_invalid_angle
	li a0, -1	#a0=-1 para ID invalido
	j end_set_head_servo
    else_if_invalid_angle:
	li t0, -1
	bne a0, t0, end_set_head_servo		#If a0!=-1 then end_set_head_servo
	li a0, -2	#a0=-2 para angulo invalido
    end_set_head_servo:
    ret



#Le a distancia indicada pelo sensor ultrasonico
#Parametros:
#   - 
#Retorno:
#   Valor obitido pela leitura do sensor 
#   -1 caso nao tenha nenhum objeto no range de ate 600cm
get_us_distance:
    li a7, 16 # a7 = 16
    ecall
    li t0, -1 # t0 = -1
    bne a0, t0, valid_distance; # if a0 == t0 then valid_distance
    li a0, 0xFFFF # a0 = 0xFFFF
    valid_distance:
    ret

#Le a posicao atual indicada pelo GPS do Uoli
#Parametros:
#   pos: ponteiro para a struct a ser preenchida com as coordenadas do GPS
#Retorno:
#   void
get_current_GPS_position:
    li a7, 19 # a7 = 19
    ecall
    ret

#Le a rotacao global indicada pelo giroscopio
#Parametros:
#   angles: ponteiro pra struct a ser preenchida com os angulos de Euler
#Retorno:
#   void
get_gyro_angles:
    li a7, 20 # a7 = 20
    ecall
    ret

#Escreve uma string no UART. Usa a syscall write com file descripitor 1
#para instruir a SOUL a escrever a string na UART
#Parametros:
#   *s: ponteiro para a string
#Retorno:
#   void
puts:
    li a7, 64 # a7 = 64
    mv  a1, a0 # a1 = a0
    mv t2, a0 #t2 = a0
    li a0, 1 # a0 = 1
    li a2, 0 # a2 = 0
    loop_puts:
        lb t1, 0(t2)
        addi a2, a2, 1	# a2 = a2 + 1
        addi t2, t2, 1	# t2 = t2 + 1
        bnez t1, loop_puts
    ecall
    ret
       
#Le o tempo atual do sistema
#Parametro:
#	Nao tem
#Retorno:
#	Tempo atual do sistema
get_time:
    li a7, 21
    ecall	#Chama s Syscall de get_time
    #O valor de retorno ja esta em a0
    ret

#Seta o tempo do sistema
#Parametro:
#	a0: Valor do novo tempo do sistema
#Retorno:
#	Void
set_time:
    #O valor do a0 para a Syscall ja veio como parametro
    li a7, 22
    ecall	#Chama a Syscall que seta o tempo do sistema
    ret
