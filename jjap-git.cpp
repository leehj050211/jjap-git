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
	char id[8];
	char date[100];
	struct FILES * files;
	STACK * next;
}STACK;

//files
typedef struct FILES{
	char dir[100];
	FILES * next;
}FILES;

struct tm* t;

char* returnDate = (char*)malloc(sizeof(char)*30);
char* returnId = (char*)malloc(sizeof(char)*7);
FILES * files;


//git functions
void init();
void add(char* dir);
void commit(STACK* node, char* data, char* nowDate, char* commitId);
void log(STACK* node);
void logFind(STACK* node, char* data);
void initBranch(STACK* node, char * data); 
void checkout(char * data);

//functions
char* genCommitId();
void loadConfig();
void saveConfig();
void loadBranch(int n);
void saveBranch();
void setcolor(int color);
char* nowDate();
void pushString(char* a, char *arr);

//user
User* nowuser = (User*)malloc(sizeof(User));

//Branch * master;
int main(){
	
	// 초기화
	STACK * node = NULL;
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
			}else if(!files->next){
				printf("먼저 git add를 해주세요.\n");
			}else{
				printf("커밋 메시지를 입력해주세요\n");
				gets(data); 
				commit(node, data , nowDate(), genCommitId());
			}
		}
		else if(strcmp(command, "git add")==0){
			if(!node){
				printf("먼저 git init을 해주세요.\n");
			}else{
				printf("add할 파일경로를 입력해주세요\n");
				gets(data);
				add(data);
			}
		}
		else if(strcmp(command, "git log")==0){
			if(!node){
				printf("먼저 git init을 해주세요.\n");
			}else{
				log(node);
			}
		}
		else if(strcmp(command, "git log -find")==0){
			if(!node){
				printf("먼저 git init을 해주세요.\n");
			}else{
				printf("찾을 커밋내용을 입력해주세요\n");
				gets(data); 
				logFind(node, data);
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
		else if(strcmp(command, "now")==0){
			printf("%s\n", nowDate());
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
	files=(FILES *)malloc(sizeof(FILES));
	files->next=NULL;
	//branch master 생성  
	nowuser->nowBranch = 0;
	nowuser->n = 0; //전체 branch의 수
	initBranch(node, "master" );
}
void commit(STACK* node, char* data, char* nowDate, char* commitId){
	STACK *temp = (STACK*)malloc(sizeof(STACK));
	temp -> next = NULL;
	strcpy(temp -> buf, data);	
	strcpy(temp -> date, nowDate);
	strcpy(temp -> id, commitId);
	temp -> files = files->next;
	
	while(node -> next){
		node = node -> next;
	}
	node -> next = temp;
	
	files = (FILES *)malloc(sizeof(FILES));
	files -> next = NULL;
}

void add(char* dir){
	FILES *temp = files;
	FILES *newFile = (FILES*)malloc(sizeof(FILES));
	newFile -> next = NULL;
	strcpy(newFile -> dir, dir);
	while(temp -> next){
		temp = temp -> next;
	}
	temp -> next = newFile;
}

void log(STACK* node){
	node=node -> next;
	FILES* files;
	while(node){
		setcolor(6);
		printf("commit %s\n", node->id);
		setcolor(7);
		printf("Author: %s <%s@gmail.com>\n", nowuser->username, nowuser->username);
		printf("Date:   %s", node->date);
		printf("\n	  %s\n\n", node->buf);
		files = node -> files;
		printf("git add files\n");
		while(files){
			printf("%s\n", files->dir);
			files=files->next;
		}
		node=node->next;
	}
}

void logFind(STACK* node, char* data){
	short flag=0;
	node=node -> next;
	while(node){
		if(strstr(node->buf, data)){
			flag=1;
			setcolor(6);
			printf("commit %s\n", node->id);
			setcolor(7);
			printf("Author: %s <%s@gmail.com>\n", nowuser->username, nowuser->username);
			printf("Date:   %s", node->date);
			printf("\n	  %s\n\n", node->buf);
		}
		node=node->next;
	}
	if(flag==0){
		printf("찾을 커밋내용이 없습니다.\n");
	}
}

char* genCommitId(){
	int i;
	char str[] = "0123456789abcdef";
	srand(time(NULL)); //시드값을 time 으로 변경(완전히 랜덤) 
//	char *returnId = (char*)malloc(sizeof(char)*7);
	for(i=0;i<7;i++){
		returnId[i] = str[rand()%(sizeof(str)-1)];
	}
	returnId[i]='\0'; 
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
	// 여기 하는 중  
	printf("브랜치를 불러오는 중...\n");
	STACK *node;
	fp_branch = fopen("./.jjap-git/branch.ini", "r");
	char dataTemp[100], idTemp[100], dateTemp[100], fileTemp[100];
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
			fgets(dateTemp, sizeof(dateTemp), fp_branch);
			fgets(dataTemp, sizeof(dataTemp), fp_branch);
			
			idTemp[strlen(idTemp)-1]='\0';// fgets에서 나오는 \n제거  
			dateTemp[strlen(dateTemp)-1]='\0';// fgets에서 나오는 \n제거
			dataTemp[strlen(dataTemp)-1]='\0';// fgets에서 나오는 \n제거
			
			printf("id: %s\n", idTemp);
			printf("date: %s\n",dateTemp); 
			printf("data: %s\n", dataTemp);
			files = (FILES *)malloc(sizeof(FILES));
			files -> next = NULL;
			while(!feof(fp_branch)){
				fgets(fpTemp, sizeof(fpTemp), fp_branch);
				if(strcmp(fpTemp, "#FILES_START\n")==0){
					break;
				}
			}
			while(!feof(fp_branch)){// 파일목록 불러오기 
				fgets(fpTemp, sizeof(fpTemp), fp_branch);
				if(strcmp(fpTemp, "#FILES_END\n")==0){// 정일목록이 끝나면 
					break;
				}else{// 파일목록 설정이 아직 남아있다면 
					fseek(fp_branch, (strlen(fpTemp)+1)*-1, SEEK_CUR);// 파일 포인터를 원래대로 되돌림 
				}
				fgets(fileTemp, sizeof(fileTemp), fp_branch);
				
				fileTemp[strlen(fileTemp)-1]='\0';// fgets에서 나오는 \n제거  
				
				printf("file: %s\n", fileTemp);
				add(fileTemp);
			}
			commit(node, dataTemp, dateTemp, idTemp);// 커밋불러오기 
		}
	}
	fclose(fp_branch);
	printf("완료!\n");
}

