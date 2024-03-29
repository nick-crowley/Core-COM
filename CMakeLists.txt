##########################################################################
# Makefile                                                               #
#  _________                          _________  ________      _____     #
#  \_   ___ \  ___________   ____     \_   ___ \ \_____  \    /     \    #
#  /    \  \/ /  _ \_  __ \_/ __ \    /    \  \/  /   |   \  /  \ /  \   #
#  \     \___(  <_> )  | \/\  ___/    \     \____/    |    \/    Y    \  #
#   \______  /\____/|__|    \___  >    \______  /\_______  /\____|__  /  #
#          \/                   \/            \/         \/         \/   #
##########################################################################
cmake_minimum_required (VERSION 3.8.2)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Solution: CoreCOM
project (CoreCOM
	#DESCRIPTION "CORE library to simplify developing COM components"
	LANGUAGES CXX)
message("solution: ${PROJECT_SOURCE_DIR}")

# Define base folder
set(BASE_FOLDER "${CMAKE_CURRENT_SOURCE_DIR}/")
set(OUTPUT_FOLDER "${BASE_FOLDER}build")

# Folders: Project
set(PROJECT_EXTRA_FOLDER    "${BASE_FOLDER}extra/")
set(PROJECT_INCLUDE_FOLDER  "${BASE_FOLDER}include/")
set(PROJECT_SAMPLE_FOLDER   "${BASE_FOLDER}sample/")
set(PROJECT_SOURCE_FOLDER   "${BASE_FOLDER}src/")
set(PROJECT_RESOURCE_FOLDER "${BASE_FOLDER}res/")

# Folders: Test code
set(PROJECT_TEST_FOLDER     "${BASE_FOLDER}test/")
set(PROJECT_MOCK_FOLDER     "${BASE_FOLDER}mock/")

# Folders: Google.Mock
set(GMOCK_FOLDER         "${BASE_FOLDER}3rdparty/googlemock/")
set(GMOCK_INCLUDE_FOLDER "${GMOCK_FOLDER}include/")
set(GMOCK_SOURCE_FOLDER  "${GMOCK_FOLDER}src/")

# Folders: Google.Test
set(GTEST_FOLDER         "${BASE_FOLDER}3rdparty/googletest/")
set(GTEST_INCLUDE_FOLDER "${GTEST_FOLDER}include/")
set(GTEST_SOURCE_FOLDER  "${GTEST_FOLDER}src/")

# Set Guideline Support Library contract violation behaviour
set(GSL_ASSERTION_BEHAVIOUR "GSL_THROW_ON_CONTRACT_VIOLATION")

# Set compiler options used by all modules
# /std:c++latest	: Preview ISO C++ 23 Standard
# /std:c17			: ISO C 17 Standard
# /permissive		: Support non-standard Microsoft COM attributes
# /Zc:enumTypes     : Deduce enumeration underlying type from enumerators, not vice versa
# /Zc:externC       : Strict enforcement of parameter types allowed in functions with C language linkage
# /Zc:externConstexpr : Allow external linkage of constexpr variables
# /Zc:lambda        : Enable conforming lambda grammar
# /Zc:hiddenFriend  : Enforces stricter name lookup for friend functions 
# /Zc:gotoScope     : Prevent skipping initialization of variables using goto
# /Zc:referenceBinding : Prevent non-const lvalue references binding to temporaries
# /Zc:strictStrings : Disable implicit conversion from string literals to mutable character-pointers
# /Zc:static_assert : Evaluate static_assert earlier for non-dependant expressions
# /Zc:rvalueCast    : Stricter interpretation of value-categories produced by casts
# /Zc:ternary       : Disable non-standard conversions in ternary operator operands
# /Zc:templateScope : Prevent shadowing template parameters
# /Zc:preprocessor	: ISO C++20 Preprocessor
# /Zc:__cplusplus   : Define __cplusplus macro
# /ZI				: PDBs for edit & continue
# /bigobj           : Increase COFF section limit from 2^16 to 2^32
# /EHa				: Enable SEH and C++ exceptions
# /W4				: Warning level 4
# /wd4100			: Disable W4100: Unreferenced formal parameter
# /wd4251			: Disable W4251: Classes containing fields of STL type (CRT compatibility warning)
# /wd4275			: Disable W4275: Classes containing bases of STL type (CRT compatibility warning)
# /wd4467           : Disable C4467: Usage of ATL attributes is deprecated
# /wd4684           : Disable C4684: 'no_injected_text': WARNING!! attribute may cause invalid code generation: use with caution
# /wd4543           : Disable C4543: Injected text suppressed by attribute 'no_injected_text'
set(GLOBAL_COMPILER_OPTIONS "/std:c++latest /std:c17 /permissive /Zc:enumTypes /Zc:externC /Zc:externConstexpr /Zc:lambda /Zc:hiddenFriend /Zc:gotoScope /Zc:referenceBinding /Zc:strictStrings /Zc:rvalueCast /Zc:ternary /Zc:templateScope /Zc:preprocessor /Zc:__cplusplus /ZI /bigobj /EHa /W4 /wd4100 /wd4251 /wd4275 /wd4467 /wd4684 /wd4543")

