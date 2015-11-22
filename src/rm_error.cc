#include <cerrno>
#include <cstdio>
#include <iostream>
#include "rm.h"

using namespace std;

//
// Error table for record manager relate things
//
static char* RM_WarnMsg[] = {

};

static char* RM_ErrorMsg[] = {
    (char*) "exceed the memory capacity of paged files", (char*) "blabla" };

void RM_PrintError(RC rc) {
  // Check the return code is within proper limits
  if (rc >= START_RM_WARN && rc <= END_RM_WARN)
    // Print warning
    cerr << "RM warning: " << RM_WarnMsg[rc - START_RM_WARN] << "\n";
  // Error codes are negative, so invert everything
  else if (-rc >= -START_RM_ERR && -rc < -END_RM_ERR)
    // Print error
    cerr << "RM error: " << RM_ErrorMsg[-rc + START_RM_ERR] << "\n";
  else
    cerr << "RM error: " << rc << " is out of bound of RM Error" << "\n";
}
