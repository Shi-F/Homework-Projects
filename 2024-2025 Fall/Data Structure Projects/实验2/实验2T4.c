#define OK 1
#define ERROR 0
#define OVERFLOW -2
#include<stdio.h>
#include<stdlib.h>
typedef int Status;

typedef struct Customer{
	int Money;
	int ArriveTime;
	int LeaveTime;
	struct Customer *next;
}Customer;

typedef struct Event{
	int OccurTime; //事件的发生时间 
	int EventType; //1表示进入，0表示离开
	struct Event *next; 
}Event,*LinkList;

typedef struct QNode{
	Customer *ctm;
	struct QNode *next;
}QNode;

typedef struct Queue{
	QNode *front;
	QNode *rear; 
}Queue,*LinkQueue;

int N,total,close_time,average_time;
int t=0; 
	
Status InputCustomer(Customer *C,int N){
	//将客户信息输入带头结点链表 
	int i=0;
	int m,at;
	Customer *p = NULL,*q = C;
	for(i=0;i<N;i++){
		p=(Customer*)malloc(sizeof(Customer));
		if(p==NULL){
			printf("内存错误！\n");
			exit(OVERFLOW); 
		}
		scanf("%d %d",&m,&at);
		p->Money = m;
		p->ArriveTime = at;
		p->LeaveTime = 0;
		p->next = NULL;
		q->next = p;
		q = p;
	} 
	return OK; 
}

Status EnQueue(LinkQueue Q,Customer *c){
	//将客户c插入队尾
	QNode *p = NULL;
	p = (QNode*)malloc(sizeof(QNode));
	p->ctm = c;
	p->next = NULL;
	Q->rear->next = p;
	Q->rear = p;
	return OK; 
} 

Status EnList(LinkList events,int time,int EventType){
	//按时间顺序插入新事件
	Event *p = events;
	Event *q = NULL;
	q = (Event*)malloc(sizeof(Event));
	q->next = NULL;
	q->EventType = EventType;
	q->OccurTime = time;
	while(p->next && p->next->OccurTime <= time){
		p = p->next;
	}
	q->next = p->next;
	p->next = q;
	return OK;
}

int QueueLength(LinkQueue Q){
	//检查队列Q长度
	int n=0;
	QNode *p = Q->front->next;
	while(p){
		n++;
		p = p->next;
	}
	//printf("QueueLength now :%d\n",n);
	return n;
}

Customer *DelFrontQueue(LinkQueue Q){
	//删去Q队头，返回队头客户指针
	Customer *c;
	QNode *p = Q->front->next;
	c = p->ctm;
	Q->front->next = p->next;
	free(p);
	if(Q->front->next == NULL)
		Q->rear = Q->front;
	return c; 
}

Event DelFirstEvent(LinkList events){
	//删除事件列表第一个结点，返回其事件
	Event *p = events->next;
	Event e;
	e.EventType = p->EventType;
	e.OccurTime = p->OccurTime;
	e.next = NULL;
	events->next = p->next;
	free(p);
	return e;
}

Status CustomerArrived(LinkList events,LinkQueue Q1,Customer *nctm){
	//客户到达事件
	//需要执行：将当前客户插入队列1；
	//将下一个客户到达事件插入事件表
	//若队列1只剩一个，将该客户离开事件插入事件表
	EnQueue(Q1,nctm);
	//printf("Customer Arrived!");
	if(nctm->next != NULL){
		EnList(events,nctm->next->ArriveTime,1);
		//printf("next arrive event set! %d\n",nctm->next->ArriveTime);
	}
	//最后一个客户不需要引入到达事件
	if(QueueLength(Q1) == 1 && nctm->ArriveTime + average_time <= close_time){
		EnList(events,nctm->ArriveTime + average_time,0);
		//printf("next leave event set! %d\n",nctm->ArriveTime + average_time);
	}
	return OK; 
}

int CheckQ2(LinkQueue Q2,int BFTotal,int time){
	//检查Q2，返回办理用户的个数
	Customer *c;
	int num = QueueLength(Q2);
	int i=0,n=0;
	//记录Q2长度 
	while(total >= BFTotal && i < num){
		//循环结束条件为当前存款数低于上一次存款前存款数
		//或检查完毕Q2
		c = DelFrontQueue(Q2);
		if(total + c->Money >= 0){
			//存款足够，处理当前客户
			if(time + average_time*n <= close_time){
				total += c->Money;
				n++;
				c->LeaveTime = time + average_time*n; 
			}
		}
		else{
			//存款不足，把当前客户放到队尾 
			EnQueue(Q2,c); 
		}
		i++;
	}
	return n;
}

