all:
	gcc -o my_signal my_signal.c -lpthread
	gcc -o my_sigaction my_sigaction.c -lpthread
	gcc -o wrong_return wrong_return.c -lpthread
	gcc -o right_return right_return.c -lpthread
	gcc -o my_kill my_kill.c -lpthread
	gcc -o simulate_ping simulate_ping.c -lpthread
	gcc -o test_setitimer test_setitimer.c -lpthread
	gcc -o sig_mask sig_mask.c -lpthread
	gcc -o sig_suspend sig_suspend.c -lpthread
	gcc -o send_data_signo send_data_signo.c -lpthread
	gcc -o recv_data_signo recv_data_signo.c -lpthread
	gcc -o wait_cycle wait_cycle.c -lpthread
	gcc -o wait_pause wait_pause.c -lpthread
	gcc -o wait_sigsuspend wait_sigsuspend.c -lpthread

clean:
	rm -rf my_signal my_sigaction wrong_return right_return my_kill simulate_ping test_setitimer sig_mask sig_suspend send_data_signo recv_data_signo wait_cycle wait_pause wait_sigsuspend