# Set linker options used by all modules
set(GLOBAL_LINKER_OPTIONS "${GLOBAL_LINKER_OPTIONS} /SAFESEH:NO")

# Set global preprocessor directives:
# _ATL_ATTRIBUTES                           : Enable non-standard Microsoft COM attributes
# BOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE 	: Disable MSVC-2017 forward-compatibility warning
# _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS 	: Disable C++17 deprecation warnings
# _SILENCE_CXX20_CODECVT_FACETS_DEPRECAT... : Disable C++20 codecvt deprecation warning
# _CRT_SECURE_NO_WARNINGS 					: Disable warnings about documented behaviour of CRT functions
# _SCL_SECURE_NO_WARNINGS					: Disable warnings about documented behaviour of C++ library functions
# GSL_ASSERTION_BEHAVIOUR 					: Globally set the contract-violation behaviour of GSL
# _UNICODE 									: Build everything in the UNICODE character set
add_definitions(-D_ATL_ATTRIBUTES
	-DBOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE 
	-D_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
	-D_SILENCE_CXX20_CODECVT_FACETS_DEPRECATION_WARNING
	-D_CRT_SECURE_NO_WARNINGS
	-D_SCL_SECURE_NO_WARNINGS
	-D${GSL_ASSERTION_BEHAVIOUR}
	-D_UNICODE
	-DNOMINMAX
)

# Set global include folders: 
# $ENV{BOOST_ROOT}	:	Boost headers
# $ENV{GSL_ROOT}	:	Guideline Support Library (GSL) headers
# Core-Platform     :	Workaround for referring to another project
include_directories($ENV{BOOST_ROOT}
	$ENV{GSL_ROOT}
	"${BASE_FOLDER}../Core-Platform/include"
)

# Set global library folders:
# $ENV{BOOST_ROOT}/stage/lib	: Boost libraries
# ${OUTPUT_FOLDER}				: Solution output folder
link_directories($ENV{BOOST_ROOT}/stage/lib ${OUTPUT_FOLDER})
	
# Remove the 'RelWithDebInfo' and 'MinSizeRel' configurations
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE TYPE INTERNAL FORCE )
#set(CMAKE_CONFIGURATION_TYPES Debug CACHE TYPE INTERNAL FORCE )

# Source files
file(GLOB_RECURSE PROJECT_SOURCE_FILES
    "${PROJECT_SOURCE_FOLDER}*.h"
    "${PROJECT_SOURCE_FOLDER}*.cpp"
)
	
