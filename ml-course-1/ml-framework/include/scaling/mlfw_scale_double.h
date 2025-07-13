#ifndef __MLFW__SCALE__DOUBLE__
#define __MLFW__SCALE__DOUBLE__
#include<mlfw_matrix.h>

mlfw_mat_double * mlfw_scale_double_min_max(mlfw_mat_double *matrix,index_t start_row_index,index_t start_column_index,index_t end_row_index,index_t end_column_index,char *min_max_file);

mlfw_mat_double * mlfw_scale_double_with_given_min_max(mlfw_mat_double *matrix,index_t start_row_index,index_t start_column_index,index_t end_row_index,index_t end_column_index,mlfw_mat_double *min_max_matrix);

#endif
