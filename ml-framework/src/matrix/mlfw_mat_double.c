#include<mlfw_matrix.h>
#include<mlfw_vector.h>
#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include<math.h>
typedef struct __mlfw_mat_double
{
	double **data;
	dimension_t rows;
	dimension_t columns;
}mlfw_mat_double;
//function to create a matrix
mlfw_mat_double * mlfw_mat_double_create_new(dimension_t rows,dimension_t columns)
{
	mlfw_mat_double *matrix;
	index_t r,k;
	if(rows<=0 || columns<=0) return NULL;
	matrix=(mlfw_mat_double *)malloc(sizeof(mlfw_mat_double));
	if(matrix==NULL) return NULL;
        matrix->data=(double **)malloc(sizeof(double *)*rows);
	if(matrix->data==NULL)
	{
		free(matrix);
		return NULL;
	}
	for(r=0;r<rows;r++)
	{
		matrix->data[r]=(double *)malloc(sizeof(double)*columns);
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
	}
	matrix->rows=rows;
	matrix->columns=columns;
	return matrix;
}
void mlfw_mat_double_destroy(mlfw_mat_double *matrix)
{
	index_t r;
	if(matrix==NULL) return;
        for(r=0;r<matrix->rows;r++)
	{
		free(matrix->data[r]);
	}
	free(matrix->data);
	free(matrix);
}
//function to load data into matrix from csv
mlfw_mat_double * mlfw_mat_double_from_csv(const char *csv_file_name,mlfw_mat_double *matrix,mlfw_row_vec_string **header)
{
	char header_string[1025];
        index_t header_index;
	int index;
	char m;
	index_t r,c;
	double value;
	char double_string[1025]; //1 extra for \0 (string terminator)
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
	matrix=mlfw_mat_double_create_new(rows,columns);
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
		 if(m=='\r') continue;
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
			double_string[index]='\0';
			value=strtod(double_string,NULL);
			index=0;
			matrix->data[r][c]=value;
			c++;
			if(c==matrix->columns)
			{
				r++;
				c=0;
			}
		}
		else
		{
			double_string[index]=m;
			index++;
		}	
	}
	fclose(file);
	return matrix;
}

double mlfw_mat_double_get(mlfw_mat_double *matrix,index_t row,index_t column)
{
	double value=0.0;
	if(matrix==NULL) return value;
	if(row<0 || row>=matrix->rows) return value;
	if(column<0 || column>=matrix->columns) return value;
	return matrix->data[row][column];
}

void mlfw_mat_double_set(mlfw_mat_double *matrix,index_t row,index_t column,double value)
{
	if(matrix==NULL) return;
	if(row<0 || row>=matrix->rows) return;
	if(column<0 || column>=matrix->columns) return;
	matrix->data[row][column]=value;
}

void mlfw_mat_double_get_dimensions(mlfw_mat_double *matrix,dimension_t *rows,dimension_t *columns)
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
void mlfw_mat_double_copy(mlfw_mat_double *target,mlfw_mat_double *source,index_t target_row_index,index_t target_column_index,index_t source_from_row_index,index_t source_from_column_index,index_t source_to_row_index,index_t source_to_column_index)
{
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
				target->data[target_r][target_c]=source->data[source_r][source_c];
			}
			source_c++;
			target_c++;
		}
		source_r++;
		target_r++;
	}	
}
void mlfw_mat_double_fill(mlfw_mat_double *matrix,index_t from_row_index,index_t from_column_index,index_t to_row_index,index_t to_column_index,double value)
{
	index_t r,c;
	if(matrix==NULL) return;
	if(from_row_index>=matrix->rows) return;
	if(from_column_index>=matrix->columns) return;
	if(from_row_index<0) from_row_index=0;
	if(from_column_index<0) from_column_index=0;
	if(to_row_index>=matrix->rows) to_row_index=matrix->rows-1;
	if(to_column_index>=matrix->columns) to_column_index=matrix->columns-1;
	if(from_row_index>to_row_index) return;
	if(from_column_index>to_column_index) return;

	for(r=from_row_index;r<=to_row_index;++r)
	{
		for(c=from_column_index;c<=to_column_index;++c)
		{
			matrix->data[r][c]=value;
		}
	}
}
mlfw_column_vec_double * mlfw_mat_double_create_column_vec(mlfw_mat_double *matrix,index_t column_index,mlfw_column_vec_double *vector)
{
	index_t r;
	if(matrix==NULL) return NULL;
	if(column_index<0 || column_index>=matrix->columns) return NULL;
	if(vector==NULL)
	{
	vector=mlfw_column_vec_double_create_new(matrix->rows);
	if(vector==NULL) return NULL;
	}
	else
	{
		if(mlfw_column_vec_double_get_size(vector)!=matrix->rows) return NULL;
	}
	for(r=0;r<matrix->rows;++r)
	{
		mlfw_column_vec_double_set(vector,r,matrix->data[r][column_index]);
	}
	return vector;
}

