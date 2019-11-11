.globl _start

#Configuracao do hardware e inicializacao do Uoli
_start:

    #Inicializa o GPT
    li t0, 0xFFFF0100
    li t1, 100
    sw t1, 0(t0)

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
    
    #Configura a pilha do usuario
    li sp, 0x7FFFFFC

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
    
    #Desvia o fluxo do programa para a main do arquivo loco.c
    la t0, main		#Grava o endereco do rotulo main
    csrw mepc, t0	#no registrador mepc
    mret		#PC <= MEPC; MIE <= MPIE; Muda modo para MPP

    loop_infinito:
    j loop_infinito


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
    csrrw a0, mscratch, a0

    #Tratador de interrupcoes e syscalls
    csrr t1, mcause
    blt t1, zero, interruption	#Se o mcause for menor que zero, desvia para interruption
    
    syscalls:				#Se o mcause for maior que zero, executa o tratamento de syscalls
	if_read_ultrasonic_sensor:	
	    li t1, 16
	    bne t1, a7, else_set_servo_angles
	    j syscall_read_ultrasonic_sensor
	else_set_servo_angles:
	    li t1, 17
	    bne t1, a7, else_set_engine_torque
	    j syscall_set_servo_angles
	else_set_engine_torque:
	    li t1, 18
	    bne t1, a7, else_read_gps
	    j syscall_set_engine_torque
	else_read_gps:
	    li t1, 19
	    bne t1, a7, else_read_gyroscope
	    j syscall_read_gps
	else_read_gyroscope:
	    li t1, 20
	    bne t1, a7, else_get_time
	    j syscall_read_gyroscope
	else_get_time:
	    li t1, 21
	    bne t1, a7, else_set_time
	    j syscall_get_time
	else_set_time:
	    li t1, 22
	    bne t1, a7, else_write
	    j syscall_set_time
	else_write:
	    li t1, 64
	    bne t1, a7, end_syscall
	    j syscall_write
	
    end_syscall:
	#Ajuste do MEPC para retornar de uma syscall
	csrr t0, mepc  # carrega endereco de retorno (endereco da instrucao que invocou a syscall)
	addi t0, t0, 4 # soma 4 no endereco de retorno (para retornar apos a ecall)
	csrw mepc, t0  # armazena endereco de retorno de volta no mepc
	j end_of_treatment	#Desvia para o fim do tratamento
    
    interruption:
	#Codigo para tratar interrupcao (do periferico GPT)
	li t0, 0xFFFF0104
	lw t1, 0(t0)
	beq zero, t1, end_of_treatment	#If interrupcao falsa then end_of_treatment
	if_int_GPT:
	    sw zero, 0(t0)	#Seta o valor zero para o verificador do GPT
	    li t0, 0xFFFF0100
	    li t1, 100
	    sw t1, 0(t0)	#Seta o GPT para interromper daqui 100ms
	    la t0, time_now
	    lw t1, 0(t0)	#t1=tempo atual
	    addi t1, t1, 100	#Soma 100ms no tempo atual
	    sw t1, 0(t0)	#Guarda o tempo atual na memoria

    end_of_treatment:

    csrrw a0, mscratch, a0
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
syscall_read_ultrasonic_sensor:
    li t1, 0xFFFF0020
    li t2, 0
    sw t2, 0(t1) 
    while_read_ultrasonic:
        lw t2, 0(t1) 
        li t3, 1 
        bne t2, t3, while_read_ultrasonic # if t2 != t3 then while_read_ultrasonic
    li t1, 0xFFFF0024 # t1 = 0xFFFF0024
    lw a0, 0(t1) 
    j end_syscall

