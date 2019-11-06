#  
#  Autores: Edson Borin e AntÃ´nio GuimarÃ£es
#  
#  Data: 2019
#
#  ATENÃ‡ÃƒO: NÃ£o modifique este Makefile. A correÃ§Ã£o do trabalho
#  serÃ¡ realizada utilizando uma cÃ³pia nÃ£o modificada deste Makefile
#  e eventuais modificaÃ§Ãµes podem fazer com que seu cÃ³digo nÃ£o 
#  funcione com o ferramental de correÃ§Ã£o.
#
#  ***************************************************************

all: loco.o bico.o soul.o
	riscv32-unknown-elf-ld loco.o bico.o soul.o -g -o uoli.x

loco.o: loco.c api_robot2.h
	riscv32-unknown-elf-gcc loco.c -c -g -o loco.o

bico.o: bico.s
	riscv32-unknown-elf-as bico.s -g -o bico.o

soul.o: soul.s
	riscv32-unknown-elf-as soul.s -g -o soul.o

clean:
	rm *.o *.x
