#include "../irdriver_api.h"

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>

#include <sys/ioctl.h>

#include <string.h>
#include <errno.h>

void signal_handler(int sig, siginfo_t *info, void *ucontext)
{
    printf("Signal caught %d, %d\n", sig, info->si_int);
    return;
}

int main(void)
{
    struct sigaction action;

    action.sa_flags = (SA_SIGINFO | SA_RESTART);
    action.sa_sigaction = signal_handler;
    if (sigaction(SIGNAL_IR, &action, NULL) != 0) {
        printf("%s: %s", __FUNCTION__, strerror(errno));
        return -1;
    }
    printf("Signal set for SIGNAL_IR = %d\n", SIGNAL_IR);
    int fd = open("/dev/irdriver", O_RDONLY);
    if (fd == -1) {
        printf("Error while opening /dev/irdriver. %s\n", strerror(errno));
        return -1;
    }

    pid_t pid = getpid();
    printf("Process pid %d\n", pid);
    int rc = ioctl(fd, CMD_REGISTER, pid);
    if (rc == -1) {
        printf("Error in ioctl. %s\n", strerror(errno));
        close(fd);
        return -1;
    }

    while (1);

    close(fd);
    return 0;
}