#include<mlfw_matrix.h>
#include<mlfw_vector.h>
#include<mlfw_learning.h>
#include<stdio.h>
#include<stdlib.h>

uint8_t screen_logger(uint64_t iteration_number,double error_value)
{
	printf("Iteration : %" PRIu64 ",Error : %40.15lf\n",iteration_number,error_value);
	return 1; // keep running
}
int main(int argc,char *argv[])
{
	mlfw_mat_double *training_examples_matrix;
	dimension_t training_examples_matrix_rows,training_examples_matrix_columns;
	mlfw_column_vec_double *training_examples_target_values_vector;
	mlfw_mat_double *test_examples_matrix;
	mlfw_column_vec_double *test_examples_target_values_vector;
	dimension_t test_examples_matrix_rows,test_examples_matrix_columns;
	mlfw_row_vec_double *trained_parameters;
	dimension_t trained_parameters_size;
	mlfw_column_vec_double *test_examples_predicted_values_vector;
	index_t i;
	double r2_score;
	if(argc!=2)
	{
		printf("Pass numbers of iterations as command line arguments\n");
		return 0;
	}

        mlfw_mat_double_get_training_testing_data("IceCreamSales.csv",&training_examples_matrix,&test_examples_matrix,20);

	if(training_examples_matrix==NULL)
	{
		printf("Unable to load IceCreamSales.csv\n");
		return 0;
	}
	mlfw_mat_double_get_dimensions(training_examples_matrix,&training_examples_matrix_rows,&training_examples_matrix_columns);
	mlfw_mat_double_get_dimensions(test_examples_matrix,&test_examples_matrix_rows,&test_examples_matrix_columns);

	training_examples_target_values_vector=mlfw_mat_double_create_column_vec(training_examples_matrix,1,NULL);

	if(training_examples_target_values_vector==NULL)
	{
		printf("Low memory\n");
		mlfw_mat_double_destroy(training_examples_matrix);
		mlfw_mat_double_destroy(test_examples_matrix);
		return 0;
	}
	mlfw_mat_double_reshape(&training_examples_matrix,training_examples_matrix_rows,training_examples_matrix_columns-1);
	if(training_examples_matrix==NULL)
	{
		printf("Low memory\n");
		mlfw_column_vec_double_destroy(training_examples_target_values_vector);
		mlfw_mat_double_destroy(test_examples_matrix);
		return 0;
	}
	training_examples_matrix_columns=training_examples_matrix_columns-1;

	trained_parameters=mlfw_linear_regression_gradient_descent_fit_line(training_examples_matrix,training_examples_target_values_vector,0.0001,atoi(argv[1]),screen_logger);
	if(trained_parameters==NULL)
	{
		printf("Low memory\n");
		mlfw_mat_double_destroy(training_examples_matrix);
		mlfw_mat_double_destroy(test_examples_matrix);
		mlfw_column_vec_double_destroy(training_examples_target_values_vector);
		return 0;
	}
	trained_parameters_size=mlfw_row_vec_double_get_size(trained_parameters);
	printf("Trained parameters are as follows\n");
	for(i=0;i<trained_parameters_size;++i)
	{
		printf("%lf  ",mlfw_row_vec_double_get(trained_parameters,i));
	}

	mlfw_mat_double_destroy(training_examples_matrix);
	mlfw_column_vec_double_destroy(training_examples_target_values_vector);

	test_examples_target_values_vector=mlfw_mat_double_create_column_vec(test_examples_matrix,1,NULL);
	if(test_examples_target_values_vector==NULL)
	{
		printf("Low memory\n");
		mlfw_mat_double_destroy(test_examples_matrix);
		mlfw_row_vec_double_destroy(trained_parameters);
		return 0;
	}
	mlfw_mat_double_reshape(&test_examples_matrix,test_examples_matrix_rows,test_examples_matrix_columns-1);
	if(test_examples_matrix==NULL)
	{
		printf("Low memory\n");
		mlfw_row_vec_double_destroy(trained_parameters);
		return 0;
	}
	test_examples_matrix_columns=test_examples_matrix_columns-1;
        test_examples_predicted_values_vector=mlfw_linear_regression_predict(test_examples_matrix,trained_parameters);
	if(test_examples_predicted_values_vector==NULL)
	{
		mlfw_mat_double_destroy(test_examples_matrix);
		mlfw_column_vec_double_destroy(test_examples_target_values_vector);
		mlfw_row_vec_double_destroy(trained_parameters);
		return 0;
	}
	r2_score=mlfw_get_r2_score(test_examples_target_values_vector,test_examples_predicted_values_vector);
	printf("Good accuracy score is anything greater than or equal to 0.7\n");
	printf("Accuracy score (0-7) is %lf\n",r2_score);
	mlfw_mat_double_destroy(test_examples_matrix);
	mlfw_row_vec_double_destroy(trained_parameters);
	mlfw_column_vec_double_destroy(test_examples_target_values_vector);
	mlfw_column_vec_double_destroy(test_examples_predicted_values_vector);
	return 0;
}
