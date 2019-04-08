#pragma once

// This header defines the ALL_CODE_ANALYSIS_WARNINGS macro which contains the warnings list
#include <codeanalysis\warnings.h>

#pragma warning (push)
#pragma warning (disable: 4365 4917 ALL_CODE_ANALYSIS_WARNINGS)
#define LOCALDB_DEFINE_PROXY_FUNCTIONS
#include <sqlncli.h>
#pragma warning (pop)