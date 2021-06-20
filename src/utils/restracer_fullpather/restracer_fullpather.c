#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    if(argc <= 2) {
        printf("Usage:   %s <executable> <parentFullPath>\n", argv[0]);
        printf("Example: %s ./a.out /home/user/workDir/project/bin\n", argv[0]);
        puts(  "Will print:     /home/user/workDir/project/bin/a.out\n");
        printf("Example: %s   cat   /home/user/workDir/project/bin\n", argv[0]);
        puts(  "Will print:     cat\n");

        return 0;
    }

    if(argv[1] == strstr(argv[1], "./")) { /* need to convert to full path */
        size_t outLen = strlen(argv[1]) + strlen(argv[2]);
        char *newStr = (char *)malloc(outLen);
        if(!newStr) {
            perror("malloc()");
            return 1;
        }
        strcat(newStr, argv[2]);
        strcat(newStr, "/");
        strcat(newStr, argv[1] + 2);

        printf("%s\n", newStr);

        free(newStr);
    } else {
        printf("%s\n", argv[1]);
    }

    return 0;
}
