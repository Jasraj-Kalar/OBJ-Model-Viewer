#ifndef GET_GL_ERROR
#define GET_GL_ERROR

#define GET_GL_ERRORS() glGetErrors(__LINE__, __FILE__)


// Public method(s)

void glGetErrors(int line, const char * file);

// Private method(s)


#endif
