#include<stdio.h>
#include<string.h>
#include<stdlib.h>
char command[100];

typedef struct User{
	char username[100];
	// 포인터 배열로 브랜치 struct 참조  
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
	printf("짭 git에 오신 것을 환영합니다\ngit 명령어를 입력해주세요. exit를 입력해 프로그램 종료\n");
	while(strcmp(command, "exit")!=0){
		gets(command);
		if(strcmp(command, "git init")==0){
			init();
			node = nowuser->arr[0]->next;
			if(nowuser->username){
				printf("git id 또는 닉네임을 입력해주세요: ");
				gets(data);
				strcpy(nowuser->username, data);
			}
			printf("현재 유저는 %s입니다.\n", nowuser->username); 
		}
		else if(strcmp(command, "git commit -m")==0){
			if(!node){
				printf("먼저 git init을 해주세요.\n");
			}else{
				printf("커밋 메시지를 입력해주세요\n");
				gets(data); 
				commit(node, data);
			}
		}
		else if(strcmp(command, "git log")==0){
			if(!node){
				printf("먼저 git init을 해주세요.\n");
			}else{
				log(node);
			}
		}
		else if(strcmp(command, "git branch")==0){
			if(!node){
				printf("먼저 git init을 해주세요.\n");
			}else{
				printf("새로운 브랜치의 이름을 입력해주세요\n");
				STACK * branchNode = NULL;
				gets(data); 
//				initBranch(branchNode, data); 
			}
		}
		else{
			printf("없는 명령어이거나 명령어가 잘못되었습니다.\n");
		}
	}
	return 0;
}
void init(){
	STACK *node = (STACK*)malloc(sizeof(STACK));
	node->next = NULL;
	//branch master 생성
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