# [OPTIMIZED] Source files to exclude by marking as header-only
file(GLOB_RECURSE PROJECT_SOURCE_IMPL_FILES
    "${PROJECT_SOURCE_FOLDER}*.cpp"
)
set_source_files_properties(${PROJECT_SOURCE_IMPL_FILES} PROPERTIES 
    HEADER_FILE_ONLY ON
)

# Optimized-build files
file(GLOB_RECURSE PROJECT_SOURCE_CC_FILES
    "${PROJECT_SOURCE_FOLDER}*.cc"
)
	
# Exported header files
file(GLOB_RECURSE PROJECT_INCLUDE_FILES
    "${PROJECT_INCLUDE_FOLDER}*.h"
)

# Define macro controlling dllexport/import specifier
set(PROJECT_EXPORT_MACRO "CORE_COM_EXPORT")
set(PROJECT_IMPORT_MACRO "CORE_COM_IMPORT")
set(PROJECT_STATIC_MACRO "CORE_COM_STATIC")

##############################################################################
#         _________ __          __  .__         .____    ._____.             #
#        /   _____//  |______ _/  |_|__| ____   |    |   |__\_ |__           #
#        \_____  \\   __\__  \\   __\  |/ ___\  |    |   |  || __ \          #
#        /        \|  |  / __ \|  | |  \  \___  |    |___|  || \_\ \         #
#       /_______  /|__| (____  /__| |__|\___  > |_______ \__||___  /         #
#               \/           \/             \/          \/       \/          #
##############################################################################
message("libCoreCOM: Generating static library project")

# Project: libCoreCOM
add_library(libCoreCOM STATIC 
	${PROJECT_SOURCE_FILES} 
	${PROJECT_SOURCE_CC_FILES} 
	${PROJECT_INCLUDE_FILES} 
)
  
# Include folders
target_include_directories(libCoreCOM BEFORE
    PRIVATE
	${PROJECT_SOURCE_FOLDER}
    PUBLIC
    ${PROJECT_INCLUDE_FOLDER}
)

# Set compile options
set_target_properties(libCoreCOM PROPERTIES
					  COMPILE_FLAGS "${GLOBAL_COMPILER_OPTIONS}"
)

# Set export macros to 'extern' (same for all dependencies)
target_compile_definitions(libCoreCOM
	PRIVATE CORE_PLATFORM_STATIC
	        ${PROJECT_STATIC_MACRO}
)

# Link CRT statically
target_compile_options(libCoreCOM
	PRIVATE $<$<CONFIG:Release>:/MT>
	PRIVATE $<$<CONFIG:Debug>:/MTd>
)

# Set project dependencies
#add_dependencies(libCoreCOM
#	libCorePlatform
#)

#############################################################################
#  ________                              .__          .____    ._____.      #
#  \______ \ ___.__. ____ _____    _____ |__| ____    |    |   |__\_ |__    #
#   |    |  <   |  |/    \\__  \  /     \|  |/ ___\   |    |   |  || __ \   #
#   |    `   \___  |   |  \/ __ \|  Y Y  \  \  \___   |    |___|  || \_\ \  #
#  /_______  / ____|___|  (____  /__|_|  /__|\___  >  |_______ \__||___  /  #
#          \/\/         \/     \/      \/        \/           \/       \/   #
#############################################################################
message("dllCoreCOM: Generating dynamic library project")

# Project: dllCoreCOM
add_library(dllCoreCOM SHARED 
	"${PROJECT_EXTRA_FOLDER}DllMain.cpp"
	${PROJECT_SOURCE_FILES} 
	${PROJECT_SOURCE_CC_FILES} 
	${PROJECT_INCLUDE_FILES} 
)

# Include folders
target_include_directories(dllCoreCOM BEFORE
    PRIVATE
	${PROJECT_SOURCE_FOLDER} 
    PUBLIC
    ${PROJECT_INCLUDE_FOLDER}
)

# Differentiate the filename of the import library
set_target_properties(dllCoreCOM PROPERTIES
					  OUTPUT_NAME "CoreCOM"
					  COMPILE_FLAGS "${GLOBAL_COMPILER_OPTIONS}"
)

