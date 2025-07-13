#include<mlfw_matrix.h>
#include<mlfw_vector.h>
#include<mlfw_operations.h>
#include<stdlib.h>
#include<stdio.h>

void score_it(char *results_file_name)  
{
	mlfw_mat_double *dataset;
	mlfw_row_vec_string *dataset_header;
        dimension_t dataset_rows,dataset_columns;
        mlfw_column_vec_double *A;
        mlfw_column_vec_double *P;
        mlfw_column_vec_double *R; // A-P
	mlfw_row_vec_double *RT;  // R transposed
	mlfw_column_vec_double *RTR; // RT*R
	double mean_of_actuals;
	mlfw_column_vec_double *M;
	mlfw_column_vec_double *AM; //A-M
	mlfw_row_vec_double *AMT; // AM transposed
        mlfw_column_vec_double *AMTAM; // AMT*AM
	
	double SSR;
	double SST;
	double r2score;

	dataset=mlfw_mat_double_from_csv(results_file_name,NULL,&dataset_header);
	if(dataset==NULL)
	{
		printf("Low memory or unable to load results file\n");
		return;
	}
	mlfw_mat_double_get_dimensions(dataset,&dataset_rows,&dataset_columns);

	A=mlfw_mat_double_create_column_vec(dataset,dataset_columns-2,NULL);  // 2nd last column of the file
	if(A==NULL)
	{
		printf("Low memory\n");
		mlfw_mat_double_destroy(dataset);
		mlfw_row_vec_string_destroy(dataset_header);
		return;
	}
	P=mlfw_mat_double_create_column_vec(dataset,dataset_columns-1,NULL); // last column of the file
	if(P==NULL)
	{
		printf("Low memory\n");
	        mlfw_mat_double_destroy(dataset);
	        mlfw_column_vec_double_destroy(A);
		mlfw_row_vec_string_destroy(dataset_header);
		return;
	}
	R=mlfw_substract_double_column_vector(A,P,NULL);
	if(R==NULL)
	{
		printf("Low memory\n");
	        mlfw_mat_double_destroy(dataset);
	        mlfw_column_vec_double_destroy(A);
	        mlfw_column_vec_double_destroy(P);		
		mlfw_row_vec_string_destroy(dataset_header);
		return;
	}
	RT=mlfw_column_vec_double_transpose(R,NULL);
	if(RT==NULL)
	{
		printf("Low memory\n");
	        mlfw_mat_double_destroy(dataset);
	        mlfw_column_vec_double_destroy(A);
	        mlfw_column_vec_double_destroy(P);	
	        mlfw_column_vec_double_destroy(R);
		mlfw_row_vec_string_destroy(dataset_header);	
		return;
	}
	RTR=mlfw_multiply_double_row_vector_with_column_vector(RT,R,NULL);
	if(RTR==NULL)
	{
		printf("Low memory\n");
	        mlfw_mat_double_destroy(dataset);
	        mlfw_column_vec_double_destroy(A);
	        mlfw_column_vec_double_destroy(P);	
	        mlfw_column_vec_double_destroy(R);	
	        mlfw_row_vec_double_destroy(RT);	
		mlfw_row_vec_string_destroy(dataset_header);
		return;
	}
	SSR=mlfw_column_vec_double_get(RTR,0);
	mean_of_actuals=mlfw_column_vec_double_get_mean(A);
	M=mlfw_column_vec_double_create_new_filled(dataset_rows,mean_of_actuals,NULL);
	if(M==NULL)
	{
		printf("Low memory\n");
	        mlfw_mat_double_destroy(dataset);
	        mlfw_column_vec_double_destroy(A);
	        mlfw_column_vec_double_destroy(P);	
	        mlfw_column_vec_double_destroy(R);	
	        mlfw_row_vec_double_destroy(RT);	
	        mlfw_column_vec_double_destroy(RTR);	
		mlfw_row_vec_string_destroy(dataset_header);
		return;
	}
	AM=mlfw_substract_double_column_vector(A,M,NULL);
	if(AM==NULL)
	{
		printf("Low memory\n");
	        mlfw_mat_double_destroy(dataset);
	        mlfw_column_vec_double_destroy(A);
	        mlfw_column_vec_double_destroy(P);	
	        mlfw_column_vec_double_destroy(R);	
	        mlfw_row_vec_double_destroy(RT);	
	        mlfw_column_vec_double_destroy(RTR);	
		mlfw_column_vec_double_destroy(M);
		mlfw_row_vec_string_destroy(dataset_header);
		return;
	}
	AMT=mlfw_column_vec_double_transpose(AM,NULL);
	if(AMT==NULL)
	{
		printf("Low memory\n");
	        mlfw_mat_double_destroy(dataset);
	        mlfw_column_vec_double_destroy(A);
	        mlfw_column_vec_double_destroy(P);	
	        mlfw_column_vec_double_destroy(R);	
	        mlfw_row_vec_double_destroy(RT);	
	        mlfw_column_vec_double_destroy(RTR);	
		mlfw_column_vec_double_destroy(M);
		mlfw_column_vec_double_destroy(AM);
		mlfw_row_vec_string_destroy(dataset_header);
		return;	
	}
	AMTAM=mlfw_multiply_double_row_vector_with_column_vector(AMT,AM,NULL);
	if(AMTAM==NULL)
	{
		printf("Low memory\n");
	        mlfw_mat_double_destroy(dataset);
	        mlfw_column_vec_double_destroy(A);
	        mlfw_column_vec_double_destroy(P);	
	        mlfw_column_vec_double_destroy(R);	
	        mlfw_row_vec_double_destroy(RT);	
	        mlfw_column_vec_double_destroy(RTR);	
		mlfw_column_vec_double_destroy(M);
		mlfw_column_vec_double_destroy(AM);
		mlfw_row_vec_double_destroy(AMT);
		mlfw_row_vec_string_destroy(dataset_header);
		return;		
	}
	SST=mlfw_column_vec_double_get(AMTAM,0);
	r2score=1-(SSR/SST);
	printf("R2Score of the test is %lf\n",r2score);

	mlfw_mat_double_destroy(dataset);
	mlfw_column_vec_double_destroy(A);
	mlfw_column_vec_double_destroy(P);
	mlfw_column_vec_double_destroy(R);
	mlfw_row_vec_double_destroy(RT);
	mlfw_column_vec_double_destroy(RTR);
	mlfw_column_vec_double_destroy(M);
	mlfw_column_vec_double_destroy(AM);
	mlfw_row_vec_double_destroy(AMT);
	mlfw_column_vec_double_destroy(AMTAM);
        mlfw_row_vec_string_destroy(dataset_header);
}
int main(int argc,char *argv[])
{
	if(argc!=2)
	{
		printf("Usage : [score_it.out results_file]\n");
		return 0;
	}
	score_it(argv[1]);
	return 0;
}
