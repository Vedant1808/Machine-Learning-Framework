#include<stdlib.h>
#include<mlfw_vector.h>
#include<string.h>
#include<stdio.h>

typedef struct __mlfw_row_vec_string
{
char **data;
dimension_t size;
}mlfw_row_vec_string;

typedef struct __mlfw_column_vec_string
{
char **data;
dimension_t size;
}mlfw_column_vec_string;

// row vector functions
mlfw_row_vec_string * mlfw_row_vec_string_create_new(dimension_t columns)
{
mlfw_row_vec_string *vector;
index_t c;
if(columns<=0) return NULL;
vector=(mlfw_row_vec_string *)malloc(sizeof(mlfw_row_vec_string));
if(vector==NULL) return NULL;
vector->data=(char **)malloc(sizeof(char *)*columns);
if(vector->data==NULL)
{
free(vector);
return NULL;
}
for(c=0;c<columns;++c) vector->data[c]=NULL;
vector->size=columns;
return vector;
}
void mlfw_row_vec_string_destroy(mlfw_row_vec_string *vector)
{
index_t c;
if(vector==NULL) return;
for(c=0;c<vector->size;++c)
{
if(vector->data[c]!=NULL) free(vector->data[c]);
}
free(vector->data);
free(vector);
}
mlfw_row_vec_string * mlfw_row_vec_string_from_csv(const char *csv_file_name)
{
    mlfw_row_vec_string *vector;
	int index;
	char m;
	index_t c;
	char string[50001]; //1 extra for \0 (string terminator)
	dimension_t columns;
     FILE *file;
     if(csv_file_name==NULL) return NULL;
	file=fopen(csv_file_name,"r");
	if(file==NULL) return NULL;
	columns=0;
	while(1)
	{
		m=fgetc(file);
		if(feof(file)) break;
		if(m==',' || m=='\n')columns++;
	}
    vector=mlfw_row_vec_string_create_new(columns);
	if(vector==NULL)
	{
		fclose(file);
		return NULL;
	}
	rewind(file); // move the internal pointer to the first byte
        // logic to populate matrix starts
	c=0;
	index=0;
	while(1)
	{
		m=fgetc(file);
		if(feof(file)) break;
		if(m==',' || m=='\n')
	        {
			string[index]='\0';
			vector->data[c]=(char *)malloc(sizeof(char)*(strlen(string)+1));
			if(vector->data[c]!=NULL)
		        {
				strcpy(vector->data[c],string);
			}
			index=0;
			c++;
		}
		else
		{
			string[index]=m;
			index++;
		}
	}
	fclose(file);
	return vector;
}
void mlfw_row_vec_string_to_csv(mlfw_row_vec_string *vector,const char *csv_file_name)
{
FILE *file;
index_t c;
if(vector==NULL || csv_file_name==NULL) return;
file=fopen(csv_file_name,"w");
if(file==NULL) return;
for(c=0;c<vector->size;++c)
{
if(vector->data[c]!=NULL) fputs(vector->data[c],file);
if(c<vector->size-1) fputc(',',file);
else fputc('\n',file);
}
fclose(file);
}
void mlfw_row_vec_string_get(mlfw_row_vec_string *vector,index_t index,char **string)
{
if(string==NULL) return;
if(vector==NULL)
{
*string=NULL;
return;
}
if(index<0 || index>=vector->size)
{
*string=NULL;
return;
}
if(vector->data[index]==NULL)
{
*string=NULL;
return;
}
*string=(char *)malloc(sizeof(char)*(strlen(vector->data[index])+1));
if(*string==NULL) return;
strcpy(*string,vector->data[index]);
}
void mlfw_row_vec_string_set(mlfw_row_vec_string *vector,index_t index,char *string)
{
if(vector==NULL || string==NULL) return;
if(index<0 || index>=vector->size) return;
if(vector->data[index]!=NULL) free(vector->data[index]);
vector->data[index]=(char *)malloc(sizeof(char)*(strlen(string)+1));
if(vector->data[index]!=NULL)
{
strcpy(vector->data[index],string);
}
}
dimension_t  mlfw_row_vec_string_get_size(mlfw_row_vec_string *vector)
{
if(vector==NULL) return 0;
return vector->size;
}
mlfw_column_vec_string * mlfw_row_vec_string_transpose(mlfw_row_vec_string *vector)
{
index_t c;
char *ptr;
mlfw_column_vec_string *transposed_vector;
if(vector==NULL) return NULL;
transposed_vector=mlfw_column_vec_string_create_new(vector->size);
if(transposed_vector==NULL) return NULL;
for(c=0;c<vector->size;++c)
{
mlfw_row_vec_string_get(vector,c,&ptr);
mlfw_column_vec_string_set(transposed_vector,c,ptr);
}
return transposed_vector;
}

