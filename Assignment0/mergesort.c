#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "sorter.h"

void merge(struct MetaData* movies, struct MetaData* left, int l, struct MetaData* right, int r, int index,int regNum, int decNum) 
{  	
	int ldex = 0 , rdex = 0, mdex =0;
	
	if (regNum == 1) {
		char* ref1;
		char* ref2;
		long iVal = strtol(left[ldex].data[index],&ref1, 10);
		long jVal = strtol(right[rdex].data[index], &ref2, 10);
		  //printf("left: %ld\r\n",iVal);
	      //printf("right:%ld\r\n",jVal);
	while ((ldex < l) && (rdex < r)) {
	  iVal = strtol(left[ldex].data[index],&ref1, 10);
	  jVal = strtol(right[rdex].data[index], &ref2, 10);
	 
		if(iVal < jVal) { 
			movies[mdex++] = left[ldex++];
		}
		else {
			movies[mdex++] = right[rdex++];
		}
	}
	
		
		while(ldex < l ) {
		movies[mdex++] = left[ldex++];
		}
		
		while(rdex < r) { 
		movies[mdex++] = right[rdex++];
		}
		
	} else if (decNum == 1) {
		while (ldex < l && rdex< r) {
		  double iVal = atof(left[ldex].data[index]);
		 double jVal = atof(right[rdex].data[index]);
		if(iVal < jVal) { 
			movies[mdex++] = left[ldex++];
		}
		else {
			movies[mdex++] = right[rdex++];
		}
		}

		while(ldex < l) {
		movies[mdex++] = left[ldex++];
		}
		while(rdex < r) {
		movies[mdex++] = right[rdex++];
		}
	
	} else {
		while((ldex<l) && (rdex< r)) {
		if((strcasecmp(left[ldex].data[index], right[rdex].data[index])) < 0) { 
			movies[mdex++] = left[ldex++];
		}
		else {
			movies[mdex++] = right[rdex++];
		}
		}
			while(ldex < l) {
			movies[mdex++] = left[ldex++];
			}
			while(rdex < r) {
			movies[mdex++] = right[rdex++];
			}
			
	
	}
}

// Recursive function to sort an array of integers. 
extern void mergeSort(struct MetaData* movies, int rows, int cols, int index,int regNum, int decNum)
{
	int i = 0;
	int mid = 0;

	if(rows < 2) return; // base condition. If the array has less than two element, do nothing. 

	mid = rows/2;  // find the mid index. 

	// create left and right subarrays
	// mid elements (from index 0 till mid-1) should be part of left sub-array 
	// and (n-mid) elements (from mid to n-1) will be part of right sub-array
	struct MetaData* left =  malloc(mid* sizeof(struct MetaData) + sizeof(char*)*cols);
	struct MetaData* right = malloc((rows-mid)*sizeof(struct MetaData) + sizeof(char*)*cols); 
	for(i = 0;i<mid;i++) {
		left[i].data = malloc(sizeof(char*) * cols);
	}
	for(i = 0; i<rows - mid; i++) {
		right[i].data = malloc(sizeof(char*) * cols);
	}
	
	for(i = 0; i<mid; i++){
		int k = 0;
		for(k = 0; k < cols; k++) {
			left[i].data[k] = movies[i].data[k];
		}
	}
	
	
	for(i = mid ; i<rows; i++){   
		int k = 0;
		for (k = 0; k < cols; k++) {
			right[i-mid].data[k] = movies[i].data[k];  
		}
	}	
	
	mergeSort(left,mid, cols, index, regNum, decNum);  // sorting the left subarray
	mergeSort(right, rows-mid, cols, index, regNum, decNum);  // sorting the right subarray
	merge(movies,left,mid,right,rows-mid, index, regNum,  decNum);  // Merging L and R into A as sorted list.
        free(left);
        free(right);
}