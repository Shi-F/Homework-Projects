#include<stdio.h>
#include<windows.h> 
#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASIBLE -1
#define OVERFLOW -2
#define NUM 50 
typedef int Status;

typedef struct LNode{
	double coef;
	int exp;
	struct LNode *next;
}LNode,*LinkList;

/*typedef struct SNode{
	char data;
	LinkList list; 
	struct SNode *next;
}SNode,*LinkStack;*/

typedef struct DivResult{
	LinkList quotient;
	LinkList remainder; 
}DivResult;//用结构体形式定义除法的结果，包括两个链表头指针，分别是商和余数 

int Locate(LinkList L,int nexp){
	//查找多项式内是否有指定的指数，有则返回该项的位置，没有返回0 
	LNode *p=L->next;
	int i=1; 
	while(p){
		if(p->exp == nexp) return i;
		p=p->next;i++;
	}
	return 0;
}

Status InitList(LinkList L){
	//在指定位置创建系数从高到低的多项式 
	LNode *p=NULL,*q=NULL;
	int nexp;
	double ncoe;
	int Locate(LinkList L,int nexp);
	while(1){
		printf("请输入指数（输入-1结束输入）：");
		scanf("%d",&nexp);
		getchar();
		while(Locate(L,nexp)){
			printf("指数重复，请重新输入:(输入-1结束输入):");
			scanf("%d",&nexp);
			getchar();
		}
		while(nexp < -1){
			printf("指数不合法，请重新输入:(输入-1结束输入):");
			scanf("%d",&nexp);
			getchar();
		}
		if(nexp == -1) break;
		printf("请输入系数：");
		scanf("%lf",&ncoe);
		getchar();
		if(ncoe != 0){
			p=(LNode*)malloc(sizeof(LNode));
			if(p==NULL){
				printf("内存错误!\n");
				exit(OVERFLOW);
			}
			p->exp = nexp;
			p->coef = ncoe;
			p->next = NULL;//完成新结点创建和输入 
			q=L;
			while(q->next && q->next->exp > p->exp){
				q=q->next;
			}
			p->next = q->next;
			q->next = p;
			//向链表中按顺序插入新结点 
			L->exp++;
		}
		//系数为0时不创建新结点，用仅含头结点的链表表示多项式0 
	}
	return OK;
}

Status PrintList(LinkList L){
	//输出多项式 
	LNode *p=L->next;
	if(p){
		printf("项数为：%d\n",L->exp);
		printf("多项式为：\n");
		while(p->next){
			if(p->exp != 1){
				if(p->next->coef > 0)
					printf("%.3lf x^%d +",p->coef,p->exp);
				else
					printf("%.3lf x^%d ",p->coef,p->exp);
			}
			else{
				if(p->next->coef > 0)
					printf("%.3lf x +",p->coef,p->exp);
				else
					printf("%.3lf x ",p->coef,p->exp);
			}
			p=p->next;
		}
		if(p->exp != 0 && p->exp != 1)
			printf("%.3lf x^%d ",p->coef,p->exp);
		else if(p->exp == 1) 
			printf("%.3lf x ",p->coef);
		else printf("%.3lf ",p->coef); 
	}
	else{
		printf("多项式为：0.000 ");
	}//分开考察多项式为0的情况 
	return OK;
}

