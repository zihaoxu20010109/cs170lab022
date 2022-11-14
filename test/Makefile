# Makefile for JOS Binaries
# Michael A. Puening  Modified by Kim Buckner
#  uses the sun os compiler made in the cs460 home area
#  depends on an environment variable $OS to be set to either 
#  sos4 or sol2.  It then uses the correct host cross-compiler to
#  generate mips executables for the simulated machine.
#

CLASS=/cs/faculty/rich/cs170
XCOMP = $(CLASS)/xcomp
XCOMP_BIN=$(XCOMP)/bin
CC = ${XCOMP_BIN}/decstation-ultrix-gcc
LD = ${XCOMP_BIN}/decstation-ultrix-ld
AR = ${XCOMP_BIN}/decstation-ultrix-ar
AS = ${XCOMP_BIN}/decstation-ultrix-as
RANLIB = ${XCOMP_BIN}/decstation-ultrix-ranlib
C2N = ${XCOMP_BIN}/coff2noff

SUPPORT=${XCOMP}/lib
CRT = ${SUPPORT}/crt0.o
ASSIST = ${SUPPORT}/assist.o
LIBSYS = ${SUPPORT}/libsys.a
LIBC = ${SUPPORT}/libc.a
LDFLAGS = -G0 -T ${SUPPORT}/noff.ld -N -L${XCOMP}/lib
LIBS = $(LIBC) $(LIBSYS)


EXECUTABLES = cpu cat catbuf argtest argstack stack halt\
		writestack segfault instrerr\
		hw cat1 cat80 alot errors\
		errors2 exec getpid fork getppid\
		hw2 hw2fork pipe-bin pipe-bin2 read-write-80 my_cat grot fd\
		fea block unblock dup dup2 pipe exit tee-pipe fork-exec-stress\
		od cpu-fork-exec-stress

start: all

ostype:
	@if [ "$$OS" = "" ]; \
	then \
	echo "OS architecture environment variable not set: exiting"; \
	exit 1; \
	fi;

all: ${EXECUTABLES}

CFLAGS = -c -I${XCOMP}/include -G0 

#ENSURE that the -G0 option is used for linker and the compiler
#if it is not used, the linker generates too many sections of assembly
#code and then coff2noff fails to convert the file


getppid: getppid.o ${CRT} ${ASSIST} ${XCOMP}/lib/libsys.a ${XCOMP}/lib/libc.a
	${LD} ${LDFLAGS} -o $@.coff ${CRT} ${ASSIST} $@.o ${LIBS}
	${C2N} $@.coff $@
	/bin/rm $@.coff

fork: fork.o ${CRT} ${ASSIST} ${XCOMP}/lib/libsys.a ${XCOMP}/lib/libc.a
	${LD} ${LDFLAGS} -o $@.coff ${CRT} ${ASSIST} $@.o ${LIBS}
	${C2N} $@.coff $@
	/bin/rm $@.coff

getpid: getpid.o ${CRT} ${ASSIST} ${XCOMP}/lib/libsys.a ${XCOMP}/lib/libc.a
	${LD} ${LDFLAGS} -o $@.coff ${CRT} ${ASSIST} $@.o ${LIBS}
	${C2N} $@.coff $@
	/bin/rm $@.coff

exec: exec.o ${CRT} ${ASSIST} ${XCOMP}/lib/libsys.a ${XCOMP}/lib/libc.a
	${LD} ${LDFLAGS} -o $@.coff ${CRT} ${ASSIST} $@.o ${LIBS}
	${C2N} $@.coff $@
	/bin/rm $@.coff

cpu: cpu.o ${CRT} ${ASSIST} ${XCOMP}/lib/libsys.a ${XCOMP}/lib/libc.a
	${LD} ${LDFLAGS} -o $@.coff ${CRT} ${ASSIST} $@.o ${LIBS}
	${C2N} $@.coff $@
	/bin/rm $@.coff

hw2: hw2.o ${CRT} ${ASSIST} ${XCOMP}/lib/libsys.a ${XCOMP}/lib/libc.a
	${LD} ${LDFLAGS} -o $@.coff ${CRT} ${ASSIST} $@.o ${LIBS}
	${C2N} $@.coff $@
	/bin/rm $@.coff