mlfw_mat_double * mlfw_mat_double_shuffle(mlfw_mat_double *matrix,uint8_t how_many_times_to_shuffle,mlfw_mat_double *shuffled_matrix)
{
	// idx=(r%(b-a+1))+a;
	int r;
	index_t a,b;
	index_t c;
	index_t u;
	index_t idx;
	index_t end_at_index;
	uint8_t j;
	double tmp_var;
	if(matrix==NULL) return NULL;
	if(how_many_times_to_shuffle==0) return NULL;
	if(shuffled_matrix==NULL)
	{
	shuffled_matrix=mlfw_mat_double_create_new(matrix->rows,matrix->columns);
	if(shuffled_matrix==NULL) return NULL;
        }
	else
	{
		if(shuffled_matrix->rows!=matrix->rows || shuffled_matrix->columns!=matrix->columns) return NULL;
	}
	mlfw_mat_double_copy(shuffled_matrix,matrix,0,0,0,0,matrix->rows-1,matrix->columns-1);

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

void mlfw_mat_double_to_csv(mlfw_mat_double *matrix,char *csv_file_name,mlfw_row_vec_string *header)
{
	index_t index;
	dimension_t header_size;
	char *ptr;
	index_t r,c;
	char separator;
	FILE *file;
	if(matrix==NULL || csv_file_name==NULL || header==NULL) return;
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
			fprintf(file,"%lf",matrix->data[r][c]);
			if(c==matrix->columns-1) // last element
			{
				separator='\n';
			}
			else
			{
				separator=',';
			}
			fputc(separator,file);
		}
	}
	fclose(file);
}


mlfw_mat_double * mlfw_mat_double_transpose(mlfw_mat_double *matrix,mlfw_mat_double *matrix_t)
{
	index_t r,c;
	if(matrix==NULL) return NULL;
	if(matrix_t==NULL)
	{
	matrix_t=mlfw_mat_double_create_new(matrix->columns,matrix->rows);
	if(matrix==NULL) return NULL;
	}
	else
	{
		if(matrix_t->rows!=matrix->columns || matrix_t->columns!=matrix->rows) return NULL;
	}
	for(r=0;r<matrix->rows;++r)
	{
		for(c=0;c<matrix->columns;++c)
		{
			matrix_t->data[c][r]=matrix->data[r][c];
		}
	}
	return matrix_t;
}

double mlfw_mat_double_get_minimum(mlfw_mat_double *matrix,index_t start_row_index,index_t start_column_index,index_t end_row_index,index_t end_column_index)
{
	double minimum;
	index_t r,c;
	if(matrix==NULL) return 0.0;
	if(start_row_index<0) start_row_index=0;
	if(start_column_index<0) start_column_index=0;
	if(end_row_index>=matrix->rows) end_row_index=matrix->rows-1;
	if(end_column_index>=matrix->columns) end_column_index=matrix->columns-1;
	if(start_row_index>end_row_index) return 0.0;
	if(start_column_index>end_column_index) return 0.0;
	minimum=matrix->data[start_row_index][start_column_index];
	for(r=start_row_index;r<=end_row_index;++r)
	{
		for(c=start_column_index;c<=end_column_index;++c)
		{
			if(matrix->data[r][c]<minimum)
			{
				minimum=matrix->data[r][c];
			}
		}
	}
	return minimum;
}

