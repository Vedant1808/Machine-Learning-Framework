#include<mlfw_matrix.h>
#include<mlfw_vector.h>
#include<mlfw_operations.h>
#include<stdio.h>
#include<stdlib.h>
int main(int argc,char *argv[])
{
	double temperature_c;
	double temperature_f;
	double profit;
	mlfw_column_vec_double *parameters_vector;
	mlfw_row_vec_double *features_vector;
	mlfw_column_vec_double *predicted_value_vector;
	FILE *file;
	if(argc!=2)
	{
		printf("Usage [./App temperature_in_celsius]\n");
		return 0;
        }
	file=fopen("trained_parameters.csv","r");
	if(file==NULL)
	{
		fprintf(stderr,"trained_parameters.csv does not exit\n");
		return 0;
	}
	temperature_c=strtod(argv[1],NULL);
	temperature_f=(temperature_c*9.0/5.0)+32.0;
	features_vector=mlfw_row_vec_double_create_new(2);
	if(features_vector==NULL)
	{
		printf("Low memory\n");
		return 0;
	}
	mlfw_row_vec_double_set(features_vector,0,1.0);  // bias
	mlfw_row_vec_double_set(features_vector,1,temperature_f);
	parameters_vector=mlfw_column_vec_double_from_csv("trained_parameters.csv");
	if(parameters_vector==NULL)
	{
		mlfw_row_vec_double_destroy(features_vector);
		printf("Low memory\n");
		return 0;
	}
	predicted_value_vector=mlfw_multiply_double_row_vector_with_column_vector(features_vector,parameters_vector);
	if(predicted_value_vector==NULL)
	{
		mlfw_row_vec_double_destroy(features_vector);
		mlfw_column_vec_double_destroy(parameters_vector);
		printf("Low memory\n");
		return 0;
	}
	profit=mlfw_column_vec_double_get(predicted_value_vector,0);
	mlfw_row_vec_double_destroy(features_vector);
	mlfw_column_vec_double_destroy(parameters_vector);
	mlfw_column_vec_double_destroy(predicted_value_vector);
	printf("Expected profit in USD : %10.2lf\n",profit);
	return 0;
}

