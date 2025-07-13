#include<mlfw_matrix.h>
#include<mlfw_set.h>
#include<mlfw_encoder.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
void mlfw_encoder_encode_one_hot(char *source,char *target,int *encode_columns,int size,int header_exists)
{
	FILE *target_file;
	mlfw_mat_string *matrix;
	mlfw_set_string **sets;
	char *string;
        char *set_string;
	dimension_t set_size;
	index_t r;
	index_t data_start_row_index;
	index_t i;
	index_t c;
	index_t j;
	dimension_t matrix_rows,matrix_columns;
	if(source==NULL || target==NULL || encode_columns==NULL || size<=0) return;
	matrix=mlfw_mat_string_from_csv(source);
	mlfw_mat_string_get_dimensions(matrix,&matrix_rows,&matrix_columns);
	for(i=0;i<size;++i)
	{
		c=encode_columns[i];
		if(c<0 || c>=matrix_columns)
		{
			mlfw_mat_string_destroy(matrix);
			return;
		}
	}
	sets=(mlfw_set_string **)malloc(sizeof(mlfw_set_string *)*size);
	if(sets==NULL)
	{
		mlfw_mat_string_destroy(matrix);
		return;
	}
	for(i=0;i<size;++i)
	{
		sets[i]=mlfw_set_string_create_new();
		if(sets[i]==NULL)
		{
			for(j=0;j<i;++j) mlfw_set_string_destroy(sets[j]);
			mlfw_mat_string_destroy(matrix);
			return;
		}
	}
	if(header_exists) data_start_row_index=1;
	else data_start_row_index=0;

	for(r=data_start_row_index;r<matrix_rows;++r)
	{
		for(i=0;i<size;++i) // loop on encode_columns array
		{
			c=encode_columns[i];
			mlfw_mat_string_get(matrix,r,c,&string);
			if(string==NULL)
			{
				for(j=0;j<size;++j)
				{
					mlfw_set_string_destroy(sets[j]);
				}
				free(sets);
				mlfw_mat_string_destroy(matrix);
				return;
			}
			else
			{
				if(mlfw_set_string_add(sets[i],string)==-1)
				{
					free(string);
				        for(j=0;j<size;++j)
			         	{
				         	mlfw_set_string_destroy(sets[j]);
			         	}
			        	free(sets);
			        	mlfw_mat_string_destroy(matrix);
			        	return;	
				}
				free(string);
			}
		}
	}

	// code to test the contents of the sets
	/*for(i=0;i<size;++i)
	{
		set_size=mlfw_set_string_get_size(sets[i]);
		for(j=0;j<set_size;++j)
		{
			mlfw_set_string_get(sets[i],j,&string);
			if(string!=NULL)
			{
				printf("%s\n",string);
				free(string);
			}
		}
	}*/

	// logic to create target file starts here
	target_file=fopen(target,"w");
	if(target_file==NULL)
	{
		for(i=0;i<size;++i) mlfw_set_string_destroy(sets[i]);
		free(sets);
		mlfw_mat_string_destroy(matrix);
		return;
	}
	if(header_exists)
	{
		for(c=0;c<matrix_columns;++c)
		{
			mlfw_mat_string_get(matrix,0,c,&string);
			if(string==NULL)
			{
				for(i=0;i<size;++i) mlfw_set_string_destroy(sets[i]);
				free(sets);
				mlfw_mat_string_destroy(matrix);
				fclose(target_file);
				target_file=fopen(target,"w"); // create blank file
				fclose(target_file);
				return;
			}	
			for(i=0;i<size;++i)
			{
				if(c==encode_columns[i]) break;
			}
			if(i<size) // found , c(th) columns is to be hot coded
		        {
				set_size=mlfw_set_string_get_size(sets[i]);
				for(j=0;j<set_size;++j)
				{
					mlfw_set_string_get(sets[i],j,&set_string);
                                        if(set_string==NULL)
					{
						free(string);
						for(i=0;i<size;++i) mlfw_set_string_destroy(sets[i]);
						free(sets);
						mlfw_mat_string_destroy(matrix);
						fclose(target_file);
						target_file=fopen(target,"w"); // create blank file
						fclose(target_file);
						return;
					}
					fputs(string,target_file);
					fputc('_',target_file);
					fputs(set_string,target_file);
					free(set_string);
					if(j<set_size-1) fputc(',',target_file);
				}
			}
			else // not found,the column not to be hot coded
			{
				fputs(string,target_file);
			}
			free(string);
			if(c==matrix_columns-1) fputc('\n',target_file);
			else fputc(',',target_file);
		}
	}// header exits part ends

	// encode data
	for(r=data_start_row_index;r<matrix_rows;++r)
	{
		for(c=0;c<matrix_columns;++c)
		{
			mlfw_mat_string_get(matrix,r,c,&string);
			if(string==NULL)
			{
				for(i=0;i<size;++i) mlfw_set_string_destroy(sets[i]);
				free(sets);
				mlfw_mat_string_destroy(matrix);
				fclose(target_file);
				target_file=fopen(target,"w"); // create blank file
				fclose(target_file);
				return;
			}
			for(i=0;i<size;++i)
			{
				if(c==encode_columns[i]) break;
			}
			if(i<size)   // found , c(th) columns is to be hot coded
		        {
				set_size=mlfw_set_string_get_size(sets[i]);
				for(j=0;j<set_size;++j)
				{
					mlfw_set_string_get(sets[i],j,&set_string);
					if(set_string==NULL)
					{
						free(string);
						for(i=0;i<size;++i) mlfw_set_string_destroy(sets[i]);
						free(sets);
						mlfw_mat_string_destroy(matrix);
						fclose(target_file);
						target_file=fopen(target,"w");  // create blank file
						fclose(target_file);
						return;
					}
				        if(strcmp(string,set_string)==0)
					{
						fputc('1',target_file);
					}
					else
					{
						fputc('0',target_file);
					}
					free(set_string);
					if(j<set_size-1) fputc(',',target_file);
				}
			}
				else  // not found , the column not to be hot coded
				{
					fputs(string,target_file);
				}
				free(string);
				if(c==matrix_columns-1) fputc('\n',target_file);
				else fputc(',',target_file);
			}
		}
		fclose(target_file);
}
