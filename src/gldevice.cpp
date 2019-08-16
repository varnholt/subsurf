#include "gldevice.h"

#ifdef WIN32
PFNGLACTIVETEXTUREARBPROC        glActiveTexture= NULL;
PFNGLCLIENTACTIVETEXTUREPROC     glClientActiveTexture= NULL;
PFNGLMULTITEXCOORD3FPROC         glMultiTexCoord3f= NULL;
PFNGLMULTITEXCOORD2FPROC         glMultiTexCoord2f= NULL;
#endif

// vertex buffer objects
PFNGLGENBUFFERSARBPROC           glGenBuffers=  NULL;
PFNGLBINDBUFFERARBPROC           glBindBuffer=  NULL;
PFNGLBUFFERDATAARBPROC           glBufferData=  NULL;
PFNGLDELETEBUFFERSARBPROC        glDeleteBuffers=  NULL;
PFNGLMAPBUFFERPROC               glMapBuffer=  NULL;
PFNGLUNMAPBUFFERPROC             glUnmapBuffer= NULL;

// shader stuff
PFNGLCREATESHADEROBJECTARBPROC   glCreateShaderObject=  NULL;
PFNGLSHADERSOURCEARBPROC         glShaderSource=  NULL;
PFNGLCOMPILESHADERARBPROC        glCompileShader=  NULL;
PFNGLCREATEPROGRAMOBJECTARBPROC  glCreateProgramObject=  NULL;
PFNGLATTACHOBJECTARBPROC         glAttachObject=  NULL;
PFNGLLINKPROGRAMARBPROC          glLinkProgram=  NULL;
PFNGLUSEPROGRAMOBJECTARBPROC     glUseProgramObject=  NULL;
PFNGLGETOBJECTPARAMETERFVARBPROC glGetObjectParameterfv=  NULL;
PFNGLGETOBJECTPARAMETERIVARBPROC glGetObjectParameteriv=  NULL;
PFNGLGETINFOLOGARBPROC           glGetInfoLog= NULL;
PFNGLGETUNIFORMLOCATIONARBPROC   glGetUniformLocation=  NULL;
PFNGLDELETEOBJECTARBPROC         glDeleteObject=  NULL;

// shader variablen
PFNGLUNIFORM1IARBPROC            glUniform1i=  NULL;
PFNGLUNIFORM1FARBPROC            glUniform1f=  NULL;
PFNGLUNIFORM2FARBPROC            glUniform2f=  NULL;
PFNGLUNIFORM3FARBPROC            glUniform3f=  NULL;
PFNGLUNIFORM4FARBPROC            glUniform4f=  NULL;
PFNGLUNIFORM1FVARBPROC           glUniform1fv=  NULL;
PFNGLUNIFORM2FVARBPROC           glUniform2fv=  NULL;
PFNGLUNIFORM3FVARBPROC           glUniform3fv=  NULL;
PFNGLUNIFORM4FVARBPROC           glUniform4fv=  NULL;

// frame buffer objects
PFNGLGENFRAMEBUFFERSEXTPROC      glGenFramebuffers=  NULL;
PFNGLBINDFRAMEBUFFEREXTPROC      glBindFramebuffer=  NULL;
PFNGLGENRENDERBUFFERSEXTPROC     glGenRenderbuffers=  NULL;
PFNGLRENDERBUFFERSTORAGEEXTPROC  glRenderbufferStorage= NULL;
PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC glFramebufferRenderbuffer= NULL;
PFNGLFRAMEBUFFERTEXTURE2DEXTPROC glFramebufferTexture2D= NULL;
PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC glCheckFramebufferStatus= NULL;
PFNGLBINDRENDERBUFFEREXTPROC     glBindRenderbuffer= NULL;
PFNGLRENDERBUFFERSTORAGEMULTISAMPLEEXTPROC glRenderbufferStorageMultisample= NULL;
PFNGLBLITFRAMEBUFFERPROC         glBlitFramebuffer= NULL;

// is set to false if one of the above functions could not be loaded
static bool extensionsAvailable= false;

