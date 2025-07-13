#include<mlfw_matrix.h>
#include<stdlib.h>
#include<stdio.h>
int main(int argc,char *argv[])
{
	char *input_file;
	char *output_file;
	char *min_max_file;
	double min,max,scaled_value;
	double value;
	mlfw_mat_double *matrix;
	mlfw_mat_double *min_max_matrix;
	dimension_t matrix_rows,matrix_columns;
	dimension_t min_max_rows,min_max_columns;
	index_t r,c;
	if(argc!=4)
        {
		printf("Usage [reverse_scale_output.out input_file output_file min_max_file]\n");
		return 0;
	}
	input_file=argv[1];
	output_file=argv[2];
	min_max_file=argv[3];
	matrix=mlfw_mat_double_from_csv(input_file);
	if(matrix==NULL)
	{
		printf("Low memory\n");
		return 0;
	}
	mlfw_mat_double_get_dimensions(matrix,&matrix_rows,&matrix_columns);
	min_max_matrix=mlfw_mat_double_from_csv(min_max_file);
	if(min_max_matrix==NULL)
	{
		printf("Low memory\n");
		return 0;
	}
	mlfw_mat_double_get_dimensions(min_max_matrix,&min_max_rows,&min_max_columns);
	min=mlfw_mat_double_get(min_max_matrix,0,min_max_columns-1);
	max=mlfw_mat_double_get(min_max_matrix,1,min_max_columns-1);
	for(r=0;r<matrix_rows;++r)
	{
		for(c=matrix_columns-2;c<matrix_columns;++c)
		{
			scaled_value=mlfw_mat_double_get(matrix,r,c);
			value=(scaled_value*(max-min))+min;
			mlfw_mat_double_set(matrix,r,c,value);
		}
	}
	mlfw_mat_double_to_csv(matrix,output_file);
	mlfw_mat_double_destroy(matrix);
	mlfw_mat_double_destroy(min_max_matrix);
	return 0;
}
