#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<dirent.h>// 폴더 관리 
char command[100], fpTemp[100];
FILE *fp_config, *fp_branch;

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
void commit(STACK* node, char* data, char* commitId);
void log(STACK* node);
//STACK* initBranch(STACK* node, char * data); 

//functions
char* genCommitId();
void loadConfig();
void saveConfig();
void loadBranch(STACK* node);
void saveBranch(STACK* node);

//user
User* nowuser = (User*)malloc(sizeof(User));

//Branch * master;

int main(){
	
	STACK * node = NULL;
	
	mkdir(".jjap-git");// .jjap-git폴더 생성 
	fp_config = fopen("./.jjap-git/config.txt", "r");
	fgets(fpTemp, sizeof(fpTemp), fp_config);// 처음 실행인지 확인 
	if(strcmp(fpTemp, "1\n")==0){// 마지막에 무조건 \n이 포함됨 
		loadConfig();
		node = nowuser->arr[0]->next;
		loadBranch(node);
	}

	char data[100];
	
	//system
	printf("짭 git에 오신 것을 환영합니다\ngit 명령어를 입력해주세요. exit를 입력해 프로그램 종료\n");
	while(strcmp(command, "exit")!=0){
		gets(command);
		if(strcmp(command, "git init")==0){
			init();
			node = nowuser->arr[0]->next;
			printf("git id 또는 닉네임을 입력해주세요: ");
			gets(data);
			strcpy(nowuser->username, data);
			printf("현재 유저는 %s입니다.\n", nowuser->username);
			 
		}
		else if(strcmp(command, "git commit -m")==0){
			if(!node){
				printf("먼저 git init을 해주세요.\n");
			}else{
				printf("커밋 메시지를 입력해주세요\n");
				gets(data); 
				commit(node, data, genCommitId());
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
	saveConfig();
	saveBranch(node);
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
void commit(STACK* node, char* data, char* commitId){
	STACK *temp = (STACK*)malloc(sizeof(STACK));
	temp -> next = NULL;
	strcpy( temp -> buf, data);
	strcpy( temp -> id, commitId);
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
	char *returnId = (char*)malloc(sizeof(char)*7);
	for(int i=0;i<7;i++){
		returnId[i] = str[rand()%(sizeof(str)-1)];
	}
	return returnId;
}
void loadConfig(){
	init();
	fscanf(fp_config, "%s", nowuser->username);
	printf("\nusername: %s\n", nowuser->username);
	fclose(fp_config);
}
void saveConfig(){
	fp_config = fopen("./.jjap-git/config.txt", "w");
	fprintf(fp_config,"1\n%s\n", nowuser->username);// 최초실행 확인과 유저 이름 설정파일에 저장 
	fclose(fp_config);
}
void loadBranch(STACK* node){
	fp_branch = fopen("./.jjap-git/branch.txt", "r");
	char dataTemp[100], idTemp[100];
	while(!feof(fp_branch)){
		fscanf(fp_branch, "%s\n", idTemp);
		fscanf(fp_branch, "%s\n", dataTemp);
		commit(node, dataTemp, idTemp);
	}
	
	fclose(fp_branch);
}
void saveBranch(STACK* node){
	fp_branch = fopen("./.jjap-git/branch.txt", "w");
	node=node -> next;
	while(node){
		fprintf(fp_branch,"%s\n%s\n", node -> id, node -> buf);// 커밋내용 파일에 저장 
		node=node -> next;
	}
	fclose(fp_branch);
}
