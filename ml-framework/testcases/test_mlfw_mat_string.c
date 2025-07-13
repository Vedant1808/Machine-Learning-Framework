#include<mlfw_matrix.h>
#include<stdio.h>
#include<stdlib.h>
int main()
{
	char *str;
	index_t r,c;
	dimension_t rows,columns;
	mlfw_mat_string *matrix;
	matrix=mlfw_mat_string_from_csv("insurance.csv");
	if(matrix==NULL)
	{
		printf("Unable to load insurance.csv\n");
		return 0;
	}
	mlfw_mat_string_get_dimensions(matrix,&rows,&columns);
	for(r=0;r<rows;++r)
	{
		for(c=0;c<columns;++c)
		{
			mlfw_mat_string_get(matrix,r,c,&str);
			if(str!=NULL)
			{
				printf("%s ",str);
				free(str);
			}
		}
		printf("\n");
	}
	mlfw_mat_string_destroy(matrix);
	return 0;
}