void saveBranch(){
	printf("브랜치 저장 중...\n");
	fp_branch = fopen("./.jjap-git/branch.ini", "w");
	STACK * node;
	FILES * files;
	for(int i=0;i<nowuser->n;i++){// 브랜치 갯수만큼 저장 
		nowuser->nowBranch=i;
		node = nowuser->arr[nowuser->nowBranch]->next;
		node = node -> next;
		fprintf(fp_branch, "#BRANCH_START\n");// 브랜치 설정 시작 표시 
		fprintf(fp_branch, "%s\n", nowuser->arr[nowuser->nowBranch]->name);// 브랜치 이름 저장 
		while(node){
			fprintf(fp_branch,"%s\n%s\n%s\n", node -> id, node->date ,node -> buf);// 커밋내용 파일에 저장 
			files = node -> files;
			fprintf(fp_branch, "#FILES_START\n");// 파일 목록 시작 표시 
			while(files){
				fprintf(fp_branch,"%s\n", files->dir);// file목록 파일에 저장 
				files=files->next;
			}
			fprintf(fp_branch, "#FILES_END\n");// 파일 목록 끝 표시 
			node=node->next;
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

char* nowDate(){
	time_t timer;
	timer = time(NULL); // 1970년 1월 1일 0시 0분 0초부터 시작하여 현재까지의 초
    t = localtime(&timer); // 포맷팅을 위해 구조체에 넣기
    returnDate[0] = '\0';
    char text[20]={};  //int 형을 담기 위한 배열 
	// 요일 push  
    switch(t->tm_wday){
    	case(0):
    		pushString("Sun ", returnDate);
			break;
		case(1):
    		pushString("Mon ", returnDate);
			break;
		case(2):
    		pushString("Tue ", returnDate);
			break;
		case(3):
    		pushString("Wed ", returnDate);
			break;
		case(4):
    		pushString("Thu ", returnDate);
			break;
		case(5):
    		pushString("Fri ", returnDate);
			break;
		case(6):
    		pushString("Sat ", returnDate);
			break;
		default:
			break;
		}
	// x월 push 
	switch(t->tm_mon + 1){
    	case(1):
    		pushString("Jan ", returnDate);
			break;
		case(2):
    		pushString("Feb ", returnDate);
			break;
		case(3):
    		pushString("Mar ", returnDate);
			break;
		case(4):
    		pushString("Apr ", returnDate);
			break;
		case(5):
    		pushString("May ", returnDate);
			break;
		case(6):
    		pushString("Jun ", returnDate);
			break;
		case(7):
    		pushString("Jul ", returnDate);
			break;
		case(8):
    		pushString("Aug ", returnDate);
			break;
		case(9):
    		pushString("Sep ", returnDate);
			break;
		case(10):
    		pushString("Oct ", returnDate);
			break;
		case(11):
    		pushString("Nov ", returnDate);
			break;
		case(12):
    		pushString("Dec ", returnDate);
			break;
		default:
			break;
		}
	// 문자열로 변환  
	sprintf(text, "%d ", t->tm_mday);   
	// 현재 일 push
	pushString(text, returnDate);
	
	sprintf(text, "%d:%d:%d %d", t->tm_hour, t->tm_min, t->tm_sec, t->tm_year+1900);
	// 시간:분:초 년도 push  
	pushString(text, returnDate);
	
	//한국 표준시 push  
	pushString(" +0900\0", returnDate);
	
    return returnDate;
}

void pushString(char* a, char *arr){ //왼쪽에 넣을 문자열 push해주는 함수  
	int i, j;
//	printf("%d\n", strlen(arr)+strlen(a));
	for(i=strlen(arr), j=0; i<strlen(arr)+strlen(a); i++, j++){
		arr[i] = a[j];	
	}
}
