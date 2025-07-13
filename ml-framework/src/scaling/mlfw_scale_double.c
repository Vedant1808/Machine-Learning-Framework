#include<mlfw_matrix.h>
#include<mlfw_scale.h>
#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<mlfw_utils.h>

mlfw_mat_double * mlfw_scale_double_min_max(mlfw_mat_double *matrix,index_t start_row_index,index_t start_column_index,index_t end_row_index,index_t end_column_index,mlfw_mat_double **min_max_matrix,mlfw_mat_double *new_matrix)
{
	dimension_t rows,columns;
	double scaled_value;
	double value;
	index_t r,c;
	index_t new_matrix_r,new_matrix_c;
	index_t i;
	dimension_t matrix_rows,matrix_columns;
	dimension_t new_matrix_rows,new_matrix_columns;
        double *min;
	double *max;

	if(matrix==NULL || min_max_matrix==NULL) return NULL;
	mlfw_mat_double_get_dimensions(matrix,&matrix_rows,&matrix_columns);
	if(start_row_index<0 || end_row_index>=matrix_rows) return NULL;
	if(start_column_index<0 || end_column_index>=matrix_columns) return NULL;
	if(start_row_index>end_row_index || start_column_index>end_column_index) return NULL;
	new_matrix_rows=end_row_index-start_row_index+1;
	new_matrix_columns=end_column_index-start_column_index+1;
	if(new_matrix==NULL)
	{
	new_matrix=mlfw_mat_double_create_new(new_matrix_rows,new_matrix_columns);
	if(new_matrix==NULL) return NULL;
	}
	else
	{
		mlfw_mat_double_get_dimensions(new_matrix,&rows,&columns);
		if(rows!=new_matrix_rows || columns!=new_matrix_columns) return NULL;
	}
	*min_max_matrix=mlfw_mat_double_create_new(2,new_matrix_columns);
	if(*min_max_matrix==NULL)
	{
		return NULL;
	}
	max=(double *)malloc(sizeof(double)*new_matrix_columns);
	if(max==NULL)
	{
		mlfw_mat_double_destroy(*min_max_matrix);
		*min_max_matrix=NULL;
		return NULL;
	}
	min=(double *)malloc(sizeof(double)*new_matrix_columns);
	if(min==NULL)
        {
		mlfw_mat_double_destroy(*min_max_matrix);
		*min_max_matrix=NULL;
		free(max);
		return NULL;
	}
	i=0;
	for(c=start_column_index;c<=end_column_index;++c)
	{
		min[i]=mlfw_mat_double_get_minimum(matrix,start_row_index,c,end_row_index,c);
		max[i]=mlfw_mat_double_get_maximum(matrix,start_row_index,c,end_row_index,c);
		i++;
	}
	r=start_row_index;
	for(new_matrix_r=0;new_matrix_r<new_matrix_rows;++new_matrix_r)
	{
		c=start_column_index;
		for(new_matrix_c=0;new_matrix_c<new_matrix_columns;++new_matrix_c)
		{
			value=mlfw_mat_double_get(matrix,r,c);
			scaled_value=(value-min[new_matrix_c])/(max[new_matrix_c]-min[new_matrix_c]);
			mlfw_mat_double_set(new_matrix,new_matrix_r,new_matrix_c,scaled_value);
			++c;
		}
		++r;
	}

	for(i=0;i<new_matrix_columns;++i)
	{
		mlfw_mat_double_set(*min_max_matrix,0,i,min[i]);
		mlfw_mat_double_set(*min_max_matrix,1,i,max[i]);
	}
	free(min);
	free(max);

	return new_matrix;
}


