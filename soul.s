.globl _start

#Configuracao do hardware e inicializacao do Uoli
_start:

    #Falta configurar o GPT!!

    #Configura o endereco para o tratador
    la t0, int_handler	#Grava o endereco do rotulo do tratador
    csrw mtvec, t0	#em t0 e coloca esse valor em mtvec
    
    #Habilita interrupcoes global
    csrr t1, mstatus
    ori t1, t1, 0x80
    csrw mstatus, t1
    #Habilita interrupcoes externas
    csrr t1, mie
    li t2, 0x800
    or t1, t1, t2
    csrw mie, t1
    #Ajusta o mscratch
    la t1, reg_buffer
    csrw mscratch, t1

    #Seta o torque dos motores para 0
    li t0, 0xFFFF0018
    sh zero, 0(t0)
    li t0, 0xFFFF001A
    sh zero, 0(t0)

    #Configura os servo motores para a posicao inicial
    #Top
    li t0, 0xFFFF001C
    li t1, 78
    sb t1, 0(t0)
    #Mid
    li t0, 0xFFFF001D
    li t1, 80
    sb t1, 0(t0)
    #Base
    li t0, 0xFFFF001E
    li t1, 31
    sb t1, 0(t0)


    #Muda para o Modo de Usuario
    csrr t1, mstatus
    li t2, ~0x1800
    and t1, t1, t2
    csrw mstatus, t1

    #FALTA CONFIGURAR A PILHA DO USUARIO E DO SISTEMA

    #Desvia o fluxo do programa para a main do arquivo loco.c
    la t0, main		#Grava o endereco do rotulo main
    csrw mepc, t0	#no registrador mepc
    mret		#PC <= MEPC; MIE <= MPIE; Muda modo para MPP


#Tratador de Interrupcoes
int_handler:
    #Salva o contexto atual
    csrrw a0, mscratch, a0 
    sw a1, 0(a0)
    sw a2, 4(a0)
    sw a3, 8(a0)
    sw a4, 12(a0)
    sw a5, 16(a0)
    sw a6, 20(a0)
    sw a7, 24(a0)
    sw s0, 28(a0)
    sw s1, 32(a0)
    sw s2, 36(a0)
    sw s3, 40(a0)
    sw s4, 44(a0)
    sw s5, 48(a0)
    sw s6, 52(a0)
    sw s7, 56(a0)
    sw s8, 60(a0)
    sw s9, 64(a0)
    sw s10, 68(a0)
    sw s11, 72(a0)
    sw ra, 76(a0)
    sw t0, 80(a0)
    sw t1, 84(a0)
    sw t2, 88(a0)
    sw t3, 92(a0)
    sw t4, 96(a0)
    sw t5, 100(a0)
    sw t6, 104(a0)
    sw sp, 108(a0)
    sw gp, 112(a0)
    sw tp, 116(a0)

    #Tratador de interrupcoes e syscalls
    blt mcause, zero, interruption	#Se o mcause for menor que zero, desvia para interruption
    syscalls:				#Se o mcause for maior que zero, executa o tratamento de syscalls
	if_read_ultrasonic_sensor:	
	    li t1, 16
	    bne t1, a7, else_set_servo_angles
	    jal read_ultrasonic_sensor
	    j end_syscall
	else_set_servo_angles:
	    li t1, 17
	    bne t1, a7, else_set_engine_torque
	    jal set_servo_angles
	    j end_syscall
	else_set_engine_torque:
	    li t1, 18
	    bne t1, a7, else_read_gps
	    jal set_engine_torque
	    j end_syscall
	else_read_gps:
	    li t1, 19
	    bne t1, a7, else_read_gyroscope
	    jal read_gps
	    j end_syscall
	else_read_gyroscope:
	    li t1, 20
	    bne t1, a7, else_get_time
	    jal read_gyroscope
	    j end_syscall
	else_get_time:
	    li t1, 21
	    bne t1, a7, else_set_time
	    jal get_time
	    j end_syscall
	else_set_time:
	    li t1, 22
	    bne t1, a7, else_write
	    jal set_time
	    j end_syscall
	else_write:
	    li t1, 64
	    bne t1, a7, end_syscall
	    jal write
	end_syscall:
	#Ajuste do MEPC para retornar de uma syscall
	csrr a1, mepc		#Carrega endereco de retorno em a1
	addi a1, a1, 4		#Soma 4 no endereco de retorno
	csrs mepc, a1		#Salva o novo endereco de retorno em MEPC
	j end_of_treatment	#Desvia para o fim do tratamento
    interruption:
	#Codigo para tratar interrupcao (do periferico GPT)
    end_of_treatment:

    lw a1, 0(a0)
    lw a2, 4(a0)
    lw a3, 8(a0)
    lw a4, 12(a0)
    lw a5, 16(a0)
    lw a6, 20(a0)
    lw a7, 24(a0)
    lw s0, 28(a0)
    lw s1, 32(a0)
    lw s2, 36(a0)
    lw s3, 40(a0)
    lw s4, 44(a0)
    lw s5, 48(a0)
    lw s6, 52(a0)
    lw s7, 56(a0)
    lw s8, 60(a0)
    lw s9, 64(a0)
    lw s10, 68(a0)
    lw s11, 72(a0)
    lw ra, 76(a0)
    lw t0, 80(a0)
    lw t1, 84(a0)
    lw t2, 88(a0)
    lw t3, 92(a0)
    lw t4, 96(a0)
    lw t5, 100(a0)
    lw t6, 104(a0)
    lw sp, 108(a0)
    lw gp, 112(a0)
    lw tp, 116(a0)
    csrrw a0, mscratch, a0
    mret

#Implementacao das SysCalls:
read_ultrasonic_sensor:
    #Codigo da funcao
    ret

set_servo_angles:
    #Codigo da funcao
    ret

set_engine_torque:
    #Codigo da funcao
    ret

read_gps:
    #Codigo da funcao
    ret

read_gyroscope:
    #Codigo da funcao
    ret

get_time:
    #Codigo da funcao
    ret

set_time:
    #Codigo da funcao
    ret

write:
    #Codigo da funcao
    ret

reg_buffer: .skip 120

