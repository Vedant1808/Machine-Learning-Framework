#include<mlfw_encoder.h>
#include<stdio.h>
int main()
{
	int encode_columns[3]={1,4,5};
	mlfw_encoder_encode_one_hot("insurance.csv","one_hot_encoded_insurance.csv",encode_columns,3,1);
	// last argument represent true (non zero) or false (zero),header exits or not
	return 0;
}
