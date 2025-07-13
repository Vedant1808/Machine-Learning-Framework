#ifndef __MLFW__DATA__ENCODER__
#define __MLFW__DATA__ENCODER__
#include<mlfw_vector.h>
void mlfw_encoder_encode_one_hot(char *source,char *target,mlfw_row_vec_string *columns_to_encode);
void mlfw_encoder_encode_binary(char *source,char *target,mlfw_row_vec_string *columns_to_encode);
void mlfw_encoder_encode(char *source,char *target,mlfw_row_vec_string *columns_to_encode,char *algorithm);
#endif
