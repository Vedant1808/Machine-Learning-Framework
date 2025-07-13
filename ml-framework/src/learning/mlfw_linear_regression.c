#include<mlfw_learning.h>
#include<mlfw_matrix.h>
#include<mlfw_vector.h>
#include<mlfw_operations.h>
#include<stdio.h>

mlfw_row_vec_double *mlfw_linear_regression_gradient_descent_fit_line(mlfw_mat_double *input_features_matrix,mlfw_column_vec_double *target_values_vector,double learning_rate,uint64_t number_of_iterations,uint8_t (*on_each_iteration)(uint64_t iteration_number,double error_value))
{
	int error_flag;

	index_t r;

	uint64_t k;

	mlfw_mat_double *I;
	dimension_t I_rows;
	dimension_t I_columns;

	mlfw_column_vec_double *A;

	mlfw_column_vec_double *m;
        
        mlfw_row_vec_double *trained_parameters;

	mlfw_column_vec_double *P;

	mlfw_column_vec_double *E;

	mlfw_row_vec_double *ET;

	mlfw_column_vec_double *ETE;

	mlfw_mat_double *IT;

	mlfw_column_vec_double *ITE;
        
	mlfw_column_vec_double *TMP;

        mlfw_column_vec_double *UM;	

	double sum_of_squared_error_values;

	double final_error_value;

        if(input_features_matrix==NULL || target_values_vector==NULL) return NULL;
	if(number_of_iterations==0 && on_each_iteration==NULL) return NULL;

	I=input_features_matrix;
	mlfw_mat_double_get_dimensions(I,&I_rows,&I_columns);
	mlfw_mat_double_reshape(&I,I_rows,I_columns+1);

	if(I==NULL)
	{
		return NULL; // later on we will introduce something to notify error
	}
	I_columns=I_columns+1;
	mlfw_mat_double_right_shift(I,1);

	mlfw_mat_double_fill(I,0,0,I_rows-1,0,1.0); // fill bias 1.0

	IT=mlfw_mat_double_transpose(I,NULL);
	if(IT==NULL)
	{
		mlfw_mat_double_left_shift(I,1);
		mlfw_mat_double_reshape(&I,I_rows,I_columns-1);
		return NULL;
	}
        A=target_values_vector;
	m=mlfw_column_vec_double_create_new_filled(I_columns,0.0,NULL);
	if(m==NULL)
	{
		mlfw_mat_double_left_shift(I,1);
		mlfw_mat_double_reshape(&I,I_rows,I_columns-1);
		mlfw_mat_double_destroy(IT);
		return NULL;
	}
        
        P=mlfw_column_vec_double_create_new(I_rows);
	if(P==NULL)
	{
                mlfw_mat_double_left_shift(I,1);
		mlfw_mat_double_reshape(&I,I_rows,I_columns-1);
                mlfw_mat_double_destroy(IT);
                mlfw_column_vec_double_destroy(m);
		return NULL;
	}
	E=mlfw_column_vec_double_create_new(I_rows);
	if(E==NULL)
	{
                mlfw_mat_double_left_shift(I,1);
		mlfw_mat_double_reshape(&I,I_rows,I_columns-1); 
                mlfw_column_vec_double_destroy(P);
                mlfw_mat_double_destroy(IT);
                mlfw_column_vec_double_destroy(m);
		return NULL;
	}
	ET=mlfw_row_vec_double_create_new(I_rows);
	if(ET==NULL)
	{
                mlfw_mat_double_left_shift(I,1);
		mlfw_mat_double_reshape(&I,I_rows,I_columns-1);
                mlfw_column_vec_double_destroy(P);
		mlfw_column_vec_double_destroy(E);
                mlfw_mat_double_destroy(IT);
                mlfw_column_vec_double_destroy(m);
		return NULL;

	}
        ETE=mlfw_column_vec_double_create_new(1);
	if(ETE==NULL)
	{
	        mlfw_mat_double_left_shift(I,1);
		mlfw_mat_double_reshape(&I,I_rows,I_columns-1);
                mlfw_column_vec_double_destroy(P);
		mlfw_column_vec_double_destroy(E);
	        mlfw_row_vec_double_destroy(ET);
                mlfw_mat_double_destroy(IT);
                mlfw_column_vec_double_destroy(m);
		return NULL;

	}
	ITE=mlfw_column_vec_double_create_new(I_columns);
	if(ITE==NULL)
	{
		mlfw_mat_double_left_shift(I,1);
		mlfw_mat_double_reshape(&I,I_rows,I_columns-1);
                mlfw_column_vec_double_destroy(P);
		mlfw_column_vec_double_destroy(E);
	        mlfw_row_vec_double_destroy(ET);
	        mlfw_column_vec_double_destroy(ETE);
                mlfw_mat_double_destroy(IT);
                mlfw_column_vec_double_destroy(m);
		return NULL;
	}
        TMP=mlfw_column_vec_double_create_new(I_columns);
	if(TMP==NULL)
	{
		mlfw_mat_double_left_shift(I,1);
		mlfw_mat_double_reshape(&I,I_rows,I_columns-1);
                mlfw_column_vec_double_destroy(P);
		mlfw_column_vec_double_destroy(E);
	        mlfw_row_vec_double_destroy(ET);
	        mlfw_column_vec_double_destroy(ETE);
	        mlfw_column_vec_double_destroy(ITE);
                mlfw_mat_double_destroy(IT);
                mlfw_column_vec_double_destroy(m);
		return NULL;
	}
	UM=mlfw_column_vec_double_create_new(I_columns);
	if(UM==NULL)
	{
		mlfw_mat_double_left_shift(I,1);
		mlfw_mat_double_reshape(&I,I_rows,I_columns-1);
                mlfw_column_vec_double_destroy(P);
		mlfw_column_vec_double_destroy(E);
	        mlfw_row_vec_double_destroy(ET);
	        mlfw_column_vec_double_destroy(ETE);
	        mlfw_column_vec_double_destroy(ITE);
	        mlfw_column_vec_double_destroy(TMP);
                mlfw_mat_double_destroy(IT);
		mlfw_column_vec_double_destroy(m);
		return NULL;
	}

	error_flag=0;
	// Operation start
	k=1;
	while(1)
	{

	if(number_of_iterations>0 && k==(number_of_iterations+1)) break;

	P=mlfw_multiply_double_matrix_with_column_vector(I,m,P);
	if(P==NULL)
	{
		error_flag=1;
		break;
	}
	E=mlfw_substract_double_column_vector(P,A,E);
	if(E==NULL)
	{
		error_flag=1;
		break;
	}

      	ET=mlfw_column_vec_double_transpose(E,ET);
        if(ET==NULL)
	{
		error_flag=1;
	 	break;
	}
	ETE=mlfw_multiply_double_row_vector_with_column_vector(ET,E,ETE);
        if(ETE==NULL)
	{
		error_flag=1;
		break;
	}

	sum_of_squared_error_values=mlfw_column_vec_double_get(ETE,0);
	final_error_value=sum_of_squared_error_values/(2*I_rows);
         
	//printf("Sum of squared error values : %41.15lf\n",sum_of_squared_error_values);
	//printf("Iteration Number  %" PRIu64 ",Error :  %41.15lf\n",k,final_error_value);

	// logic to update m and c,technically our m column vector
	ITE=mlfw_multiply_double_matrix_with_column_vector(IT,E,ITE);
	if(ITE==NULL)
	{
		error_flag=1;
		break;

	}
         
        TMP=mlfw_multiply_double_scalar_with_column_vector((learning_rate*(1.0/I_rows)),ITE,TMP);
	if(TMP==NULL)
	{
		error_flag=1;
		break;
	}
        
        UM=mlfw_substract_double_column_vector(m,TMP,UM);
        if(UM==NULL)
	{
		error_flag=1;
		break;
	}

        // set data from UM to m
	for(r=0;r<I_columns;++r)
	{
		mlfw_column_vec_double_set(m,r,mlfw_column_vec_double_get(UM,r));
	}

        // one iterations completed , so if on_each_iteration is not NULL, call the callback
	if(on_each_iteration!=NULL)
	{
		if(on_each_iteration(k,final_error_value)==0) break;
	}	
	++k;
        }
	
	if(error_flag==1)
	{
	// release all
	printf("Dimension of container is incorrect\n");	
	mlfw_mat_double_left_shift(I,1);
	mlfw_mat_double_reshape(&I,I_rows,I_columns-1);
	mlfw_column_vec_double_destroy(P);
	mlfw_column_vec_double_destroy(E);
	mlfw_row_vec_double_destroy(ET);
	mlfw_column_vec_double_destroy(ETE);
	mlfw_column_vec_double_destroy(ITE);
        mlfw_column_vec_double_destroy(TMP);
	mlfw_column_vec_double_destroy(UM);
        mlfw_column_vec_double_destroy(m);
	mlfw_mat_double_destroy(IT);
        return NULL;
	}

	// release resources
		
	mlfw_column_vec_double_destroy(P);
	mlfw_column_vec_double_destroy(E);
	mlfw_row_vec_double_destroy(ET);
	mlfw_column_vec_double_destroy(ETE);
	mlfw_column_vec_double_destroy(ITE);
	mlfw_column_vec_double_destroy(TMP);
	mlfw_column_vec_double_destroy(UM);
	mlfw_mat_double_destroy(IT);
	trained_parameters=mlfw_column_vec_double_transpose(m,NULL);
	mlfw_column_vec_double_destroy(m);
        return trained_parameters;
}

