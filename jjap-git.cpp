#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<direct.h> //���� ���� ��� 
#include<dirent.h>// ���� ���� 
#include<time.h> // �ð� -srand�� ���� 
#include<windows.h> //���� �� ���� 

char command[100], fpTemp[100];
FILE *fp_config, *fp_branch;

typedef struct User{
	char username[100];
	// ������ �迭�� �귣ġ struct ����  
	struct Branch* arr[4]; // branch 0, 1, 2, 3  
	int nowBranch;
	int n; //��ü branch�� �� 
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
	
	// �ʱ�ȭ
	STACK * node = NULL;
	nowuser->nowBranch = 0;
	nowuser->n = 0;
	
	mkdir(".jjap-git");// .jjap-git���� ���� 
	fp_config = fopen("./.jjap-git/config.ini", "r");
	fgets(fpTemp, sizeof(fpTemp), fp_config);// ó�� �������� Ȯ�� 
	if(strcmp(fpTemp, "#INIT\n")==0){// �������� ������ \n�� ���Ե�  �������� 
		setcolor(5);
		loadConfig();
		node = nowuser->arr[0]->next;
		setcolor(7);
	}

	char data[100];
	
	//system
	printf("¬ git�� ���� ���� ȯ���մϴ�\ngit ��ɾ �Է����ּ���. exit�� �Է��� ���α׷� ����\n");
	
	//���� ���� ��� 
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
			printf("git id �Ǵ� �г����� �Է����ּ���: ");
			gets(data);
			strcpy(nowuser->username, data);
			printf("���� ������ %s�Դϴ�.\n", nowuser->username);		 
		}
		else if(strcmp(command, "git commit -m")==0){
			if(!node){
				printf("���� git init�� ���ּ���.\n");
			}else if(!files->next){
				printf("���� git add�� ���ּ���.\n");
			}else{
				printf("Ŀ�� �޽����� �Է����ּ���\n");
				gets(data); 
				commit(node, data , nowDate(), genCommitId());
			}
		}
		else if(strcmp(command, "git add")==0){
			if(!node){
				printf("���� git init�� ���ּ���.\n");
			}else{
				printf("add�� ���ϰ�θ� �Է����ּ���\n");
				gets(data);
				add(data);
			}
		}
		else if(strcmp(command, "git log")==0){
			if(!node){
				printf("���� git init�� ���ּ���.\n");
			}else{
				log(node);
			}
		}
		else if(strcmp(command, "git log -find")==0){
			if(!node){
				printf("���� git init�� ���ּ���.\n");
			}else{
				printf("ã�� Ŀ�Գ����� �Է����ּ���\n");
				gets(data); 
				logFind(node, data);
			}
		}
		else if(strcmp(command, "git branch")==0){
			if(!node){
				printf("���� git init�� ���ּ���.\n");
			}else{
				printf("���ο� �귣ġ�� �̸��� �Է����ּ���\n");
				STACK * branchNode = (STACK*)malloc(sizeof(STACK));
				gets(data); 
				initBranch(branchNode, data); 
			}
		}
		else if(strcmp(command, "git checkout")==0){
			if(!node){
				printf("���� git init�� ���ּ���.\n");
			}
			else{
				printf("�̵��� �귣ġ�� �̸��� �Է����ּ���\n");
				gets(data);
				int before = nowuser->nowBranch;
				checkout(data);
				if(nowuser->nowBranch!=before){
					node = nowuser->arr[nowuser->nowBranch]->next;
					printf("branch %s�� ����Ǿ����ϴ�.\n",data);
				}
			}
		}
		else if(strcmp(command, "now")==0){
			printf("%s\n", nowDate());
		}
		//���� ó�� �߰� 
		else if(strcmp(command, "exit")==0){}
		else{
			printf("���� ��ɾ��̰ų� ��ɾ �߸��Ǿ����ϴ�.\n");
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
	//branch master ����  
	nowuser->nowBranch = 0;
	nowuser->n = 0; //��ü branch�� ��
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
		printf("ã�� Ŀ�Գ����� �����ϴ�.\n");
	}
}

char* genCommitId(){
	int i;
	char str[] = "0123456789abcdef";
	srand(time(NULL)); //�õ尪�� time ���� ����(������ ����) 
//	char *returnId = (char*)malloc(sizeof(char)*7);
	for(i=0;i<7;i++){
		returnId[i] = str[rand()%(sizeof(str)-1)];
	}
	returnId[i]='\0'; 
	return returnId;
}

void loadConfig(){
	printf("jjap-git ���������� �ҷ����� ��...\n");
	int n;
	while(!feof(fp_config)){
		fgets(fpTemp, sizeof(fpTemp), fp_config);
		if(strcmp(fpTemp, "#USERNAME\n")==0){
			break;
		}
	}
	fgets(nowuser->username, sizeof(nowuser->username), fp_config);
	nowuser->username[strlen(nowuser->username)-1]='\0';// fgets���� ������ \n����
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
	printf("�Ϸ�!\n");
	loadBranch(n);
}

void saveConfig(){
	printf("jjap-git ���������� ���� ��...\n");
	fp_config = fopen("./.jjap-git/config.ini", "w");
	fprintf(fp_config,"#INIT\n");// ���ʽ��� Ȯ������
	fprintf(fp_config,"#USERNAME\n%s\n", nowuser->username);// ���� �̸� ����
	fprintf(fp_config,"#BRANCH_COUNT\n%d\n", nowuser->n);// �귣ġ ���� ����
	fclose(fp_config);
	printf("�Ϸ�!\n");
}

