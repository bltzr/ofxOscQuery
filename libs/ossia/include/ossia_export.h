
#ifndef OSSIA_EXPORT_H
#define OSSIA_EXPORT_H

#ifdef OSSIA_STATIC_DEFINE
#  define OSSIA_EXPORT
#  define OSSIA_NO_EXPORT
#else
#  ifndef OSSIA_EXPORT
#    ifdef ossia_EXPORTS
        /* We are building this library */
#      define OSSIA_EXPORT 
#    else
        /* We are using this library */
#      define OSSIA_EXPORT 
#    endif
#  endif

#  ifndef OSSIA_NO_EXPORT
#    define OSSIA_NO_EXPORT 
#  endif
#endif

#ifndef OSSIA_DEPRECATED
#  define OSSIA_DEPRECATED __attribute__ ((__deprecated__))
#endif

#ifndef OSSIA_DEPRECATED_EXPORT
#  define OSSIA_DEPRECATED_EXPORT OSSIA_EXPORT OSSIA_DEPRECATED
#endif

#ifndef OSSIA_DEPRECATED_NO_EXPORT
#  define OSSIA_DEPRECATED_NO_EXPORT OSSIA_NO_EXPORT OSSIA_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef OSSIA_NO_DEPRECATED
#    define OSSIA_NO_DEPRECATED
#  endif
#endif

#endif