LinkList AddList(LinkList La,LinkList Lb){
	//创建一个新多项式，内容为两个输入的多项式相加，返回结果的指针 
	LinkList Lc=(LinkList)malloc(sizeof(LNode));
	if(Lc==NULL){
		printf("内存错误!\n");
		exit(OVERFLOW);
	}
	Lc->exp=0;
	Lc->next = NULL;
	LNode *p=NULL,*q=Lc;
	LNode *pa=La->next,*pb=Lb->next;
	while(pa && pb){
		p=(LNode*)malloc(sizeof(LNode));
		if(p==NULL){
			printf("内存错误!\n");
			exit(OVERFLOW);
		}
		if(pa->exp > pb->exp) {
			p->exp = pa->exp;
			p->coef = pa->coef;
			p->next = NULL;
			q->next = p;
			q = p;
			pa = pa->next;
			Lc->exp++;
		}
		else if(pa->exp < pb->exp) {
			p->exp = pb->exp;
			p->coef = pb->coef;
			p->next = NULL;
			q->next = p;
			q = p;
			pb = pb->next;
			Lc->exp++;
		}//将指数较大的项放入Lc 
		else{
			if(pa->coef + pb->coef > 0.00001 || pa->coef + pb->coef < -0.00001){ //即pa与pb系数相加不为0，考虑浮点数的运算误差 
				p->exp = pa->exp;
				p->coef = pa->coef + pb->coef;
				p->next = NULL;
				q->next = p;
				q = p;
				pa = pa->next;
				pb = pb->next;
				Lc->exp++;
			}
			else{
			 	pa = pa->next;
				pb = pb->next;
				free(p);
			}//若出现指数相等的项，计算相加后系数是否为0，再加入Lc 
		}
	}
	while(pa){
		p=(LNode*)malloc(sizeof(LNode));
		if(p==NULL){
			printf("内存错误!\n");
			exit(OVERFLOW);
		}
		p->exp = pa->exp;
		p->coef = pa->coef;
		p->next = NULL;
		q->next = p;
		q = p;
		pa = pa->next;
		Lc->exp++;
	}
	while(pb){
		p=(LNode*)malloc(sizeof(LNode));
		if(p==NULL){
			printf("内存错误!\n");
			exit(OVERFLOW);
		}
		p->exp = pb->exp;
		p->coef = pb->coef;
		p->next = NULL;
		q->next = p;
		q = p;
		pb = pb->next;
		Lc->exp++;
	}//将La，Lb中剩余的项加入Lc 
	return Lc; 
} 

LinkList SubList(LinkList La,LinkList Lb){
	//创建一个新多项式，内容为两个输入的多项式相减，返回结果的指针
	//内容与加法基本相同，只将pb系数改为负数 
	LinkList Lc=(LinkList)malloc(sizeof(LNode));
	if(Lc==NULL){
		printf("内存错误!\n");
		exit(OVERFLOW);
	}
	Lc->exp=0;
	Lc->next = NULL;
	LNode *p=NULL,*q=Lc;
	LNode *pa=La->next,*pb=Lb->next;
	while(pa && pb){
		p=(LNode*)malloc(sizeof(LNode));
		if(p==NULL){
			printf("内存错误!\n");
			exit(OVERFLOW);
		}
		if(pa->exp > pb->exp) {
			p->exp = pa->exp;
			p->coef = pa->coef;
			p->next = NULL;
			q->next = p;
			q = p;
			pa = pa->next;
			Lc->exp++;
		}
		else if(pa->exp < pb->exp) {
			p->exp = pb->exp;
			p->coef = -pb->coef;
			p->next = NULL;
			q->next = p;
			q = p;
			pb = pb->next;
			Lc->exp++;
		}
		else{
			if(pa->coef - pb->coef > 0.00001 || pa->coef - pb->coef < -0.00001){ //即pa与pb系数相加不为0，考虑浮点数的运算误差 
				p->exp = pa->exp;
				p->coef = pa->coef - pb->coef;
				p->next = NULL;
				q->next = p;
				q = p;
				pa = pa->next;
				pb = pb->next;
				Lc->exp++;
			}
			else{
			 	pa = pa->next;
				pb = pb->next;
				free(p);
			}
		}
	}
	while(pa){
		p=(LNode*)malloc(sizeof(LNode));
		if(p==NULL){
			printf("内存错误!\n");
			exit(OVERFLOW);
		}
		p->exp = pa->exp;
		p->coef = pa->coef;
		p->next = NULL;
		q->next = p;
		q = p;
		pa = pa->next;
		Lc->exp++;
	}
	while(pb){
		p=(LNode*)malloc(sizeof(LNode));
		if(p==NULL){
			printf("内存错误!\n");
			exit(OVERFLOW);
		}
		p->exp = pb->exp;
		p->coef = -pb->coef;
		p->next = NULL;
		q->next = p;
		q = p;
		pb = pb->next;
		Lc->exp++;
	}
	return Lc; 
} 

double EvaluateList(LinkList L,double x){
	//代入x的值计算多项式的值并返回
	double sum = 0,pow;
	int i;
	LNode *p = L->next;
	while(p){
		pow = 1;
		for(i=0;i < p->exp;i++)
			pow *= x;
		sum += p->coef * pow;
		p = p->next;
	}
	return sum;
}

Status DestroyList(LinkList L){
	//释放L包括头结点内的所有结点 
	LNode *p = L,*q = NULL;
	while(p){
		q = p;
		p = p->next;
		free(q);
	}
	return OK;
}