// column vector functions
mlfw_column_vec_string * mlfw_column_vec_string_create_new(dimension_t rows)
{
mlfw_column_vec_string *vector;
index_t r;
if(rows<=0) return NULL;
vector=(mlfw_column_vec_string *)malloc(sizeof(mlfw_column_vec_string));
if(vector==NULL) return NULL;
vector->data=(char **)malloc(sizeof(char *)*rows);
if(vector->data==NULL)
{
free(vector);
return NULL;
}
for(r=0;r<rows;++r) vector->data[r]=NULL;
vector->size=rows;
return vector;
}
void mlfw_column_vec_string_destroy(mlfw_column_vec_string *vector)
{
index_t r;
if(vector==NULL) return;
for(r=0;r<vector->size;++r)
{
if(vector->data[r]!=NULL) free(vector->data[r]);
}
free(vector->data);
free(vector);
}
mlfw_column_vec_string * mlfw_column_vec_string_from_csv(const char *csv_file_name)
{
    mlfw_column_vec_string *vector;
	int index;
	char m;
	index_t r;
	char string[50001]; //1 extra for \0 (string terminator)
	dimension_t rows;
     FILE *file;
     if(csv_file_name==NULL) return NULL;
	file=fopen(csv_file_name,"r");
	if(file==NULL) return NULL;
	rows=0;
	while(1)
	{
		m=fgetc(file);
		if(feof(file)) break;
		if(m==',' || m=='\n') rows++;
	}
    vector=mlfw_column_vec_string_create_new(rows);
	if(vector==NULL)
	{
		fclose(file);
		return NULL;
	}
	rewind(file); // move the internal pointer to the first byte
        // logic to populate matrix starts
	r=0;
	index=0;
	while(1)
	{
		m=fgetc(file);
		if(feof(file)) break;
		if(m==',' || m=='\n')
	        {
			string[index]='\0';
			vector->data[r]=(char *)malloc(sizeof(char)*(strlen(string)+1));
			if(vector->data[r]!=NULL)
		        {
				strcpy(vector->data[r],string);
			}
			index=0;
			r++;
		}
		else
		{
			string[index]=m;
			index++;
		}
	}
	fclose(file);
	return vector;
}
void mlfw_column_vec_string_to_csv(mlfw_column_vec_string *vector,const char *csv_file_name)
{
FILE *file;
index_t r;
if(vector==NULL || csv_file_name==NULL) return;
file=fopen(csv_file_name,"w");
if(file==NULL) return;
for(r=0;r<vector->size;++r)
{
if(vector->data[r]!=NULL) fputs(vector->data[r],file);
if(r<vector->size-1) fputc(',',file);
else fputc('\n',file);
}
fclose(file);
}
void mlfw_column_vec_string_get(mlfw_column_vec_string *vector,index_t index,char **string)
{
if(string==NULL) return;
if(vector==NULL)
{
*string=NULL;
return;
}
if(index<0 || index>=vector->size)
{
*string=NULL;
return;
}
if(vector->data[index]==NULL)
{
*string=NULL;
return;
}
*string=(char *)malloc(sizeof(char)*(strlen(vector->data[index])+1));
if(*string==NULL) return;
strcpy(*string,vector->data[index]);
}
void mlfw_column_vec_string_set(mlfw_column_vec_string *vector,index_t index,char *string)
{
if(vector==NULL || string==NULL) return;
if(index<0 || index>=vector->size) return;
if(vector->data[index]!=NULL) free(vector->data[index]);
vector->data[index]=(char *)malloc(sizeof(char)*(strlen(string)+1));
if(vector->data[index]!=NULL)
{
strcpy(vector->data[index],string);
}
}
dimension_t  mlfw_column_vec_string_get_size(mlfw_column_vec_string *vector)
{
if(vector==NULL) return 0;
return vector->size;
}
mlfw_row_vec_string * mlfw_column_vec_string_transpose(mlfw_column_vec_string *vector)
{
index_t r;
char *ptr;
mlfw_row_vec_string *transposed_vector;
if(vector==NULL) return NULL;
transposed_vector=mlfw_row_vec_string_create_new(vector->size);
if(transposed_vector==NULL) return NULL;
for(r=0;r<vector->size;++r)
{
mlfw_column_vec_string_get(vector,r,&ptr);
mlfw_row_vec_string_set(transposed_vector,r,ptr);
}
return transposed_vector;
}
