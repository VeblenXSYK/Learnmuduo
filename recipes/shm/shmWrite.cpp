#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "common.h"

#define BUFSZ 512
#define PATHNAME "."

int main(int argc, char *argv[])
{
	// 显示路径信息
	struct stat buf;
	int ret = stat(PATHNAME, &buf);
	if(ret) {
		perror("stat\n");
	} else {
		printf("pathname info: st_dev=%x, st_ino=%x\n", (int)buf.st_dev, (int)buf.st_ino);
	}
	
	// 创建key值
	key_t key = ftok(PATHNAME, 0x38);
	if(key == -1) {
		perror("ftok\n");
	} else {
		printf("key = %x\n", key);
	}

	// 创建共享内存
	int shmid = shmget(key, sizeof(struct sm_msg), IPC_CREAT|0666);
	if(shmid < 0) {
		perror("shmget\n");
		exit(-1);
	}

	// 映射共享内存到本进程的虚拟地址
	char *shmadd = (char *)shmat(shmid, NULL, 0);
	if(shmadd < 0) {
		perror("shmat\n");
		exit(-1);
	}
	
	struct sm_msg *msg = (struct sm_msg *)shmadd;
	
	// 设置互斥锁属性
	pthread_mutexattr_t ma;
	pthread_mutexattr_init(&ma);
	pthread_mutexattr_setpshared(&ma, PTHREAD_PROCESS_SHARED);
	
	// 初始化互斥锁
	pthread_mutex_init(&msg->sm_mutex,&ma);

	// // 拷贝数据至共享内存
	// printf("copy data to shared-memory\n");
	// bzero(shmadd, BUFSZ);
	// strcpy(shmadd, "how are you!!!");
	
	int i = 0;
	char buffer[1024];
	char buffer1[1024];
	while(1) {
		bzero(buffer, sizeof(buffer));
		bzero(buffer1, sizeof(buffer1));
		
		if(i++ % 2 == 0) {
			sprintf(buffer, "veblen, my pid is %u", (unsigned int)getpid());
			sprintf(buffer1, ", what about you?");
		}
		else {
			sprintf(buffer, "VEBLEN, MY PID IS %u", (unsigned int)getpid());
			sprintf(buffer1, ", WHAT ABOUT YOU?");
		}
		
		pthread_mutex_lock(&msg->sm_mutex);
		// 分两次拷贝主要为了验证互斥锁是否有效
		memcpy(msg->buf, buffer, strlen(buffer));
		memcpy(msg->buf + strlen(buffer), buffer1, strlen(buffer1));
		pthread_mutex_unlock(&msg->sm_mutex);
	}

	return 0;
}
