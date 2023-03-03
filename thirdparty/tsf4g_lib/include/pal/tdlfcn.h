/*
**  @file $RCSfile: tdlfcn.h,v $
**  general description of this module
**  $Id: tdlfcn.h,v 1.2 2009-01-23 09:34:20 kent Exp $
**  @author $Author: kent $
**  @date $Date: 2009-01-23 09:34:20 $
**  @version $Revision: 1.2 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#ifndef TDLFCN_H
#define TDLFCN_H

#if defined(_WIN32) || defined(_WIN64)

#ifdef __cplusplus
extern "C"
{
#endif

	/* The MODE argument to `dlopen' contains one of the following: */
	#define RTLD_LAZY	0x00001	/* Lazy function call binding.  */
	#define RTLD_NOW	0x00002	/* Immediate function call binding.  */
	#define	RTLD_BINDING_MASK   0x3	/* Mask of binding time value.  */
	#define RTLD_NOLOAD	0x00004	/* Do not load the object.  */
	#define RTLD_DEEPBIND	0x00008	/* Use deep binding.  */

	/* If the following bit is set in the MODE argument to `dlopen',
	   the symbols of the loaded object and its dependencies are made
	   visible as if the object were linked directly into the program.  */
	#define RTLD_GLOBAL	0x00100

	/* Unix98 demands the following flag which is the inverse to RTLD_GLOBAL.
	   The implementation does this by default and so we can define the
	   value to zero.  */
	#define RTLD_LOCAL	0

	/* Do not delete object when closed.  */
	#define RTLD_NODELETE	0x01000

	/*
	#define dlopen(name, flag)		( (void*)LoadLibrary(name) )

	#define dlerror()				((char*)NULL)

	#define dlsym(handle, symbol) 	( (void*) GetProcAddress((HMODULE)handle, symbol) )

	#define dlclose(handle) 		( CloseHandle((HANDLE)handle) ? 0 : -1 )*/

	void *dlopen(const char *filename, int flag);
	char *dlerror(void);
	void *dlsym(void *handle, const char *symbol);
	int dlclose(void *handle);

#ifdef __cplusplus
}
#endif

#else

#include <dlfcn.h>

#endif

#endif /* TDLFCN_H */

