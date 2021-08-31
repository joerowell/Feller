#!/usr/bin/env bash

if [ -f Feller.hpp ]; then
    rm Feller.hpp
fi

touch Feller.hpp
cat src/Feller_Feller.hpp    >> Feller.hpp
cat src/Feller_Util.hpp >> Feller.hpp
cat src/Feller_Util.cpp >> Feller.hpp

cat src/Feller_LoggingMode.hpp >> Feller.hpp

cat src/Feller_AnyType.hpp >> Feller.hpp
cat src/Feller_AnyType.cpp >> Feller.hpp

cat src/Feller_ContiguousLogStorage.hpp >> Feller.hpp
cat src/Feller_ContiguousLogStorage.cpp >> Feller.hpp

cat src/Feller_Data.hpp >> Feller.hpp
cat src/Feller_Data.cpp >> Feller.hpp

cat src/Feller_EventLog.hpp >> Feller.hpp
cat src/Feller_EventLog.cpp >> Feller.hpp

cat src/Feller_Logger.hpp >> Feller.hpp
cat src/Feller_Logger.cpp >> Feller.hpp

cat src/Feller_MutexLock.hpp >> Feller.hpp
cat src/Feller_MutexLock.cpp >> Feller.hpp

cat src/Feller_NoLock.hpp >> Feller.hpp
cat src/Feller_NoLock.cpp >> Feller.hpp

cat src/Feller_TestData.hpp >> Feller.hpp
cat src/Feller_TestData.cpp >> Feller.hpp



cat src/Feller_StaticLoggingPolicy.hpp >> Feller.hpp
cat src/Feller_StaticLoggingPolicy.cpp >> Feller.hpp
cat src/Feller_ConditionalLoggingPolicy.hpp >> Feller.hpp
cat src/Feller_ConditionalLoggingPolicy.cpp >> Feller.hpp

cat src/Feller_LogEverything.hpp >> Feller.hpp
cat src/Feller_LogEverything.cpp >> Feller.hpp
cat src/Feller_LogNothing.hpp >> Feller.hpp
cat src/Feller_LogNothing.cpp >> Feller.hpp

cat src/Feller_Decl.hpp >> Feller.hpp
cat src/Feller_Decl.cpp >> Feller.hpp


# Remove the include statements that rely on other files
grep -v '^#include \"' Feller.hpp > Feller_t.hpp
mv Feller_t.hpp Feller.hpp