// pick a function-pointer from the device driver by name
void* getProcAddress(const char* name)
{
   void *ptr= NULL;

#ifdef WIN32
   ptr= (void*)wglGetProcAddress(name);
#else
   //ptr= (void*)glXGetProcAddressARB((GLubyte*)name);
#endif

   if (!ptr)
   {
      qDebug("Fatal error: extension '%s' does not exist!\n", name);
      extensionsAvailable= false;
   }

   return ptr;
}

// load required extensions
bool initExtensions()
{
   extensionsAvailable= true;

#ifdef WIN32
   // multi-texturing
   glClientActiveTexture= (PFNGLCLIENTACTIVETEXTUREPROC)getProcAddress("glClientActiveTextureARB");
   glActiveTexture=       (PFNGLACTIVETEXTUREARBPROC) getProcAddress("glActiveTextureARB");
   glMultiTexCoord3f=     (PFNGLMULTITEXCOORD3FPROC) getProcAddress("glMultiTexCoord3fARB");
   glMultiTexCoord2f=     (PFNGLMULTITEXCOORD2FPROC) getProcAddress("glMultiTexCoord2fARB");
#endif

   // vertex buffer objects
   glGenBuffers=          (PFNGLGENBUFFERSARBPROC)    getProcAddress("glGenBuffersARB");
   glBindBuffer=          (PFNGLBINDBUFFERARBPROC)    getProcAddress("glBindBufferARB");
   glBufferData=          (PFNGLBUFFERDATAARBPROC)    getProcAddress("glBufferDataARB");
   glDeleteBuffers=       (PFNGLDELETEBUFFERSARBPROC) getProcAddress("glDeleteBuffersARB");
   glMapBuffer=           (PFNGLMAPBUFFERPROC)        getProcAddress("glMapBufferARB");
   glUnmapBuffer=         (PFNGLUNMAPBUFFERPROC)      getProcAddress("glUnmapBufferARB");

   // shader stuff
   glCreateShaderObject=  (PFNGLCREATESHADEROBJECTARBPROC)  getProcAddress("glCreateShaderObjectARB");
   glShaderSource=        (PFNGLSHADERSOURCEARBPROC)        getProcAddress("glShaderSourceARB");
   glCompileShader=       (PFNGLCOMPILESHADERARBPROC)       getProcAddress("glCompileShaderARB");
   glCreateProgramObject= (PFNGLCREATEPROGRAMOBJECTARBPROC) getProcAddress("glCreateProgramObjectARB");
   glAttachObject=        (PFNGLATTACHOBJECTARBPROC)        getProcAddress("glAttachObjectARB");
   glLinkProgram=         (PFNGLLINKPROGRAMARBPROC)         getProcAddress("glLinkProgramARB");
   glUseProgramObject=    (PFNGLUSEPROGRAMOBJECTARBPROC)    getProcAddress("glUseProgramObjectARB");
   glGetObjectParameterfv=(PFNGLGETOBJECTPARAMETERFVARBPROC)getProcAddress("glGetObjectParameterfvARB");
   glGetObjectParameteriv=(PFNGLGETOBJECTPARAMETERIVARBPROC)getProcAddress("glGetObjectParameterivARB");
   glGetUniformLocation=  (PFNGLGETUNIFORMLOCATIONARBPROC)  getProcAddress("glGetUniformLocationARB");
   glDeleteObject=        (PFNGLDELETEOBJECTARBPROC)        getProcAddress("glDeleteObjectARB");
   glUniform1i=           (PFNGLUNIFORM1IARBPROC)           getProcAddress("glUniform1iARB");
   glUniform1f=           (PFNGLUNIFORM1FARBPROC)           getProcAddress("glUniform1fARB");
   glUniform2f=           (PFNGLUNIFORM2FARBPROC)           getProcAddress("glUniform2fARB");
   glUniform3f=           (PFNGLUNIFORM3FARBPROC)           getProcAddress("glUniform3fARB");
   glUniform4f=           (PFNGLUNIFORM4FARBPROC)           getProcAddress("glUniform4fARB");
   glUniform1fv=          (PFNGLUNIFORM1FVARBPROC)          getProcAddress("glUniform1fvARB");
   glUniform2fv=          (PFNGLUNIFORM2FVARBPROC)          getProcAddress("glUniform2fvARB");
   glUniform3fv=          (PFNGLUNIFORM3FVARBPROC)          getProcAddress("glUniform3fvARB");
   glUniform4fv=          (PFNGLUNIFORM4FVARBPROC)          getProcAddress("glUniform4fvARB");
   glGetInfoLog=          (PFNGLGETINFOLOGARBPROC)          getProcAddress("glGetInfoLogARB");

   return extensionsAvailable;
}

