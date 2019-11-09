.globl set_torque
.globl set_engine_torque

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