hw2fork: hw2fork.o ${CRT} ${ASSIST} ${XCOMP}/lib/libsys.a ${XCOMP}/lib/libc.a
	${LD} ${LDFLAGS} -o $@.coff ${CRT} ${ASSIST} $@.o ${LIBS}
	${C2N} $@.coff $@
	/bin/rm $@.coff

pipe-bin: pipe-bin.o ${CRT} ${ASSIST} ${XCOMP}/lib/libsys.a ${XCOMP}/lib/libc.a
	${LD} ${LDFLAGS} -o $@.coff ${CRT} ${ASSIST} $@.o ${LIBS}
	${C2N} $@.coff $@
	/bin/rm $@.coff

pipe-bin2: pipe-bin2.o ${CRT} ${ASSIST} ${XCOMP}/lib/libsys.a ${XCOMP}/lib/libc.a
	${LD} ${LDFLAGS} -o $@.coff ${CRT} ${ASSIST} $@.o ${LIBS}
	${C2N} $@.coff $@
	/bin/rm $@.coff

errors2: errors2.o ${CRT} ${ASSIST} ${XCOMP}/lib/libsys.a ${XCOMP}/lib/libc.a
	${LD} ${LDFLAGS} -o $@.coff ${CRT} ${ASSIST} $@.o ${LIBS}
	${C2N} $@.coff $@
	/bin/rm $@.coff

errors: errors.o ${CRT} ${ASSIST} ${XCOMP}/lib/libsys.a ${XCOMP}/lib/libc.a
	${LD} ${LDFLAGS} -o $@.coff ${CRT} ${ASSIST} $@.o ${LIBS}
	${C2N} $@.coff $@
	/bin/rm $@.coff

alot: alot.o ${CRT} ${ASSIST} ${XCOMP}/lib/libsys.a ${XCOMP}/lib/libc.a
	${LD} ${LDFLAGS} -o $@.coff ${CRT} ${ASSIST} $@.o ${LIBS}
	${C2N} $@.coff $@
	/bin/rm $@.coff

cat: cat.o ${CRT} ${ASSIST} ${XCOMP}/lib/libsys.a ${XCOMP}/lib/libc.a
	${LD} ${LDFLAGS} -o $@.coff ${CRT} ${ASSIST} $@.o ${LIBS}
	${C2N} $@.coff $@
	/bin/rm $@.coff

catbuf: catbuf.o ${CRT} ${ASSIST} ${XCOMP}/lib/libsys.a ${XCOMP}/lib/libc.a
	${LD} ${LDFLAGS} -o $@.coff ${CRT} ${ASSIST} $@.o ${LIBS}
	${C2N} $@.coff $@
	/bin/rm $@.coff

cat1: cat1.o ${CRT} ${ASSIST} ${XCOMP}/lib/libsys.a ${XCOMP}/lib/libc.a
	${LD} ${LDFLAGS} -o $@.coff ${CRT} ${ASSIST} $@.o ${LIBS}
	${C2N} $@.coff $@
	/bin/rm $@.coff

cat80: cat80.o ${CRT} ${ASSIST} ${XCOMP}/lib/libsys.a ${XCOMP}/lib/libc.a
	${LD} ${LDFLAGS} -o $@.coff ${CRT} ${ASSIST} $@.o ${LIBS}
	${C2N} $@.coff $@
	/bin/rm $@.coff

argtest: argtest.o ${CRT} ${ASSIST} ${XCOMP}/lib/libsys.a ${XCOMP}/lib/libc.a
	${LD} ${LDFLAGS} -o $@.coff ${CRT} ${ASSIST} $@.o ${LIBS}
	${C2N} $@.coff $@
	/bin/rm $@.coff

argstack: argstack.o ${CRT} ${ASSIST} ${XCOMP}/lib/libsys.a ${XCOMP}/lib/libc.a
	${LD} ${LDFLAGS} -o $@.coff ${CRT} ${ASSIST} $@.o ${LIBS}
	${C2N} $@.coff $@
	/bin/rm $@.coff

stack: stack.o ${CRT} ${ASSIST} ${XCOMP}/lib/libsys.a ${XCOMP}/lib/libc.a
	${LD} ${LDFLAGS} -o $@.coff ${CRT} ${ASSIST} $@.o ${LIBS}
	${C2N} $@.coff $@
	/bin/rm $@.coff

