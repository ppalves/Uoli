.globl _start

#Configuracao do hardware e inicializacao do Uoli
_start:
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

    #FALTA CONFIGURAR AS PARTES DO ROBO E ALGUNS PERIFERICOS

    #Muda para o Modo de Usuario
    csrr t1, mstatus
    li t2, ~0x1800
    and t1, t1, t2
    csrw mstatus, t1
    
    la t0, main		#Grava o endereco do rotulo main
    csrw mepc, t0	#No registrador mepc

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

    #TODO O CODIGO DO TRATADOR

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


reg_buffer: .skip 120
