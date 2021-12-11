#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<direct.h> //현재 파일 경로 
#include<dirent.h>// 폴더 관리 
#include<time.h> // 시간 -srand를 위한 
#include<windows.h> //글자 색 변경 

char command[100], fpTemp[100];
FILE *fp_config, *fp_branch;

typedef struct User{
	char username[100];
	// 포인터 배열로 브랜치 struct 참조  
	struct Branch* arr[4]; // branch 0, 1, 2, 3  
	int nowBranch;
	int n; //전체 branch의 수 
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
void initBranch(STACK* node, char * data); 
void checkout(char * data); 

//functions
char* genCommitId();
void loadConfig();
void saveConfig();
void loadBranch(int n);
void saveBranch();
void setcolor(int color);

//user
User* nowuser = (User*)malloc(sizeof(User));

//Branch * master;
int main(){
	
	STACK * node = NULL;
	// 초기화 
	nowuser->nowBranch = 0;
	nowuser->n = 0;
	
	mkdir(".jjap-git");// .jjap-git폴더 생성 
	fp_config = fopen("./.jjap-git/config.ini", "r");
	fgets(fpTemp, sizeof(fpTemp), fp_config);// 처음 실행인지 확인 
	if(strcmp(fpTemp, "#INIT\n")==0){// 마지막에 무조건 \n이 포함됨  ㅇㅋ접수 
		setcolor(5);
		loadConfig();
		node = nowuser->arr[0]->next;
		setcolor(7);
	}

	char data[100];
	
	//system
	printf("짭 git에 오신 것을 환영합니다\ngit 명령어를 입력해주세요. exit를 입력해 프로그램 종료\n");
	
	//현재 파일 경로 
	char curDir[1000];
  	_getcwd(curDir,1000);
	
	while(strcmp(command, "exit")!=0){
		setcolor(6);
		printf("%s", curDir);
		setcolor(3);
		if(nowuser->n>0){
			printf(" (%s)\n",nowuser->arr[nowuser->nowBranch]->name);
		}
		else{
			printf("\n");
		}
		setcolor(7);
		printf("$ ");
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
				STACK * branchNode = (STACK*)malloc(sizeof(STACK));
				gets(data); 
				initBranch(branchNode, data); 
			}
		}
		else if(strcmp(command, "git checkout")==0){
			if(!node){
				printf("먼저 git init을 해주세요.\n");
			}
			else{
				printf("이동할 브랜치의 이름을 입력해주세요\n");
				gets(data);
				int before = nowuser->nowBranch;
				checkout(data);
				if(nowuser->nowBranch!=before){
					node = nowuser->arr[nowuser->nowBranch]->next;
					printf("branch %s로 변경되었습니다.\n",data);
				}
			}
		}
		//예외 처리 추가 
		else if(strcmp(command, "exit")==0){}
		else{
			printf("없는 명령어이거나 명령어가 잘못되었습니다.\n");
		}
	}
	setcolor(5);
	saveConfig();
	saveBranch();
	setcolor(7);
	printf("bye");
	return 0;
}
void init(){
	STACK *node = (STACK*)malloc(sizeof(STACK));
	node->next = NULL;
	//branch master 생성
	nowuser->nowBranch = 0;
	nowuser->n = 0; //전체 branch의 수
	initBranch(node, "master");
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
	while(node){
		setcolor(6);
		printf("commit %s\n", node->id);
		setcolor(7);
		printf("Author: %s <%s@gmail.com>\n", nowuser->username, nowuser->username);
//		printf("Date: 12-07\n");
		printf("\n	  %s\n\n", node->buf);
		node=node -> next;
	}
}

char* genCommitId(){
	char str[] = "0123456789abcdef";
	srand(time(NULL)); //시드값을 time 으로 변경(완전히 랜덤) 
	char *returnId = (char*)malloc(sizeof(char)*7);
	for(int i=0;i<7;i++){

		returnId[i] = str[rand()%(sizeof(str)-1)];
	}
	return returnId;
}

void loadConfig(){
	printf("jjap-git 설정파일을 불러오는 중...\n");
	int n;
	while(!feof(fp_config)){
		fgets(fpTemp, sizeof(fpTemp), fp_config);
		if(strcmp(fpTemp, "#USERNAME\n")==0){
			break;
		}
	}
	fgets(nowuser->username, sizeof(nowuser->username), fp_config);
	nowuser->username[strlen(nowuser->username)-1]='\0';// fgets에서 나오는 \n제거
	printf("username: %s\n", nowuser->username);
	while(!feof(fp_config)){
		fgets(fpTemp, sizeof(fpTemp), fp_config);
		if(strcmp(fpTemp, "#BRANCH_COUNT\n")==0){
			break;
		}
	}
	fscanf(fp_config, "%d\n", &n);
	printf("branch: %d\n", n);
	fclose(fp_config);
	printf("완료!\n");
	loadBranch(n);
}

