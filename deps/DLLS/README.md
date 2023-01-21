# DLL dependencies

This directory contains `.dll` dependencies which Visual Studio 2022 post-build event copies to the output directory. That way we provide .dll files to our executable in a safer way, so only this program can use them.
