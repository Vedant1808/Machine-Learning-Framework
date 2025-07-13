#include<mlfw_matrix.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<time.h>
typedef struct __mlfw_mat_string
{
	char ***data;
	dimension_t rows;
	dimension_t columns;
}mlfw_mat_string;


mlfw_mat_string * mlfw_mat_string_create_new(dimension_t rows,dimension_t columns)
{
	mlfw_mat_string *matrix;
	index_t r,k,c;
	if(rows<=0 || columns<=0) return NULL;
	matrix=(mlfw_mat_string *)malloc(sizeof(mlfw_mat_string));
	if(matrix==NULL) return NULL;
        matrix->data=(char ***)malloc(sizeof(char **)*rows);
	if(matrix->data==NULL)
	{
		free(matrix);
		return NULL;
	}
	for(r=0;r<rows;r++)
	{
		matrix->data[r]=(char **)malloc(sizeof(char *)*columns);
		if(matrix->data[r]==NULL)
		{
			for(k=0;k<r;k++)
			{
				free(matrix->data[k]);
			}
			free(matrix->data);
			free(matrix);
			return NULL;
		}
		else
		{
			for(c=0;c<columns;++c) matrix->data[r][c]=NULL;  // very very imp
		}
	}
	matrix->rows=rows;
	matrix->columns=columns;
	return matrix;
}
void mlfw_mat_string_destroy(mlfw_mat_string *matrix)
{
	index_t r,c;
	if(matrix==NULL) return;
        for(r=0;r<matrix->rows;r++)
	{
		for(c=0;c<matrix->columns;++c)
		{
			if(matrix->data[r][c]!=NULL) free(matrix->data[r][c]);
		}
		free(matrix->data[r]);
	}
	free(matrix->data);
	free(matrix);
}
//function to load data into matrix from csv
mlfw_mat_string * mlfw_mat_string_from_csv(const char *csv_file_name,mlfw_mat_string *matrix,mlfw_row_vec_string **header)
{
	char header_string[1025];
	index_t header_index;
	int index;
	char m;
	index_t r,c;
	char string[50001]; //1 extra for \0 (string terminator)
	dimension_t rows,columns;
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
	columns=0;
	while(1)
	{
		m=fgetc(file);
		if(feof(file)) break;
		if(m=='\r') continue;
		if(rows==0)
		{
			if(m==',') columns++;
		}
		if(m=='\n') rows++;
	}
	columns++; //if 7 commas in a line , that means 8 columns;
        if(columns!=mlfw_row_vec_string_get_size(*header))
	{
		mlfw_row_vec_string_destroy(*header);
		fclose(file);
		*header=NULL;
		return NULL;
	}
        if(matrix==NULL)
	{	
	matrix=mlfw_mat_string_create_new(rows,columns);
	if(matrix==NULL)
	{
		fclose(file);
		mlfw_row_vec_string_destroy(*header);
		*header=NULL;
		return NULL;
	}
	}
	else
	{
		if(matrix->rows!=rows || matrix->columns!=columns)
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
		if(m=='\r') continue; // not necessary
                if(m=='\n') break;
	}
	// logic to populate matrix starts
	r=0;
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
			matrix->data[r][c]=(char *)malloc(sizeof(char)*(strlen(string)+1));
			if(matrix->data[r][c]!=NULL)
		        {
				strcpy(matrix->data[r][c],string);
			}
			index=0;
			c++;
			if(c==matrix->columns)
			{
				r++;
				c=0;
			}
		}
		else
		{
			string[index]=m;
			index++;
		}	
	}
	fclose(file);
	return matrix;
}

void mlfw_mat_string_to_csv(mlfw_mat_string *matrix,const char *csv_file_name,mlfw_row_vec_string *header)
{
FILE *file;
index_t index;
dimension_t header_size;
char *ptr;
index_t r,c;
if(matrix==NULL) return;
if(csv_file_name==NULL || header==NULL) return;
header_size=mlfw_row_vec_string_get_size(header);
if(header_size!=matrix->columns) return;
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

for(r=0;r<matrix->rows;++r)
{
for(c=0;c<matrix->columns;++c)
{
if(matrix->data[r][c]!=NULL) fputs(matrix->data[r][c],file);
if(c<matrix->columns-1) fputc(',',file);
else fputc('\n',file);
}
}
fclose(file);
}

void mlfw_mat_string_get(mlfw_mat_string *matrix,index_t row,index_t column,char **string)
{
	if(string==NULL) return;
	if(matrix==NULL)
	{
		*string=NULL;
		return;
	}
	if(row<0 || row>=matrix->rows)
	{
		*string=NULL;
		return;
	}
	if(column<0 || column>=matrix->columns)
	{
		*string=NULL;
		return;
	}
	if(matrix->data[row][column]==NULL)
	{
		*string=NULL;
		return;
	}
	*string=(char *)malloc(sizeof(char)*(strlen(matrix->data[row][column])+1));
	if(*string==NULL) return;
	strcpy(*string,matrix->data[row][column]);
}

