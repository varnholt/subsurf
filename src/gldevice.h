#pragma once

#include <QGLWidget>
#include "glext.h"

bool initExtensions();

// multi texturing
#ifdef WIN32
// multi-texture: die gibt's schon unter linux!
extern PFNGLCLIENTACTIVETEXTUREPROC glClientActiveTexture;
extern PFNGLACTIVETEXTUREARBPROC    glActiveTexture;
extern PFNGLMULTITEXCOORD3FPROC     glMultiTexCoord3f;
extern PFNGLMULTITEXCOORD2FPROC     glMultiTexCoord2f;
#endif

// vertex buffer objects
extern PFNGLGENBUFFERSARBPROC    glGenBuffers;
extern PFNGLBINDBUFFERARBPROC    glBindBuffer;
extern PFNGLBUFFERDATAARBPROC    glBufferData;
extern PFNGLDELETEBUFFERSARBPROC glDeleteBuffers;
extern PFNGLMAPBUFFERPROC        glMapBuffer;
extern PFNGLUNMAPBUFFERPROC      glUnmapBuffer;

// shader stuff
extern PFNGLCREATESHADEROBJECTARBPROC   glCreateShaderObject;
extern PFNGLSHADERSOURCEARBPROC         glShaderSource;
extern PFNGLCOMPILESHADERARBPROC        glCompileShader;
extern PFNGLCREATEPROGRAMOBJECTARBPROC  glCreateProgramObject;
extern PFNGLATTACHOBJECTARBPROC         glAttachObject;
extern PFNGLLINKPROGRAMARBPROC          glLinkProgram;
extern PFNGLUSEPROGRAMOBJECTARBPROC     glUseProgramObject;
extern PFNGLGETOBJECTPARAMETERFVARBPROC glGetObjectParameterfv;
extern PFNGLGETOBJECTPARAMETERIVARBPROC glGetObjectParameteriv;
extern PFNGLGETINFOLOGARBPROC           glGetInfoLog;
extern PFNGLGETUNIFORMLOCATIONARBPROC   glGetUniformLocation;
extern PFNGLDELETEOBJECTARBPROC         glDeleteObject;

extern PFNGLUNIFORM1IARBPROC            glUniform1i;
extern PFNGLUNIFORM1FARBPROC            glUniform1f;
extern PFNGLUNIFORM2FARBPROC            glUniform2f;
extern PFNGLUNIFORM3FARBPROC            glUniform3f;
extern PFNGLUNIFORM4FARBPROC            glUniform4f;
extern PFNGLUNIFORM1FVARBPROC           glUniform1fv;
extern PFNGLUNIFORM2FVARBPROC           glUniform2fv;
extern PFNGLUNIFORM3FVARBPROC           glUniform3fv;
extern PFNGLUNIFORM4FVARBPROC           glUniform4fv;

unsigned int uploadTexture32(unsigned int *data, int width, int height);


// bind sampler id to logical texture unit
// "id" is the texture sampler in the shader - get the id using getShaderVariable("samplerName")
// "unit" is the logical texture unit (GL_TEXTURE0, 1, 2, ...)
void bindSampler(int id, int unit);
unsigned int createVertexBuffer(int size, bool dyn = false);
void* lockVertexBuffer(unsigned int buf);
void unlockVertexBuffer(unsigned int buf);
unsigned int createIndexBuffer(int size, bool dyn = false);
void* lockIndexBuffer(unsigned int buf);
void unlockIndexBuffer(unsigned int buf);
void deleteBuffer(unsigned int buffer);

