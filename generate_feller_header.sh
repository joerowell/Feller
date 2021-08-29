#!/usr/bin/env bash

if [ -f ./src/Feller.hpp ]; then
    rm ./src/Feller.hpp
fi

touch src/Feller.hpp
cat src/Feller_Feller.hpp    >> src/Feller.hpp
cat src/Feller_Util.hpp >> src/Feller.hpp
cat src/Feller_Util.cpp >> src/Feller.hpp


cat src/Feller_AnyType.hpp >> src/Feller.hpp
cat src/Feller_AnyType.cpp >> src/Feller.hpp

cat src/Feller_ContiguousLogStorage.hpp >> src/Feller.hpp
cat src/Feller_ContiguousLogStorage.cpp >> src/Feller.hpp

cat src/Feller_Data.hpp >> src/Feller.hpp
cat src/Feller_Data.cpp >> src/Feller.hpp

cat src/Feller_EventLog.hpp >> src/Feller.hpp
cat src/Feller_EventLog.cpp >> src/Feller.hpp

cat src/Feller_Logger.hpp >> src/Feller.hpp
cat src/Feller_Logger.cpp >> src/Feller.hpp

cat src/Feller_MutexLock.hpp >> src/Feller.hpp
cat src/Feller_MutexLock.cpp >> src/Feller.hpp

cat src/Feller_NoLock.hpp >> src/Feller.hpp
cat src/Feller_NoLock.cpp >> src/Feller.hpp

cat src/Feller_TestData.hpp >> src/Feller.hpp
cat src/Feller_TestData.cpp >> src/Feller.hpp

cat src/Feller_Decl.hpp >> src/Feller.hpp

# Remove the include statements that rely on other files
grep -v '^#include \"' src/Feller.hpp > src/Feller_t.hpp
mv src/Feller_t.hpp src/Feller.hpp