# Set linker dependencies
target_link_libraries(dllCoreCOM
	#dllCorePlatform
	"${BASE_FOLDER}../Core-Platform/build/$(Configuration)/CorePlatform.lib"
)

# Set export macros to 'dllexport' for this library and 'dllimport' for dependencies
target_compile_definitions(dllCoreCOM
	PRIVATE CORE_PLATFORM_IMPORT
	        ${PROJECT_EXPORT_MACRO}
)

# Link CRT dynamically
target_compile_options(dllCoreCOM
	PRIVATE $<$<CONFIG:Release>:/MD>
	PRIVATE $<$<CONFIG:Debug>:/MDd>
)

# Set project dependencies
#add_dependencies(dllCoreCOM
#	dllCorePlatform
#)

#####################################################################
#    _____   __    __         ._____.           __                 	#
#   /  _  \_/  |__/  |________|__\_ |__  __ ___/  |_  ____   ______	#
#  /  /_\  \   __\   __\_  __ \  || __ \|  |  \   __\/ __ \ /  ___/	#
# /    |    \  |  |  |  |  | \/  || \_\ \  |  /|  | \  ___/ \___ \ 	#
# \____|__  /__|  |__|  |__|  |__||___  /____/ |__|  \___  >____  >	#
#         \/                          \/                 \/     \/ 	#
#####################################################################
message("dllAttributes: Generating attributes usage sample")

# Project: dllAttributes
add_library(dllAttributes SHARED 
	"${PROJECT_SAMPLE_FOLDER}dllAttributes.cpp"
	"${PROJECT_SAMPLE_FOLDER}dllAttributes.midl"
	"${PROJECT_SAMPLE_FOLDER}dllAttributes.def"
)

# Include folders
target_include_directories(dllAttributes BEFORE
    PRIVATE
	${PROJECT_SAMPLE_FOLDER} 
	${PROJECT_RESOURCE_FOLDER} 
    PUBLIC
    ${PROJECT_INCLUDE_FOLDER}            
)
	
# Set linker dependencies
target_link_libraries(dllAttributes
	libCoreCOM
	"${BASE_FOLDER}../Core-Platform/build/$(Configuration)/libCorePlatform.lib"
)

# Define MIDL flags
set(BASIC_SERVER_MIDL_OPTIONS "/MIDL:@${PROJECT_SAMPLE_FOLDER}dllAttributes.midl /IDLOUT:$(ProjectDir)MIDL\\$(ProjectName).idl")

# Set compiler & linker & MIDL flags
set_target_properties(dllAttributes PROPERTIES
					  COMPILE_FLAGS "${GLOBAL_COMPILER_OPTIONS}"
					  LINK_FLAGS "${GLOBAL_LINKER_OPTIONS} /SUBSYSTEM:WINDOWS ${BASIC_SERVER_MIDL_OPTIONS}"
					  OUTPUT_NAME "dllAttributes"
					  FOLDER "Samples"
)

# Set export macros for 'extern' for all dependencies
target_compile_definitions(dllAttributes
	PRIVATE CORE_PLATFORM_STATIC 
	        ${PROJECT_STATIC_MACRO} 
			CORE_BUILDING_DLL
)

# Link CRT statically
target_compile_options(dllAttributes
	PRIVATE $<$<CONFIG:Release>:/MT>
	PRIVATE $<$<CONFIG:Debug>:/MTd>
)

# Set project dependencies
add_dependencies(dllAttributes
	libCoreCOM
	#libCorePlatform
)

