#include<mlfw_matrix.h>
#include<mlfw_vector.h>
#include<mlfw_operations.h>
#include<stdio.h>
#include<stdio_ext.h>  // for linux only

int main()
{
	double temperature_c;
	double temperature_f;
	double profit;
	mlfw_column_vec_double *parameters_vector;
	mlfw_row_vec_double *features_vector;
	mlfw_column_vec_double *predicted_value_vector;
	FILE *file;
	printf("-------------------------------------------------------------------------\n");
	printf("             Ice Cream Sales Profit Predictor\n");
	printf("-------------------------------------------------------------------------\n");
	file=fopen("trained_parameters.csv","r");
	if(file==NULL)
	{
		printf("trained_parameters.csv does not exit\n");
		return 0;
	}
	printf("Enter temperature (in celsius)  : ");
	scanf("%lf",&temperature_c);
	__fpurge(stdin);
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

