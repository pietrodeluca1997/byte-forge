#include "ECS_Vector.h"
#include <stdlib.h>
#include <string.h>

bool ECS_Vector_New(size_t element_size, ECS_Vector** out_vector)
{
    if (!out_vector || element_size == 0) return false;
    
    *out_vector = (ECS_Vector*) calloc(1, sizeof(ECS_Vector));

    if(!*out_vector) return false;

    (*out_vector)->element_size = element_size;

    return true;
}

bool ECS_Vector_Add(ECS_Vector* vector, void* new_element)
{
    if (!vector || !new_element) return false;

    vector->elements = realloc(vector->elements, vector->element_size * (vector->element_count + 1));

    if (!vector->elements) return false;

    memcpy((char*) vector->elements + vector->element_size * vector->element_count, new_element, vector->element_size);
    
    vector->element_count++;

    return true;
}

void* ECS_Vector_Get(ECS_Vector* vector, uint64_t index)
{
    if (!vector || index >= vector->element_count) return NULL;
    
    return vector->elements + vector->element_size * index;
}

void ECS_Vector_Free(ECS_Vector **vector)
{
    if (!vector || !*vector) return;
    
    free((*vector)->elements);
    free(*vector);
    *vector = NULL;
}