Status ClearList(LinkList L){
	//释放L头结点外的所有结点 
	LNode *p = L->next,*q = NULL;
	while(p){
		q = p;
		p = p->next;
		free(q);
	}
	L->next = NULL;
	L->exp = 0;
	return OK;
}

Status InsertNode(LinkList L,int nexp,double ncoe){
	//插入新结点
	LNode *p = NULL, *q = NULL;
	p=(LNode*)malloc(sizeof(LNode));
	if(p==NULL){
		printf("内存错误!\n");
		exit(OVERFLOW);
	}
	p->exp = nexp;
	p->coef = ncoe;
	p->next = NULL;//完成新结点创建和输入 
	q = L;
	while(q->next && q->next->exp > p->exp){
		q=q->next;
	}
	p->next = q->next;
	q->next = p;
	//向链表中按顺序插入新结点 
	L->exp++;
	return OK;
}

Status DeleteNode(LinkList L,int location){
	//删除指定位置的结点
	LNode *p = NULL, *q = L;
	int i;
	for(i=1;i<location;i++)
		q = q->next;//q移动到需删除结点的前一位 
	p = q->next;
	q->next = p->next;
	free(p);
	L->exp--; 
	return OK;
} 

Status EditNode(LinkList L,int location,int nexp,double ncoe){
	LNode *p = NULL, *q = L;
	int i;
	for(i=1;i<location;i++)
		q = q->next;//q移动到需修改结点的前一位 
	p = q->next;
	q->next = p->next;//移除p 
	p->exp = nexp;
	p->coef = ncoe;
	p->next = NULL;//给p赋新值 
	q = L;
	while(q->next && q->next->exp > p->exp){
		q=q->next;
	}
	p->next = q->next;
	q->next = p;//插入p 
	return OK;
}

LinkList DiffList(LinkList L){
	//计算L的一阶导数存入新链表，返回新链表的头指针
	LinkList Ln=(LinkList)malloc(sizeof(LNode));
	if(Ln==NULL){
		printf("内存错误!\n");
		exit(OVERFLOW);
	}
	Ln->exp = 0;
	Ln->next = NULL;
	LNode *p = NULL,*q=Ln;
	LNode *pl = L->next;
	while(pl && pl->exp != 0){ //仅考察非常数项的导数 
		p=(LNode*)malloc(sizeof(LNode));
		if(p==NULL){
			printf("内存错误!\n");
			exit(OVERFLOW);
		}
		p->exp = pl->exp - 1;
		p->coef = pl->coef * pl->exp;
		p->next = NULL;
		q->next = p;
		q = p;
		pl = pl->next;
		Ln->exp++;
	}
	return Ln;
}

LinkList InteList(LinkList L){
	//计算L的一阶积分（常数项为0）存入新链表，返回新链表的头指针
	LinkList Ln=(LinkList)malloc(sizeof(LNode));
	if(Ln==NULL){
		printf("内存错误!\n");
		exit(OVERFLOW);
	}
	Ln->exp = 0;
	Ln->next = NULL;
	LNode *p = NULL,*q=Ln;
	LNode *pl = L->next;
	while(pl){
		p=(LNode*)malloc(sizeof(LNode));
		if(p==NULL){
			printf("内存错误!\n");
			exit(OVERFLOW);
		}
		p->exp = pl->exp + 1;
		p->coef = pl->coef / (pl->exp + 1);
		p->next = NULL;
		q->next = p;
		q = p;
		pl = pl->next;
		Ln->exp++;
	}
	return Ln;
}

LinkList MultList(LinkList La,LinkList Lb){
	//创建一个新多项式，内容为两个输入的多项式相乘，返回结果的指针 
	LinkList Lc=(LinkList)malloc(sizeof(LNode));
	LinkList Lsum=(LinkList)malloc(sizeof(LNode));
	LinkList temp = NULL;
	if(Lc==NULL || Lsum==NULL){
		printf("内存错误!\n");
		exit(OVERFLOW);
	}
	Lc->exp=0;
	Lc->next = NULL;
	Lsum->exp=0;
	Lsum->next = NULL;
	LNode *p=NULL,*q=NULL;
	LNode *pa=La->next,*pb=Lb->next;
	while(pa){
		pb = Lb->next;
		q = Lc;
		while(pb){
			p=(LNode*)malloc(sizeof(LNode));
			if(p==NULL){
				printf("内存错误!\n");
				exit(OVERFLOW);
			}
			p->exp = pa->exp + pb->exp;
			p->coef = pa->coef * pb->coef;
			p->next = NULL;
			q->next = p;
			q = p;
			pb = pb->next;
			Lc->exp++;
		}
		temp = Lsum;
		Lsum = AddList(temp,Lc);
		DestroyList(temp);
		ClearList(Lc); 
		pa = pa->next;
	}
	free(Lc);
	return Lsum; 
} 

