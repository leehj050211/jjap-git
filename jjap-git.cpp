#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<dirent.h>// ���� ���� 
char command[100], fpTemp[100];
FILE *fp_config, *fp_branch;

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
	
	mkdir(".jjap-git");// .jjap-git���� ���� 
	fp_config = fopen("./.jjap-git/config.txt", "r");
	fgets(fpTemp, sizeof(fpTemp), fp_config);// ó�� �������� Ȯ�� 
	if(strcmp(fpTemp, "1\n")==0){// �������� ������ \n�� ���Ե� 
		loadConfig();
		node = nowuser->arr[0]->next;
		loadBranch(node);
	}

	char data[100];
	
	//system
	printf("¬ git�� ���� ���� ȯ���մϴ�\ngit ��ɾ �Է����ּ���. exit�� �Է��� ���α׷� ����\n");
	while(strcmp(command, "exit")!=0){
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
				STACK * branchNode = NULL;
				gets(data); 
//				initBranch(branchNode, data); 
			}
		}
		else{
			printf("���� ��ɾ��̰ų� ��ɾ �߸��Ǿ����ϴ�.\n");
		}
	}
	saveConfig();
	saveBranch(node);
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
	fprintf(fp_config,"1\n%s\n", nowuser->username);// ���ʽ��� Ȯ�ΰ� ���� �̸� �������Ͽ� ���� 
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
		fprintf(fp_branch,"%s\n%s\n", node -> id, node -> buf);// Ŀ�Գ��� ���Ͽ� ���� 
		node=node -> next;
	}
	fclose(fp_branch);
}
