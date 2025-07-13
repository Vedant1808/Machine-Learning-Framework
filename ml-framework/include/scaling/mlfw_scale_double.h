#ifndef __MLFW__SCALE__DOUBLE__
#define __MLFW__SCALE__DOUBLE__
#include<mlfw_matrix.h>

mlfw_mat_double * mlfw_scale_double(char *dataset_file_name,mlfw_row_vec_string *columns_to_scale,char *parameters_file_name,char *algorithm,mlfw_mat_double *matrix);

mlfw_mat_double * mlfw_scale_double_with_given_parameters(char *dataset_file_name,char *parameters_file_name,char *algorithm,mlfw_mat_double *matrix);

mlfw_mat_double * mlfw_scale_double_min_max(mlfw_mat_double *matrix,index_t start_row_index,index_t start_column_index,index_t end_row_index,index_t end_column_index,mlfw_mat_double **min_max_matrix,mlfw_mat_double *scaled_matrix);

mlfw_mat_double * mlfw_scale_double_with_given_min_max(mlfw_mat_double *matrix,index_t start_row_index,index_t start_column_index,index_t end_row_index,index_t end_column_index,mlfw_mat_double *min_max_matrix,mlfw_mat_double *scaled_matrix);

mlfw_mat_double * mlfw_scale_double_z_score(mlfw_mat_double *matrix,index_t start_row_index,index_t start_column_index,index_t end_row_index,index_t end_column_index,mlfw_mat_double **mean_standard_deviation_matrix,mlfw_mat_double *scaled_matrix);

mlfw_mat_double * mlfw_scale_double_z_score_with_given_mean_standard_deviation(mlfw_mat_double *matrix,index_t start_row_index,index_t start_column_index,index_t end_row_index,index_t end_column_index,mlfw_mat_double *mean_standard_deviation_matrix,mlfw_mat_double *scaled_matrix);

#endif