DivResult DivList(LinkList La,LinkList Lb){
	//输入被除式与除式，以结构体形式输出商和余数的头指针
	DivResult res;
	LinkList Mult=NULL,temp = NULL;
	LNode *pa = La->next,*pb = Lb->next;
	LNode *p = NULL,*q = NULL;
	res.quotient = (LinkList)malloc(sizeof(LNode));
	p = (LNode*)malloc(sizeof(LNode));
	if(p==NULL){
		printf("内存错误!\n");
		exit(OVERFLOW);
	}
	p->coef = pa->coef / pb->coef;
	p->exp = pa->exp - pb->exp;
	p->next = NULL;
	res.quotient->next = p;
	res.quotient->exp = 1;
	q = p;
	Mult = MultList(res.quotient,Lb);
	res.remainder = SubList(La,Mult);
	DestroyList(Mult),Mult = NULL;
	while(res.remainder->next != NULL && res.remainder->next->exp >= pb->exp){
		p = (LNode*)malloc(sizeof(LNode));
		if(p==NULL){
			printf("内存错误!\n");
			exit(OVERFLOW);
		}
		p->coef = res.remainder->next->coef / pb->coef;
		p->exp = res.remainder->next->exp - pb->exp;
		p->next = NULL;
		q->next = p;
		q = p;
		res.quotient->exp++;
		Mult = MultList(res.quotient,Lb);
		temp = res.remainder;
		res.remainder = SubList(La,Mult);
		DestroyList(temp);temp=NULL;
		DestroyList(Mult);Mult=NULL;
	}
	return res;
}

/*char Precede(char a,char b){
	int la,lb;
	if(a != '('){
	if(a == '\0') la=0;
	if(a == '+' || a == '-') la=1;
	if(a == '*' || a == '/') la=2;
	if(b == '\0'|| b == ')') lb=0;
	if(b == '+' || b == '-') lb=1;
	if(b == '*' || b == '/') lb=2;
	if(b == '(') lb=3;
	if(la < lb) return '<';
	else return '>';
	}
	else{
		if(b == ')') return '=';
		else return '<';
	}	
}

LinkList Operate(LinkList a,char theta,LinkList b){
	LinkList res=NULL;
	DivResult divres;
	switch(theta){
		case '+':
			res = AddList(a,b);
			break;
		case '-':
			res = SubList(a,b);
		case '*':
			res = MultList(a,b);
		case '/':
			divres = DivList(a,b);
			res = divres.quotient;
			DestroyList(divres.remainder);
	}
	return res;
}

LinkList CalculateList(char *formula,LinkList *p){
	//读入表达式字符串和指针数组地址，返回四则运算结果
	LinkStack OPND = NULL,OPTR = NULL;
	SNode *ps = NULL,*qs = NULL;
	char *pc = formula;
	char theta;
	LinkList a=NULL,b=NULL;
	OPND = (LinkStack)malloc(sizeof(SNode));
	if(OPND==NULL){
		printf("内存错误!\n");
		exit(OVERFLOW);
	}
	OPTR = (LinkStack)malloc(sizeof(SNode));
	if(OPTR==NULL){
		printf("内存错误!\n");
		exit(OVERFLOW);
	}
	OPND->next = NULL;OPTR->next = NULL;
	ps = (SNode*)malloc(sizeof(SNode));
	if(ps==NULL){
		printf("内存错误!\n");
		exit(OVERFLOW);
	}
	ps->data = '\0';
	ps->next = OPTR->next;OPTR->next = ps;
	//将结尾符'\0'置入OPTR栈 
	while(*pc != '\0' || OPTR->next->data != '\0'){
		ps = (SNode*)malloc(sizeof(SNode));
		if(ps==NULL){
			printf("内存错误!\n");
			exit(OVERFLOW);
		}
		if(*pc >= '1' && *pc <= '9'){
			ps->list = p[*pc-'0'-1];
			ps->next = OPND->next;
			OPND->next = ps;
			pc++;//若为编号则将编号多项式头指针入OPND栈 
		}
		else switch(Precede(OPTR->next->data,*pc)){
			case '<':
				ps->data = *pc;
				ps->next = OPTR->next;
				OPTR->next = ps;
				pc++;
				break;
			case '=':
				qs = OPTR->next;
				OPTR->next = qs->next;
				free(qs);
				free(ps);
				pc++;
				break;
			case '>':
				qs = OPTR->next;
				OPTR->next = qs->next;
				theta = qs->data;
				free(qs);
				qs = OPND->next;
				OPND->next = qs->next;
				b = qs->list;
				free(qs);
				qs = OPND->next;
				OPND->next = qs->next;
				a = qs->list;
				free(qs);
				ps->list = Operate(a,theta,b);
				ps->next = OPND->next;
				OPND->next = ps;
				break; 
		}
	}
	a = OPND->next->list;
	free(OPTR->next);free(OPTR);
	free(OPND->next);free(OPND);
	return a; 
}*/