###########################################################################################
# .___      __________                          _________                                 #
# |   | ____\______   \_______  ____   ____    /   _____/ ______________  __ ___________  #
# |   |/    \|     ___/\_  __ \/  _ \_/ ___\   \_____  \_/ __ \_  __ \  \/ // __ \_  __ \ #
# |   |   |  \    |     |  | \(  <_> )  \___   /        \  ___/|  | \/\   /\  ___/|  | \/ #
# |___|___|  /____|     |__|   \____/ \___  > /_______  /\___  >__|    \_/  \___  >__|    #
#          \/                             \/          \/     \/                 \/        #
###########################################################################################
message("dllInProcServer: Generating in-process server sample")

# Project: dllInProcServer
add_library(dllInProcServer SHARED 
	"${PROJECT_SAMPLE_FOLDER}dllInProcServer.cpp"
	"${PROJECT_SAMPLE_FOLDER}dllInProcServer.midl"
	"${PROJECT_SAMPLE_FOLDER}dllInProcServer.def"
)

# Include folders
target_include_directories(dllInProcServer BEFORE
    PRIVATE
	${PROJECT_SAMPLE_FOLDER} 
	${PROJECT_RESOURCE_FOLDER} 
    PUBLIC
    ${PROJECT_INCLUDE_FOLDER}            
)
	
# Set linker dependencies
target_link_libraries(dllInProcServer
	libCoreCOM
	"${BASE_FOLDER}../Core-Platform/build/$(Configuration)/libCorePlatform.lib"
)

# Define MIDL flags
set(BASIC_SERVER_MIDL_OPTIONS "/MIDL:@${PROJECT_SAMPLE_FOLDER}dllInProcServer.midl /IDLOUT:$(ProjectDir)MIDL\\$(ProjectName).idl")

# Set compiler & linker & MIDL flags
set_target_properties(dllInProcServer PROPERTIES
					  COMPILE_FLAGS "${GLOBAL_COMPILER_OPTIONS}"
					  LINK_FLAGS "${GLOBAL_LINKER_OPTIONS} /SUBSYSTEM:WINDOWS ${BASIC_SERVER_MIDL_OPTIONS}"
					  OUTPUT_NAME "dllInProcServer"
					  FOLDER "Samples"
)

# Set export macros for 'extern' for all dependencies
target_compile_definitions(dllInProcServer
	PRIVATE CORE_PLATFORM_STATIC 
	        ${PROJECT_STATIC_MACRO} 
	        CORE_BUILDING_DLL
)

# Link CRT statically
target_compile_options(dllInProcServer
	PRIVATE $<$<CONFIG:Release>:/MT>
	PRIVATE $<$<CONFIG:Debug>:/MTd>
)

# Set project dependencies
add_dependencies(dllInProcServer
	libCoreCOM
	#libCorePlatform
)

######################################################################################################
# ________          __ __________                          _________                                 #
# \_____  \  __ ___/  |\______   \_______  ____   ____    /   _____/ ______________  __ ___________  #
#  /   |   \|  |  \   __\     ___/\_  __ \/  _ \_/ ___\   \_____  \_/ __ \_  __ \  \/ // __ \_  __ \ #
# /    |    \  |  /|  | |    |     |  | \(  <_> )  \___   /        \  ___/|  | \/\   /\  ___/|  | \/ #
# \_______  /____/ |__| |____|     |__|   \____/ \___  > /_______  /\___  >__|    \_/  \___  >__|    #
#         \/                                         \/          \/     \/                 \/        #
######################################################################################################
message("exeOutProcServer: Generating out-of-process server sample")

# Project: exeOutProcServer
add_executable(exeOutProcServer 
	"${PROJECT_SAMPLE_FOLDER}exeOutProcServer.cpp"
	"${PROJECT_SAMPLE_FOLDER}exeOutProcServer.midl"
)

# Include folders
target_include_directories(exeOutProcServer BEFORE
    PRIVATE
	${PROJECT_SAMPLE_FOLDER} 
	${PROJECT_RESOURCE_FOLDER} 
    PUBLIC
    ${PROJECT_INCLUDE_FOLDER}            
)
	
# Set linker dependencies
target_link_libraries(exeOutProcServer
	libCoreCOM
	"${BASE_FOLDER}../Core-Platform/build/$(Configuration)/libCorePlatform.lib"
)

