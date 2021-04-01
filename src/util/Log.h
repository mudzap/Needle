#ifndef _LOG_
#define _LOG_

#include <stdio.h>
#include <iostream>
#include <string>

#define SHMY_LOGE(...) printf(__VA_ARGS__)
#define SHMY_LOGD(...) printf(__VA_ARGS__)
#define SHMY_LOGV(...) printf(__VA_ARGS__)

#if !defined SHMY_LOG_VERBOSE && !defined SHMY_LOG_DEBUG && !defined SHMY_LOG_ERROR
//printf("No log level set, setting debug level\n");
#define SHMY_LOG_DEBUG
#endif

#ifdef SHMY_LOG_VERBOSE
//printf("Verbose log level set\n");
#endif

#ifdef SHMY_LOG_DEBUG
//printf("Debug log level set\n");
#undef SHMY_LOGV
#define SHMY_LOGV(f_, ...)
#endif

#ifdef SHMY_LOG_ERROR
//printf("Error log level set\n");
#undef SHMY_LOGV
#define SHMY_LOGV(f_, ...)
#undef SHMY_LOGD
#define SHMY_LOGD(f_, ...)
#endif


#endif