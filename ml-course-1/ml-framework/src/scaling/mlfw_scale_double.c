#include<mlfw_matrix.h>
#include<mlfw_scale.h>
#include<stdlib.h>
#include<stdio.h>

mlfw_mat_double * mlfw_scale_double_min_max(mlfw_mat_double *matrix,index_t start_row_index,index_t start_column_index,index_t end_row_index,index_t end_column_index,char *min_max_file)
{
	double scaled_value;
	double value;
	FILE *file;
	index_t r,c;
	index_t new_matrix_r,new_matrix_c;
	index_t i;
	dimension_t matrix_rows,matrix_columns;
	dimension_t new_matrix_rows,new_matrix_columns;
	mlfw_mat_double *new_matrix;
        double *min;
	double *max;

	if(matrix==NULL) return NULL;
	mlfw_mat_double_get_dimensions(matrix,&matrix_rows,&matrix_columns);
	if(start_row_index<0 || end_row_index>=matrix_rows) return NULL;
	if(start_column_index<0 || end_column_index>=matrix_columns) return NULL;
	if(start_row_index>end_row_index || start_column_index>end_column_index) return NULL;
	new_matrix_rows=end_row_index-start_row_index+1;
	new_matrix_columns=end_column_index-start_column_index+1;
	new_matrix=mlfw_mat_double_create_new(new_matrix_rows,new_matrix_columns);
	if(new_matrix==NULL) return NULL;
	max=(double *)malloc(sizeof(double)*new_matrix_columns);
	if(max==NULL) return NULL;
	min=(double *)malloc(sizeof(double)*new_matrix_columns);
	if(min==NULL)
        {
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

	file=fopen(min_max_file,"w");
	if(file==NULL)
	{
		free(min);
		free(max);
		mlfw_mat_double_destroy(new_matrix);
		return NULL;
	}
	for(i=0;i<new_matrix_columns;++i)
	{
		fprintf(file,"%lf",min[i]);
		if(i==new_matrix_columns-1) fputc('\n',file);
		else fputc(',',file);
	}
	for(i=0;i<new_matrix_columns;++i)
	{
		fprintf(file,"%lf",max[i]);
		if(i==new_matrix_columns-1) fputc('\n',file);
		else fputc(',',file);
	}
	fclose(file);
	free(min);
	free(max);

	return new_matrix;
}


mlfw_mat_double * mlfw_scale_double_with_given_min_max(mlfw_mat_double *matrix,index_t start_row_index,index_t start_column_index,index_t end_row_index,index_t end_column_index,mlfw_mat_double *min_max_matrix)
{
	double scaled_value;
	double value;
	double min;
	double max;
	index_t r,c;
	index_t new_matrix_r,new_matrix_c;
	dimension_t matrix_rows,matrix_columns;
	dimension_t min_max_rows,min_max_columns;
	dimension_t new_matrix_rows,new_matrix_columns;
	mlfw_mat_double *new_matrix;
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
	
	new_matrix=mlfw_mat_double_create_new(new_matrix_rows,new_matrix_columns);
	if(new_matrix==NULL) return NULL;

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

