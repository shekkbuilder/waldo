#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "arguments.h"

struct arguments_* alloc_arguments() {
	struct arguments_ *result = NULL;

	result = malloc (sizeof(struct arguments_));
	if (result == NULL) {
		fprintf(stderr, "Unable to alloc memory for arguments structure\n");
		return NULL;
	}

	result->hash_id = UINT_MAX;
	result->scatter_id = UINT_MAX;
	result->msg_from_file = 0;
	result->just_zeroize = 0;
	result->suffix = NULL;

	result->image = NULL;
	result->message = NULL;

	return result;
}

void free_arguments(struct arguments_ *args) {
	if (args == NULL)
		return;

	if (args->suffix != NULL)
		free (args->suffix);
	if (args->image != NULL)
		free (args->image);
	if (args->message != NULL)
		free (args->message);

	free(args);
}

void print_arguments(struct arguments_ *args) {
	printf("The parsed arguments structure:\n");
	printf("Hash id: %d\n", args->hash_id);
	printf("Scatter id: %d\n", args->scatter_id);
	printf("Message from file: %s\n", args->msg_from_file != 0 ? "yes" : "no");
	printf("Just zeroize: %s\n", args->just_zeroize != 0 ? "yes" : "no");
	printf("Output file suffix: %s\n", args->suffix);
	printf("Image file: %s\n", args->image);
	printf("Message: %s\n", args->message);
	printf("\n");
}

struct arguments_* parse_arguments(int argc, char *argv[]) {
	int i;
	struct arguments_ *result = alloc_arguments();

	for (i = 1; i < argc; ) {
		if (argv[i][0] != '-')
			break;

		switch(argv[i][1]) {
			case 'h':
				result->hash_id = atoi(argv[i+1]);
				i+=2;
				break;
			case 's':
				result->scatter_id = atoi(argv[i+1]);
				i+=2;
				break;
			case 'f':
				result->msg_from_file = 1;
				i++;
				break;
			case 'z':
				result->just_zeroize = 1;
				i++;
				break;
			case 'r':
				result->suffix = strdup(argv[i+1]);
				i+=2;
				break;
			default:
				fprintf(stderr, "Unrecognized parameter %s\n", argv[i]);
				free_arguments(result);
		}
	}

	result->image = strdup(argv[i]);
	i++;

	if (argv[i][0] == '-') {
		fprintf(stderr, "Wrong argument placement\n");
		goto out_fail;
	}

	result->message = strdup(argv[i]);

	return result;

out_fail:
	free_arguments(result);
	return NULL;
}
