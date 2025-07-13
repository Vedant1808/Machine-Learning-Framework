#include<mlfw_utils.h>
#include<stdio.h>

int mlfw_strcmp_case_insensitive(const char *left,const char *right)
{
	// this function should return 0 if both are same
	// if the left operand is greater then the right one then return +ve
	// else return -ve
	char a,b;
	if(left==NULL && right==NULL) return 0;
	if(left==NULL) return -1;
	if(right==NULL) return -1;
	while(*left!='\0' && *right!='\0')
	{
		a=*left;
		b=*right;
		if(a>=97 && a<=122) a-=32;
		if(b>=97 && b<=122) b-=32;
		if(a!=b) return a-b;
		left++;
		right++;
	}
	return (*left)-(*right);
}

void mlfw_uint32_to_binary(uint32_t number,char *string)
{
	int i;
	int j;
	int bit;
	for(j=0,i=31;i>=0;--i,++j)
	{
		bit=(number>>i) & 1;
		if(bit==0) string[j]='0';
		else string[j]='1';
	}
	string[j]='\0';
}