mlfw_mat_double * mlfw_scale_double_with_given_min_max(mlfw_mat_double *matrix,index_t start_row_index,index_t start_column_index,index_t end_row_index,index_t end_column_index,mlfw_mat_double *min_max_matrix,mlfw_mat_double *new_matrix)
{
	dimension_t rows,columns;
	double scaled_value;
	double value;
	double min;
	double max;
	index_t r,c;
	index_t new_matrix_r,new_matrix_c;
	dimension_t matrix_rows,matrix_columns;
	dimension_t min_max_rows,min_max_columns;
	dimension_t new_matrix_rows,new_matrix_columns;
	if(matrix==NULL || min_max_matrix==NULL) return NULL;
	mlfw_mat_double_get_dimensions(matrix,&matrix_rows,&matrix_columns);
	mlfw_mat_double_get_dimensions(min_max_matrix,&min_max_rows,&min_max_columns);
	if(min_max_rows!=2) return NULL;

	if(start_row_index<0 || end_row_index>=matrix_rows) return NULL;
	if(start_column_index<0 || end_column_index>=matrix_columns) return NULL;
	if(start_row_index>end_row_index || start_column_index>end_column_index) return NULL;
	new_matrix_rows=end_row_index-start_row_index+1;
	new_matrix_columns=end_column_index-start_column_index+1;
	if(min_max_columns!=new_matrix_columns) return NULL;
	if(new_matrix==NULL)
	{
	new_matrix=mlfw_mat_double_create_new(new_matrix_rows,new_matrix_columns);
	if(new_matrix==NULL) return NULL;
        }
	else
	{
		mlfw_mat_double_get_dimensions(new_matrix,&rows,&columns);
		if(rows!=new_matrix_rows || columns!=new_matrix_columns) return NULL;
	}
	r=start_row_index;
	for(new_matrix_r=0;new_matrix_r<new_matrix_rows;++new_matrix_r)
	{
		c=start_column_index;
		for(new_matrix_c=0;new_matrix_c<new_matrix_columns;++new_matrix_c)
		{
			min=mlfw_mat_double_get(min_max_matrix,0,new_matrix_c);
			max=mlfw_mat_double_get(min_max_matrix,1,new_matrix_c);
			value=mlfw_mat_double_get(matrix,r,c);
			scaled_value=(value-min)/(max-min);
			mlfw_mat_double_set(new_matrix,new_matrix_r,new_matrix_c,scaled_value);
			++c;
		}
		++r;
	}

	return new_matrix;
}

mlfw_mat_double * mlfw_scale_double_z_score(mlfw_mat_double *matrix,index_t start_row_index,index_t start_column_index,index_t end_row_index,index_t end_column_index,mlfw_mat_double **mean_standard_deviation_matrix,mlfw_mat_double *new_matrix)
{
	dimension_t rows,columns;
	double scaled_value;
	double value;
	index_t r,c;
	index_t new_matrix_r,new_matrix_c;
	index_t i;
	dimension_t matrix_rows,matrix_columns;
	dimension_t new_matrix_rows,new_matrix_columns;
        double *mean;
	double *standard_deviation;

	if(matrix==NULL || mean_standard_deviation_matrix==NULL) return NULL;
	mlfw_mat_double_get_dimensions(matrix,&matrix_rows,&matrix_columns);
	if(start_row_index<0 || end_row_index>=matrix_rows) return NULL;
	if(start_column_index<0 || end_column_index>=matrix_columns) return NULL;
	if(start_row_index>end_row_index || start_column_index>end_column_index) return NULL;
	new_matrix_rows=end_row_index-start_row_index+1;
	new_matrix_columns=end_column_index-start_column_index+1;
	if(new_matrix==NULL)
	{
	new_matrix=mlfw_mat_double_create_new(new_matrix_rows,new_matrix_columns);
	if(new_matrix==NULL) return NULL;
	}
	else
	{
		mlfw_mat_double_get_dimensions(new_matrix,&rows,&columns);
		if(rows!=new_matrix_rows || columns!=new_matrix_columns) return NULL;
	}
	*mean_standard_deviation_matrix=mlfw_mat_double_create_new(2,new_matrix_columns);
	if(*mean_standard_deviation_matrix==NULL)
	{
		return NULL;
	}
	mean=(double *)malloc(sizeof(double)*new_matrix_columns);
	if(mean==NULL)
	{
		mlfw_mat_double_destroy(*mean_standard_deviation_matrix);
		*mean_standard_deviation_matrix=NULL;
		return NULL;
	}
	standard_deviation=(double *)malloc(sizeof(double)*new_matrix_columns);
	if(standard_deviation==NULL)
        {
		mlfw_mat_double_destroy(*mean_standard_deviation_matrix);
		*mean_standard_deviation_matrix=NULL;
		free(mean);
		return NULL;
	}
	i=0;
	for(c=start_column_index;c<=end_column_index;++c)
	{
		mean[i]=mlfw_mat_double_get_mean(matrix,start_row_index,c,end_row_index,c);
		standard_deviation[i]=mlfw_mat_double_get_standard_deviation(matrix,start_row_index,c,end_row_index,c);
		i++;
	}
	r=start_row_index;
	for(new_matrix_r=0;new_matrix_r<new_matrix_rows;++new_matrix_r)
	{
		c=start_column_index;
		for(new_matrix_c=0;new_matrix_c<new_matrix_columns;++new_matrix_c)
		{
			value=mlfw_mat_double_get(matrix,r,c);
			scaled_value=(value-mean[new_matrix_c])/(standard_deviation[new_matrix_c]);
			mlfw_mat_double_set(new_matrix,new_matrix_r,new_matrix_c,scaled_value);
			++c;
		}
		++r;
	}

	for(i=0;i<new_matrix_columns;++i)
	{
		mlfw_mat_double_set(*mean_standard_deviation_matrix,0,i,mean[i]);
		mlfw_mat_double_set(*mean_standard_deviation_matrix,1,i,standard_deviation[i]);
	}
	free(mean);
	free(standard_deviation);
	return new_matrix;
}


