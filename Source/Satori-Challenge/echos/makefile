CC=gcc
# C99 / PIE off / NX on / Canary on / RELRO off
CFLAFS= -std=c99 -no-pie -z noexecstack -fstack-protector -z norelro

echos: echos.c
	$(CC) -o echos -s $(CFLAFS) echos.c

clean:
	@rm echos