void loadBranch(int n){
	// ���� �ϴ� ��  
	printf("�귣ġ�� �ҷ����� ��...\n");
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
		fgets(fpTemp, sizeof(fpTemp), fp_branch);// �귣ġ �̸� �ҷ�����
		fpTemp[strlen(fpTemp)-1]='\0';// fgets���� ������ \n����
		STACK * branchNode = (STACK*)malloc(sizeof(STACK));
		branchNode->next = NULL;
		initBranch(branchNode, fpTemp);// �귣ġ ���� 
		node = nowuser->arr[i]->next;
		while(!feof(fp_branch)){// Ŀ�Գ��� �ҷ�����
			fgets(fpTemp, sizeof(fpTemp), fp_branch);
			if(strcmp(fpTemp, "#BRANCH_END\n")==0){// �귣ġ ������ ������ 
				break;
			}else{// �귣ġ ������ ���� �����ִٸ� 
				fseek(fp_branch, (strlen(fpTemp)+1)*-1, SEEK_CUR);// ���� �����͸� ������� �ǵ��� 
			}
			fgets(idTemp, sizeof(idTemp), fp_branch);
			fgets(dateTemp, sizeof(dateTemp), fp_branch);
			fgets(dataTemp, sizeof(dataTemp), fp_branch);
			
			idTemp[strlen(idTemp)-1]='\0';// fgets���� ������ \n����  
			dateTemp[strlen(dateTemp)-1]='\0';// fgets���� ������ \n����
			dataTemp[strlen(dataTemp)-1]='\0';// fgets���� ������ \n����
			
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
			while(!feof(fp_branch)){// ���ϸ�� �ҷ����� 
				fgets(fpTemp, sizeof(fpTemp), fp_branch);
				if(strcmp(fpTemp, "#FILES_END\n")==0){// ���ϸ���� ������ 
					break;
				}else{// ���ϸ�� ������ ���� �����ִٸ� 
					fseek(fp_branch, (strlen(fpTemp)+1)*-1, SEEK_CUR);// ���� �����͸� ������� �ǵ��� 
				}
				fgets(fileTemp, sizeof(fileTemp), fp_branch);
				
				fileTemp[strlen(fileTemp)-1]='\0';// fgets���� ������ \n����  
				
				printf("file: %s\n", fileTemp);
				add(fileTemp);
			}
			commit(node, dataTemp, dateTemp, idTemp);// Ŀ�Ժҷ����� 
		}
	}
	fclose(fp_branch);
	printf("�Ϸ�!\n");
}

void saveBranch(){
	printf("�귣ġ ���� ��...\n");
	fp_branch = fopen("./.jjap-git/branch.ini", "w");
	STACK * node;
	FILES * files;
	for(int i=0;i<nowuser->n;i++){// �귣ġ ������ŭ ���� 
		nowuser->nowBranch=i;
		node = nowuser->arr[nowuser->nowBranch]->next;
		node = node -> next;
		fprintf(fp_branch, "#BRANCH_START\n");// �귣ġ ���� ���� ǥ�� 
		fprintf(fp_branch, "%s\n", nowuser->arr[nowuser->nowBranch]->name);// �귣ġ �̸� ���� 
		while(node){
			fprintf(fp_branch,"%s\n%s\n%s\n", node -> id, node->date ,node -> buf);// Ŀ�Գ��� ���Ͽ� ���� 
			files = node -> files;
			fprintf(fp_branch, "#FILES_START\n");// ���� ��� ���� ǥ�� 
			while(files){
				fprintf(fp_branch,"%s\n", files->dir);// file��� ���Ͽ� ���� 
				files=files->next;
			}
			fprintf(fp_branch, "#FILES_END\n");// ���� ��� �� ǥ�� 
			node=node->next;
		}
		fprintf(fp_branch, "#BRANCH_END\n");// �귣ġ ���� �� ǥ�� 
	}
	fclose(fp_branch);
	printf("�Ϸ�!\n");
}

void initBranch(STACK* node, char * data){ //node�� ���ο� �귣ġ�� ù��° ���
	node->next=NULL;
	if(nowuser->n>4){
		printf("branch�� �ִ�� 4���Դϴ�");
	}
	else{
//		branch newbranch ����
		Branch *newbranch = (Branch*)malloc(sizeof(Branch));
		strcpy(newbranch->name , data);
		newbranch->next = node;
		nowuser->arr[nowuser->n] = newbranch;
		(nowuser->n)++;
		printf("branch %s ����\n",data);
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
	printf("�ش� branch���� �����ϴ�.\n");
}

void setcolor(int color){
	SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), color);
}

char* nowDate(){
	time_t timer;
	timer = time(NULL); // 1970�� 1�� 1�� 0�� 0�� 0�ʺ��� �����Ͽ� ��������� ��
    t = localtime(&timer); // �������� ���� ����ü�� �ֱ�
    returnDate[0] = '\0';
    char text[20]={};  //int ���� ��� ���� �迭 
	// ���� push  
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
	// x�� push 
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
	// ���ڿ��� ��ȯ  
	sprintf(text, "%d ", t->tm_mday);   
	// ���� �� push
	pushString(text, returnDate);
	
	sprintf(text, "%d:%d:%d %d", t->tm_hour, t->tm_min, t->tm_sec, t->tm_year+1900);
	// �ð�:��:�� �⵵ push  
	pushString(text, returnDate);
	
	//�ѱ� ǥ�ؽ� push  
	pushString(" +0900\0", returnDate);
	
    return returnDate;
}

void pushString(char* a, char *arr){ //���ʿ� ���� ���ڿ� push���ִ� �Լ�  
	int i, j;
//	printf("%d\n", strlen(arr)+strlen(a));
	for(i=strlen(arr), j=0; i<strlen(arr)+strlen(a); i++, j++){
		arr[i] = a[j];	
	}
}
