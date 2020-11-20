7 of about 69
Fwd:
Inbox
	x
VYSHNAV RAJ S
	
AttachmentsNov 16, 2019, 5:06 PM (5 days ago)
	
to me

---------- Forwarded message ---------
From: VYSHNAV RAJ S <svyshnavraj@gmail.com>
Date: Fri, Oct 25, 2019, 1:13 AM
Subject:
To: VYSHNAV RAJ S <svyshnavraj@gmail.com>




4 Attachments
	
	
	

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX_SIZE 100

// Global variables

char string[MAX_SIZE];
int length = 0;
int lookup = 0;
int error = 0;

// function prototype

void E();
void T();
void F();
void E_();
void T_();

// util

int match(char ch){
	if(lookup < length && string[lookup] == ch){
		lookup++;
		return 1;
	}
	return 0;
}

// variables

void E(){
	T();
	E_();
}

void T(){
	F();
	T_();
}

void F(){
	if(match('(')){
		E();
		if(match(')') == 0)
			error = 1;
	}
	else{
		if(match('i') == 0)
			error = 1;
	}
}

void E_(){
	if(match('+')){
		T();
		E_();
	}
}

void T_(){
	if(match('*')){
		F();
		T_();
	}
}

// driver function

void main(){
	printf("Enter the string to be parsed: ");
	scanf("%s", string);
	length = strlen(string);

	printf("String: %s\nLength: %d\n", string, length);
	E();

	printf("Lookup position: %d\nError found: %d\n\n", lookup, error);

	if(lookup == length && error == 0){
		printf("String parsed successfully.\n");
	}
	else{
		printf("Error parsing string.\n");
	}
}

