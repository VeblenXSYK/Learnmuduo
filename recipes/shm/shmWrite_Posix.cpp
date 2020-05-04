#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */


#define ERR_EXIT(m) \
    do { \
        perror(m); \
        exit(EXIT_FAILURE); \
    } while(0)

typedef struct stu
{
    char name[32];
    int age;
} STU;

int main(void)
{
    int shmid = shm_open("/veblen", O_CREAT | O_RDWR, 0666);
    if (shmid == -1)
        ERR_EXIT("shm_open");

	if (ftruncate(shmid, sizeof(STU)) == -1)
		ERR_EXIT("ftruncate");
	
    struct stat buf;
    if (fstat(shmid, &buf) == -1)
        ERR_EXIT("fstat");

    printf("size=%ld, mode=%o\n", buf.st_size, buf.st_mode & 0777);

    STU *p;
    p = (STU *)mmap(NULL, buf.st_size, PROT_WRITE, MAP_SHARED, shmid, 0);
    if (p == MAP_FAILED)
        ERR_EXIT("mmap");

    strcpy(p->name, "chenqi");
    p->age = 20;

    close(shmid);
    return 0;
}
