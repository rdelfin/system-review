#include <stdio.h>
#include <errno.h>
#include <err.h>
#include <string.h>
#include <sys/utsname.h>

int main() {
    struct utsname uname_struct;
    if(uname(&uname_struct) != 0) {
        err(1, "Error running uname. Error: %s\n", strerror(errno));
        return 1;
    }

    printf("System info:\n");
    printf("\tSys Name:\t%s\n", uname_struct.sysname);
    printf("\tNode Name:\t%s\n", uname_struct.nodename);
    printf("\tRelease:\t%s\n", uname_struct.release);
    printf("\tVersion:\t%s\n", uname_struct.version);
    printf("\tMachine:\t%s\n", uname_struct.machine);
}
