#include<mlfw_scale.h>
#include<mlfw_matrix.h>
#include<stdio.h>
#include<stdlib.h>
int main(int argc,char *argv[])
{
	dimension_t matrix_rows,matrix_columns;
	mlfw_mat_double *matrix;
	mlfw_mat_double *scaled_matrix;

	char *input_file;
	char *output_file;
	char *min_max_file;
	index_t from_column_index;
	index_t to_column_index;
	if(argc!=6)
	{
		printf("Usage : [min_max_scaler.out input_file output_file from_column to_column min_max_file]\n");
		return 0;
	}
	input_file=argv[1];
	output_file=argv[2];
	from_column_index=atoi(argv[3]);
	to_column_index=atoi(argv[4]);
	min_max_file=argv[5];
	matrix=mlfw_mat_double_from_csv(input_file);
	if(matrix==NULL)
	{
		return 0;
	}
	mlfw_mat_double_get_dimensions(matrix,&matrix_rows,&matrix_columns);
	scaled_matrix=mlfw_scale_double_min_max(matrix,0,from_column_index,matrix_rows-1,to_column_index,min_max_file);
	if(scaled_matrix==NULL)
	{
		mlfw_mat_double_destroy(matrix);
		return 0;
	}
	mlfw_mat_double_to_csv(scaled_matrix,output_file);
	mlfw_mat_double_destroy(matrix);
	mlfw_mat_double_destroy(scaled_matrix);
	return 0;
}