halt: halt.o ${CRT} ${ASSIST} ${XCOMP}/lib/libsys.a ${XCOMP}/lib/libc.a
	${LD} ${LDFLAGS} -o $@.coff ${CRT} ${ASSIST} $@.o ${LIBS}
	${C2N} $@.coff $@
	/bin/rm $@.coff

writestack: writestack.o ${CRT} ${ASSIST} ${XCOMP}/lib/libsys.a ${XCOMP}/lib/libc.a
	${LD} ${LDFLAGS} -o $@.coff ${CRT} ${ASSIST} $@.o ${LIBS}
	${C2N} $@.coff $@
	/bin/rm $@.coff

segfault: segfault.o ${CRT} ${ASSIST} ${XCOMP}/lib/libsys.a ${XCOMP}/lib/libc.a
	${LD} ${LDFLAGS} -o $@.coff ${CRT} ${ASSIST} $@.o ${LIBS}
	${C2N} $@.coff $@
	/bin/rm $@.coff

buserror: buserror.o ${CRT} ${ASSIST} ${XCOMP}/lib/libsys.a ${XCOMP}/lib/libc.a
	${LD} ${LDFLAGS} -o $@.coff ${CRT} ${ASSIST} $@.o ${LIBS}
	${C2N} $@.coff $@
	/bin/rm $@.coff

instrerr: instrerr.o ${CRT} ${ASSIST} ${XCOMP}/lib/libsys.a ${XCOMP}/lib/libc.a
	${LD} ${LDFLAGS} -o $@.coff ${CRT} ${ASSIST} $@.o ${LIBS}
	${C2N} $@.coff $@
	/bin/rm $@.coff

hw: hw.o ${CRT} ${ASSIST} ${XCOMP}/lib/libsys.a ${XCOMP}/lib/libc.a
	${LD} ${LDFLAGS} -o $@.coff ${CRT} ${ASSIST} $@.o ${LIBS}
	${C2N} $@.coff $@
	/bin/rm $@.coff

pipe_test: pipe_test.o ${CRT} ${ASSIST} ${XCOMP}/lib/libsys.a ${XCOMP}/lib/libc.a
	${LD} ${LDFLAGS} -o $@.coff ${CRT} ${ASSIST} $@.o ${LIBS}
	${C2N} $@.coff $@
	/bin/rm $@.coff

park_test: park_test.o ${CRT} ${ASSIST} ${XCOMP}/lib/libsys.a ${XCOMP}/lib/libc.a
	${LD} ${LDFLAGS} -o $@.coff ${CRT} ${ASSIST} $@.o ${LIBS}
	${C2N} $@.coff $@
	/bin/rm $@.coff

bigtest: bigtest.o ${CRT} ${ASSIST} ${XCOMP}/lib/libsys.a ${XCOMP}/lib/libc.a
	${LD} ${LDFLAGS} -o $@.coff ${CRT} ${ASSIST} $@.o ${LIBS}
	${C2N} $@.coff $@
	/bin/rm $@.coff

md: md.o ${CRT} ${ASSIST} ${XCOMP}/lib/libsys.a ${XCOMP}/lib/libc.a
	${LD} ${LDFLAGS} -o $@.coff ${CRT} ${ASSIST} $@.o ${LIBS}
	${C2N} $@.coff $@
	/bin/rm $@.coff

read-write-80: read-write-80.o ${CRT} ${ASSIST} ${XCOMP}/lib/libsys.a ${XCOMP}/lib/libc.a
	${LD} ${LDFLAGS} -o $@.coff ${CRT} ${ASSIST} $@.o ${LIBS}
	${C2N} $@.coff $@
	/bin/rm $@.coff

my_cat: my_cat.o ${CRT} ${ASSIST} ${XCOMP}/lib/libsys.a ${XCOMP}/lib/libc.a
	${LD} ${LDFLAGS} -o $@.coff ${CRT} ${ASSIST} $@.o ${LIBS}
	${C2N} $@.coff $@
	/bin/rm $@.coff

grot: grot.o ${CRT} ${ASSIST} ${XCOMP}/lib/libsys.a ${XCOMP}/lib/libc.a
	${LD} ${LDFLAGS} -o $@.coff ${CRT} ${ASSIST} $@.o ${LIBS}
	${C2N} $@.coff $@
	/bin/rm $@.coff

