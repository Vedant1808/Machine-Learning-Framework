#include<mlfw_vector.h>
#include<stdio.h>
#include<stdlib.h>
int main()
{
	dimension_t size;
	index_t c;
	char *ptr;
	mlfw_row_vec_string *vector;
	vector=mlfw_row_vec_string_from_csv("row_vector_string.csv");
	if(vector==NULL)
	{
		printf("Unable to load row_vector_string.csv\n");
		return 0;
	}
	size=mlfw_row_vec_string_get_size(vector);
	for(c=0;c<size;++c)
	{
		mlfw_row_vec_string_get(vector,c,&ptr);
		if(ptr!=NULL)
		{
			printf("%s\n",ptr);
			free(ptr);
		}	
	}
	mlfw_row_vec_string_to_csv(vector,"test_row_vector2.csv");
	mlfw_row_vec_string_destroy(vector);
	return 0;
}