# Define MIDL flags
set(BASIC_SERVER_MIDL_OPTIONS "/MIDL:@${PROJECT_SAMPLE_FOLDER}exeOutProcServer.midl /IDLOUT:$(ProjectDir)MIDL\\$(ProjectName).idl")

# Set compiler & linker & MIDL flags
set_target_properties(exeOutProcServer PROPERTIES
					  COMPILE_FLAGS "${GLOBAL_COMPILER_OPTIONS}"
					  LINK_FLAGS "${GLOBAL_LINKER_OPTIONS} /SUBSYSTEM:WINDOWS ${BASIC_SERVER_MIDL_OPTIONS}"
					  OUTPUT_NAME "exeOutProcServer"
					  FOLDER "Samples"
)

# Set export macros for 'extern' for all dependencies
target_compile_definitions(exeOutProcServer
	PRIVATE CORE_PLATFORM_STATIC 
	        ${PROJECT_STATIC_MACRO}
)

# Link CRT statically
target_compile_options(exeOutProcServer
	PRIVATE $<$<CONFIG:Release>:/MT>
	PRIVATE $<$<CONFIG:Debug>:/MTd>
)

# Set project dependencies
add_dependencies(exeOutProcServer
	libCoreCOM
	#libCorePlatform
)

##################################################################################################
# ________          __ __________                        __________                              #
# \_____  \  __ ___/  |\______   \_______  ____   ____   \______   \_______  _______  ______.__. #
#  /   |   \|  |  \   __\     ___/\_  __ \/  _ \_/ ___\   |     ___/\_  __ \/  _ \  \/  <   |  | #
# /    |    \  |  /|  | |    |     |  | \(  <_> )  \___   |    |     |  | \(  <_> >    < \___  | #
# \_______  /____/ |__| |____|     |__|   \____/ \___  >  |____|     |__|   \____/__/\_ \/ ____| #
#         \/                                         \/                                \/\/      #
##################################################################################################
message("dllOutProcProxy: Generating out-of-process proxy sample")

# Project: dllOutProcProxy
add_library(dllOutProcProxy SHARED 
	"${OUTPUT_FOLDER}/MIDL/dllOutProcProxy.c"
	"${OUTPUT_FOLDER}/MIDL/exeOutProcServer_p.c"
	"${OUTPUT_FOLDER}/MIDL/exeOutProcServer_i.c"
	"${OUTPUT_FOLDER}/MIDL/exeOutProcServer.h"
	"${PROJECT_SAMPLE_FOLDER}dllOutProcProxy.def"
)

# Specify the source/headers may not exist (but should be compiled)
set_source_files_properties("${OUTPUT_FOLDER}/MIDL/dllOutProcProxy.c" 
	"${OUTPUT_FOLDER}/MIDL/exeOutProcServer_p.c" 
	"${OUTPUT_FOLDER}/MIDL/exeOutProcServer_i.c" 
	"${OUTPUT_FOLDER}/MIDL/exeOutProcServer.h" PROPERTIES
	GENERATED ON
	HEADER_FILE_ONLY OFF
	LANGUAGE C
)

# Set linker dependencies
target_link_libraries(dllOutProcProxy
	kernel32.lib rpcns4.lib rpcrt4.lib uuid.lib
)

# Set language flag; compile as 'C'
set_target_properties(dllOutProcProxy PROPERTIES
                      LINKER_LANGUAGE C
					  COMPILE_FLAGS "/TC"  
					  OUTPUT_NAME "dllOutProcProxy"
					  FOLDER "Samples"
)

# Set macros for building proxy
target_compile_definitions(dllOutProcProxy
	PRIVATE REGISTER_PROXY_DLL  #PROXY_CLSID=AB4CB84C-CDCC-4525-AFB0-0B0D5798078D
)

