#ifndef __MLFW__MAT__STRING__
#define __MLFW__MAT__STRING__

struct __mlfw_mat_string;
typedef struct __mlfw_mat_string mlfw_mat_string;

mlfw_mat_string * mlfw_mat_string_create_new(dimension_t rows,dimension_t columns);
void mlfw_mat_string_destroy(mlfw_mat_string *matrix);
mlfw_mat_string * mlfw_mat_string_from_csv(const char *csv_file_name,mlfw_mat_string *matrix,mlfw_row_vec_string **header);
void mlfw_mat_string_to_csv(mlfw_mat_string *matrix,const char *csv_file_name,mlfw_row_vec_string *header);

void mlfw_mat_string_get(mlfw_mat_string *matrix,index_t row,index_t column,char **string);
void mlfw_mat_string_set(mlfw_mat_string *matrix,index_t row,index_t column,char *string);

void mlfw_mat_string_get_dimensions(mlfw_mat_string *matrix,dimension_t *rows,dimension_t *columns);

mlfw_mat_string * mlfw_mat_string_transpose(mlfw_mat_string *matrix,mlfw_mat_string *transposed_matrix);

void mlfw_mat_string_copy(mlfw_mat_string *target,mlfw_mat_string *source,index_t target_row_index,index_t target_column_index,index_t source_from_row_index,index_t source_from_column_index,index_t source_to_row_index,index_t source_to_column_index);

mlfw_mat_string * mlfw_mat_string_shuffle(mlfw_mat_string *matrix,uint8_t shuffle_times,mlfw_mat_string *shuffled_matrix);

#endif
