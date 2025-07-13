#include<mlfw_matrix.h>
#include<mlfw_vector.h>
#include<mlfw_scale.h>
#include<stdio.h>
int main()
{
	mlfw_mat_double *scaled_matrix;
	mlfw_row_vec_string *matrix_header;
	
	scaled_matrix=mlfw_scale_double_with_given_parameters("dummy.csv","z_score.csv","z-score",NULL);
	if(scaled_matrix==NULL)
	{
		printf("Low memory or some other reason,scaling not done\n");
		return 0;
	}
	matrix_header=mlfw_row_vec_string_create_new(4);
	if(matrix_header==NULL)
	{
		printf("Low memory\n");
		mlfw_mat_double_destroy(scaled_matrix);
		return 0;
	}
	mlfw_row_vec_string_set(matrix_header,0,"age");
	mlfw_row_vec_string_set(matrix_header,1,"height");
	mlfw_row_vec_string_set(matrix_header,2,"salary");
	mlfw_row_vec_string_set(matrix_header,3,"incentive");
	mlfw_mat_double_to_csv(scaled_matrix,"scaled_dummy_with_given_parameters.csv",matrix_header);
	
	mlfw_mat_double_destroy(scaled_matrix);
	mlfw_row_vec_string_destroy(matrix_header);
	
	return 0;
}
