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
		exit(-1);
	} else {
		printf("pathname info: st_dev=%x, st_ino=%x\n", (int)buf.st_dev, (int)buf.st_ino);
	}
	
	// 创建key值
	key_t key = ftok(PATHNAME, 0x38);
	if(key == -1) {
		perror("ftok\n");
		exit(-1);
	} else {
		printf("key = %x\n", key);
	}

	// 打开共享内存
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

	// // 读共享内存区数据
	// printf("data = [%s]\n", shmadd);
	
	struct sm_msg *msg = (struct sm_msg *)shmadd;
	
	char buffer[1024];
	while(1) {
		bzero(buffer, sizeof(buffer));
		
		pthread_mutex_lock(&msg->sm_mutex);
		memcpy(buffer, msg->buf, 100);
		pthread_mutex_unlock(&msg->sm_mutex);
		
		printf("read from shared-memory: %s\n", buffer);
	}
	
	// 分离共享内存和当前进程
	ret = shmdt(shmadd);
	if(ret < 0) {
		perror("shmdt\n");
		exit(-1);
	}
	
	// 删除共享内存
	ret = shmctl(shmid, IPC_RMID, NULL);
	if(ret < 0) {
		perror("shmctl\n");
		exit(-1);
	} else {
		printf("del shared-memory\n");
	}

	return 0;
}