double mlfw_mat_double_get_maximum(mlfw_mat_double *matrix,index_t start_row_index,index_t start_column_index,index_t end_row_index,index_t end_column_index)
{
	double maximum;
	index_t r,c;
	if(matrix==NULL) return 0.0;
	if(start_row_index<0) start_row_index=0;
	if(start_column_index<0) start_column_index=0;
	if(end_row_index>=matrix->rows) end_row_index=matrix->rows-1;
	if(end_column_index>=matrix->columns) end_column_index=matrix->columns-1;
	if(start_row_index>end_row_index) return 0.0;
	if(start_column_index>end_column_index) return 0.0;
	maximum=matrix->data[start_row_index][start_column_index];
	for(r=start_row_index;r<=end_row_index;++r)
	{
		for(c=start_column_index;c<=end_column_index;++c)
		{
			if(matrix->data[r][c]>maximum)
			{
				maximum=matrix->data[r][c];
			}
		}
	}
	return maximum;
}

double mlfw_mat_double_get_mean(mlfw_mat_double *matrix,index_t start_row_index,index_t start_column_index,index_t end_row_index,index_t end_column_index)
{
	double mean;
	dimension_t elements_count;
	double sum;
	index_t r,c;
	if(matrix==NULL) return 0.0;
	if(start_row_index<0) start_row_index=0;
	if(start_column_index<0) start_column_index=0;
	if(end_row_index>=matrix->rows) end_row_index=matrix->rows-1;
	if(end_column_index>=matrix->columns) end_column_index=matrix->columns-1;
	if(start_row_index>end_row_index) return 0.0;
	if(start_column_index>end_column_index) return 0.0;
	sum=0.0;
	elements_count=0;
	for(r=start_row_index;r<=end_row_index;++r)
	{
		for(c=start_column_index;c<=end_column_index;++c)
		{
			sum=sum+matrix->data[r][c];
			++elements_count;
		}
	}
	mean=sum/elements_count;
	return mean;
}

double mlfw_mat_double_get_standard_deviation(mlfw_mat_double *matrix,index_t start_row_index,index_t start_column_index,index_t end_row_index,index_t end_column_index)
{
	double standard_deviation;
	double mean;
	double sum;
	double square;
	double diff;
	dimension_t elements_count;

	index_t r,c;
	if(matrix==NULL) return 0.0;
	if(start_row_index<0) start_row_index=0;
	if(start_column_index<0) start_column_index=0;
	if(end_row_index>=matrix->rows) end_row_index=matrix->rows-1;
	if(end_column_index>=matrix->columns) end_column_index=matrix->columns-1;
	if(start_row_index>end_row_index) return 0.0;
	if(start_column_index>end_column_index) return 0.0;
	mean=mlfw_mat_double_get_mean(matrix,start_row_index,start_column_index,end_row_index,end_column_index);
	sum=0.0;
	elements_count=0;
	for(r=start_row_index;r<=end_row_index;++r)
	{
		for(c=start_column_index;c<=end_column_index;++c)
		{
			diff=matrix->data[r][c]-mean;
			square=diff*diff;
			sum=sum+square;
			++elements_count;
		}
	}
	mean=sum/elements_count;
	standard_deviation=sqrt(mean);
	return standard_deviation;
}

