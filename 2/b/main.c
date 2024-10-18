#include "array.h"


int main()
{
    Array arr = array_new(2);
    array_insertBack(&arr, 3);
    array_insertBack(&arr, 4);
    array_insertBack(&arr, 5);
    array_insertBack(&arr, 6);
    array_insertBack(&arr, 7);
    array_insertBack(&arr, 8);
    array_insertBack(&arr, 9);
	array_print(arr);
	array_destroy(arr);
    return 0;
}
   /*  // Check if the array is full
  if (a->back == a->capacity) {
    // Reallocate memory if full
   long new_capacity = a->capacity * 2;  // Double the capacity
    long *new_data = realloc(a->data, sizeof(long) * new_capacity);
    assert(new_data != NULL);  // Check for realloc failure

    // Update array data pointer and capacity
    a->data = new_data;
    a->capacity = new_capacity;
  }*/
