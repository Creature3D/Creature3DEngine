
#include <rvrutils/CodeLineException.h>

using namespace rvrutils;

bool
BaseCodeLineException::sm_notify_on_exception = false;

bool
BaseCodeLineException::sm_dump_on_exception = false;

bool
BaseCodeLineException::sm_print_file = true;

bool
BaseCodeLineException::sm_print_line = true;

bool
BaseCodeLineException::sm_short_file = true;

unsigned int
BaseCodeLineException::sm_max_file_length = 15;
