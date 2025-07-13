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
mlfw_row_vec_string * mlfw_row_vec_string_from_csv(const char *csv_file_name,mlfw_row_vec_string *vector,mlfw_row_vec_string **header)
{
	char header_string[1025];
	index_t header_index;
	dimension_t columns;
	int index;
	char m;
	index_t c;
	char string[50001]; //1 extra for \0 (string terminator)
     FILE *file;
     if(csv_file_name==NULL|| header==NULL) return NULL;
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

	columns=0;
	while(1)
	{
		m=fgetc(file);
		if(feof(file)) break;
		if(m=='\r') continue;
		if(m==',' || m=='\n')columns++;
	}
	if(columns!=mlfw_row_vec_string_get_size(*header))
	{
		fclose(file);
		mlfw_row_vec_string_destroy(*header);
		*header=NULL;
		return NULL;
	}
	if(vector==NULL)
	{
    vector=mlfw_row_vec_string_create_new(columns);
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
                 if(vector->size!=columns)
		 {
			 fclose(file);
			 mlfw_row_vec_string_destroy(*header);
			 *header=NULL;
			 return NULL;
		 }
	}
	rewind(file); // move the internal pointer to the first byte
        // skip the first line
	while(1)
	{
		m=fgetc(file);
		if(feof(file)) break;
		if(m=='\r') continue;
		if(m=='\n') break;
	}
        // logic to populate matrix starts
	c=0;
	index=0;
	while(1)
	{
		m=fgetc(file);
		if(feof(file)) break;
		if(m=='\r') continue;
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
void mlfw_row_vec_string_to_csv(mlfw_row_vec_string *vector,const char *csv_file_name,mlfw_row_vec_string *header)
{
	index_t index;
	dimension_t header_size;
	char *ptr;
FILE *file;
index_t c;
if(vector==NULL || csv_file_name==NULL || header==NULL) return;
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
mlfw_column_vec_string * mlfw_row_vec_string_transpose(mlfw_row_vec_string *vector,mlfw_column_vec_string *transposed_vector)
{
index_t c;
char *ptr;
if(vector==NULL) return NULL;
if(transposed_vector==NULL)
{
transposed_vector=mlfw_column_vec_string_create_new(vector->size);
if(transposed_vector==NULL) return NULL;
}
else
{
	if(transposed_vector->size!=vector->size) return NULL;
}
for(c=0;c<vector->size;++c)
{
mlfw_row_vec_string_get(vector,c,&ptr);
mlfw_column_vec_string_set(transposed_vector,c,ptr);
free(ptr);
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
mlfw_column_vec_string * mlfw_column_vec_string_from_csv(const char *csv_file_name,mlfw_column_vec_string *vector,mlfw_row_vec_string **header)
{
	char header_string[1025];
	index_t header_index;
	dimension_t columns;
	int index;
	char m;
	index_t r;
	char string[50001]; //1 extra for \0 (string terminator)
	dimension_t rows;
     FILE *file;
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

	rows=0;
	while(1)
	{
		m=fgetc(file);
		if(feof(file)) break;
		if(m=='\r') continue;
		if(m==',' || m=='\n') rows++;
	}
	if(vector==NULL)
	{
    vector=mlfw_column_vec_string_create_new(rows);
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
		if(vector->size!=rows)
		{
			mlfw_row_vec_string_destroy(*header);
			fclose(file);
			*header=NULL;
			return NULL;
		}
	}
	rewind(file); // move the internal pointer to the first byte
	// skip the first line
	while(1)
	{
		m=fgetc(file);
		if(feof(file)) break;
		if(m=='\r') continue;
		if(m=='\n') break;
	}
        // logic to populate matrix starts
	r=0;
	index=0;
	while(1)
	{
		m=fgetc(file);
		if(feof(file)) break;
		if(m=='\r') continue;
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
void mlfw_column_vec_string_to_csv(mlfw_column_vec_string *vector,const char *csv_file_name,mlfw_row_vec_string *header)
{
	index_t index;
	dimension_t header_size;
	char *ptr;
FILE *file;
index_t r;
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
mlfw_row_vec_string * mlfw_column_vec_string_transpose(mlfw_column_vec_string *vector,mlfw_row_vec_string *transposed_vector)
{
index_t r;
char *ptr;
if(vector==NULL) return NULL;
if(transposed_vector==NULL)
{
transposed_vector=mlfw_row_vec_string_create_new(vector->size);
if(transposed_vector==NULL) return NULL;
}
else
{
	if(transposed_vector->size!=vector->size) return NULL;
}
for(r=0;r<vector->size;++r)
{
mlfw_column_vec_string_get(vector,r,&ptr);
mlfw_row_vec_string_set(transposed_vector,r,ptr);
free(ptr);
}
return transposed_vector;
}
