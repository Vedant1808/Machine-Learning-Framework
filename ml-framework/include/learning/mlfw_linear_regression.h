#ifndef __MLFW_LINEAR_REGRESSION__
#define __MLFW_LINEAR_REGRESSION__
#include<mlfw_matrix.h>
#include<mlfw_vector.h>
#include<inttypes.h>
mlfw_row_vec_double * mlfw_linear_regression_gradient_descent_fit_line(mlfw_mat_double *input_features_matrix,mlfw_column_vec_double *target_values_vector,double learning_rate,uint64_t number_of_iterations,uint8_t (*on_each_iteration)(uint64_t iteration_number,double error_value));

mlfw_column_vec_double * mlfw_linear_regression_predict(mlfw_mat_double *input_features_matrix,mlfw_row_vec_double *trained_parameters);
#endif
