#include<stdio.h>
#include<stdlib.h>
#include<mlfw_matrix.h>
#include<mlfw_vector.h>
#include<mlfw_operations.h>

mlfw_column_vec_double * mlfw_multiply_double_row_vector_with_column_vector(mlfw_row_vec_double *left_vector,mlfw_column_vec_double *right_vector)
{
	mlfw_column_vec_double *product_vector;
	double product;
	index_t i;
	dimension_t left_vector_size;
	dimension_t right_vector_size;
	double left_vector_value;
	double right_vector_value;

	if(left_vector==NULL || right_vector==NULL) return NULL;
	left_vector_size=mlfw_row_vec_double_get_size(left_vector);
	right_vector_size=mlfw_column_vec_double_get_size(right_vector);
	if(left_vector_size!=right_vector_size) return NULL;

	//create column vector to store result of row_vector x column_vector
	//100% result will be a column vector of 1 row
	product_vector=mlfw_column_vec_double_create_new(1);
	if(product_vector==NULL) return NULL;
	product=0.0;
	for(i=0;i<left_vector_size;++i)
	{
		left_vector_value=mlfw_row_vec_double_get(left_vector,i);
		right_vector_value=mlfw_column_vec_double_get(right_vector,i);
		product=product+(left_vector_value*right_vector_value);
	}
	mlfw_column_vec_double_set(product_vector,0,product);
	return product_vector;
}
mlfw_column_vec_double * mlfw_multiply_double_matrix_with_column_vector(mlfw_mat_double *left_matrix,mlfw_column_vec_double *right_vector)
{
         dimension_t left_matrix_rows;
	 dimension_t left_matrix_columns;

	 dimension_t right_vector_size;

	 double left_matrix_value;
	 double right_vector_value;

	 double product;
	 mlfw_column_vec_double *product_vector;

	 index_t r,c;

	 if(left_matrix==NULL || right_vector==NULL) return NULL;
	 mlfw_mat_double_get_dimensions(left_matrix,&left_matrix_rows,&left_matrix_columns);
	 right_vector_size=mlfw_column_vec_double_get_size(right_vector);
	 if(left_matrix_columns!=right_vector_size) return NULL;

	 product_vector=mlfw_column_vec_double_create_new(left_matrix_rows);
	 if(product_vector==NULL) return NULL;

	 for(r=0;r<left_matrix_rows;++r)
	 {
		 product=0;
		 for(c=0;c<left_matrix_columns;++c)
		 {
			 left_matrix_value=mlfw_mat_double_get(left_matrix,r,c);
			 right_vector_value=mlfw_column_vec_double_get(right_vector,c);
			 product=product+(left_matrix_value*right_vector_value);
		 }
		 mlfw_column_vec_double_set(product_vector,r,product);
	 }
	 return product_vector;
}
mlfw_column_vec_double * mlfw_substract_double_column_vector(mlfw_column_vec_double *left_vector,mlfw_column_vec_double *right_vector)
{
	mlfw_column_vec_double *difference_vector;
	double difference;
	index_t i;
	dimension_t left_vector_size;
	dimension_t right_vector_size;
	double left_vector_value;
	double right_vector_value;

	if(left_vector==NULL || right_vector==NULL) return NULL;
	left_vector_size=mlfw_column_vec_double_get_size(left_vector);
	right_vector_size=mlfw_column_vec_double_get_size(right_vector);
	if(left_vector_size!=right_vector_size) return NULL;

	difference_vector=mlfw_column_vec_double_create_new(left_vector_size);
	if(difference_vector==NULL) return NULL;
	for(i=0;i<left_vector_size;++i)
	{
		left_vector_value=mlfw_column_vec_double_get(left_vector,i);
		right_vector_value=mlfw_column_vec_double_get(right_vector,i);
		difference=left_vector_value-right_vector_value;
	        mlfw_column_vec_double_set(difference_vector,i,difference);	
	}
	return difference_vector;
}

mlfw_column_vec_double *mlfw_multiply_double_scalar_with_column_vector(double scalar_value,mlfw_column_vec_double *vector)
{
	mlfw_column_vec_double *product_vector;
	dimension_t vector_size;
	double value;
	double product;
	index_t i;
	if(vector==NULL) return NULL;
	vector_size=mlfw_column_vec_double_get_size(vector);
	product_vector=mlfw_column_vec_double_create_new(vector_size);
	if(product_vector==NULL) return NULL;
	for(i=0;i<vector_size;++i)
	{
		value=mlfw_column_vec_double_get(vector,i);
		product=value*scalar_value;
		mlfw_column_vec_double_set(product_vector,i,product);
	}
	return product_vector;
}