syscall_set_servo_angles:
    if_servo_1:
	    li t1, 0
	    bne a0, t1, else_servo_2	#if a0!=1 then else_servo_2
	    li t1, 16
	    blt a1, t1, invalid_angle	#if a1<16 then invalid_angle
	    li t1, 116
	    blt t1, a1, invalid_angle	#if a1>116 then invalid_angle
	    li t1, 0xFFFF001E		#Recebe o endereco de memoria do motor base
	    sb a1, 0(t1)			#Salva o valor de a1 em 0xFFFF001E
	    li a0, 0
	    j else_servo
    else_servo_2:
	    li t1, 1
	    bne a0, t1, else_servo_3
	    li t1, 52
	    blt a1, t1, invalid_angle	#if a1<52 then invalid_angle
	    li t1, 90
	    blt t1, a1, invalid_angle	#if a1>90 then invalid_angle
	    li t1, 0xFFFF001D		#Recebe o endereco de memoria do motor base
	    sb a1, 0(t1)			#Salva o valor de a1 em 0xFFFF001D
	    li a0, 0
	    j else_servo
    else_servo_3:
	    li t1, 2
	    bne a0, t1, invalid_id
	    blt a1, zero, invalid_angle	#if a1<0 then invalid_angle
	    li t1, 156
	    blt t1, a1, invalid_angle	#if a1>156 then invalid_angle
	    li t1, 0xFFFF001C		#Recebe o endereco de memoria do motor base
	    sb a1, 0(t1)			#Salva o valor de a1 em 0xFFFF001C
	    li a0, 0
	    j else_servo
    invalid_id:		#Caso o id seja invalido
	    li a0, -2	#coloca o -2 em a0
	    j else_servo
    invalid_angle:	#Caso o angulo seja invalido
	    li a0, -1	#coloca o -1 em a0
    else_servo:
    j end_syscall

syscall_set_engine_torque:
    if_engine_0:
	    bne a0, zero, else_engine_1	#if a0!=0 then else_engine_1
	    li t0, 0xFFFF001A
	    sh a1, 0(t0)			#Seta o valor de a1 como o torque do motor 0
	    j end_set_engine
    else_engine_1:
	    li t1, 1
	    bne a0, t1, invalid_engine_id	#if a0!=2 then invalid_engine_id
	    li t0, 0xFFFF0018
	    sh a1, 0(t0)			#Seta o valor de a1 como o torque do motor 1
	    li a0, 0
	    j end_set_engine
    invalid_engine_id:		#if a0!=1 and a0!=2 then a0=-1 and return
	    li a0, -1
    end_set_engine:
    j end_syscall

syscall_read_gps:
    li t1, 0xFFFF0004
    li t2, 0
    sw t2, 0(t1) 
    while_read_gps:
        lw t2, 0(t1) 
        li t3, 1 
        bne t2, t3, while_read_gps # if t2 != t3 then while_read_gps
    li t1, 0xFFFF0008 # t1 = 0xFFFF0008
    lw t2, 0(t1) # 
    sw t2, 0(a0) # 
    li t1, 0xFFFF000C # t1 = 0xFFFF0008
    lw t2, 0(t1) # 
    sw t2, 4(a0) # 
    li t1, 0xFFFF0010 # t1 = 0xFFFF0008
    lw t2, 0(t1) # 
    sw t2, 8(a0) # 
    j end_syscall

syscall_read_gyroscope:
    li t1, 0xFFFF0004
    li t2, 0
    sw t2, 0(t1) 
    while_read_gyroscope:
        lw t2, 0(t1) 
        li t3, 1 
        bne t2, t3, while_read_gyroscope # if t2 != t3 then while_read_gps
    li t1, 0xFFFF0014 # t1 = 0xFFFF0014
    lw t1, 0(t1) # 
    srli t2, t1, 20
    slli t3, t1, 12
    srli t3, t3, 22
    slli t4, t1, 22
    srli t4, t4, 22
    sw t2, 0(a0) # 
    sw t3, 4(a0) # 
    sw t4, 8(a0) #     
    j end_syscall

syscall_get_time:
    la t0, time_now	#t0=endereco do tempo atual
    lw a0, 0(t0)	#a0=tempo atual
    j end_syscall

syscall_set_time:
    la t0, time_now	#t0=endereco do tempo atual
    sw a0, 0(t0)	#Coloca o valor de a0 como novo tempo atual
    j end_syscall

syscall_write:
    li t0, 0
    li t2, 0xFFFF0109 # t2 = 0xFFFF0109

    loop_syscall_write:
#        lb t1, t0(a1) 
#        sb t1, t0(t2) 
        addi t0, t0, 1; # t0 = t0 + 1
        blt t0, a2, loop_syscall_write # if t0 < a2 then loop_syscall_write
    
    li t1, 0xFFFF0108 # t1 = 0xFFFF0108
    li t2, 1 # t2 = 1
    li t3, 0 # t3 = 0
    sw t2, 0(t1) 

    li a0, 0 # a0 = 0
    

    loop_write_on_screen:
        lw t2, 0(t1)
        addi a0, a0, 1; # a0 = a0 + 1 
        bne t2, t3, loop_write_on_screen # if t2 != t3 then loop_write_on_screen
                
    j end_syscall

time_now: .skip 4
reg_buffer: .skip 120

