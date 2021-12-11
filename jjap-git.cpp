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
	// �ʱ�ȭ 
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
			}else{
				printf("Ŀ�� �޽����� �Է����ּ���\n");
				gets(data); 
				commit(node, data, genCommitId());
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
	//branch master ����
	nowuser->nowBranch = 0;
	nowuser->n = 0; //��ü branch�� ��
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
	srand(time(NULL)); //�õ尪�� time ���� ����(������ ����) 
	char *returnId = (char*)malloc(sizeof(char)*7);
	for(int i=0;i<7;i++){

		returnId[i] = str[rand()%(sizeof(str)-1)];
	}
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
	printf("�귣ġ�� �ҷ����� ��...\n");
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
			idTemp[strlen(idTemp)-1]='\0';// fgets���� ������ \n����
			fgets(dataTemp, sizeof(dataTemp), fp_branch);
			dataTemp[strlen(dataTemp)-1]='\0';// fgets���� ������ \n����
			printf("id: %s\n", idTemp);
			printf("data: %s\n", dataTemp);
			commit(node, dataTemp, idTemp);// Ŀ�Ժҷ����� 
		}
	}
	fclose(fp_branch);
	printf("�Ϸ�!\n");
}

void saveBranch(){
	printf("�귣ġ ���� ��...\n");
	fp_branch = fopen("./.jjap-git/branch.ini", "w");
	STACK * node;
	for(int i=0;i<nowuser->n;i++){// �귣ġ ������ŭ ���� 
		nowuser->nowBranch=i;
		node = nowuser->arr[nowuser->nowBranch]->next;
		node = node -> next;
		fprintf(fp_branch, "#BRANCH_START\n");// �귣ġ ���� ���� ǥ�� 
		fprintf(fp_branch, "%s\n", nowuser->arr[nowuser->nowBranch]->name);// �귣ġ �̸� ���� 
		while(node){
			fprintf(fp_branch,"%s\n%s\n", node -> id, node -> buf);// Ŀ�Գ��� ���Ͽ� ���� 
			node=node -> next;
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