mlfw_column_vec_double * mlfw_linear_regression_predict(mlfw_mat_double *input_features_matrix,mlfw_row_vec_double *trained_parameters)
{
	dimension_t trained_parameters_size;
	mlfw_mat_double *I;
	dimension_t I_rows;
	dimension_t I_columns;

	mlfw_column_vec_double *m;

	mlfw_column_vec_double *P;

	if(input_features_matrix==NULL || trained_parameters==NULL) return NULL;

	I=input_features_matrix;
	mlfw_mat_double_get_dimensions(I,&I_rows,&I_columns);

	trained_parameters_size=mlfw_row_vec_double_get_size(trained_parameters);
	if(trained_parameters_size!=I_columns+1) return NULL;

	mlfw_mat_double_reshape(&I,I_rows,I_columns+1);
	if(I==NULL) return NULL;
	I_columns=I_columns+1;
	mlfw_mat_double_right_shift(I,1);
	mlfw_mat_double_fill(I,0,0,I_rows-1,0,1.0);

	m=mlfw_row_vec_double_transpose(trained_parameters,NULL);
	if(m==NULL)
	{
		mlfw_mat_double_left_shift(I,1);
		mlfw_mat_double_reshape(&I,I_rows,I_columns-1);
		return NULL;
	}

	P=mlfw_multiply_double_matrix_with_column_vector(I,m,NULL);
	if(P==NULL)
	{
		mlfw_mat_double_left_shift(I,1);
		mlfw_mat_double_reshape(&I,I_rows,I_columns-1);
		mlfw_column_vec_double_destroy(m);
		return NULL;
	}

	mlfw_mat_double_left_shift(I,1);
	mlfw_mat_double_reshape(&I,I_rows,I_columns-1);
	mlfw_column_vec_double_destroy(m);
	return P;   // return the column vector with predicted values
}
