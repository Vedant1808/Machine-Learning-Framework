#include<mlfw_matrix.h>
#include<mlfw_vector.h>
#include<stdio.h>
#include<stdlib.h>
int main()
{
	dimension_t size,rows,columns;
	index_t r,c;
	double value;
	char *ptr;
	mlfw_mat_double *matrix;
	mlfw_row_vec_string *header;
	matrix=mlfw_mat_double_from_csv("IceCreamSales.csv",NULL,&header);
	if(matrix==NULL)
	{
		printf("Unable to load IceCreamSales.csv\n");
		return 0;
	}
	size=mlfw_row_vec_string_get_size(header);
	for(int i=0;i<size;++i)
	{
		mlfw_row_vec_string_get(header,i,&ptr);
		if(ptr!=NULL)
		{
			printf("%s ",ptr);
			free(ptr);
		}
	}
	printf("\n");
	mlfw_mat_double_get_dimensions(matrix,&rows,&columns);
	for(r=0;r<rows;++r)
	{
		for(c=0;c<columns;++c)
		{
			value=mlfw_mat_double_get(matrix,r,c);
			printf("%lf ",value);
		}
		printf("\n");
	}
	mlfw_row_vec_string_destroy(header);
	mlfw_mat_double_destroy(matrix);
	return 0;
}
