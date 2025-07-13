#ifndef __MLFW__SET__STRING__
#define __MLFW__SET__STRING__

struct __mlfw_set_string;
typedef struct __mlfw_set_string mlfw_set_string;

mlfw_set_string * mlfw_set_string_create_new();
void mlfw_set_string_destroy(mlfw_set_string *set);

void mlfw_set_string_get(mlfw_set_string *set,index_t i,char **string);
// returns -1 in case of low memory or 0 incase of added or not added because of duplicate
int mlfw_set_string_add(mlfw_set_string *set,char *string);

dimension_t mlfw_set_string_get_size(mlfw_set_string *set);

#endif
