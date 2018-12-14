#ifndef  	__QUEUE_H
#define 	__QUEUE_H

#ifdef 1

#define OK 1
#define ERROR 0

/*定义循环队列的数组空间大小*/
#define QUEMAXSIZE 11


//GPIO值保存
typedef struct {
	uint8_t prev;
	uint8_t value;
} GpioValueItem;


//ADC状态值保存
typedef struct {
	uint16_t prev;
	uint16_t value;
	uint16_t threshold;
} AdcValueItem;

//事件数据记录
typedef struct {
	uint32_t currenttime;
	uint16_t  value;
	uint8_t event;
}StateData;	

//事件记录
typedef struct {
	uint8_t event;
	uint16_t value;
}FileRecord;
typedef struct Myqueue
{
	StateData pData[QUEMAXSIZE];  //数据
	uint8_t front;			//队列首位置
	uint8_t rear;				//队列尾位置
}MyQueue;



uint16_t QueueInit(MyQueue *q);
uint16_t QueueLength(MyQueue *q);
uint16_t FullQueue(MyQueue *q);
uint16_t EmptyQueue(MyQueue *q);
uint16_t DepartQueue(MyQueue *q,StateData *pData,uint32_t nowtime);
uint16_t EnterQueue(MyQueue *q,uint8_t index,uint16_t data);


void PrintQueue(MyQueue *pQueue);

#endif


#endif