#include <stdio.h>
#include <stdlib.h>

int main(void){
	char** pp_data = NULL;
	int n;
	int length;
	int i;

	printf("Input case: ");
	scanf("%d", &n);


	//1. define & dynamic allocation
	pp_data = (char**)malloc(sizeof(char*) * n);
	
	for(i = 0; i < n; i++) {
		printf("%d's length: ", i + 1);
		scanf("%d", &length);
		pp_data[i] = (char*)malloc(sizeof(char) * (length + 1));
		printf("Input data : ");
		scanf("%s", pp_data[i]);
	}
	
	//2. print string
	for(i = 0; i < n; i++) {
		printf("%s\n", pp_data[i]);
	}

	//3. free memory
	for(i = 0; i < n; i++) {
		free(pp_data[i]);
	}
	free(pp_data);

	return 0;
}