mlfw_mat_double * mlfw_scale_double_z_score_with_given_mean_standard_deviation(mlfw_mat_double *matrix,index_t start_row_index,index_t start_column_index,index_t end_row_index,index_t end_column_index,mlfw_mat_double *mean_standard_deviation_matrix,mlfw_mat_double *new_matrix)
{
        dimension_t rows,columns;
	double scaled_value;
	double value;
	double mean;
	double standard_deviation;
	index_t r,c;
	index_t new_matrix_r,new_matrix_c;
	dimension_t matrix_rows,matrix_columns;
	dimension_t mean_standard_deviation_matrix_rows,mean_standard_deviation_matrix_columns;
	dimension_t new_matrix_rows,new_matrix_columns;
	if(matrix==NULL || mean_standard_deviation_matrix==NULL) return NULL;
	mlfw_mat_double_get_dimensions(matrix,&matrix_rows,&matrix_columns);
	mlfw_mat_double_get_dimensions(mean_standard_deviation_matrix,&mean_standard_deviation_matrix_rows,&mean_standard_deviation_matrix_columns);
	if(mean_standard_deviation_matrix_rows!=2) return NULL;

	if(start_row_index<0 || end_row_index>=matrix_rows) return NULL;
	if(start_column_index<0 || end_column_index>=matrix_columns) return NULL;
	if(start_row_index>end_row_index || start_column_index>end_column_index) return NULL;
	new_matrix_rows=end_row_index-start_row_index+1;
	new_matrix_columns=end_column_index-start_column_index+1;
	if(mean_standard_deviation_matrix_columns!=new_matrix_columns) return NULL;
	if(new_matrix==NULL)
	{
	new_matrix=mlfw_mat_double_create_new(new_matrix_rows,new_matrix_columns);
	if(new_matrix==NULL) return NULL;
        }
	else
	{
		mlfw_mat_double_get_dimensions(new_matrix,&rows,&columns);
		if(rows!=new_matrix_rows || columns!=new_matrix_columns) return NULL;
	}
	r=start_row_index;
	for(new_matrix_r=0;new_matrix_r<new_matrix_rows;++new_matrix_r)
	{
		c=start_column_index;
		for(new_matrix_c=0;new_matrix_c<new_matrix_columns;++new_matrix_c)
		{
			mean=mlfw_mat_double_get(mean_standard_deviation_matrix,0,new_matrix_c);
			standard_deviation=mlfw_mat_double_get(mean_standard_deviation_matrix,1,new_matrix_c);
			value=mlfw_mat_double_get(matrix,r,c);
			scaled_value=(value-mean)/standard_deviation;
			mlfw_mat_double_set(new_matrix,new_matrix_r,new_matrix_c,scaled_value);
			++c;
		}
		++r;
	}

	return new_matrix;

}

