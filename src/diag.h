#ifndef _MACROS_H_
#define _MACROS_H_

/* Call _Pragma(...) macro with stringified argument */
#define PRAGMA(x)	_Pragma(#x)

/* Have the compiler print a TODO mesage */
#define TODO(msg)	PRAGMA( message( "TODO: " msg ) )

#endif /* _MACROS_H_ */
