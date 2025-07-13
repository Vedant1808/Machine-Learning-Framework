#include<mlfw_utils.h>
#include<stdio.h>
#include<inttypes.h>
int main()
{
	uint32_t number=65;
	char string[33]; // 1 extra for \0
	mlfw_uint32_to_binary(number,string);
	printf("%s\n",string);
	return 0;

}
