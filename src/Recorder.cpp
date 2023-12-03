#include "Recorder.h"

// what a mess
#define CHECK_WRITE(ptr, size, len, file) { if (size_t bwritten = fwrite(ptr, size, len, file) != len) { print_error("Error in write"); fprintf(stderr, "Expected %lu, got %lu\n", size * len, bwritten); } }
#define CHECK_READ(ptr, size, len, file) { if (size_t bread = fread(ptr, size, len, file) != len) { print_error("Error in read"); fprintf(stderr, "Expected %lu (%lu * %lu), got %lu\n", size * len, size, len, bread); } }
#define SEEK_ERROR(f) if (f == -1) { print_error("IO error"); }

RecorderOutput::RecorderOutput(const std::string &recordingPath)
{
	file = fopen(recordingPath.c_str(), "w");
	if (file == nullptr) {
		print_error("Error creating file");
	}
	
	// pre allocate file here.................................... didnt do it because will definitely fail on windows
	// starts at 0, 0, 0, needs to be changed right before closing. could just skip but this is safer
	MetaData meta(0, 0, 0);
	CHECK_WRITE(&meta, sizeof(MetaData), 1, file);
}

void RecorderOutput::setColorsAndMetadata(const ParticleColor *colors, GLuint total_ticks, GLuint total_particles) {
	long color_offset = ftell(file);
	CHECK_WRITE(colors, sizeof(ParticleColor), total_particles, file);

	rewind(file);
	MetaData meta(total_ticks, total_particles, color_offset);
	CHECK_WRITE(&meta, sizeof(MetaData), 1, file);


	// printf("saved metadata: %u ticks, %u particles and %lu offset\n", meta.total_ticks, meta.total_particles, meta.color_offset);
}

RecorderOutput::~RecorderOutput() {
	fflush(file);
	if (fclose(file) == EOF) {
		print_error("Error closing file");
	}
}

void RecorderOutput::saveFrame(GLuint len, const GLfloat *current_x, const GLfloat *current_y) {
	CHECK_WRITE(&len, sizeof(GLuint), 1, file);
	if (len > 0) {
		CHECK_WRITE(current_x, sizeof(GLfloat), len, file);
		// printf("offset after x is %lu (+%u)\n", ftell(file), len);
		CHECK_WRITE(current_y, sizeof(GLfloat), len, file);
		// printf("offset after y is %lu (+%u)\n", ftell(file), len);
	}
}






RecorderInput::RecorderInput(const std::string &recordingPath) {
	file = fopen(recordingPath.c_str(), "r");
	if (!file) {
		print_error("Error opening file");
	}
}

RecorderInput::~RecorderInput() {
	fflush(file);
	if (fclose(file) == EOF) {
		print_error("Error closing file");
	}
}

MetaData RecorderInput::readMetadata() {
	// rewind(file);
	MetaData data(0, 0, 0);
	CHECK_READ(&data, sizeof(MetaData), static_cast<size_t>(1), file);
	// printf("read metadata: %u ticks, %u particles and %lu offset\n", data.total_ticks, data.total_particles, data.color_offset);
	return data;
}

void RecorderInput::readColors(const MetaData &data, ParticleColor *colors) {
	SEEK_ERROR(fseek(file, data.color_offset, SEEK_SET));
	CHECK_READ(colors, sizeof(ParticleColor), static_cast<size_t>(data.total_particles), file);
}

GLuint RecorderInput::readFrame(GLfloat *current_x, GLfloat *current_y) {
	GLuint len = 0;
	CHECK_READ(&len, sizeof(GLuint), static_cast<size_t>(1), file);
	size_t wtf = static_cast<size_t>(len);
	// printf("len is %lu\n", wtf);
	if (wtf > 0) {
		// do I assume len exists now??
		CHECK_READ(current_x, sizeof(GLfloat), wtf, file);
		CHECK_READ(current_y, sizeof(GLfloat), wtf, file);
	}
	return len;
}

void RecorderInput::seekToFrames() {
	SEEK_ERROR(fseek(file, sizeof(MetaData), SEEK_SET));
}
