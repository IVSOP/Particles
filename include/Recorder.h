#ifndef RECORDER_H
#define RECORDER_H

#include "common.h"
#include "Sandbox.h"


// I wanted to do some 'complex' things so I did things C style, I prefer it



struct MetaData {
	GLuint total_ticks;
	GLuint total_particles;
	long color_offset;

	MetaData(GLuint total_ticks, GLuint total_particles, size_t color_offset) : total_ticks(total_ticks), total_particles(total_particles), color_offset(color_offset) { }
};

// colors is just an array of ParticleColor, right after metadata

// frames are just after colors, and are
/*
	GLuint len
	array of current_x
	array of current_y
*/

// I had a skill issue trying to put this into nice structs since array size varies


// I don't care about having to repeat code, this is simpler to use
// also for simplicity, both of these classes are stateless except for their files

class RecorderOutput {
public:
	FILE *file;

	RecorderOutput() = delete;
	RecorderOutput(const std::string &recordingPath);
	~RecorderOutput();

	void setColorsAndMetadata(const ParticleColor *colors, GLuint total_ticks, GLuint total_particles);
	void saveFrame(GLuint len, const GLfloat *current_x, const GLfloat *current_y);
};

class RecorderInput {
public:
	FILE *file;

	RecorderInput() = delete;
	RecorderInput(const std::string &recordingPath);
	~RecorderInput();

	MetaData readMetadata(); // use this to allocate sandbox size, get number of frames to read, etc.
	void readColors(const MetaData &data, ParticleColor *colors);
	GLuint readFrame(GLfloat *current_x, GLfloat *current_y);
	void seekToFrames();
};

#endif
