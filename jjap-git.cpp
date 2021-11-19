#include <stdio.h>
#include <string.h>
char command[100];
int main(){
	printf("짭 git에 오신 것을 환영합니다\n밑에서 git 명령어를 입력해주세요. exit를 입력해 프로그램 종료\n");
	while(strcmp(command, "exit")!=0){
		scanf("%s\n", command);
	}	
	return 0;
}
