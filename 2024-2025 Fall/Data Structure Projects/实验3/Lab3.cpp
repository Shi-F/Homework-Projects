//需要完成的任务：
//压缩文件： 
//	读入文件，统计每种字节出现的次数（256种） 
//	根据字节出现的次数创建Huffman树
//	由Huffman树得到各字节的Huffman编码
//	读入文件，按字节将对应的编码写入.huff文件
//	将压缩前文件类型、各字节出现的次数信息写入.huff文件
//解压缩文件：
//	读入.huff文件，根据文件开头的信息获得Huffman树 
//	根据Huffman编码，将对应的字节写入文件
//----------------------------------------------------------
//.huff文件的结构为：
//char FileType[FILE_TYPE_LENGTH] 源文件类型（,xxx） 
//int FileSize 源文件大小（用于解压时确定是否解码结束） 
//int count[256] 各字节按顺序的出现次数 
//Huffman码比特流 
 
#include<stdio.h>
#include<stdlib.h> 
#include<windows.h>
#include<string.h>
#define OK 1
#define TRUE 1
#define FALSE 0
#define OVERFLOW -2 
#define FILE_TYPE_LENGTH 6
typedef int Status;

typedef struct Info{
	int count;	//无符号字符出现的次数
	char *HCode;	//字符对应的Huffman编码 
}Info;	//文件内无符号字符的信息 

typedef struct HTNode{
	unsigned char data;
	int weight;
	int lchild,rchild;
	int parent;
}HTNode,*HuffmanTree;

int menu(){
	//输出菜单，返回操作
	int operation;
 	printf("Huffman码压缩/解压程序\n");
	printf("----------------------\n"); 
	printf("1.压缩文件\n");
	printf("2.解压文件\n");
	printf("0.退出\n");
	printf("----------------------\n");
	printf("请输入操作："); 
	scanf("%d",&operation);
	return operation;
}

Status CountByte(FILE *OriginFile,Info *FileByteInfo,int &types,int &FileSize){
	//统计字符出现次数 
	unsigned char ReadBuff[1024];
	int ReadSize;
	int i;
	//每次读取1KB数据 
	while(1){
		ReadSize = fread(ReadBuff,1,1024,OriginFile);
		if(ReadSize == 0){
			//读取完毕 
			break;
		}
		FileSize += ReadSize;
		for(i=0;i<ReadSize;i++){
			if(FileByteInfo[ReadBuff[i]].count == 0)
				types++;
			FileByteInfo[ReadBuff[i]].count++;
		}
	}
	return OK;
}

Status FindSmallest(HuffmanTree T,int i,int &s1,int &s2){
	//找到数组中权重最小且未被连接过的两个结点 
	int j = 1;
	while(T[j].parent) j++;
	s1 = j;
	for(j++;j < i;j++){
		if(T[j].weight < T[s1].weight && T[j].parent == 0) s1=j;
	}
	j=1;
	while(T[j].parent || j == s1) j++;
	s2 = j;
	for(j++;j < i;j++){
		if(T[j].weight < T[s2].weight && j!=s1 && T[j].parent == 0) s2=j;
	}
	return OK;
}

Status InitHuffmanTree(HuffmanTree &T,Info *FileByteInfo,int types){
	//建立Huffman树（顺序存储类型） 
	T = (HTNode*)malloc((sizeof(HTNode)*types*2));
	int i=1,j=0;
	int s1,s2;
	for(i=1;i<=2*types-1;i++){
		T[i].lchild = 0; T[i].rchild = 0;
		T[i].parent = 0;
	} 
	i=1;
	for(j=0;j<256;j++){
		if(FileByteInfo[j].count){
			T[i].data = j;
			T[i].weight = FileByteInfo[j].count;
			i++;
		}
	}//完成每个结点的输入
	for(i=types+1;i <= 2*types-1;i++){
		FindSmallest(T,i,s1,s2);
		//找到1到i-1中权重最小且未被连接过的两个结点
		T[i].lchild = s1;
		T[i].rchild = s2;
		T[i].weight = T[s1].weight+T[s2].weight;
		T[s1].parent = i;T[s2].parent = i;
		//完成连接 
	}//完成Huffman树的创建，最后一个节点为根
	return OK;
}

