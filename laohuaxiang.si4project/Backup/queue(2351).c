#include "./queue/queue.h"

#ifdef 1
//���г�ʼ��
uint16_t QueueInit(MyQueue *q)
{
	q->front = 0;
	q->rear = 0;
	return OK;
}
//������Ԫ�صĸ���
uint16_t QueueLength(MyQueue *q)  
{  
    return  (q->rear - q->front + QUEMAXSIZE) % QUEMAXSIZE ;  
}  

//�ж϶����Ƿ���
uint16_t FullQueue(MyQueue *q)
{
	if((q->rear + 1) % QUEMAXSIZE == q->front)
		return  OK;
	else
		return 	ERROR;
}

//�ж϶����Ƿ�Ϊ��
uint16_t EmptyQueue(MyQueue *q)
{
	return q->front == q->rear;
}

//���
uint16_t EnterQueue(MyQueue *q,uint8_t index,uint16_t data)
{
	uint32_t intime;
	if(FullQueue(q))
		return ERROR;
	q->pData[q->rear].value = data;
	q->pData[q->rear].event = index;
	q->rear = (q->rear + 1) % QUEMAXSIZE;

	return OK;
}
//����
uint16_t DepartQueue(MyQueue *q,StateData *pData,uint32_t nowtime)
{
	 if( EmptyQueue(q))  
      return ERROR;  
		if(nowtime > q->pData[q->front].currenttime)
		{
			pData->currenttime = q->pData[q->front].currenttime;
			pData->event = q->pData[q->front].event;
			pData->value = q->pData[q->front].value;
			q->front = (q->front + 1) % QUEMAXSIZE;  
 		
			return OK;
		}	
			return ERROR;
}

//��ӡ��������
void PrintQueue(MyQueue *pQueue)
{
	int i,num;
	if(pQueue)
	{
		i = pQueue->front;
		num = QUEMAXSIZE - 1;
		while(0 != num)
		{
//			printf("%d  %d   %d  \r\n",pQueue->pData[i++].currenttime,pQueue->pData[i++].event,pQueue->pData[i++].value);
			--num;
		}
	}
}





#endif




















