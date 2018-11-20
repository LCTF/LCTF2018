#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int ReadBytes(char *buffer, int bytes_no) 
{ 
    int bytes_read = 0;

    while (bytes_read < bytes_no) 
    {
        ssize_t ret = read(STDIN_FILENO, &buffer[bytes_read], bytes_no - bytes_read); 
        if (ret == 0){
            exit(1); 
        }
        bytes_read += ret;
        if(buffer[bytes_read-1]=='\n'){
            break;
        }
        //printf("byte_read=%p\n", (void*)bytes_read);
        //printf("ret=%p\n", (void*)ret);
    }
    return bytes_read;
}

char buff[0x100];

int main()
{
    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);

    int size;
    char data[0x100];
    while(1)
    {
        printf( "enter the size:\n");
        fflush(stdout);
        scanf("%99s", data);
        size = atoi(data);
        printf("size is %d\n", size);
        if (size <= 0)
        {
            break;
        }
        int retval = ReadBytes(buff, size);
        //printf("retval=%d\n", retval);
        write(1, buff, retval);
    }
    //printf("hello\n");
}

