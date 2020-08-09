a.out: sim_uniform.o impl.o
	gcc sim_uniform.o impl.o

sim_uniform.o: sim_uniform.c header.h
	gcc -c sim_uniform.c

impl.o: impl.c header.h
	gcc -c impl.c
