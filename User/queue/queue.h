#ifndef  	__QUEUE_H
#define 	__QUEUE_H

#ifdef 1

#define OK 1
#define ERROR 0

/*����ѭ�����е�����ռ��С*/
#define QUEMAXSIZE 11


//GPIOֵ����
typedef struct {
	uint8_t prev;
	uint8_t value;
} GpioValueItem;


//ADC״ֵ̬����
typedef struct {
	uint16_t prev;
	uint16_t value;
	uint16_t threshold;
} AdcValueItem;

//�¼����ݼ�¼
typedef struct {
	uint32_t currenttime;
	uint16_t  value;
	uint8_t event;
}StateData;	

//�¼���¼
typedef struct {
	uint8_t event;
	uint16_t value;
}FileRecord;
typedef struct Myqueue
{
	StateData pData[QUEMAXSIZE];  //����
	uint8_t front;			//������λ��
	uint8_t rear;				//����βλ��
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