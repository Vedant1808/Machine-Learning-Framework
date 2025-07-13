#include<mlfw_matrix.h>
#include<stdio.h>
int main()
{
	mlfw_mat_double *matrix;
	dimension_t rows,columns;
	index_t r,c;
	matrix=mlfw_mat_double_create_new(2,3);
	if(matrix==NULL)
	{
		printf("Low memory\n");
		return 0;
	}
	mlfw_mat_double_set(matrix,0,0,10.23);
	mlfw_mat_double_set(matrix,0,1,20.23);
	mlfw_mat_double_set(matrix,0,2,30.23);
	mlfw_mat_double_set(matrix,1,0,110.23);
	mlfw_mat_double_set(matrix,1,1,120.23);
	mlfw_mat_double_set(matrix,1,2,130.23);

	mlfw_mat_double_get_dimensions(matrix,&rows,&columns);
	printf("Before applying right shift\n");
	for(r=0;r<rows;++r)
	{
		for(c=0;c<columns;++c)
		{
			printf("%lf ",mlfw_mat_double_get(matrix,r,c));
		}
		printf("\n");
	}
	mlfw_mat_double_right_shift(matrix,1);
	printf("After applying right shift\n");
	for(r=0;r<rows;++r)
	{
		for(c=0;c<columns;++c)
		{
			printf("%lf ",mlfw_mat_double_get(matrix,r,c));
		}
		printf("\n");
	}
	mlfw_mat_double_destroy(matrix);
	return 0;

}
