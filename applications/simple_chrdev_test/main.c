# include <fcntl.h>
# include <stdio.h>
# include <unistd.h>

# include <sys/ioctl.h>

# include <string.h>
# include <errno.h>
#include <signal.h>


static void sig_handler(int sig, siginfo_t *si, void *u)
{
    printf("Received signal");
    if (si) {
        printf("Received signal %d, si_int=%d\n", sig, si->si_int);
        fflush(stdout);
    }
}

int main(void)
{
    struct sigaction sa;
    pid_t pid = getpid();

    sa.sa_sigaction = sig_handler;
    sa.sa_flags = SA_SIGINFO;

    int fd = open("/dev/irdriver", O_WRONLY);
    if (fd == -1) {
        printf("Error: open failed. %s\n", strerror(errno));
        return -1;
    }

    if (sigaction(SIGIO, &sa, NULL) == -1) {
        printf("Error: sigaction failed. %s\n", strerror(errno));
        return -1;
    }

    if (ioctl(fd, 1, pid) == -1) {
        printf("Error: ioctl failed. %s\n", strerror(errno));
        return -1;
    }

    while (1)
    {
        pause();
    }
    close(fd);

    return 0;
}