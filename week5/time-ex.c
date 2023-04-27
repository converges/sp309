#include <sys/times.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

static void pr_times(clock_t, struct tms *, struct tms *);
static void do_cmd(char *);

void err_sys(char *p) { perror(p); exit(-1); }

int main(int argc, char *argv[]) {

	for (int i=1; i<argc; i++) {
		do_cmd(argv[i]);
	}

	exit(0);

	return 0;
}

static void do_cmd(char *cmd) {

	/* execute and time the "cmd" */

	struct tms tmsstart, tmsend;
	clock_t start, end;
	int status;

	printf("\ncommand: %s\n", cmd);

	if ( (start=times(&tmsstart)) == -1 ) {
	       err_sys("times error");
	}

	if ( (status=system(cmd) < 0 ) ) {
		err_sys("system() error");
	}

	if ( (end=times(&tmsend)) == -1 ) {
		err_sys("times error");
	}

	pr_times(end-start, &tmsstart, *tmsend);
}

static void pr_times(clock_t real, struct tms *tmsstart, struct tms *tmsend) {

	static long clktck = 0;

	if (clktck == 0) {
		/* fetch clock ticks per second first time */

		if ( (clktck=sysconf(_SC_CLK_TCK)) < 0 ) {
			err_sys("sysconf error");
		}

		printf("    real: %7.2f\n", real / (double) clktck);
		printf("    user: %7.2f\n", (tmsend->tms_utime - tmsstart->tms_utime) / (double) clktck);
	}

}

