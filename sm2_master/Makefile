target:
	gcc main.c -o sm2 -L /usr/lib -lverify -lmiracl
rebuild:
	gcc -shared -fPIC -c sm2.c
	gcc -shared -fPIC -c sm2_enc.c
	gcc -shared -fPIC -c kdf.c
	gcc -shared -fPIC -c sm3.c
	gcc -shared -fPIC -c sm2_key_ex.c
	gcc -shared -fPIC -c sm2_sv.c
	gcc -shared -fPIC -o libverify.so *.o
	cp libverify.so /home/ling/semi
	sudo mv /home/ling/semi/libverify.so /usr/lib
