#include<stdio.h>
#include<stdlib.h>
#include<mlfw_vector.h>

typedef struct __mlfw_column_vec_double
{
	double *data;
	dimension_t size;
}mlfw_column_vec_double;
typedef struct __mlfw_row_vec_double
{
	double *data;
	dimension_t size;
}mlfw_row_vec_double;

// column vector function implementation
mlfw_column_vec_double * mlfw_column_vec_double_create_new(dimension_t size)
{
	mlfw_column_vec_double *vector;
	if(size<=0) return NULL;
	vector=(mlfw_column_vec_double *)malloc(sizeof(mlfw_column_vec_double));
	if(vector==NULL) return NULL;
	vector->data=(double *)malloc(sizeof(double)*size);
	if(vector->data==NULL)
	{
		free(vector);
		return NULL;
	}
        vector->size=size;
	return vector;
}
void mlfw_column_vec_double_destroy(mlfw_column_vec_double *vector)
{
	if(vector==NULL) return;
	free(vector->data);
	free(vector);
}
double mlfw_column_vec_double_get(mlfw_column_vec_double *vector,index_t index)
{
	if(vector==NULL) return 0.0;
	if(index<0 || index>=vector->size) return 0.0;
	return vector->data[index];
}
void mlfw_column_vec_double_set(mlfw_column_vec_double *vector,index_t index,double value)
{
	if(vector==NULL) return;
	if(index<0 || index>=vector->size) return;
	vector->data[index]=value;
}
mlfw_column_vec_double * mlfw_column_vec_double_create_new_filled(dimension_t size,double value)
{
	index_t i;
	mlfw_column_vec_double *vector;
	if(size<=0) return NULL;
	vector=mlfw_column_vec_double_create_new(size);
	if(vector==NULL) return NULL;
	for(i=0;i<vector->size;i++) vector->data[i]=value;
	return vector;
}
mlfw_row_vec_double * mlfw_column_vec_double_transpose(mlfw_column_vec_double *vector)
{
	index_t i;
	mlfw_row_vec_double *transposed_vector;
	if(vector==NULL) return NULL;
	transposed_vector=mlfw_row_vec_double_create_new(vector->size);
	if(transposed_vector==NULL) return NULL;
	for(i=0;i<transposed_vector->size;i++)
	{
		transposed_vector->data[i]=vector->data[i];
	}
	return transposed_vector;
}
dimension_t mlfw_column_vec_double_get_size(mlfw_column_vec_double *vector)
{
	if(vector==NULL) return 0;
	return vector->size;
}
double mlfw_column_vec_double_get_mean(mlfw_column_vec_double *vector)
{
	double mean,sum;
	index_t i;
	if(vector==NULL) return 0;
	for(i=0,sum=0.0;i<vector->size;++i) sum+=vector->data[i];
	mean=sum/(double)vector->size;
	return mean;
}
void mlfw_column_vec_double_to_csv(mlfw_column_vec_double *vector,char *csv_file_name)
{
	FILE *file;
	index_t i;
	if(vector==NULL || csv_file_name==NULL) return;
	file=fopen(csv_file_name,"w");
	if(file==NULL) return;
	for(i=0;i<vector->size;++i)
	{
		fprintf(file,"%lf",vector->data[i]);
		if(i==vector->size-1) fputc('\n',file);
		else fputc(',',file);
	}
	fclose(file);
}
mlfw_column_vec_double * mlfw_column_vec_double_from_csv(char *csv_file_name)
{
	mlfw_column_vec_double *vector;
	FILE *file;
	dimension_t size;
	char m;
	char double_string[1025];
	index_t i,j;
	double value;
	char *end;
	if(csv_file_name==NULL) return NULL;
	file=fopen(csv_file_name,"r");
	if(file==NULL) return NULL;
	size=0;
	while(1)
	{
		m=fgetc(file);
		if(feof(file)) break;
		if(m==',') size++;
	}
	size++;
	vector=mlfw_column_vec_double_create_new(size);
	if(vector==NULL)
	{
		fclose(file);
		return NULL;
	}
	rewind(file);
	i=0;
	j=0;
	while(1)
	{
		m=fgetc(file);
		if(feof(file)) break;
		if(m==',' || m=='\n')
		{
			double_string[i]='\0';
			end=NULL;
			value=strtod(double_string,&end);
			mlfw_column_vec_double_set(vector,j,value);
			i=0;
			j++;
		}
		else
		{
			double_string[i]=m;
			i++;
		}
	}
	fclose(file);
	return vector;
}