# Set project dependencies
add_dependencies(dllOutProcProxy
	exeOutProcServer
)

###############################################################################
#   ________                     .__           ___________              __    #
#  /  _____/  ____   ____   ____ |  |   ____   \__    ___/___   _______/  |_  #
# /   \  ___ /  _ \ /  _ \ / ___\|  | _/ __ \    |    |_/ __ \ /  ___/\   __\ #
# \    \_\  (  <_> |  <_> ) /_/  >  |_\  ___/    |    |\  ___/ \___ \  |  |   #
#  \______  /\____/ \____/\___  /|____/\___  >   |____| \___  >____  > |__|   #
#         \/             /_____/           \/               \/     \/         #
###############################################################################
message("dllGoogleTest: Generating GoogleTest project")

# Project: dllGoogleTest
add_library(dllGoogleTest SHARED 
    "${GTEST_SOURCE_FOLDER}gtest-all.cc"
    "${GMOCK_SOURCE_FOLDER}gmock-all.cc"
    "${GTEST_SOURCE_FOLDER}gtest_main.cc"
)

# Include folders
target_include_directories(dllGoogleTest BEFORE
    PRIVATE
	${GTEST_FOLDER}
	${GMOCK_FOLDER}
    PUBLIC
    ${GTEST_INCLUDE_FOLDER}
	${GMOCK_INCLUDE_FOLDER}
)

# Set compiler & linker flags
set_target_properties(dllGoogleTest PROPERTIES
					  COMPILE_FLAGS "${GLOBAL_COMPILER_OPTIONS}"
					  LINK_FLAGS "${GLOBAL_LINKER_OPTIONS}"
					  OUTPUT_NAME "GoogleTest"
					  FOLDER "Testing"
)

# Specify GTest should be build as a DLL
target_compile_definitions(dllGoogleTest
	PRIVATE GTEST_CREATE_SHARED_LIBRARY=1
)

# Link CRT dynamically
target_compile_options(dllGoogleTest
	PRIVATE $<$<CONFIG:Release>:/MD>
	PRIVATE $<$<CONFIG:Debug>:/MDd>
)

####################################################################
#  ____ ___      .__  __    ___________              __            #
# |    |   \____ |__|/  |_  \__    ___/___   _______/  |_  ______  #
# |    |   /    \|  \   __\   |    |_/ __ \ /  ___/\   __\/  ___/  #
# |    |  /   |  \  ||  |     |    |\  ___/ \___ \  |  |  \___ \   #
# |______/|___|  /__||__|     |____| \___  >____  > |__| /____  >  #
#              \/                        \/     \/            \/   #
####################################################################
message("exeComTests: Generating unit-test project")

# Test files
file(GLOB_RECURSE PROJECT_TEST_FILES
    "${PROJECT_TEST_FOLDER}*.h"
    "${PROJECT_TEST_FOLDER}*.cpp"
    )
	
# Test files
file(GLOB_RECURSE PROJECT_TEST_CC_FILES
    "${PROJECT_TEST_FOLDER}*.h"
    "${PROJECT_TEST_FOLDER}*.cc"
    )

# Project: exeComTests
add_executable(exeComTests
	${PROJECT_TEST_CC_FILES}
	${PROJECT_TEST_FILES}
)

# Exclude .cpp files from building
set_source_files_properties(${PROJECT_TEST_FILES} PROPERTIES 
    HEADER_FILE_ONLY ON
)

# Include folders
target_include_directories(exeComTests BEFORE
    PRIVATE
	${PROJECT_INCLUDE_FOLDER} 
	${PROJECT_SOURCE_FOLDER} 
	${PROJECT_TEST_FOLDER} 
)

# Consume GoogleTest as DLL
target_compile_definitions(exeComTests
	PRIVATE GTEST_LINKED_AS_SHARED_LIBRARY=1
)