fd: fd.o ${CRT} ${ASSIST} ${XCOMP}/lib/libsys.a ${XCOMP}/lib/libc.a
	${LD} ${LDFLAGS} -o $@.coff ${CRT} ${ASSIST} $@.o ${LIBS}
	${C2N} $@.coff $@
	/bin/rm $@.coff

fea: fea.o ${CRT} ${ASSIST} ${XCOMP}/lib/libsys.a ${XCOMP}/lib/libc.a
	${LD} ${LDFLAGS} -o $@.coff ${CRT} ${ASSIST} $@.o ${LIBS}
	${C2N} $@.coff $@
	/bin/rm $@.coff

block: block.o ${CRT} ${ASSIST} ${XCOMP}/lib/libsys.a ${XCOMP}/lib/libc.a
	${LD} ${LDFLAGS} -o $@.coff ${CRT} ${ASSIST} $@.o ${LIBS}
	${C2N} $@.coff $@
	/bin/rm $@.coff

dup: dup.o ${CRT} ${ASSIST} ${XCOMP}/lib/libsys.a ${XCOMP}/lib/libc.a
	${LD} ${LDFLAGS} -o $@.coff ${CRT} ${ASSIST} $@.o ${LIBS}
	${C2N} $@.coff $@
	/bin/rm $@.coff

dup2: dup2.o ${CRT} ${ASSIST} ${XCOMP}/lib/libsys.a ${XCOMP}/lib/libc.a
	${LD} ${LDFLAGS} -o $@.coff ${CRT} ${ASSIST} $@.o ${LIBS}
	${C2N} $@.coff $@
	/bin/rm $@.coff

unblock: unblock.o ${CRT} ${ASSIST} ${XCOMP}/lib/libsys.a ${XCOMP}/lib/libc.a
	${LD} ${LDFLAGS} -o $@.coff ${CRT} ${ASSIST} $@.o ${LIBS}
	${C2N} $@.coff $@
	/bin/rm $@.coff

pipe: pipe.o ${CRT} ${ASSIST} ${XCOMP}/lib/libsys.a ${XCOMP}/lib/libc.a
	${LD} ${LDFLAGS} -o $@.coff ${CRT} ${ASSIST} $@.o ${LIBS}
	${C2N} $@.coff $@
	/bin/rm $@.coff

exit: exit.o ${CRT} ${ASSIST} ${XCOMP}/lib/libsys.a ${XCOMP}/lib/libc.a
	${LD} ${LDFLAGS} -o $@.coff ${CRT} ${ASSIST} $@.o ${LIBS}
	${C2N} $@.coff $@
	/bin/rm $@.coff

tee-pipe: tee-pipe.o ${CRT} ${ASSIST} ${XCOMP}/lib/libsys.a ${XCOMP}/lib/libc.a
	${LD} ${LDFLAGS} -o $@.coff ${CRT} ${ASSIST} $@.o ${LIBS}
	${C2N} $@.coff $@
	/bin/rm $@.coff

fork-exec-stress: fork-exec-stress.o ${CRT} ${ASSIST} ${XCOMP}/lib/libsys.a ${XCOMP}/lib/libc.a
	${LD} ${LDFLAGS} -o $@.coff ${CRT} ${ASSIST} $@.o ${LIBS}
	${C2N} $@.coff $@
	/bin/rm $@.coff

od: od.o ${CRT} ${ASSIST} ${XCOMP}/lib/libsys.a ${XCOMP}/lib/libc.a
	${LD} ${LDFLAGS} -o $@.coff ${CRT} ${ASSIST} $@.o ${LIBS}
	${C2N} $@.coff $@
	/bin/rm $@.coff

cpu-fork-exec-stress: cpu-fork-exec-stress.o ${CRT} ${ASSIST} ${XCOMP}/lib/libsys.a ${XCOMP}/lib/libc.a
	${LD} ${LDFLAGS} -o $@.coff ${CRT} ${ASSIST} $@.o ${LIBS}
	${C2N} $@.coff $@
	/bin/rm $@.coff


.c.o:
	${CC} ${CFLAGS} $*.c

clean:
	/bin/rm -f *.s *.o *.coff $(EXECUTABLES) core a.out

tidy:
	/bin/rm -f *.o *.coff core a.out

move:
	mv ${EXECUTABLES} ..
