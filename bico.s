.globl set_torque

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
    
