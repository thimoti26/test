#include "Define.h"

long long fequenceTimer = 1; //toutes les secondes

sigset_t mask;

void Coucou(int a) {
    static int i = 0;
    string message = "GetVersion(";
    message += GetMacAddr();
    message += ")_";
    char Nombre[5];
    sprintf(Nombre, "%d", i);
    message += Nombre;
    write_server(message.c_str());
    i++;
}

void handler(int sig, siginfo_t *si, void *uc) {
    signal(sig, Coucou);
}

void InitTimer() {
    timer_t timerid;
    struct sigevent sev;
    struct itimerspec its;
    long long freq_nanosecs;
    struct sigaction sa;

    /* Establish handler for timer signal */

    printf("Establishing handler for signal %d\n", SIG);
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = handler;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIG, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    /* Block timer signal temporarily */

    printf("Blocking signal %d\n", SIG);
    sigemptyset(&mask);
    sigaddset(&mask, SIG);
    if (sigprocmask(SIG_SETMASK, &mask, NULL) == -1) {
        perror("sigprocmask");
        exit(EXIT_FAILURE);
    }
    /* Create the timer */

    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = SIG;
    sev.sigev_value.sival_ptr = &timerid;
    if (timer_create(CLOCKID, &sev, &timerid) == -1) {
        perror("timer_create");
        exit(EXIT_FAILURE);
    }

    printf("timer ID is 0x%lx\n", (long) timerid);

    /* Start the timer */

    its.it_value.tv_sec = fequenceTimer;
    its.it_value.tv_nsec = 0;
    its.it_interval.tv_sec = its.it_value.tv_sec;
    its.it_interval.tv_nsec = its.it_value.tv_nsec;

    if (timer_settime(timerid, 0, &its, NULL) == -1) {
        perror("timer_settime");
        exit(EXIT_FAILURE);
    }
    sleep(1);

    /* Unlock the timer signal, so that timer notification
       can be delivered */

    printf("Unblocking signal %d\n", SIG);

}

void DemarrageTimer() {
    if (sigprocmask(SIG_UNBLOCK, &mask, NULL) == -1) {
        perror("sigprocmask");
        exit(EXIT_FAILURE);
    }
}