int menu(){
	int ope;
	do{
	printf("--------多项式计算器--------\n");
	printf("1、输入并创建多项式\n");
	printf("2、输出多项式\n");
	printf("3、求和\n");
	printf("4、求差\n");
	printf("5、求值\n");
	printf("6、销毁\n");
	printf("7、清空\n");
	printf("8、修改\n");
	printf("9、微分\n");
	printf("10、不定积分\n");
	printf("11、定积分\n");
	printf("12、乘法和乘方\n");
	printf("13、除法\n");
	/*printf("14. 多项式的四则运算\n");*/
	printf("0、退出程序\n");
	printf("----------------------------\n");
	printf("请输入操作：");
	scanf("%d",&ope);
	getchar();
	system("cls");
	if(ope<0 || ope>14){
		printf("输入有误，请重新输入！\n");
		getchar();
		system("cls");
	}
	}while(ope<0 || ope>14);
	return ope;
}

int main(){
	LinkList p[NUM] = {NULL};
	//char formula[NUM*5] = {0}; 
	int i,j,m,n,l;
	double x,y;
	LinkList result=NULL,temp=NULL;
	DivResult divresult; 
	char option;
	while(1){
	switch(menu()){
		case 0: 
			for(i=0;i<NUM;i++){
				if(p[i])
					DestroyList(p[i]);
			}
			printf("感谢您的使用，再会！\n");
			return OK;
			
		case 1:
			i=0;
			while(p[i]) i++;
			p[i]=(LinkList)malloc(sizeof(LNode));
			if(p[i]==NULL){
				printf("内存错误!\n");
				exit(OVERFLOW);
			}
			p[i]->exp=0;p[i]->next=NULL;
			if(InitList(p[i]))
				printf("创建成功，多项式编号为%d，按回车键返回菜单\n",i+1);
			getchar();
			system("cls");
			break;
			
		case 2:
			printf("请输入多项式编号：");
			scanf("%d",&i);
			getchar();
			if(p[i-1]){
				if(PrintList(p[i-1])){
					putchar('\n');
					printf("输出完毕，按回车键返回菜单\n");
				}
			}
			else printf("该编号下无多项式！\n"); 
			getchar();
			system("cls");
			break;
		
		case 3:
			printf("请输入需要相加的第一个多项式编号：");
			scanf("%d",&i);
			getchar();
			printf("请输入需要相加的第二个多项式编号：");
			scanf("%d",&j); 
			getchar();
			if(p[i-1] && p[j-1]){
				result=AddList(p[i-1],p[j-1]);
				PrintList(result);
				putchar('\n');
				printf("要存储该结果吗？（Y/N）：");
				option = getchar();
				getchar();
				while(option != 'Y' && option != 'N'){
					printf("请重新输入:");
					option = getchar();
					getchar();
				}
				if(option == 'Y'){
					i=0;
					while(p[i]) i++;
					p[i]=result;
					printf("存储成功，多项式编号为%d，按回车键返回菜单\n",i+1);
				}
				if(option == 'N'){
					DestroyList(result);
					printf("计算完毕，按回车键返回菜单\n",i+1);
				}
			}
			else printf("该编号下无多项式！\n");
			getchar();
			system("cls");
			break;
		
		case 4:
			printf("请输入被减数多项式编号：");
			scanf("%d",&i);
			getchar();
			printf("请输入减数多项式编号：");
			scanf("%d",&j); 
			getchar();
			if(p[i-1] && p[j-1]){
				result=SubList(p[i-1],p[j-1]);
				PrintList(result);
				putchar('\n');
				printf("要存储该结果吗？（Y/N）：");
				option = getchar();
				getchar();
				while(option != 'Y' && option != 'N'){
					printf("请重新输入:");
					option = getchar();
					getchar();
				}
				if(option == 'Y'){
					i=0;
					while(p[i]) i++;
					p[i]=result;
					printf("存储成功，多项式编号为%d，按回车键返回菜单\n",i+1);
				}
				if(option == 'N'){
					DestroyList(result);
					printf("计算完毕，按回车键返回菜单\n",i+1);
				}
			}
			else printf("该编号下无多项式！\n");
			getchar();
			system("cls");
			break;
		
		case 5:
			printf("请输入多项式编号：");
			scanf("%d",&i);
			getchar();
			if(p[i-1]){
				printf("请输入x的值：");
				scanf("%lf",&x);
				getchar();
				x = EvaluateList(p[i-1],x);
				printf("代入计算结果为 %.3lf，按回车键返回菜单\n",x);
			}
			else printf("该编号下无多项式！\n"); 
			getchar();
			system("cls");
			break;
			
		case 6:
			printf("请输入多项式编号：");
			scanf("%d",&i);
			getchar();
			if(p[i-1]){
				if(DestroyList(p[i-1]))
					p[i-1]=NULL;
					printf("销毁完毕，按回车键返回菜单\n");
				}
			else printf("该编号下无多项式！\n"); 
			getchar();
			system("cls");
			break;
			
		case 7:
			printf("请输入多项式编号：");
			scanf("%d",&i);
			getchar();
			if(p[i-1])
				if(ClearList(p[i-1]))
					printf("清空完毕，按回车键返回菜单\n");
			else printf("该编号下无多项式！\n"); 
			getchar();
			system("cls");
			break;
			
		case 8:
			printf("请输入多项式编号：");
			scanf("%d",&i);
			getchar();
			if(p[i-1]){
				do{
					printf("请选择操作：\n");
					printf("1、插入新的结点\n");
					printf("2、删除已有结点\n");
					printf("3、修改已有结点的系数和指数\n");
					printf("0、退出\n");
					scanf("%d",&n);
					getchar();
					system("cls");
					if(n<0 || n>3){
						printf("输入有误，请重新输入！\n");
						getchar();
						system("cls");
					}
				}while(n<0 || n>3);
				switch(n){
					case 1:
						do{
							printf("请输入新结点的指数（输入-1取消）：");
							scanf("%d",&j);
							getchar();
							while(j < -1){
								printf("指数不合法，请重新输入:(输入-1结束输入):");
								scanf("%d",&j);
								getchar();
							} 
							l = Locate(p[i-1],j);
							if(l) printf("该指数项已存在，请重新输入！\n");
						}while(l);
						if(j == -1) break;
						printf("请输入新结点的系数：");
						scanf("%lf",&x);
						getchar();
						if(x != 0)
							if(InsertNode(p[i-1],j,x)){
								printf("插入成功！当前");
								PrintList(p[i-1]);
								putchar('\n');
							}
						break;
						
					case 2:
						do{
							printf("请输入删除结点的指数（输入-1取消）：");
							scanf("%d",&j);
							getchar();
							l = Locate(p[i-1],j);
							if(!l && j >= 0) printf("未找到该项，请重新输入！\n"); 
						}while(!l && j >= 0);
						if(j == -1) break;
						if(DeleteNode(p[i-1],l)){
							printf("删除成功！当前");
							PrintList(p[i-1]);
							putchar('\n');
						}
						break;
						
					case 3:		
						do{
							printf("请输入需修改结点的指数（输入-1取消）：");
							scanf("%d",&j);
							getchar();
							l = Locate(p[i-1],j);
							if(!l && j >= 0) printf("未找到该项，请重新输入！\n"); 
						}while(!l && j >= 0);
						if(j == -1) break;
						do{
							printf("请输入新指数：");
							scanf("%d",&m);
							getchar();
							n = Locate(p[i-1],m);
							if(m != j && n) printf("已存在该指数项，请重新输入！\n"); 
						}while(m != j && n);
						printf("请输入新系数：");
						scanf("%lf",&x);
						getchar();
						if(x != 0){
							if(EditNode(p[i-1],l,m,x)){
								printf("修改成功！当前");
								PrintList(p[i-1]);
								putchar('\n');
							}
						}
						else if(DeleteNode(p[i-1],l)){
								printf("修改成功！当前");
								PrintList(p[i-1]);
								putchar('\n');
							}
						break;
					case 0: break;				
				}
			}
			else printf("该编号下无多项式！\n");
			printf("按回车键继续\n"); 
			getchar();
			system("cls");
			break;
			
		case 9:
			printf("请输入需要微分的多项式编号：");
			scanf("%d",&i);
			getchar();
			if(p[i-1]){
				printf("请输入需求的微分阶数：");
				scanf("%d",&n); 
				getchar();
				while(n <= 0){
					printf("需求的微分阶数输入有误，请重新输入：");
					scanf("%d",&n);
					getchar();
				}
				result=DiffList(p[i-1]);
				for(j=1;j<n;j++){
					temp = result;
					result = DiffList(temp);
					DestroyList(temp);
				}
				temp = NULL;
				PrintList(result);
				putchar('\n');
				printf("要存储该结果吗？（Y/N）：");
				option = getchar();
				getchar();
				while(option != 'Y' && option != 'N'){
					printf("请重新输入:");
					option = getchar();
					getchar();
				}
				if(option == 'Y'){
					i=0;
					while(p[i]) i++;
					p[i]=result;
					printf("存储成功，多项式编号为%d，按回车键返回菜单\n",i+1);
				}
				if(option == 'N'){
					DestroyList(result);
					printf("计算完毕，按回车键返回菜单\n",i+1);
				}
			}
			else printf("该编号下无多项式！\n");
			getchar();
			system("cls");
			break; 
			
		case 10:
			printf("请输入需要求不定积分的多项式编号：");
			scanf("%d",&i);
			getchar();
			if(p[i-1]){
				result=InteList(p[i-1]);
				PrintList(result);
				printf("+C （C为任意常数）\n");
				printf("要存储该结果吗？（Y/N）：");
				option = getchar();
				getchar();
				while(option != 'Y' && option != 'N'){
					printf("请重新输入:");
					option = getchar();
					getchar();
				}
				if(option == 'Y'){
					i=0;
					while(p[i]) i++;
					p[i]=result;
					printf("存储成功，多项式编号为%d，按回车键返回菜单\n",i+1);
				}
				if(option == 'N'){
					DestroyList(result);
					printf("计算完毕，按回车键返回菜单\n",i+1);
				}
			}
			else printf("该编号下无多项式！\n");
			getchar();
			system("cls");
			break; 
			
		case 11:
			printf("请输入需要求定积分的多项式编号：");
			scanf("%d",&i);
			getchar();
			if(p[i-1]){
				printf("请输入定积分起点：");
				scanf("%lf",&x); 
				getchar();
				printf("请输入定积分终点：");
				scanf("%lf",&y); 
				getchar();
				result=InteList(p[i-1]);
				printf("定积分结果为：%.3lf \n",EvaluateList(result,y)-EvaluateList(result,x));
				printf("计算完毕，按回车键返回菜单\n",i+1);
			}
			else printf("该编号下无多项式！\n");
			getchar();
			system("cls");
			break; 
		
		case 12:
			do{
				printf("1. 多项式相乘\n");
				printf("2. 多项式乘方\n");
				printf("请输入操作：");
				scanf("%d",&n);
				getchar();
				system("cls");
				if(n != 1 && n != 2){
					printf("输入有误，请重新输入！\n");
					getchar();
					system("cls");
				}
			}while(n != 1 && n != 2);
			if(n == 1){
				printf("请输入需要相乘的第一个多项式编号：");
				scanf("%d",&i);
				getchar();
				printf("请输入需要相乘的第二个多项式编号：");
				scanf("%d",&j); 
				getchar();
				if(p[i-1] && p[j-1]){
					result=MultList(p[i-1],p[j-1]);
					PrintList(result);
					putchar('\n');
					printf("要存储该结果吗？（Y/N）：");
					option = getchar();
					getchar();
					while(option != 'Y' && option != 'N'){
						printf("请重新输入:");
						option = getchar();
						getchar();
					}
					if(option == 'Y'){
						i=0;
						while(p[i]) i++;
						p[i]=result;
						printf("存储成功，多项式编号为%d，按回车键返回菜单\n",i+1);
					}
					if(option == 'N'){
						DestroyList(result);
						printf("计算完毕，按回车键返回菜单\n",i+1);
					}
				}
				else printf("该编号下无多项式！\n");
			}
			if(n == 2){
				printf("请输入需要求乘方的多项式编号：");
				scanf("%d",&i);
				getchar();
				if(p[i-1]){
					printf("请输入乘方次数：");
					scanf("%d",&n); 
					getchar();
					while(n <= 1){
						printf("乘方次数输入有误，请重新输入：");
						scanf("%d",&n);
						getchar();
					}
					result = MultList(p[i-1],p[i-1]);
					for(j=2;j<n;j++){
						temp = result;
						result = MultList(temp,p[i-1]);
						DestroyList(temp);
					}
					temp = NULL;
					PrintList(result);
					putchar('\n'); 
					printf("要存储该结果吗？（Y/N）：");
					option = getchar();
					getchar();
					while(option != 'Y' && option != 'N'){
						printf("请重新输入:");
						option = getchar();
						getchar();
					}
					if(option == 'Y'){
						i=0;
						while(p[i]) i++;
						p[i]=result;
						printf("存储成功，多项式编号为%d，按回车键返回菜单\n",i+1);
					}
					if(option == 'N'){
						DestroyList(result);
						printf("计算完毕，按回车键返回菜单\n",i+1);
					}
				}
				else printf("该编号下无多项式！\n");
			}
			getchar();
			system("cls");
			break;
			
		case 13:
			printf("请输入被除数多项式编号：");
			scanf("%d",&i);
			getchar();
			printf("请输入除数多项式编号：");
			scanf("%d",&j); 
			getchar();
			if(p[i-1] && p[j-1]){
				if(p[i-1]->next == NULL){
					printf("被除数为0，结果为0\n"); 
				}
				else{
					if(p[j-1]->next == NULL){
						printf("除数为0\n");
					}
					else{
						if(p[i-1]->next->exp < p[j-1]->next->exp){
							printf("被除数最高次数小于除数最高次数\n");
						}
						else{
								divresult = DivList(p[i-1],p[j-1]);
								printf("商");
								PrintList(divresult.quotient);
								putchar('\n');
								printf("余数");
								PrintList(divresult.remainder); 
								putchar('\n');
								printf("要存储商吗？（Y/N）：");
								option = getchar();
								getchar();
								while(option != 'Y' && option != 'N'){
									printf("请重新输入:");
									option = getchar();
									getchar();
								}
								if(option == 'Y'){
									i=0;
									while(p[i]) i++;
									p[i] = divresult.quotient;
									printf("存储成功，多项式编号为%d\n",i+1);
								}
								if(option == 'N'){
									DestroyList(divresult.quotient);
								}
								printf("要存储余数吗？（Y/N）：");
								option = getchar();
								getchar();
								while(option != 'Y' && option != 'N'){
									printf("请重新输入:");
									option = getchar();
									getchar();
								}
								if(option == 'Y'){
									i=0;
									while(p[i]) i++;
									p[i] = divresult.remainder;
									printf("存储成功，多项式编号为%d，按回车键返回菜单\n",i+1);
								}
								if(option == 'N'){
									DestroyList(divresult.remainder);
									printf("计算完毕，按回车键返回菜单\n",i+1);
								}
							}
						}
					}
				}
			else printf("该编号下无多项式！\n");
			getchar();
			system("cls");
			break;
		
		/*case 14:
			printf("请输入四则运算式（用数字编号表示）：\n");
			gets(formula);
			result = CalculateList(formula,p);
			PrintList(result);
			putchar('\n');
			printf("要存储该结果吗？（Y/N）：");					
			option = getchar();
			getchar();
			while(option != 'Y' && option != 'N'){
				printf("请重新输入:");
				option = getchar();
				getchar();
			}
			if(option == 'Y'){
				i=0;
				while(p[i]) i++;
				p[i]=result;
				printf("存储成功，多项式编号为%d，按回车键返回菜单\n",i+1);
			}
			if(option == 'N'){
				DestroyList(result);
				printf("计算完毕，按回车键返回菜单\n",i+1);
			}
			getchar();
			system("cls");
			break;*/
		}
	}
}