// laedt eine 32bit (argb, width*height) auf die grafikkarte
// und gibt ein handle zurueck
unsigned int uploadTexture32(unsigned int *data, int width, int height)
{
   unsigned int tex;
   glGenTextures(1, &tex);
   glBindTexture(GL_TEXTURE_2D, tex);
   glTexImage2D(
      GL_TEXTURE_2D,
      0,
      GL_RGBA,
      width,
      height,
      0,
      GL_RGBA,
      GL_UNSIGNED_BYTE,
      data
   );
   // set linear filtering
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   // set texture repeating
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   return tex;
}


// bind sampler id to logical texture unit
// "id" is the texture sampler in the shader - get the id using getShaderVariable("samplerName")
// "unit" is the logical texture unit (GL_TEXTURE0, 1, 2, ...)
void bindSampler(int id, int unit)
{
   glUniform1i(id, unit);
}

// create vertex buffer. "size" in bytes
// this basically allocates gpu memory
unsigned int createVertexBuffer(int size, bool dyn)
{
   unsigned int buf = 0;

   glGenBuffers( 1, &buf);
   glBindBuffer( GL_ARRAY_BUFFER_ARB, buf );
   if (!dyn)
      glBufferData( GL_ARRAY_BUFFER_ARB, size, NULL, GL_STATIC_DRAW_ARB );
   else
      glBufferData( GL_ARRAY_BUFFER_ARB, size, NULL, GL_DYNAMIC_DRAW_ARB );

   return buf;
}


// lock vertex buffer object for writing
// returns a pointer to the locked buffer
// buffer cant be used for rendering until "unlock" is called
void* lockVertexBuffer(unsigned int buf)
{
   void *ptr= NULL;

   glBindBuffer( GL_ARRAY_BUFFER_ARB, buf );

   ptr= glMapBuffer( GL_ARRAY_BUFFER_ARB, GL_WRITE_ONLY );

   return ptr;
}

// unlock vertex buffer object
void unlockVertexBuffer(unsigned int buf)
{
   glBindBuffer( GL_ARRAY_BUFFER_ARB, buf );
   glUnmapBuffer( GL_ARRAY_BUFFER_ARB );
}

// create index buffer (triangle indices)
unsigned int createIndexBuffer(int size, bool dyn)
{
   unsigned int buf = 0;

   glGenBuffers( 1, &buf);
   glBindBuffer( GL_ELEMENT_ARRAY_BUFFER_ARB, buf );
   if (!dyn)
      glBufferData( GL_ELEMENT_ARRAY_BUFFER_ARB, size, NULL, GL_STATIC_DRAW_ARB );
   else
      glBufferData( GL_ELEMENT_ARRAY_BUFFER_ARB, size, NULL, GL_DYNAMIC_DRAW_ARB );

   return buf;
}

// lock index buffer for writing
void* lockIndexBuffer(unsigned int buf)
{
   void *ptr= NULL;

   glBindBuffer( GL_ELEMENT_ARRAY_BUFFER_ARB, buf );

   ptr= glMapBuffer( GL_ELEMENT_ARRAY_BUFFER_ARB, GL_WRITE_ONLY );

   return ptr;
}

// unlock index buffer
void unlockIndexBuffer(unsigned int buf)
{
   glBindBuffer( GL_ELEMENT_ARRAY_BUFFER_ARB, buf );
   glUnmapBuffer( GL_ELEMENT_ARRAY_BUFFER_ARB );
}

// delete vertex-/index-buffer
void deleteBuffer(unsigned int buffer)
{
   glDeleteBuffers(1, &buffer);
}