// row vector function implementations
mlfw_row_vec_double * mlfw_row_vec_double_create_new(dimension_t size)
{
	mlfw_row_vec_double *vector;
	if(size<=0) return NULL;
	vector=(mlfw_row_vec_double *)malloc(sizeof(mlfw_row_vec_double));
	if(vector==NULL) return NULL;
	vector->data=(double *)malloc(sizeof(double)*size);
	if(vector->data==NULL)
	{
		free(vector);
		return NULL;
	}
        vector->size=size;
	return vector;
}
void mlfw_row_vec_double_destroy(mlfw_row_vec_double *vector)
{
	if(vector==NULL) return;
	free(vector->data);
	free(vector);
}
double mlfw_row_vec_double_get(mlfw_row_vec_double *vector,index_t index)
{
	if(vector==NULL) return 0.0;
	if(index<0 || index>=vector->size) return 0.0;
	return vector->data[index];
}
void mlfw_row_vec_double_set(mlfw_row_vec_double *vector,index_t index,double value)
{
	if(vector==NULL) return;
	if(index<0 || index>=vector->size) return;
	vector->data[index]=value;
}
mlfw_row_vec_double * mlfw_row_vec_double_create_new_filled(dimension_t size,double value)
{
	index_t i;
	mlfw_row_vec_double *vector;
	if(size<=0) return NULL;
	vector=mlfw_row_vec_double_create_new(size);
	if(vector==NULL) return NULL;
	for(i=0;i<vector->size;i++) vector->data[i]=value;
	return vector;
}
mlfw_column_vec_double * mlfw_row_vec_double_transpose(mlfw_row_vec_double *vector)
{
	index_t i;
	mlfw_column_vec_double *transposed_vector;
	if(vector==NULL) return NULL;
	transposed_vector=mlfw_column_vec_double_create_new(vector->size);
	if(transposed_vector==NULL) return NULL;
	for(i=0;i<transposed_vector->size;i++)
	{
		transposed_vector->data[i]=vector->data[i];
	}
	return transposed_vector;
}
dimension_t mlfw_row_vec_double_get_size(mlfw_row_vec_double *vector)
{
	if(vector==NULL) return 0;
	return vector->size;
}
double mlfw_row_vec_double_get_mean(mlfw_row_vec_double *vector)
{
	double mean,sum;
	index_t i;
	if(vector==NULL) return 0;
	for(i=0,sum=0.0;i<vector->size;++i) sum+=vector->data[i];
	mean=sum/(double)vector->size;
	return mean;
}


void mlfw_row_vec_double_to_csv(mlfw_row_vec_double *vector,char *csv_file_name)
{
	FILE *file;
	index_t i;
	if(vector==NULL || csv_file_name==NULL) return;
	file=fopen(csv_file_name,"w");
	if(file==NULL) return;
	for(i=0;i<vector->size;++i)
	{
		fprintf(file,"%lf",vector->data[i]);
		if(i==vector->size-1) fputc('\n',file);
		else fputc(',',file);
	}
	fclose(file);
}
mlfw_row_vec_double * mlfw_row_vec_double_from_csv(char *csv_file_name)
{
	mlfw_row_vec_double *vector;
	FILE *file;
	dimension_t size;
	char m;
	char double_string[1025];
	index_t i,j;
	double value;
	char *end;
	if(csv_file_name==NULL) return NULL;
	file=fopen(csv_file_name,"r");
	if(file==NULL) return NULL;
	size=0;
	while(1)
	{
		m=fgetc(file);
		if(feof(file)) break;
		if(m==',') size++;
	}
	size++;
	vector=mlfw_row_vec_double_create_new(size);
	if(vector==NULL)
	{
		fclose(file);
		return NULL;
	}
	rewind(file);
	i=0;
	j=0;
	while(1)
	{
		m=fgetc(file);
		if(feof(file)) break;
		if(m==',' || m=='\n')
		{
			double_string[i]='\0';
			end=NULL;
			value=strtod(double_string,&end);
			mlfw_row_vec_double_set(vector,j,value);
			i=0;
			j++;
		}
		else
		{
			double_string[i]=m;
			i++;
		}
	}
	fclose(file);
	return vector;
}



