#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "mergesort.c"
#include "sorter.h"


	char ** headers;
	int dynaCols = 0;
	int dynaRows = 0;
	int regNum = 0; //set to false
	int decNum = 0; // set to false

void print(struct MetaData* movies, int r, int c){
	int i = 0, k =0;
	for (i = 0; i < r; i++) {
		for (k = 0; k < c ; k++) {
		  //printf("%s",movies[i].data[k]);
		  char* ch =movies[i].data[k];
			if (strchr(ch,',')) {
			  if(k==(c-1)){
				printf("\"%s\"", movies[i].data[k]);
			  }else{
			  printf("\"%s\",", movies[i].data[k]);
			  }
			} else {
			  if(k==(c-1)){
				printf("%s",movies[i].data[k]); 
			  }else{
				printf("%s,",movies[i].data[k]); 
			  }
			}
				
		}
		printf("\n\n");
	}
}


int isNum(char* str) {
	if (str[0] >= '0' && str[0] <= '9') { //only check first character, is that ok?
		return 1;  //the string is a number
	} 
	return 0;
}

int isDouble(char* str) { //call this if isNum returns 1
	int len = strlen(str);
	int i = 0;
	for (i = 0; i < len; i++) {
		if (str[i] == '.') {
			return 1;
		}
	}
	return 0;
}
		
		
void printData(struct MetaData* movies, int r, int c)
{
	
	int i= 0;
	int j = 0;
	for (i = 0; i < r; i++){
		for (j = 0; j < c ; j++){
			printf("[%s] ",movies[i].data[j]);
		}
		printf("\n\n");
	}
}

int isBlank(const char *word) {
  
  while (*word != '\0') {
    if (!isspace((unsigned char)*word))
      return 0;
    word++;
  }
  return 1;
}



char* trim(char* word){ 
  
	if(word==NULL){
		return NULL;
	}

	if(word[0]=='\0'){
		return word;
	}

	if(isBlank(word) ==1){
		word="";
	} else {
		int i = 0, start = 0;
		int k= strlen(word);; //j to start at end
		int end=k; //index of where the last position of a letter is starting from the back

		while(word[i]==' '){
			i++;//if its a space goes to next position
			start=i; //gets the new position of the string
		}//at the end of the loop, start will be at the index of the first letter position

		while(word[k-1]==' '){
			k=k-1;
			end=k;
	    }

	   int subLen = end - start; //length of string
	   word = word + start;//orgword starts at beginning;
	   char* newWord = strdup(word);
	   newWord[subLen] = '\0';
	   return newWord;
   }

   return word;//trying to do a substring method to get from start to stop.

  }
	  


