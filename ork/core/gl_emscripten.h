#ifndef _GL_EMSCRIPTEN_H_
#define _GL_EMSCRIPTEN_H_

#ifdef __cplusplus
extern "C" {
#endif

void* emscripten_GetProcAddress_full(const char *name_);

#ifdef __cplusplus
}
#endif

#endif
