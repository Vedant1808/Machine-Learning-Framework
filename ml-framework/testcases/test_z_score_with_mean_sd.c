#include<mlfw_scale.h>
#include<mlfw_matrix.h>
#include<mlfw_vector.h>
#include<stdio.h>
int main()
{
	mlfw_mat_double *matrix,*scaled_matrix;
	mlfw_mat_double *mean_standard_deviation_matrix;
	mlfw_row_vec_string *header;
	mlfw_row_vec_string *mean_sd_header;
	dimension_t rows,columns;
	matrix=mlfw_mat_double_from_csv("dummy2.csv",NULL,&header);
	if(matrix==NULL)
	{
		printf("Unable to load dummy2.csv\n");
		return 0;
	}
	mean_standard_deviation_matrix=mlfw_mat_double_from_csv("mean_sd.csv",NULL,&mean_sd_header);
	if(mean_standard_deviation_matrix==NULL)
	{
		mlfw_mat_double_destroy(matrix);
		printf("Low memory or Unable to load mean_sd.csv\n");
		return 0;
	}
	mlfw_mat_double_get_dimensions(matrix,&rows,&columns);
	scaled_matrix=mlfw_scale_double_z_score_with_given_mean_standard_deviation(matrix,0,0,rows-1,columns-1,mean_standard_deviation_matrix,NULL);
	mlfw_mat_double_to_csv(scaled_matrix,"scaled_dummy2.csv",header);
	mlfw_row_vec_string_destroy(header);
	mlfw_row_vec_string_destroy(mean_sd_header);
	mlfw_mat_double_destroy(matrix);
	mlfw_mat_double_destroy(mean_standard_deviation_matrix);
	mlfw_mat_double_destroy(scaled_matrix);
	return 0;
}
