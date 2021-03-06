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
    int shmid = shm_open("/veblen", O_RDONLY, 0);
    if (shmid == -1)
        ERR_EXIT("shm_open");
	
    struct stat buf;
    if (fstat(shmid, &buf) == -1)
        ERR_EXIT("fstat");

    printf("size=%ld, mode=%o\n", buf.st_size, buf.st_mode & 0777);

    STU *p;
    p = (STU *)mmap(NULL, buf.st_size, PROT_READ, MAP_SHARED, shmid, 0);
    if (p == MAP_FAILED)
        ERR_EXIT("mmap");

	// 尝试向共享内存写入数据（出现段错误）
	// p->age = 30;
	
    printf("name=%s age=%d\n", p->name, p->age);
    close(shmid);
    return 0;
}