mlfw_mat_double *mlfw_scale_double(char *dataset_file_name,mlfw_row_vec_string *columns_to_scale,char *parameters_file_name,char *algorithm,mlfw_mat_double *matrix)
{
	mlfw_row_vec_string *matrix_header;
	dimension_t matrix_rows,matrix_columns;

	index_t i,j;
	dimension_t columns_to_scale_size;
	dimension_t matrix_header_size;

	char *scale_column_name;
	char *column_name;

	mlfw_mat_double *scaled_column_matrix;
	mlfw_mat_double *parameters_matrix;
	mlfw_mat_double *tmp_matrix;

	uint8_t algorithm_code;
	uint8_t MIN_MAX_ALGORITHM=1;
	uint8_t Z_SCORE_ALGORITHM=2;

	if(dataset_file_name==NULL || columns_to_scale==NULL || parameters_file_name==NULL)
	{
		return NULL;
	}
	if(algorithm==NULL) return NULL;

	if(mlfw_strcmp_case_insensitive(algorithm,"min-max")==0)
	{
		algorithm_code=MIN_MAX_ALGORITHM;
	}else if(mlfw_strcmp_case_insensitive(algorithm,"z-score")==0)
	{
		algorithm_code=Z_SCORE_ALGORITHM;
	}else
	{
		return NULL;
	}
	columns_to_scale_size=mlfw_row_vec_string_get_size(columns_to_scale);
	if(columns_to_scale_size==0) return NULL;

	if(algorithm_code==MIN_MAX_ALGORITHM)
	{
		parameters_matrix=mlfw_mat_double_create_new(2,columns_to_scale_size);
	        if(parameters_matrix==NULL) return NULL;
	}
	else if(algorithm_code==Z_SCORE_ALGORITHM)
	{
		parameters_matrix=mlfw_mat_double_create_new(2,columns_to_scale_size);
	        if(parameters_matrix==NULL) return NULL;
	}

	matrix=mlfw_mat_double_from_csv(dataset_file_name,matrix,&matrix_header);
	if(matrix==NULL)
	{
		mlfw_mat_double_destroy(parameters_matrix);
		return NULL;
	}
	
	mlfw_mat_double_get_dimensions(matrix,&matrix_rows,&matrix_columns);
	matrix_header_size=mlfw_row_vec_string_get_size(matrix_header);

	scaled_column_matrix=mlfw_mat_double_create_new(matrix_rows,1);
	if(scaled_column_matrix==NULL)
	{
		mlfw_mat_double_destroy(parameters_matrix);
		mlfw_mat_double_destroy(matrix);
		mlfw_row_vec_string_destroy(matrix_header);
		return NULL;
	}
	
	// iterate the columns_to_scale_vector
	for(i=0;i<columns_to_scale_size;++i)
	{
		// extract column name from columns to scale
		mlfw_row_vec_string_get(columns_to_scale,i,&scale_column_name);
		if(scale_column_name==NULL)
		{
		mlfw_mat_double_destroy(parameters_matrix);
		mlfw_mat_double_destroy(matrix);
		mlfw_row_vec_string_destroy(matrix_header);
		mlfw_mat_double_destroy(scaled_column_matrix);
		return NULL;
		}

		// search the scale_column_name in headers vector
		for(j=0;j<matrix_header_size;++j)
		{
                     mlfw_row_vec_string_get(matrix_header,j,&column_name);
		     if(column_name==NULL)
		     {
		         mlfw_mat_double_destroy(parameters_matrix);
		         mlfw_mat_double_destroy(matrix);
		         mlfw_row_vec_string_destroy(matrix_header);
		         mlfw_mat_double_destroy(scaled_column_matrix);
		         free(scale_column_name);
		         return NULL;
		     }
		     if(mlfw_strcmp_case_insensitive(scale_column_name,column_name)==0)
		     {
			     // scale j(th) column over here
			     if(algorithm_code==MIN_MAX_ALGORITHM)
			     {
				     if(mlfw_scale_double_min_max(matrix,0,j,matrix_rows-1,j,&tmp_matrix,scaled_column_matrix)!=NULL)
				     {
					   mlfw_mat_double_copy(matrix,scaled_column_matrix,0,j,0,0,matrix_rows-1,0);
					   mlfw_mat_double_set(parameters_matrix,0,i,mlfw_mat_double_get(tmp_matrix,0,0));
					   mlfw_mat_double_set(parameters_matrix,1,i,mlfw_mat_double_get(tmp_matrix,1,0));
					   mlfw_mat_double_destroy(tmp_matrix);
				     }
				     else
				     {
		                           mlfw_mat_double_destroy(parameters_matrix);
		                           mlfw_mat_double_destroy(matrix);
		                           mlfw_row_vec_string_destroy(matrix_header);
		                           mlfw_mat_double_destroy(scaled_column_matrix);
		                           free(scale_column_name);
					   free(column_name);
		                           return NULL;
				     }
			     }else if(algorithm_code==Z_SCORE_ALGORITHM)
			     {
				     if(mlfw_scale_double_z_score(matrix,0,j,matrix_rows-1,j,&tmp_matrix,scaled_column_matrix)!=NULL)
				     {
					     mlfw_mat_double_copy(matrix,scaled_column_matrix,0,j,0,0,matrix_rows-1,0);
					     mlfw_mat_double_set(parameters_matrix,0,i,mlfw_mat_double_get(tmp_matrix,0,0));
					     mlfw_mat_double_set(parameters_matrix,1,i,mlfw_mat_double_get(tmp_matrix,1,0));
					     mlfw_mat_double_destroy(tmp_matrix);
				     }
				     else
				     {
		                           mlfw_mat_double_destroy(parameters_matrix);
		                           mlfw_mat_double_destroy(matrix);
		                           mlfw_row_vec_string_destroy(matrix_header);
		                           mlfw_mat_double_destroy(scaled_column_matrix);
		                           free(scale_column_name);
					   free(column_name);
		                           return NULL;
				     }
			     }
			     free(column_name);
			     break;
		     }
		     free(column_name);
		}// inner loop ends
		 free(scale_column_name);
		 if(j==matrix_header_size)// the column name in columns_to_scale is incorrect
		 {
			 // release everything created till now and return
		 }
	}// outer loop ends
	
	// create parameters file
	mlfw_mat_double_to_csv(parameters_matrix,parameters_file_name,columns_to_scale);

        mlfw_mat_double_destroy(scaled_column_matrix);
	mlfw_mat_double_destroy(parameters_matrix);
	mlfw_row_vec_string_destroy(matrix_header);
	
	return matrix;
}

