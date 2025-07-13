#include<mlfw_encoder.h>
#include<mlfw_vector.h>
#include<stdio.h>
int main()
{
	mlfw_row_vec_string *columns_to_encode;
	columns_to_encode=mlfw_row_vec_string_create_new(3);
	if(columns_to_encode==NULL)
	{
		printf("Low memory\n");
		return 0;
	}
	mlfw_row_vec_string_set(columns_to_encode,0,"sex");
	mlfw_row_vec_string_set(columns_to_encode,1,"smoker");
	mlfw_row_vec_string_set(columns_to_encode,2,"region");
	mlfw_encoder_encode_binary("insurance.csv","be_insurance.csv",columns_to_encode);
	return 0;
}
