#include "common.h"

// will break on huge files, bad error checking
const GLchar *readFromFile(char *filepath) {
	FILE *fp = fopen(filepath, "r");
	if (fp == NULL) {
		fprintf(stderr, "%s\n", filepath);
		perror("Invalid file path");
		exit(1);
	}
	fseek(fp, 0L, SEEK_END);
	unsigned long size = ftell(fp);
	fseek(fp, 0L, SEEK_SET);
	GLchar *ret = new GLchar[size + 1];
	size_t charread = fread(ret, sizeof(GLchar), size, fp);
	if (charread != size) {
		fprintf(stderr, "Did not read all chars: %ld vs %ld\n", size, charread);
		exit(1);
	}
	ret[charread] = '\0';
	fclose(fp);
	return ret;
}