void mlfw_mat_string_set(mlfw_mat_string *matrix,index_t row,index_t column,char *string)
{
	if(matrix==NULL || string==NULL) return;
	if(row<0 || row>=matrix->rows) return;
	if(column<0 || column>=matrix->columns) return;
	if(matrix->data[row][column]!=NULL) free(matrix->data[row][column]);
	matrix->data[row][column]=(char *)malloc(sizeof(char)*(strlen(string)+1));
	if(matrix->data[row][column]!=NULL)
	{
		strcpy(matrix->data[row][column],string);
	}
}

void mlfw_mat_string_get_dimensions(mlfw_mat_string *matrix,dimension_t *rows,dimension_t *columns)
{
	if(matrix==NULL) return;
	if(rows!=NULL)
	{
		*rows=matrix->rows;
	}
	if(columns!=NULL)
	{
		*columns=matrix->columns;
	}
}
mlfw_mat_string * mlfw_mat_string_transpose(mlfw_mat_string *matrix,mlfw_mat_string *transposed_matrix)
{
	char *ptr;
	index_t r,c;
	if(matrix==NULL) return NULL;
	if(transposed_matrix==NULL)
	{
	transposed_matrix=mlfw_mat_string_create_new(matrix->columns,matrix->rows);
	if(transposed_matrix==NULL) return NULL;
	}
	else
	{
		if(transposed_matrix->rows!=matrix->columns || transposed_matrix->columns!=matrix->rows) return NULL;
	}
	for(r=0;r<matrix->rows;++r)
	{
		for(c=0;c<matrix->columns;++c)
		{
			mlfw_mat_string_get(matrix,r,c,&ptr);
			mlfw_mat_string_set(transposed_matrix,c,r,ptr);
			free(ptr);
		}
	}
	return transposed_matrix;
}



mlfw_mat_string * mlfw_mat_string_shuffle(mlfw_mat_string *matrix,uint8_t how_many_times_to_shuffle,mlfw_mat_string *shuffled_matrix)
{
	// idx=(r%(b-a+1))+a;
	int r;
	index_t a,b;
	index_t c;
	index_t u;
	index_t idx;
	index_t end_at_index;
	uint8_t j;
	char * tmp_var;
	if(matrix==NULL) return NULL;
	if(how_many_times_to_shuffle==0) return NULL;
	if(shuffled_matrix==NULL)
	{
	shuffled_matrix=mlfw_mat_string_create_new(matrix->rows,matrix->columns);
	if(shuffled_matrix==NULL) return NULL;
        }
	else
	{
		if(shuffled_matrix->rows!=matrix->rows || shuffled_matrix->columns!=matrix->columns) return NULL;
	}
	mlfw_mat_string_copy(shuffled_matrix,matrix,0,0,0,0,matrix->rows-1,matrix->columns-1);

	b=shuffled_matrix->rows-1; // last row index
	srand(time(NULL));
        end_at_index=shuffled_matrix->rows-3;
        for(j=1;j<how_many_times_to_shuffle;++j)
	{
		for(u=0;u<=end_at_index;++u)
		{
			a=u+1;
			r=rand();
			idx=(r%(b-a+1))+a;
		        // swap data at u and idx row
			for(c=0;c<shuffled_matrix->columns;++c)
			{
				tmp_var=shuffled_matrix->data[u][c];
				shuffled_matrix->data[u][c]=shuffled_matrix->data[idx][c];
				shuffled_matrix->data[idx][c]=tmp_var;
			}	
		}
	}
        return shuffled_matrix;	
}

void mlfw_mat_string_copy(mlfw_mat_string *target,mlfw_mat_string *source,index_t target_row_index,index_t target_column_index,index_t source_from_row_index,index_t source_from_column_index,index_t source_to_row_index,index_t source_to_column_index)
{
	char *ptr;
	index_t target_r;
	index_t target_c;

	index_t source_r;
	index_t source_c;

	if(target==NULL || source==NULL) return;

	if(target_row_index<0 || target_row_index>=target->rows) return;
	if(target_column_index<0 || target_column_index>=target->columns) return;

	if(source_from_row_index<0 || source_from_row_index>=source->rows) return;
	if(source_from_column_index<0 || source_from_column_index>=source->columns) return;

	if(source_to_row_index<0 || source_to_row_index>=source->rows) return;
	if(source_to_column_index<0 || source_to_column_index>=source->columns) return;

	if(source_from_row_index>source_to_row_index) return;
	if(source_from_column_index>source_to_column_index) return;

	target_r=target_row_index;
	source_r=source_from_row_index;
	while(source_r<=source_to_row_index)
	{
		target_c=target_column_index;
		source_c=source_from_column_index;
		while(source_c<=source_to_column_index)
		{
			if(target_r>=0 && target_r<target->rows && target_c>=0 && target_c<target->columns)
			{
				if(target->data[target_r][target_c]!=NULL)
				{
					free(target->data[target_r][target_c]);
					target->data[target_r][target_c]=NULL;
				}
				mlfw_mat_string_get(source,source_r,source_c,&ptr);
				if(ptr!=NULL)
				{
					mlfw_mat_string_set(target,target_r,target_c,ptr);
					free(ptr);
				}
			}
			source_c++;
			target_c++;
		}
		source_r++;
		target_r++;
	}	
}


