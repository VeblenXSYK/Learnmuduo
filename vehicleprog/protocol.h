#ifndef PROTOCOL_H
#define PROTOCOL_H

#define		INFO_OK				0     /*正常*/
#define		INFO_LOOPSPEED		7　   /*线圈测速*/
#define		INFO_NOT_LINE		8　　 /*非直线前进*/
#define     RECV_HeadLen        12    /*接收的协议头的长度*/
#define     RECV_GaugeLen       1004  /*接收仪表数据长度*/
#define     RECV_DataLen        1005  /*接收到的全部数据长度*/ 
#define     RECV_TotalLen       1017  /**/

typedef char			CHAR;
typedef unsigned char	UCHAR;
typedef unsigned short	USINT;
typedef short			SINT;
typedef	unsigned int	UINT;
typedef	int				INT;
typedef long			LONG;
typedef unsigned long	ULONG;
typedef float			FLOAT;

typedef struct _HEAD
{
	CHAR	head;	/* 包标识@和!*/
	CHAR	code0;	/* 指令代码低位*/
	CHAR	code1;	/* 指令代码高位*/
	CHAR	lcr;	/* 填充字节为四字节对齐*/
	UINT	seq;	/*发送帧序列号*/
	UINT	len; 	/* 数据长+1个校验字节*/
}EZ_HEAD;

typedef struct _TIMETAG
{
	long	tv_sec;
	long	tv_usec;
	USINT	year;
	USINT	month;
	USINT	day;
	USINT	hour;
	USINT	minute;
	USINT	second;
	UINT	msecond;
}EZ_TIMETAG;

typedef struct _WIN_AXLE
{
	FLOAT		fWl;		/*左轮重*/
	FLOAT		fWr;		/*右轮重*/
	FLOAT		fW;			/*轴重*/
	FLOAT		fSpl;		/*左轮速度*/
	FLOAT		fSpr;		/*右轮速度*/
	FLOAT		fV;			/*轴速度*/
	FLOAT		fDis;		/*与前轴的轴距，第一轴为0*/
	FLOAT		fAngle;		/*未用*/
	UCHAR		cIndex;		/*未用*/
	UCHAR		cCenter; 	/*未用*/
	UCHAR		fType;		/*未用*/
	UCHAR		cPad[1];
}EZ_AXLE;

#define		MAX_AXLE_NUM		8
typedef struct _vehicle
{
	CHAR                cDir;				/*行车方向，前进为+，后退为－*/
	CHAR                cDircSet;			/*待定*/
	CHAR                cAxleCount;			/*轴数*/
	CHAR     	        cLevel;				/*称重质量信息*/
	INT 				nType;				/*轴型*/
	FLOAT               fWeight;			/*车辆总重*/
	FLOAT               fV;					/*车辆速度*/
	EZ_TIMETAG          begin;
	EZ_TIMETAG          end;
	EZ_AXLE             Axle[MAX_AXLE_NUM];	/*每轴的数据*/
	CHAR                nPad1[52];			/*填充*/
	CHAR                cColumn[4];			/*压过的列*/
	CHAR                cFn[256];			/*波形文件名*/
	CHAR                nPad2[340];			/*填充*/
}EZ_VEHICLE;

#endif