void saveConfig(){
	printf("jjap-git 설정파일을 저장 중...\n");
	fp_config = fopen("./.jjap-git/config.ini", "w");
	fprintf(fp_config,"#INIT\n");// 최초실행 확인저장
	fprintf(fp_config,"#USERNAME\n%s\n", nowuser->username);// 유저 이름 저장
	fprintf(fp_config,"#BRANCH_COUNT\n%d\n", nowuser->n);// 브랜치 갯수 저장
	fclose(fp_config);
	printf("완료!\n");
}

void loadBranch(int n){
	printf("브랜치를 불러오는 중...\n");
	STACK *node;
	fp_branch = fopen("./.jjap-git/branch.ini", "r");
	char dataTemp[100], idTemp[100];
	for(int i=0;i<n;i++){
		while(!feof(fp_branch)){
			fgets(fpTemp, sizeof(fpTemp), fp_branch);
			if(strcmp(fpTemp, "#BRANCH_START\n")==0){
				break;
			}
		}
		fgets(fpTemp, sizeof(fpTemp), fp_branch);// 브랜치 이름 불러오기
		fpTemp[strlen(fpTemp)-1]='\0';// fgets에서 나오는 \n제거
		STACK * branchNode = (STACK*)malloc(sizeof(STACK));
		branchNode->next = NULL;
		initBranch(branchNode, fpTemp);// 브랜치 생성 
		node = nowuser->arr[i]->next;
		while(!feof(fp_branch)){// 커밋내용 불러오기 
			fgets(fpTemp, sizeof(fpTemp), fp_branch);
			if(strcmp(fpTemp, "#BRANCH_END\n")==0){// 브랜치 설정이 끝나면 
				break;
			}else{// 브랜치 설정이 아직 남아있다면 
				fseek(fp_branch, (strlen(fpTemp)+1)*-1, SEEK_CUR);// 파일 포인터를 원래대로 되돌림 
			}
			fgets(idTemp, sizeof(idTemp), fp_branch);
			idTemp[strlen(idTemp)-1]='\0';// fgets에서 나오는 \n제거
			fgets(dataTemp, sizeof(dataTemp), fp_branch);
			dataTemp[strlen(dataTemp)-1]='\0';// fgets에서 나오는 \n제거
			printf("id: %s\n", idTemp);
			printf("data: %s\n", dataTemp);
			commit(node, dataTemp, idTemp);// 커밋불러오기 
		}
	}
	fclose(fp_branch);
	printf("완료!\n");
}

void saveBranch(){
	printf("브랜치 저장 중...\n");
	fp_branch = fopen("./.jjap-git/branch.ini", "w");
	STACK * node;
	for(int i=0;i<nowuser->n;i++){// 브랜치 갯수만큼 저장 
		nowuser->nowBranch=i;
		node = nowuser->arr[nowuser->nowBranch]->next;
		node = node -> next;
		fprintf(fp_branch, "#BRANCH_START\n");// 브랜치 설정 시작 표시 
		fprintf(fp_branch, "%s\n", nowuser->arr[nowuser->nowBranch]->name);// 브랜치 이름 저장 
		while(node){
			fprintf(fp_branch,"%s\n%s\n", node -> id, node -> buf);// 커밋내용 파일에 저장 
			node=node -> next;
		}
		fprintf(fp_branch, "#BRANCH_END\n");// 브랜치 설정 끝 표시 
	}
	fclose(fp_branch);
	printf("완료!\n");
}

void initBranch(STACK* node, char * data){ //node는 새로운 브랜치의 첫번째 노드
	node->next=NULL;
	if(nowuser->n>4){
		printf("branch의 최대는 4개입니다");
	}
	else{
//		branch newbranch 생성
		Branch *newbranch = (Branch*)malloc(sizeof(Branch));
		strcpy(newbranch->name , data);
		newbranch->next = node;
		nowuser->arr[nowuser->n] = newbranch;
		(nowuser->n)++;
		printf("branch %s 생성\n",data);
	} 
}

void checkout(char * data) {
	int total = nowuser->n;
	for(int i=0; i<total; i++){
		if(strcmp(nowuser->arr[i]->name, data)==0){
			nowuser->nowBranch=i;
			return;
		}
	}
	printf("해당 branch명이 없습니다.\n");
}

void setcolor(int color){
	SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), color);
}
