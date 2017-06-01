//
//  services.h
//  chip8emu
//
//  Created by Venkat Srinivasan on 2/22/17.
//  Copyright © 2017 Venkat Srinivasan. All rights reserved.
//

#ifndef services_h
#define services_h

#include <stdio.h>
#include <inttypes.h>

#if defined(MASTER_DEBUG)
#define  debug_verbose
#define  error_verbose
#define  access_verbose
#endif

void mypanic(char* errmsg);

void assertionfail();
#define MYASSERT(exp)  if (!exp) assertionfail();

#if defined(debug_verbose)
    #define DEBUGPRINT(_exp) do { _exp fflush(0); } while (0)
#else
    #define DEBUGPRINT(_exp) do { } while (0)
#endif

#if defined(error_verbose)
#define ERRORPRINT(_exp) do { _exp fflush(0); } while (0)
#else
#define ERRORPRINT(_exp) do { } while (0)
#endif

#if defined(access_verbose)
#define ACCESSPRINT(_exp) do { _exp fflush(0); } while (0)
#else
#define ACCESSPRINT(_exp) do { } while (0)
#endif

#endif /* services_h */
