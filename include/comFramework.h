#pragma once
#ifndef ComFramework_h_included
#	define ComFramework_h_included
#endif

#include "../src/PlatformSdk.h"
#include "../src/StdLibrary.h"
#include "../src/libBoost.h"

#include "nstd/experimental/abstract.h"
#include "nstd/experimental/implicit.h"
#include "nstd/experimental/intern.h"
#include "nstd/experimental/satisfies.h"
#include "nstd/experimental/lengthof.h"

#include "meta/TagTypes.h"
#include "meta/TypeTraits.h"
#include "meta/Concepts.h"
#include "meta/Tuple.h"

#include "win/DWord.h"
#include "win/Exception.h"
#include "core/ThrowIfEmpty.h"
