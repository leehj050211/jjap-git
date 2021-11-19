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
	printf("¬ git�� ���� ���� ȯ���մϴ�\n�ؿ��� git ��ɾ �Է����ּ���. exit�� �Է��� ���α׷� ����\n");
	while(strcmp(command, "exit")!=0){
		gets(command);
		if(strcmp(command, "git init")==0){
			node = init();
		}
		else if(strcmp(command, "git commit -m")==0){
			printf("Ŀ�� �޽����� �Է����ּ���");
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

