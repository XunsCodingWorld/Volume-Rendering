CPP = g++
INC = -I../cryphutil -I../ImageWriter
C_FLAGS = -fPIC -g -c $(INC)

LINK = g++ -fPIC -g
LOCAL_UTIL_LIBRARIES = -L../lib -lcryph -lCOGLImageWriter

OBJS = main.o Reader.o VoxelVolume.o

main: $(OBJS) ../lib/libcryph.so ../lib/libCOGLImageWriter.so
	$(LINK) -o main $(OBJS) $(LOCAL_UTIL_LIBRARIES)

../lib/libcryph.so: ../cryphutil/AffPoint.h ../cryphutil/AffPoint.c++ ../cryphutil/AffVector.h ../cryphutil/AffVector.c++ ../cryphutil/Matrix4x4.h ../cryphutil/Matrix4x4.c++
	(cd ../cryphutil; make)

../lib/libCOGLImageWriter.so: ../ImageWriter/ImageWriter.h ../ImageWriter/ImageWriter.c++ ../ImageWriter/PNGImageWriter.h ../ImageWriter/PNGImageWriter.c++ ../ImageWriter/BMPImageWriter.h ../ImageWriter/BMPImageWriter.c++ ../ImageWriter/JPEGImageWriter.h ../ImageWriter/JPEGImageWriter.c++
	(cd ../ImageWriter; make)

main.o: main.c++
	$(CPP) $(C_FLAGS) main.c++
Reader.o: Reader.h Reader.c++
	$(CPP) $(C_FLAGS) Reader.c++
VoxelVolume.o: VoxelVolume.h VoxelVolume.c++
	$(CPP) $(C_FLAGS) VoxelVolume.c++
