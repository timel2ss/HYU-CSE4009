#include <stdio.h>
#include <stdlib.h>
#define SIZE 10

int compare(int a, int b){
    if(a > b)  
    	return 1;
    else 
       	return -1;
}
    

/***************************************************************
define bubble sort that uses pointer to 'compare' function above
***************************************************************/

void bubbleSort(int* n_array, int (*compareFunc)(int, int)) {
	int i, j;
	for(i = 0; i < SIZE - 1; i++) {
		for(j = 1; j <= SIZE - 1 - i; j++) {
			if(compareFunc(n_array[j-1], n_array[j]) == 1) {
				int temp = n_array[j-1];
				n_array[j-1] = n_array[j];
				n_array[j] = temp;
			}
		}
	}
}

int main(){
    int n_array[SIZE] = {2,-9, 10, 15, 1, 3, -12, 5, 4, 1};
    // expect result => -12 -9 1 1 2 3 4 5 10 15

    // define a function pointer
    bubbleSort(n_array, compare);
    
    
	int i;
	for(i = 0; i < SIZE; i++) {
		printf("%d ", n_array[i]);
	}
	// -12 -9 1 1 2 3 4 5 10 15
	
    return 0;
}
