all:
	gcc -o pipe pipe.c
	gcc -o dual_pipe dual_pipe.c
	gcc -o monitor monitor.c
	gcc -o ctrlprocess ctrlprocess.c
	gcc -o procread procread.c
	gcc -o procwrite procwrite.c
	gcc -o namedpipeserver namedpipeserver.c
	gcc -o namedpipeclient namedpipeclient.c
	gcc -o ftok ftok.c
	gcc -o sendmsg sendmsg.c
	gcc -o rcvmsg rcvmsg.c
	gcc -o opmsg opmsg.c
	gcc -o msgserver msgserver.c
	gcc -o msgclient msgclient.c
	gcc -o semserver semserver.c
	gcc -o semclient semclient.c
	gcc -o semaphore semaphore.c
	gcc -o writer -I./ writer.c
	gcc -o reader -I./ reader.c


clean:
	rm -rf pipe dual_pipe procread procwrite namedpipeserver namedpipeclient ftok sendmsg rcvmsg opmsg msgserver msgclient semserver semclient semaphore writer reader monitor ctrlprocess readfifo writefifo

