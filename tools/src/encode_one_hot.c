#include<mlfw_encoder.h>
#include<mlfw_vector.h>
#include<stdio.h>
#include<stdlib.h>
int main(int argc,char *argv[])
{
	mlfw_row_vec_string *columns_to_encode;
        char *input_file;	
	char *output_file;
	index_t i;
	if(argc<4)
	{
		printf("Usage : [encode_one_hot input_file output_file column_name1 column_name2 ...]\n");
		return 0;
	}
	input_file=argv[1];
	output_file=argv[2];
	columns_to_encode=mlfw_row_vec_string_create_new(argc-3);
	if(columns_to_encode==NULL)
	{
		printf("Low memory\n");
		return 0;
	}
	for(i=3;i<argc;++i)
	{
		mlfw_row_vec_string_set(columns_to_encode,i-3,argv[i]);
	}
	mlfw_encoder_encode_one_hot(input_file,output_file,columns_to_encode);
	mlfw_row_vec_string_destroy(columns_to_encode);
	return 0;
}
