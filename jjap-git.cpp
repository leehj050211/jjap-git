#include<stdio.h>
#include<string.h>
#include<stdlib.h>
char command[100];

//stack
typedef struct STACK{
	char buf[100];
	STACK * next;
}STACK;
//function
STACK* init(void);
STACK* commit(STACK* node, char * data);

int main(){

	STACK * node;
	
	char data[100];
	
	//system
	printf("짭 git에 오신 것을 환영합니다\n밑에서 git 명령어를 입력해주세요. exit를 입력해 프로그램 종료\n");
	while(strcmp(command, "exit")!=0){
		gets(command);
		if(strcmp(command, "git init")==0){
			node = init();
		}
		else if(strcmp(command, "git commit -m")==0){
			printf("커밋 메시지를 입력해주세요");
			gets(data); 
			node = commit(node, data);
		}
	}
	return 0;
}
STACK * init(void){
	STACK *node = (STACK*)malloc(sizeof(STACK));
	node->next = NULL;
	return node;
}
STACK * commit(STACK* node, char * data){
	STACK *temp = (STACK*)malloc(sizeof(STACK));
	temp -> next = NULL;
	strcpy( temp -> buf, data);
	node -> next = temp;
	return node; 
}

