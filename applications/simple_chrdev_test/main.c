# include <fcntl.h>
# include <stdio.h>
# include <unistd.h>

# include <sys/ioctl.h>

# include <string.h>
# include <errno.h>

int main(void)
{
    long args = 0;
    int fd = open("/dev/irdriver", O_WRONLY);
    if (fd == -1) {
        printf("Error: open failed. %s\n", strerror(errno));
        return -1;
    }

    if (ioctl(fd, 1, &args) == -1) {
        printf("Error: ioctl failed. %s\n", strerror(errno));
        return -1;
    }
    close(fd);

    return 0;
}