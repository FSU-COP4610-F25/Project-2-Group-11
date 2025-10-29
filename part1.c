#include <unistd.h>

int main() {
    char buffer[128];

    getpid();                   
    getppid();                 
    getuid();                 
    getcwd(buffer, sizeof(buffer));
    write(1, "Done\n", 5);     

    return 0;
}

