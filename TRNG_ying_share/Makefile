trng:
	gcc -o target target.c -L /usr/lib -ltrng_ying -lcrypto	
	@#gcc target.c sm3.o TRNG_ying.o -lcrypto -o target
rebuild:
	gcc -shared -fPIC -c TRNG_ying.c
	@#gcc -c TRNG_ying.c 
	@#gcc -c sm3.c
	gcc -shared -fPIC -c sm3.c
	gcc -shared -fPIC -o libtrng_ying.so sm3.o TRNG_ying.o
	sudo mv libtrng_ying.so /usr/lib

