#include<mlfw_matrix.h>
int main()
{
	mlfw_mat_string *matrix;
	matrix=mlfw_mat_string_create_new(2,3);
	mlfw_mat_string_set(matrix,0,0,"One");
	mlfw_mat_string_set(matrix,0,1,"Two");
	mlfw_mat_string_set(matrix,0,2,"Three");
	mlfw_mat_string_set(matrix,1,0,"Four");
	mlfw_mat_string_set(matrix,1,1,"Five");
	mlfw_mat_string_set(matrix,1,2,"Six");
	mlfw_mat_string_to_csv(matrix,"matrix.csv");
	return 0;
}
