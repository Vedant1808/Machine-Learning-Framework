#include<mlfw_scale.h>
#include<mlfw_matrix.h>
#include<mlfw_vector.h>
#include<stdio.h>
int main()
{
	mlfw_mat_double *matrix,*scaled_matrix;
	mlfw_row_vec_string *header;
	dimension_t rows,columns;
	matrix=mlfw_mat_double_from_csv("dummy.csv",NULL,&header);
	if(matrix==NULL)
	{
		printf("Unable to load dummy.csv\n");
		return 0;
	}
	mlfw_mat_double_get_dimensions(matrix,&rows,&columns);
	scaled_matrix=mlfw_scale_double_z_score(matrix,0,0,rows-1,columns-1,"mean_sd.csv",NULL);
	mlfw_mat_double_to_csv(scaled_matrix,"scaled_dummy.csv",header);
	mlfw_row_vec_string_destroy(header);
	mlfw_mat_double_destroy(matrix);
	mlfw_mat_double_destroy(scaled_matrix);
	return 0;
}
