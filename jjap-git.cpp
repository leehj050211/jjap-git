#include<stdio.h>
#include<string.h>
#include<stdlib.h>
char command[100];

typedef struct User{
	char username[100];
	// ������ �迭�� �귣ġ struct ����  
	struct Branch* arr[3]; // branch 0, 1, 2, 3  
	int nowBranch;
}User;

//branch
typedef struct Branch{
	char name[100];
	struct STACK * next;
}Branch;

//stack
typedef struct STACK{
	char buf[100];
	char id[7];
	STACK * next;
}STACK;


//git functions
void init();
void commit(STACK* node, char * data);
void log(STACK* node);
//STACK* initBranch(STACK* node, char * data); 

//functions
char* genCommitId();


//user
User* nowuser = (User*)malloc(sizeof(User));
//Branch * master;

int main(){
	
	STACK * node = NULL;
	
	char data[100];
	
	//system
	printf("¬ git�� ���� ���� ȯ���մϴ�\ngit ��ɾ �Է����ּ���. exit�� �Է��� ���α׷� ����\n");
	while(strcmp(command, "exit")!=0){
		gets(command);
		if(strcmp(command, "git init")==0){
			init();
			node = nowuser->arr[0]->next;
			if(nowuser->username){
				printf("git id �Ǵ� �г����� �Է����ּ���: ");
				gets(data);
				strcpy(nowuser->username, data);
			}
			printf("���� ������ %s�Դϴ�.\n", nowuser->username); 
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
		}
		else if(strcmp(command, "git branch")==0){
			if(!node){
				printf("���� git init�� ���ּ���.\n");
			}else{
				printf("���ο� �귣ġ�� �̸��� �Է����ּ���\n");
				STACK * branchNode = NULL;
				gets(data); 
//				initBranch(branchNode, data); 
			}
		}
		else{
			printf("���� ��ɾ��̰ų� ��ɾ �߸��Ǿ����ϴ�.\n");
		}
	}
	return 0;
}
void init(){
	STACK *node = (STACK*)malloc(sizeof(STACK));
	node->next = NULL;
	//branch master ����
	Branch *master = (Branch*)malloc(sizeof(Branch));
	strcpy(master->name , "master");
	master->next =node;
	nowuser->arr[0] = master;
	nowuser->nowBranch = 0; 
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
//	printf("%s\n", nowuser->username);
	while(node){
		printf("commit %s\n", node -> id);
		printf("Author: %s <%s@gmail.com>\n", nowuser->username, nowuser->username);
//		printf("Date: 12-07\n");
		printf("\n	  %s\n\n", node->buf);
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