void mlfw_mat_double_get_training_testing_data(char *csv_file_name,mlfw_mat_double **training_data_matrix,mlfw_mat_double **testing_data_matrix,uint8_t testing_data_percentage)
{
	mlfw_row_vec_string *header;
	mlfw_mat_double *shuffled_matrix;
	mlfw_mat_double *matrix;
	mlfw_mat_double *minor_matrix;
	mlfw_mat_double *major_matrix;
	dimension_t minor_rows;
	dimension_t major_rows;
	dimension_t shuffled_matrix_rows,shuffled_matrix_columns;
	index_t i,j;

	if(csv_file_name==NULL)
	{
		return;
	}
	if(training_data_matrix==NULL || testing_data_matrix==NULL)
	{
		if(training_data_matrix!=NULL) *training_data_matrix=NULL;
		if(testing_data_matrix!=NULL) *testing_data_matrix=NULL;
		return;
	}
	if(testing_data_percentage<=0 || testing_data_percentage>=100)
	{
		*testing_data_matrix=NULL;
		*training_data_matrix=NULL;
		return;
	}
	matrix=mlfw_mat_double_from_csv(csv_file_name,NULL,&header);
	if(matrix==NULL)
	{
		*testing_data_matrix=NULL;
		*training_data_matrix=NULL;
		return;
	}
	shuffled_matrix=mlfw_mat_double_shuffle(matrix,3,NULL);  // shuffle 3 times
	if(shuffled_matrix==NULL)
	{
		*testing_data_matrix=NULL;
		*training_data_matrix=NULL;
		mlfw_mat_double_destroy(matrix);
		mlfw_row_vec_string_destroy(header);
		return;
	}
	mlfw_mat_double_get_dimensions(shuffled_matrix,&shuffled_matrix_rows,&shuffled_matrix_columns);
	minor_rows=(dimension_t)(testing_data_percentage*shuffled_matrix_rows)/100;
	major_rows=shuffled_matrix_rows-minor_rows;

	major_matrix=(mlfw_mat_double *)malloc(sizeof(mlfw_mat_double));
	if(major_matrix==NULL)
	{
		*testing_data_matrix=NULL;
		*training_data_matrix=NULL;
		mlfw_mat_double_destroy(matrix);
		mlfw_row_vec_string_destroy(header);
		mlfw_mat_double_destroy(shuffled_matrix);
		return;
	}
	major_matrix->rows=major_rows;
	major_matrix->columns=shuffled_matrix->columns;
	major_matrix->data=(double **)malloc(sizeof(double *)*major_rows);
	if(major_matrix->data==NULL)
	{
		*testing_data_matrix=NULL;
		*training_data_matrix=NULL;
		mlfw_mat_double_destroy(matrix);
		mlfw_row_vec_string_destroy(header);
		mlfw_mat_double_destroy(shuffled_matrix);
		free(major_matrix);
		return;
	}
	for(j=0,i=0;i<major_rows;++i,++j)
	{
		major_matrix->data[i]=shuffled_matrix->data[j];
	}
	minor_matrix=(mlfw_mat_double *)malloc(sizeof(mlfw_mat_double));
	if(minor_matrix==NULL)
	{
		*testing_data_matrix=NULL;
		*training_data_matrix=NULL;
		mlfw_mat_double_destroy(matrix);
		mlfw_row_vec_string_destroy(header);
		mlfw_mat_double_destroy(shuffled_matrix);
		free(major_matrix->data);
		free(major_matrix);
		return;
	}
	minor_matrix->rows=minor_rows;
	minor_matrix->columns=shuffled_matrix->columns;
	minor_matrix->data=(double **)malloc(sizeof(double *)*minor_rows);
	if(minor_matrix->data==NULL)
	{
		*testing_data_matrix=NULL;
		*training_data_matrix=NULL;
		mlfw_mat_double_destroy(matrix);
		mlfw_row_vec_string_destroy(header);
		mlfw_mat_double_destroy(shuffled_matrix);
		free(major_matrix->data);
		free(major_matrix);
		free(minor_matrix);
		return;
	}
	for(i=0;i<minor_rows;++i,++j)
	{
		minor_matrix->data[i]=shuffled_matrix->data[j];
	}

	free(shuffled_matrix->data);
	free(shuffled_matrix);
	mlfw_row_vec_string_destroy(header);
	mlfw_mat_double_destroy(matrix);
	*training_data_matrix=major_matrix;
	*testing_data_matrix=minor_matrix;
	return;

}

