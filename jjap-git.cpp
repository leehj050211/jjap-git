#include <stdio.h>
#include <string.h>
char command[100];
int main(){
	printf("¬ git�� ���� ���� ȯ���մϴ�\n�ؿ��� git ��ɾ �Է����ּ���. exit�� �Է��� ���α׷� ����\n");
	while(strcmp(command, "exit")!=0){
		scanf("%s\n", command);
	}	
	return 0;
}