int main (int argc, char **argv)
{
	if (argc < 3) {
		fprintf(stderr, "ERROR: not enough parameters\n");
		exit(1);
	}
	if (argc > 3) {
		fprintf(stderr, "ERROR: too many paramters\n");
		exit(1);
	}
	
	if(strcmp(argv[1], "-c") != 0) {             //DOESNT WORK COME BACK
		fprintf(stderr, "ERROR: must specify -c\n");
		exit(1);
	}

	char buffer[1000];
	char* token;
	char* newBuffer;
	
	int cols = 0;


	fgets(buffer, 1000, stdin); //gets us headings of the data
	newBuffer= strdup(buffer);
	newBuffer[strlen(newBuffer)-2]='\0';
	char* copy = strdup(newBuffer);
	
	while((token= strsep(&copy, ","))!= NULL){
	  dynaCols++;
	}
	
	headers = malloc(sizeof(newBuffer)*dynaCols);
	
	while((token= strsep(&newBuffer, ","))!= NULL){
		headers[cols] = token;
   	    cols++;

	}
	
	
	int iter = 0;
	int colNum = -1;
	char* column = argv[2];
	for (iter = 0; iter < dynaCols; iter++) {
		char* check = headers[iter];
		if (strcmp(check, column) == 0) {
			colNum = iter;
			break;
		}
	}
	
	if (colNum == -1) {
		fprintf(stderr, "ERROR: user did not enter appropriate header\n");
		exit(1);
	}
	
	

	int staticRows = 20;
	char** arrStdin = (char**)malloc(sizeof(char*) * staticRows);
	//int iter;
	iter = 0;
	for (iter = 0; iter < staticRows; iter++){
		arrStdin[iter] = malloc (sizeof(char) * (1000+1));}
		
	char* currLine = NULL;
	size_t len = 0;
	ssize_t read;

	while((read = getline(&currLine,&len, stdin))!=-1)
	{
		char* dupRow = strdup(currLine);
		//printf("%s\n", dupRow);
		if(dynaRows == staticRows)
		{
			staticRows = staticRows*2;
			arrStdin = (char**) realloc(arrStdin, staticRows* sizeof(char*));

			int a = 0; 
			for (a = dynaRows; a < staticRows; a++)
			{
				arrStdin[a] = malloc(sizeof(char) * (1000+1));
			}
		}
		
		strcpy(arrStdin[dynaRows], dupRow);
		
		dynaRows++;
	}
	
	for(iter = 0; iter < dynaRows; iter++)
	{
		//printf("%s\n", arrStdin[iter]);
	}
	
	//printf("\n\n\n\n");
	//Creation of struct
	int rows = dynaRows;
	//int cols = dynaCols;
	
	
	 int q=0;
	struct MetaData* movies = malloc(rows*sizeof(struct MetaData) + sizeof(char*)*dynaCols);
	char* output;


	for (q=0; q < rows; q++) {
	  movies[q].data = malloc(sizeof(char*)*dynaCols);  //making room for the string fields
	}

	int r=0;
	int c=0;
	int xIndex = 0;
	for(xIndex = 0; xIndex < dynaRows; xIndex++)
	{ //while we take in data for each row
		char* rowcpy = strdup(arrStdin[xIndex]); //now we need to tokenize this string
		rowcpy[strlen(rowcpy)-2]='\0';
			while((output = strsep(&rowcpy,",")) != NULL ){
				if (output[0] == '\"') {
					//printf("%s\n", output);
					char cleanTok[1000] = "";
					output = strtok(output, "\""); //term without beginning ""
					int len= strlen(output);
					int done=0;
						while(done!=1 && output!=NULL){
						len= strlen(output);
						if(output[len-1]=='\"'){ //we have no commas
							int i=0;
							len= strlen(output);
							int cleanLen= strlen(cleanTok);
							while(output[len-1]=='\"'){
								len--;
							}
							while(i<len){
								cleanTok[cleanLen]=output[i];
								cleanLen++;
								i++;
							}
							len= strlen(cleanTok);
							cleanTok[len]='\0';
							done=1;
						}else{ // we now need to find the other terms
							strcat(cleanTok, output); //copy old term into clean tok
							cleanTok[strlen(cleanTok)] = ','; //add a comma
							output = strsep(&rowcpy,","); //must check if the name actually contains a comma (we know if there is frontquote then theres end quote)
						}//end else

					}//end inner while
					output= strdup(cleanTok);
					output=trim(output);
				}//end main if
			output=trim(output); 
			movies[r].data[c] = output; 
			c++;
		}//end of while
		c=0;
		r++;

	} //end of main while
	cols = dynaCols; //double checking
	rows = dynaRows; //double checking
	

	int i;
	for(i =0; i < dynaCols; i++) {
		if (isNum(movies[i].data[colNum]) == 1) {  
			regNum = 1;
			break;
		}
	}
	
	for(i =0; i < dynaCols; i++) {
		if (regNum ==1) {
			if (isDouble(movies[i].data[colNum]) == 1) {  
			regNum = 0;
			decNum = 1;
			break;
			}
		}
	}
		
   
	mergeSort(movies, rows, cols, colNum, regNum, decNum);
	print(movies,rows, cols);

	free(movies);
	
  return 0;
}