import os
os.system('cmake -DBUILD_ONLY=ON -DBUILD_BIOINFO_LIB=ON -DBUILD_SAPP_LIB=ON -S . -B build')
os.system('cmake --build build')
os.system('cmake --install build')
