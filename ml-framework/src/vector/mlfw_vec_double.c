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
mlfw_column_vec_double * mlfw_column_vec_double_create_new_filled(dimension_t size,double value,mlfw_column_vec_double *vector)
{
	index_t i;
	if(size<=0) return NULL;
	if(vector==NULL)
	{
	vector=mlfw_column_vec_double_create_new(size);
	if(vector==NULL) return NULL;
	}
	else
	{
		if(vector->size!=size) return NULL;
	}
	for(i=0;i<vector->size;i++) vector->data[i]=value;
	return vector;
}
mlfw_row_vec_double * mlfw_column_vec_double_transpose(mlfw_column_vec_double *vector,mlfw_row_vec_double *transposed_vector)
{
	index_t i;
	if(vector==NULL) return NULL;
	if(transposed_vector==NULL)
	{
	transposed_vector=mlfw_row_vec_double_create_new(vector->size);
	if(transposed_vector==NULL) return NULL;
	}
	else
	{
		if(transposed_vector->size!=vector->size) return NULL;
	}
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
void mlfw_column_vec_double_to_csv(mlfw_column_vec_double *vector,char *csv_file_name,mlfw_row_vec_string *header)
{
	index_t index;
	dimension_t header_size;
	char *ptr;
	FILE *file;
	index_t i;
	if(vector==NULL || csv_file_name==NULL || header==NULL) return;
	header_size=mlfw_row_vec_string_get_size(header);
	if(header_size!=1) return;
	file=fopen(csv_file_name,"w");
	if(file==NULL) return;
       // code to write header
		for(index=0;index<header_size;++index)
		{
			mlfw_row_vec_string_get(header,index,&ptr);
			if(ptr!=NULL)
			{
				fputs(ptr,file);
				free(ptr);
			}
			if(index<header_size-1) fputc(',',file);
			else fputc('\n',file);
		}
	// code to write data

	for(i=0;i<vector->size;++i)
	{
		fprintf(file,"%lf",vector->data[i]);
		if(i==vector->size-1) fputc('\n',file);
		else fputc(',',file);
	}
	fclose(file);
}
mlfw_column_vec_double * mlfw_column_vec_double_from_csv(char *csv_file_name,mlfw_column_vec_double *vector,mlfw_row_vec_string **header)
{
	char header_string[1025];
	index_t header_index;
	dimension_t columns;
	index_t index;
	FILE *file;
	dimension_t size;
	char m;
	char double_string[1025];
	index_t i,j;
	double value;
	char *end;
	if(csv_file_name==NULL || header==NULL) return NULL;
	file=fopen(csv_file_name,"r");
	if(file==NULL) return NULL;
        // logic to read the first line starts here
	columns=0;
	while(1)
	{
		m=fgetc(file);
		if(feof(file)) break;
		if(m=='\r') continue;
		if(m==',') columns++;
		if(m=='\n') break;
	}
	columns++; /// if 0 commas , then 1 columns, if 3 commas then 4 columns
        if(columns!=1)
	{
		fclose(file);
		*header=NULL;
		return NULL;
	}
	*header=mlfw_row_vec_string_create_new(columns);
	if(*header==NULL) return NULL;
	rewind(file);
	index=0;
	header_index=0;
	while(1)
	{
		m=fgetc(file);
		if(feof(file)) break;
		if(m=='\r') continue;
		if(m==',')
		{
			header_string[index]='\0';
			mlfw_row_vec_string_set(*header,header_index,header_string);
			header_index++;
			index=0;
			continue;
		}
		if(m=='\n')
		{
			header_string[index]='\0';
			mlfw_row_vec_string_set(*header,header_index,header_string);
			break;
		}
		header_string[index]=m;
		index++;
	}

	// logic to read the first line ends here

	size=0;
	while(1)
	{
		m=fgetc(file);
		if(feof(file)) break;
		if(m=='\r') continue;
		if(m==',') size++;
	}
	size++;
	if(vector==NULL)
	{
	vector=mlfw_column_vec_double_create_new(size);
	if(vector==NULL)
	{
		fclose(file);
	        mlfw_row_vec_string_destroy(*header);
		*header=NULL;
		return NULL;
	}
	}
	else
	{
		if(vector->size!=size)
		{
			mlfw_row_vec_string_destroy(*header);
			fclose(file);
			*header=NULL;
			return NULL;
		}
	}
	rewind(file);
	// skip the first line
	while(1)
	{
		m=fgetc(file);
		if(feof(file)) break;
		if(m=='\r') continue;
		if(m=='\n') break;
	}
	i=0;
	j=0;
	while(1)
	{
		m=fgetc(file);
		if(feof(file)) break;
		if(m=='\r') continue;
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
mlfw_row_vec_double * mlfw_row_vec_double_create_new_filled(dimension_t size,double value,mlfw_row_vec_double *vector)
{
	index_t i;
	if(size<=0) return NULL;
	if(vector==NULL)
	{
	vector=mlfw_row_vec_double_create_new(size);
	if(vector==NULL) return NULL;
	}
	else
	{
		if(vector->size!=size) return NULL;
	}
	for(i=0;i<vector->size;i++) vector->data[i]=value;
	return vector;
}
mlfw_column_vec_double * mlfw_row_vec_double_transpose(mlfw_row_vec_double *vector,mlfw_column_vec_double *transposed_vector)
{
	index_t i;
	if(vector==NULL) return NULL;
	if(transposed_vector==NULL)
	{
	transposed_vector=mlfw_column_vec_double_create_new(vector->size);
	if(transposed_vector==NULL) return NULL;
	}
	else
	{
		if(transposed_vector->size!=vector->size) return NULL;
	}
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


void mlfw_row_vec_double_to_csv(mlfw_row_vec_double *vector,char *csv_file_name,mlfw_row_vec_string *header)
{
	index_t index;
	dimension_t header_size;
	char *ptr;
	FILE *file;
	index_t i;
	if(vector==NULL || csv_file_name==NULL || header==NULL)  return;
	header_size=mlfw_row_vec_string_get_size(header);
	if(header_size!=vector->size) return;
        file=fopen(csv_file_name,"w");
	if(file==NULL) return;
	// code to write header
		for(index=0;index<header_size;++index)
		{
			mlfw_row_vec_string_get(header,index,&ptr);
			if(ptr!=NULL)
			{
				fputs(ptr,file);
				free(ptr);
			}
			if(index<header_size-1) fputc(',',file);
			else fputc('\n',file);
		}
	// code to write data
	for(i=0;i<vector->size;++i)
	{
		fprintf(file,"%lf",vector->data[i]);
		if(i==vector->size-1) fputc('\n',file);
		else fputc(',',file);
	}
	fclose(file);
}
mlfw_row_vec_double * mlfw_row_vec_double_from_csv(char *csv_file_name,mlfw_row_vec_double *vector,mlfw_row_vec_string **header)
{
	char header_string[1025];
	index_t header_index;
	dimension_t columns;
	index_t index;
	FILE *file;
	dimension_t size;
	char m;
	char double_string[1025];
	index_t i,j;
	double value;
	char *end;
	if(csv_file_name==NULL || header==NULL) return NULL;
	file=fopen(csv_file_name,"r");
	if(file==NULL) return NULL;
        // logic to read the first line starts here
	columns=0;
	while(1)
	{
		m=fgetc(file);
		if(feof(file)) break;
		if(m=='\r') continue;
		if(m==',') columns++;
		if(m=='\n') break;
	}
	columns++; /// if 0 commas , then 1 columns, if 3 commas then 4 columns
	*header=mlfw_row_vec_string_create_new(columns);
	if(*header==NULL) return NULL;
	rewind(file);
	index=0;
	header_index=0;
	while(1)
	{
		m=fgetc(file);
		if(feof(file)) break;
		if(m=='\r') continue;
		if(m==',')
		{
			header_string[index]='\0';
			mlfw_row_vec_string_set(*header,header_index,header_string);
			header_index++;
			index=0;
			continue;
		}
		if(m=='\n')
		{
			header_string[index]='\0';
			mlfw_row_vec_string_set(*header,header_index,header_string);
			break;
		}
		header_string[index]=m;
		index++;
	}
	// logic to read the first line ends here

	size=0;
	while(1)
	{
		m=fgetc(file);
		if(feof(file)) break;
		if(m=='\r') continue;
		if(m==',') size++;
	}
	size++;
        if(size!=mlfw_row_vec_string_get_size(*header))
	{
		mlfw_row_vec_string_destroy(*header);
		*header=NULL;
		fclose(file);
		return NULL;
	}

	if(vector==NULL)
	{
	vector=mlfw_row_vec_double_create_new(size);
	if(vector==NULL)
	{
		fclose(file);
		mlfw_row_vec_string_destroy(*header);
		*header=NULL;
		return NULL;
	}
	}
	else
	{
		if(vector->size!=size)
		{
		      mlfw_row_vec_string_destroy(*header);
		      fclose(file);
		      *header=NULL;
		      return NULL;
		}
	}
	rewind(file);
	// skip the file line
	while(1)
	{
		m=fgetc(file);
		if(feof(file)) break;
		if(m=='\r') continue;
		if(m=='\n') break;
	}
	i=0;
	j=0;
	while(1)
	{
		m=fgetc(file);
		if(feof(file)) break;
		if(m=='\r') continue;
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



