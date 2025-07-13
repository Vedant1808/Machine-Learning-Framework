#include<mlfw_matrix.h>
#include<mlfw_vector.h>
#include<mlfw_scale.h>
#include<stdio.h>
int main()
{
	mlfw_mat_double *scaled_matrix;
	mlfw_row_vec_string *columns_to_scale;
	mlfw_row_vec_string *matrix_header;
	columns_to_scale=mlfw_row_vec_string_create_new(4);
	if(columns_to_scale==NULL)
	{
		printf("Low memory\n");
		return 0;
	}
	mlfw_row_vec_string_set(columns_to_scale,0,"age");
	mlfw_row_vec_string_set(columns_to_scale,1,"height");
	mlfw_row_vec_string_set(columns_to_scale,2,"salary");
	mlfw_row_vec_string_set(columns_to_scale,3,"incentive");

	scaled_matrix=mlfw_scale_double("dummy.csv",columns_to_scale,"z_score.csv","z-score",NULL);
	if(scaled_matrix==NULL)
	{
		printf("Low memory or some other reason,scaling not done\n");
		mlfw_row_vec_string_destroy(columns_to_scale);
		return 0;
	}
	matrix_header=mlfw_row_vec_string_create_new(4);
	if(matrix_header==NULL)
	{
		printf("Low memory\n");
		mlfw_row_vec_string_destroy(columns_to_scale);
		mlfw_mat_double_destroy(scaled_matrix);
		return 0;
	}
	mlfw_row_vec_string_set(matrix_header,0,"age");
	mlfw_row_vec_string_set(matrix_header,1,"height");
	mlfw_row_vec_string_set(matrix_header,2,"salary");
	mlfw_row_vec_string_set(matrix_header,3,"incentive");
	mlfw_mat_double_to_csv(scaled_matrix,"scaled_dummy.csv",matrix_header);
	
	mlfw_row_vec_string_destroy(columns_to_scale);
	mlfw_mat_double_destroy(scaled_matrix);
	mlfw_row_vec_string_destroy(matrix_header);
	
	return 0;
}
