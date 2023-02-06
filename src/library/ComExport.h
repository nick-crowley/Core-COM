#pragma once
#if defined(BuildCoreComStaticLib)
#	define ComExport 
#elif defined(BuildCoreComDLL)
#	define ComExport __declspec(dllexport)
#else
#	define ComExport __declspec(dllimport)
#endif