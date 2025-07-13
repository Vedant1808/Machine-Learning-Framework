#include<mlfw_matrix.h>
#include<mlfw_vector.h>
#include<stdio.h>

int main(int argc,char *argv[])
{
	mlfw_row_vec_string *header;
	mlfw_mat_double *minor_matrix;
	mlfw_mat_double *major_matrix;
	mlfw_mat_double_get_training_testing_data("IceCreamSales.csv",&major_matrix,&minor_matrix,20);
	if(major_matrix==NULL)
	{
		printf("Unable to create testing & training dataset\n");
		return 0;
	}
	header=mlfw_row_vec_string_create_new(2);
	if(header==NULL)
	{
		printf("Low memory\n");
		return 0;
	}
	mlfw_row_vec_string_set(header,0,"temperature");
	mlfw_row_vec_string_set(header,1,"profit");
	mlfw_mat_double_to_csv(major_matrix,"training_data.csv",header);
	mlfw_mat_double_to_csv(minor_matrix,"testing_data.csv",header);
	mlfw_row_vec_string_destroy(header);
	mlfw_mat_double_destroy(minor_matrix);
	mlfw_mat_double_destroy(major_matrix);
	printf("training_data.csv and testing_data.csv created\n");
	return 0;
}