Status HuffmanCoding(HuffmanTree T,Info *FileByteInfo,int types){
	//根据Huffman树得到各字符的Huffman编码
	char *cd = (char*)malloc(sizeof(char)*types);
	//构建工作区cd
	cd[types-1] = '\0';
	int i;
	int c,f,start;
	for(i=1;i<=types;i++){
		start = types-1;
		c = i; f = T[i].parent;
		while(f){
			if(c == T[f].lchild){
				cd[--start] = '0';
			}
			else{
				cd[--start] = '1';
			}
			c = f;
			f = T[f].parent;
		}
		FileByteInfo[T[i].data].HCode = (char*)malloc(sizeof(char)*(types-start));
		strcpy(FileByteInfo[T[i].data].HCode,&cd[start]);
	}
	return OK; 
}

Status ReNaming(char *FileName,char *FileType,int mode){
	//更改文件后缀
	//mode 1为更改为.huff后缀 
	//mode 2为将.huff后缀改回原后缀 
	char *p = FileName,*q,*pt = FileType;
	while(*p != '.') p++;
	q = p;
	if(mode == 1){
		while(*q){
			*pt = *q;
			pt++;
			*q = '\0';
			q++;
		}
		//清空原有后缀，将原有后缀保存到FileType中 
		strcpy(p,".huff");
	}
	if(mode == 2){
		while(*q){
			*q = '\0';
			q++;
		}
		//清空原有后缀 
		strcpy(p,FileType);
	}
	return OK;
}

Status WriteInfo(FILE *NewFile,Info *FileByteInfo){
	//将各字节的出现次数写入文件 
	int i;
	for(i=0;i<256;i++){
		fwrite(&(FileByteInfo[i].count),sizeof(int),1,NewFile);
	}
	return OK;
} 

Status WriteHuffmanCode(FILE *OriginFile,FILE *NewFile,Info *FileByteInfo){
	//按源文件写入对应的Huffman编码
	unsigned char ReadBuff[1024];
	int ReadSize;
	unsigned char ch = 0;
	int BitCount = 0; 
	char *code = NULL;
	int i;
	//每次读取1KB数据 
	while(1){
		ReadSize = fread(ReadBuff,1,1024,OriginFile);
		if(ReadSize == 0){
			//读取完毕 
			break;
		}
		for(i=0;i<ReadSize;i++){
			code = FileByteInfo[ReadBuff[i]].HCode;
			while(*code){
				ch = ch << 1;
				if(*code == '1')
					ch = ch | 1;
				BitCount++;
				if(BitCount == 8){
					fputc(ch,NewFile);
					BitCount = 0;
				}
				code++;
			}
		}
	}
	if(BitCount != 0){
		//剩余不足1字节的编码未写入 
		ch = ch << (8-BitCount);
		fwrite(&ch,1,1,NewFile);
	}
	return OK; 
}

Status HuffmanCompress(Info *FileByteInfo){
	//压缩过程
	char FileName[100] = {0};
	char FileType[FILE_TYPE_LENGTH] = {0};
	int types = 0,FileSize = 0;
	HuffmanTree T = NULL;
	FILE *OriginFile,*NewFile;
	printf("请输入文件路径:\n");
	scanf("%s",FileName);
	if(!(OriginFile = fopen(FileName,"rb"))){
		printf("打开文件错误！\n");
		exit(ERROR);
	}
	//打开源文件
	CountByte(OriginFile,FileByteInfo,types,FileSize);
	//完成字符数统计
	InitHuffmanTree(T,FileByteInfo,types);
	//创建Huffman树 
	HuffmanCoding(T,FileByteInfo,types);
	//完成Huffman编码 
	rewind(OriginFile);
	ReNaming(FileName,FileType,1);
	if(!(NewFile = fopen(FileName,"wb"))){
		printf("打开文件错误！\n");
		exit(ERROR);
	}
	//创建.huff文件
	fwrite(FileType,sizeof(char),FILE_TYPE_LENGTH,NewFile);
	//写入源文件类型
	fwrite(&FileSize,sizeof(int),1,NewFile);
	//写入源文件大小 
	WriteInfo(NewFile,FileByteInfo);
	//写入字符数信息 
	WriteHuffmanCode(OriginFile,NewFile,FileByteInfo);
	//写入Huffman编码
	fclose(OriginFile);
	fclose(NewFile);
	printf("压缩完成！压缩文件存储路径为：%s\n",FileName);
	return OK; 
}