Status DestroyQueue(LinkQueue Q){
	//删除队列
	QNode *p = Q->front,*q;
	while(p){
		q = p->next;
		free(p);
		p = q;
	}
	return OK;
}

Status DestroyCustomers(Customer *c){
	//释放顾客链表 
	Customer *p = c,*q;
	while(p){
		q = p->next;
		free(p);
		p = q;
	}
	return OK;
}

Status CustomerDeparture(LinkList events,Event pe,LinkQueue Q1,LinkQueue Q2){
	//客户离开事件
	//需要执行：移除队列1队头客户
	//判断该用户是否去队列2
	//判断该用户是否是存款用户 
	//若队列1不空，队列1中下一个客户的离开事件插入事件表 
	Customer *c; 
	c = DelFrontQueue(Q1);
	int n=0;
	if(total + c->Money >= 0){
		//用户不需去队列2，修改存款，计算离开时间 
		total += c->Money;
		c->LeaveTime = pe.OccurTime;
		//printf("leaving!\n"); 
	}
	else{
		//用户需要去队列2
		EnQueue(Q2,c);
		//printf("moving!\n"); 
	}
	if(c->Money > 0){
		//若为存款用户，检查队列2
		n = CheckQ2(Q2,total - c->Money,pe.OccurTime);
		//n标记Q2中办理手续的用户数 
	}
	if(QueueLength(Q1) > 0 && pe.OccurTime + average_time + n*average_time <= close_time){
		//队列1不空
		EnList(events,pe.OccurTime + average_time + n*average_time ,0);
		//printf("next leave event set! %d\n",pe.OccurTime + average_time + n*average_time);
		//下一个离开事件发生的时间为：
		//当前时间+下一个客户接待时间+处理Q2所需时间 
	}
	return OK;
}

int main(){
	int TotalTime = 0;
	scanf("%d %d %d %d",&N,&total,&close_time,&average_time);
	Customer *customers,*nctm = NULL;
	customers = (Customer*)malloc(sizeof(Customer));
	customers->next = NULL;
	InputCustomer(customers,N);
	nctm = customers->next;
	//生成并读取客户信息链表
	LinkList events;
	Event pe; 
	events = (LinkList)malloc(sizeof(Event));
	events->next = NULL;
	EnList(events,nctm->ArriveTime,1);
	//创建事件列表，将第一个客户到达事件插入链表 
	LinkQueue Q1,Q2;
	Q1 = (LinkQueue)malloc(sizeof(Queue));
	Q2 = (LinkQueue)malloc(sizeof(Queue)); 
	Q1->front = (QNode*)malloc(sizeof(QNode));
	Q2->front = (QNode*)malloc(sizeof(QNode));
	Q1->front->next = NULL;
	Q2->front->next = NULL;
	Q1->rear = Q1->front;
	Q2->rear = Q2->front;
	//创建含头结点队列Q1,Q2并完成初始化  
	while(events->next != NULL){
		pe = DelFirstEvent(events);
		if(pe.EventType == 1){
			//为到达事件 
			CustomerArrived(events,Q1,nctm);
			nctm = nctm->next;
		}
		if(pe.EventType == 0){
			//为离开事件 
			CustomerDeparture(events,pe,Q1,Q2);
		}
	}
	//处理完所有事件 
	nctm = customers->next;
	while(nctm){
		if(nctm->LeaveTime != 0){
			printf("%d\n",nctm->LeaveTime - nctm->ArriveTime - average_time);
			TotalTime += nctm->LeaveTime - nctm->ArriveTime - average_time;
		}
		else{
			//leavetime为0说明一直等待到银行关门 
			printf("%d\n",close_time - nctm->ArriveTime);
			TotalTime += close_time - nctm->ArriveTime; 
		}
		nctm = nctm->next;
	}
	printf("%d",TotalTime/N);
	free(events);
	DestroyQueue(Q1);
	DestroyQueue(Q2);
	free(Q1);free(Q2);
	DestroyCustomers(customers);//清理内存 
	return OK;
}