mlfw_mat_double * mlfw_scale_double_with_given_parameters(char *dataset_file_name,char *parameters_file_name,char *algorithm,mlfw_mat_double *matrix)
{
	mlfw_row_vec_string *columns_to_scale;
	mlfw_row_vec_string *matrix_header;
	dimension_t matrix_rows,matrix_columns;

	index_t i,j;
	dimension_t columns_to_scale_size;
	dimension_t matrix_header_size;

	char *scale_column_name;
	char *column_name;

	mlfw_mat_double *scaled_column_matrix;
	mlfw_mat_double *parameters_matrix;
	mlfw_mat_double *tmp_matrix;

	uint8_t algorithm_code;
	uint8_t MIN_MAX_ALGORITHM=1;
	uint8_t Z_SCORE_ALGORITHM=2;

	if(dataset_file_name==NULL || columns_to_scale==NULL || parameters_file_name==NULL)
	{
		return NULL;
	}
	if(algorithm==NULL) return NULL;

	if(mlfw_strcmp_case_insensitive(algorithm,"min-max")==0)
	{
		algorithm_code=MIN_MAX_ALGORITHM;
	}else if(mlfw_strcmp_case_insensitive(algorithm,"z-score")==0)
	{
		algorithm_code=Z_SCORE_ALGORITHM;
	}else
	{
		return NULL;
	}
	parameters_matrix=mlfw_mat_double_from_csv(parameters_file_name,NULL,&columns_to_scale);
	if(parameters_matrix==NULL) return NULL;
	columns_to_scale_size=mlfw_row_vec_string_get_size(columns_to_scale);

	matrix=mlfw_mat_double_from_csv(dataset_file_name,matrix,&matrix_header);
	if(matrix==NULL)
	{
		mlfw_mat_double_destroy(parameters_matrix);
		mlfw_row_vec_string_destroy(columns_to_scale);
		return NULL;
	}
	
	mlfw_mat_double_get_dimensions(matrix,&matrix_rows,&matrix_columns);
	matrix_header_size=mlfw_row_vec_string_get_size(matrix_header);

	scaled_column_matrix=mlfw_mat_double_create_new(matrix_rows,1);
	if(scaled_column_matrix==NULL)
	{
		mlfw_mat_double_destroy(parameters_matrix);
		mlfw_row_vec_string_destroy(columns_to_scale);
		mlfw_mat_double_destroy(matrix);
		mlfw_row_vec_string_destroy(matrix_header);
		return NULL;
	}
	tmp_matrix=mlfw_mat_double_create_new(2,1);
	// iterate the columns_to_scale_vector
	for(i=0;i<columns_to_scale_size;++i)
	{
		// extract column name from columns to scale
		mlfw_row_vec_string_get(columns_to_scale,i,&scale_column_name);
		if(scale_column_name==NULL)
		{
		mlfw_mat_double_destroy(parameters_matrix);
		mlfw_row_vec_string_destroy(columns_to_scale);
		mlfw_mat_double_destroy(matrix);
		mlfw_row_vec_string_destroy(matrix_header);
		mlfw_mat_double_destroy(scaled_column_matrix);
		mlfw_mat_double_destroy(tmp_matrix);
		return NULL;
		}

		// search the scale_column_name in headers vector
		for(j=0;j<matrix_header_size;++j)
		{
                     mlfw_row_vec_string_get(matrix_header,j,&column_name);
		     if(column_name==NULL)
		     {
		         mlfw_mat_double_destroy(parameters_matrix);
			 mlfw_row_vec_string_destroy(columns_to_scale);
		         mlfw_mat_double_destroy(matrix);
		         mlfw_row_vec_string_destroy(matrix_header);
		         mlfw_mat_double_destroy(scaled_column_matrix);
			 mlfw_mat_double_destroy(tmp_matrix);
		         free(scale_column_name);
		         return NULL;
		     }
		     if(mlfw_strcmp_case_insensitive(scale_column_name,column_name)==0)
		     {
			     // scale j(th) column over here
			     if(algorithm_code==MIN_MAX_ALGORITHM)
			     {
				     mlfw_mat_double_set(tmp_matrix,0,0,mlfw_mat_double_get(parameters_matrix,0,i));
				     mlfw_mat_double_set(tmp_matrix,1,0,mlfw_mat_double_get(parameters_matrix,1,i));
				     if(mlfw_scale_double_with_given_min_max(matrix,0,j,matrix_rows-1,j,tmp_matrix,scaled_column_matrix)!=NULL)
				     {
					   mlfw_mat_double_copy(matrix,scaled_column_matrix,0,j,0,0,matrix_rows-1,0);
				     }
				     else
				     {
		                           mlfw_mat_double_destroy(parameters_matrix);
					   mlfw_row_vec_string_destroy(columns_to_scale);
		                           mlfw_mat_double_destroy(matrix);
		                           mlfw_row_vec_string_destroy(matrix_header);
		                           mlfw_mat_double_destroy(scaled_column_matrix);
					   mlfw_mat_double_destroy(tmp_matrix);
		                           free(scale_column_name);
					   free(column_name);
		                           return NULL;
				     }
			     }else if(algorithm_code==Z_SCORE_ALGORITHM)
			     {
				     mlfw_mat_double_set(tmp_matrix,0,0,mlfw_mat_double_get(parameters_matrix,0,i));
				     mlfw_mat_double_set(tmp_matrix,1,0,mlfw_mat_double_get(parameters_matrix,1,i));
				     if(mlfw_scale_double_z_score_with_given_mean_standard_deviation(matrix,0,j,matrix_rows-1,j,tmp_matrix,scaled_column_matrix)!=NULL)
				     {
					     mlfw_mat_double_copy(matrix,scaled_column_matrix,0,j,0,0,matrix_rows-1,0);
				     }
				     else
				     {
		                           mlfw_mat_double_destroy(parameters_matrix);
					   mlfw_row_vec_string_destroy(columns_to_scale);
		                           mlfw_mat_double_destroy(matrix);
		                           mlfw_row_vec_string_destroy(matrix_header);
		                           mlfw_mat_double_destroy(scaled_column_matrix);
					   mlfw_mat_double_destroy(tmp_matrix);
		                           free(scale_column_name);
					   free(column_name);
		                           return NULL;
				     }
			     }
			     free(column_name);
			     break;
		     }
		     free(column_name);
		}// inner loop ends
		 free(scale_column_name);
		 if(j==matrix_header_size)// the column name in columns_to_scale is incorrect
		 {
			 // release everything created till now and return
		 }
	}// outer loop ends
	
	// create parameters file
	mlfw_mat_double_to_csv(parameters_matrix,parameters_file_name,columns_to_scale);

        mlfw_mat_double_destroy(scaled_column_matrix);
	mlfw_mat_double_destroy(parameters_matrix);
	mlfw_row_vec_string_destroy(columns_to_scale);
	mlfw_row_vec_string_destroy(matrix_header);
	mlfw_mat_double_destroy(tmp_matrix);
	
	return matrix;
}

