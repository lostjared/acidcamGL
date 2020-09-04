#ifndef __GET_OPT__H_
#define __GET_OPT__H_

#include<cstdio>
#include<cstring>
extern int opterr,
  optind,
  optopt,
  optreset;
extern char    *optarg;

// found this on stackoverflow
extern int getopt(int nargc, char * const nargv[], const char *ostr);

#endif
