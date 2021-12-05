#include<stdio.h>
#include<string.h>
#include<stdlib.h>
char command[100];

//stack
typedef struct STACK{
	char buf[100];
	char id[7];
	STACK * next;
}STACK;
//git functions
STACK* init();
void commit(STACK* node, char * data);
void log(STACK* node);
//functions
char* genCommitId();

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
				commit(node, data);
			}
		}
		else if(strcmp(command, "git log")==0){
			if(!node){
				printf("���� git init�� ���ּ���.\n");
			}else{
				log(node);
			}
		}else{
			printf("���� ��ɾ��̰ų� ��ɾ �߸��Ǿ����ϴ�.\n");
		}
	}
	return 0;
}
STACK* init(){
	STACK *node = (STACK*)malloc(sizeof(STACK));
	node->next = NULL;
	return node;
}
void commit(STACK* node, char * data){
	STACK *temp = (STACK*)malloc(sizeof(STACK));
	temp -> next = NULL;
	strcpy( temp -> buf, data);
	strcpy( temp -> id, genCommitId());
	while(node -> next){
		node=node -> next;
	}
	node -> next = temp;
}
void log(STACK* node){
	node=node -> next;
	while(node){
		printf("%s : %s\n", node -> id, node -> buf);
		node=node -> next;
	}
}
char* genCommitId(){
	char str[] = "0123456789abcdef";
	char *returnId = (char*)malloc(7);
	for(int i=0;i<7;i++){
		returnId[i] = str[rand()%(sizeof(str)-1)];
	}
	return returnId;
}
