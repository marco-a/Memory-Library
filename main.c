#include "utilities.h"

int main(int argc, const char * argv[]) {
	uint_t *array;
	
	// Allocate 10 ints
	array = mem_alloc(10, uint_t);

	if (array != nil) {
		fprintf(stderr, "Sucessfully allocated %zu ints\n", mem_get_num_elements(array));

		array[0] = 1;
		array[1] = 3;
		array[2] = 3;
		array[3] = 7;

		// Shrink array to 5 ints
		if (mem_shrink(array, 5)) {
			fprintf(stderr, "Successfully shrank to %zu ints\n", mem_get_num_elements(array));

			// Grew array to 10 ints
			if (mem_grow(array, 5)) {
				fprintf(stderr, "Successfully grew to %zu ints\n", mem_get_num_elements(array));

				fprintf(stderr, "Number is: %u%u%u%u\n", array[0], array[1], array[2], array[3]);

				mem_dump(array);
			}
		}

		if (mem_release(array)) {
			fprintf(stderr, "Memory deallocated successfully!\n\n");
		}

		array = mem_alloc_static(10, uint_t);

		if (array != nil) {
			fprintf(stderr, "Successfully allocated %zu Bytes\n", mem_get_static_size(array));

			array[0] = 4;
			array[1] = 2;

			mem_dump_static(array);

			fprintf(stderr, "Number is: %u%u\n", array[0], array[1]);

			if (mem_free(array)) {
				fprintf(stderr, "Static memory deallocated succesfully!\n\n");
			}
		}
	}
	
	return 0;
}

