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
STACK* init();
STACK* commit(STACK* node, char * data);
void log(STACK* node);

int main(){

	STACK * node = NULL;
	
	char data[100];
	
	//system
	printf("¬ git�� ���� ���� ȯ���մϴ�\ngit ��ɾ �Է����ּ���. exit�� �Է��� ���α׷� ����\n");
	while(strcmp(command, "exit")!=0){
		gets(command);
		if(strcmp(command, "git init")==0){
			node = init();
		}
		else if(strcmp(command, "git commit -m")==0){
			if(!node){
				printf("���� git init�� ���ּ���.\n");
			}else{
				printf("Ŀ�� �޽����� �Է����ּ���\n");
				gets(data); 
				node = commit(node, data);
			}
		}
		else if(strcmp(command, "git log")==0){
			if(!node){
				printf("���� git init�� ���ּ���.\n");
			}else{
				log(node);
			}
		}
	}
	return 0;
}
STACK* init(){
	STACK *node = (STACK*)malloc(sizeof(STACK));
	node->next = NULL;
	return node;
}
STACK* commit(STACK* node, char * data){
	STACK *temp = (STACK*)malloc(sizeof(STACK));
	temp -> next = NULL;
	strcpy( temp -> buf, data);
	node -> next = temp;
	return node;
}
void log(STACK* node){
	while(node){
		printf("%s\n", node -> buf);
		node=node -> next;
	}
}