void mlfw_mat_double_reshape(mlfw_mat_double **matrix_to_reshape,dimension_t new_rows_count,dimension_t new_columns_count)
{
	mlfw_mat_double *matrix;
	double **new_data_array;
	double *new_row;
	index_t i;

	if(matrix_to_reshape==NULL) return;
	if(new_rows_count<=0 || new_columns_count<=0)
	{
		mlfw_mat_double_destroy(*matrix_to_reshape);
		*matrix_to_reshape=NULL;
		return;
	}
	matrix=*matrix_to_reshape;
	if(matrix->rows==new_rows_count && matrix->columns==new_rows_count) return;

	if(new_rows_count>matrix->rows) // increase number of rows
	{
		new_data_array=(double **)realloc(matrix->data,sizeof(double *)*new_rows_count);
		if(new_data_array==NULL)
		{
			mlfw_mat_double_destroy(matrix);
			*matrix_to_reshape=NULL;
			return;
		}
		matrix->data=new_data_array;
		for(i=matrix->rows;i<new_rows_count;++i) matrix->data[i]=NULL;
		matrix->rows=new_rows_count;
	}else if(new_rows_count<matrix->rows) // decrease number of rows
	{
		for(i=new_rows_count;i<matrix->rows;++i)
		{
			free(matrix->data[i]);
		}
		new_data_array=(double **)realloc(matrix->data,sizeof(double *)*new_rows_count);
		if(new_data_array==NULL)
		{
			for(i=0;i<new_rows_count;++i)
			{
				free(matrix->data[i]);
			}
			free(matrix->data);
			free(matrix);
			*matrix_to_reshape=NULL;
			return;
		}
		matrix->data=new_data_array;
		matrix->rows=new_rows_count;
	}
	// now lets think about columns
	if(new_columns_count==matrix->columns) return;

	// logic to reshape content of each row
	for(i=0;i<matrix->rows;++i)
	{
		new_row=(double *)realloc(matrix->data[i],sizeof(double)*new_columns_count);
		if(new_row==NULL)
		{
			mlfw_mat_double_destroy(matrix);
			*matrix_to_reshape=NULL;
			return;
		}
		matrix->data[i]=new_row;
	}
	matrix->columns=new_columns_count;
}

void mlfw_mat_double_right_shift(mlfw_mat_double *matrix,dimension_t how_many_places_to_shift)
{
	int64_t r,c,new_c;
	if(matrix==NULL) return;
	if(how_many_places_to_shift<=0) return;
	if(how_many_places_to_shift>matrix->columns) how_many_places_to_shift=matrix->columns;
	for(r=0;r<matrix->rows;++r)
	{
		c=matrix->columns-1;
		while(1)
		{
			new_c=c+how_many_places_to_shift;
			if(new_c<matrix->columns)
			{
				matrix->data[r][new_c]=matrix->data[r][c];
			}
			matrix->data[r][c]=0.0;
			if(c==0) break;
			c--;
		}
	}
}

void mlfw_mat_double_left_shift(mlfw_mat_double *matrix,dimension_t how_many_places_to_shift)
{
	int64_t r,c,new_c;
	if(matrix==NULL) return;
	if(how_many_places_to_shift<=0) return;
	if(how_many_places_to_shift>matrix->columns) how_many_places_to_shift=matrix->columns;
	for(r=0;r<matrix->rows;++r)
	{
		c=0;
		while(c<matrix->columns)
		{
			new_c=c-how_many_places_to_shift;
			if(new_c>=0)
			{
				matrix->data[r][new_c]=matrix->data[r][c];
			}
			matrix->data[r][c]=0.0;
			if(c==0) break;
			c++;
		}
	}

}
