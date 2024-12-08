#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef struct ECS_Vector
{
    void* elements;
    uint64_t element_count;
    size_t element_size;
} ECS_Vector;

bool ECS_Vector_New(size_t element_size, ECS_Vector** out_vector);
bool ECS_Vector_Add(ECS_Vector* vector, void* new_element);
void* ECS_Vector_Get(ECS_Vector* vector, uint64_t index);
void ECS_Vector_Free(ECS_Vector** vector);