Status GetInfo(FILE *OriginFile,Info *FileByteInfo,int &types){
	//从文件中读入字符数信息
	int i;
	for(i=0;i<256;i++){
		fread(&(FileByteInfo[i].count),sizeof(int),1,OriginFile);
		if(FileByteInfo[i].count) types++;
	}
	return OK; 
} 

Status HuffmanDecoding(HuffmanTree T,int types,int FileSize,FILE *OriginFile,FILE *NewFile){
	//按Huffman编码还原源文件 
	unsigned char ReadBuff[1024];
	int ReadSize;
	int SizeCount = 0; 
	int i;
	int pt = 2*types - 1; //从根开始 
	unsigned char ch = 0;
	int BitCount = 0; 
	//每次读取1KB数据 
	while(1){
		ReadSize = fread(ReadBuff,1,1024,OriginFile);
		if(ReadSize == 0){
			//读取完毕 
			break;
		}
		for(i=0;i<ReadSize;i++){
			ch = ReadBuff[i];
			for(BitCount = 0;BitCount < 8;BitCount++){
				if(ch & 0x80){
					//读取位为1 
					pt = T[pt].rchild;
				}
				else{
					//读取位为0 
					pt = T[pt].lchild;
				}
				if(T[pt].lchild == 0 && T[pt].rchild == 0){
					//找到叶子结点，输出对应字节
					fputc(T[pt].data,NewFile);
					pt = 2*types - 1;
					//pt重置为根
					SizeCount++;
					if(SizeCount == FileSize){
						break;
					}
				}
				ch = ch << 1;
			}
		}
	}
	return OK;
}

Status HuffmanDecompress(Info *FileByteInfo){
	//解压过程
	char FileName[100] = {0};
	char FileType[FILE_TYPE_LENGTH] = {0};
	int types = 0,FileSize = 0;
	HuffmanTree T = NULL;
	FILE *OriginFile,*NewFile;
	printf("请输入文件路径:\n");
	scanf("%s",FileName);
	if(!(OriginFile = fopen(FileName,"rb"))){
		printf("打开文件错误！\n");
		exit(ERROR);
	}
	//打开压缩文件
	fread(FileType,sizeof(char),FILE_TYPE_LENGTH,OriginFile);
	//读入源文件类型
	ReNaming(FileName,FileType,2);
	if(!(NewFile = fopen(FileName,"wb"))){
		printf("打开文件错误！\n");
		exit(ERROR);
	}
	//创建源文件
	fread(&FileSize,sizeof(int),1,OriginFile);
	//读入源文件大小 
	GetInfo(OriginFile,FileByteInfo,types);
	//读入字节数信息
	InitHuffmanTree(T,FileByteInfo,types);
	//创建Huffman树
	HuffmanDecoding(T,types,FileSize,OriginFile,NewFile); 
	//按Huffman编码还原源文件 
	printf("解压完成！文件存储路径为：%s\n",FileName);
}
	
Status main(){
	Info FileByteInfo[256];	//数组下标即为对应字节内容 
	int i;
	HuffmanTree T = NULL;
	for(i = 0;i < 256;i++){
		FileByteInfo[i].count = 0;
		FileByteInfo[i].HCode = NULL;
	}//初始化 
	while(1){
		switch(menu()){
			case 1:
				HuffmanCompress(FileByteInfo);
				break;
			case 2:
				HuffmanDecompress(FileByteInfo);
				break;
			case 0:
				printf("感谢您的使用，再会！\n");
				return OK;
		}
		for(i = 0;i < 256;i++){
			FileByteInfo[i].count = 0;
			if(FileByteInfo[i].HCode) free(FileByteInfo[i].HCode);
			FileByteInfo[i].HCode = NULL;
		}
		if(T){
			free(T);
			T = NULL;
		}//清空数据 
	}
}