# Set compiler & linker flags
set_target_properties(exeComTests PROPERTIES
					  COMPILE_FLAGS "${GLOBAL_COMPILER_OPTIONS}"
					  LINK_FLAGS "${GLOBAL_LINKER_OPTIONS} /SUBSYSTEM:CONSOLE"
					  FOLDER "Testing"
)

# Set linker dependencies
target_link_libraries(exeComTests
	dllGoogleTest
	"${BASE_FOLDER}../Core-Platform/build/$(Configuration)/CorePlatform.lib" 
	dllCoreCOM
)

# Link dependencies dynamically
target_compile_definitions(exeComTests
	PRIVATE CORE_PLATFORM_IMPORT 
	        ${PROJECT_IMPORT_MACRO}
)

# Link CRT dynamically
target_compile_options(exeComTests
	PRIVATE $<$<CONFIG:Release>:/MD>
	PRIVATE $<$<CONFIG:Debug>:/MDd>
)

# Set project dependencies
add_dependencies(exeComTests
	dllGoogleTest
	#dllCorePlatform
	dllCoreCOM
)
#################################################################################
#     ___________.__.__           .____                                __   
#     \_   _____/|__|  |   ____   |    |   _____  ___.__. ____  __ ___/  |_ 
#      |    __)  |  |  | _/ __ \  |    |   \__  \<   |  |/  _ \|  |  \   __\
#      |     \   |  |  |_\  ___/  |    |___ / __ \\___  (  <_> )  |  /|  |  
#      \___  /   |__|____/\___  > |_______ (____  / ____|\____/|____/ |__|  
#          \/                 \/          \/    \/\/                        
#################################################################################

# Extra files
file(GLOB_RECURSE PROJECT_EXTRA_FILES
    "${PROJECT_EXTRA_FOLDER}*.h"
    "${PROJECT_EXTRA_FOLDER}*.cpp"
)

# Sample code
file(GLOB_RECURSE PROJECT_SAMPLE_FILES
    "${PROJECT_SAMPLE_FOLDER}*.h"
    "${PROJECT_SAMPLE_FOLDER}*.cpp"
)

# Sample code build/config files
file(GLOB_RECURSE PROJECT_SAMPLE_BUILD_FILES
    "${PROJECT_SAMPLE_FOLDER}*.midl"
    "${PROJECT_SAMPLE_FOLDER}*.def"
)

# Project file groups
source_group(TREE ${PROJECT_SOURCE_FOLDER}    PREFIX "Build Files"    FILES ${PROJECT_SOURCE_CC_FILES}})
source_group(TREE ${PROJECT_EXTRA_FOLDER}     PREFIX "Extra Files"    FILES ${PROJECT_EXTRA_FILES})
source_group(TREE ${PROJECT_SAMPLE_FOLDER}    PREFIX "Extra Files"    FILES ${PROJECT_SAMPLE_BUILD_FILES})
source_group(TREE ${PROJECT_INCLUDE_FOLDER}   PREFIX "Header Files"   FILES ${PROJECT_INCLUDE_FILES})
source_group(TREE ${PROJECT_SAMPLE_FOLDER}    PREFIX "Source Files"   FILES ${PROJECT_SAMPLE_FILES})
source_group(TREE ${PROJECT_SOURCE_FOLDER}    PREFIX "Source Files"   FILES ${PROJECT_SOURCE_FILES})
source_group(TREE ${PROJECT_RESOURCE_FOLDER}  PREFIX "Resource Files" FILES ${PROJECT_RESOURCE_FILES})

# unit-test file groups
#source_group(TREE ${PROJECT_TEST_FOLDER}   PREFIX "Build Files"  FILES ${PROJECT_UNIT_TEST_CC_FILES})
source_group(TREE ${PROJECT_TEST_FOLDER}   PREFIX "Test Files"   FILES ${PROJECT_UNIT_TEST_FILES})
source_group(TREE ${PROJECT_MOCK_FOLDER}   PREFIX "Mock Files"   FILES ${PROJECT_MOCK_FILES})
