HEADER_PATH = 	-I ../Headers \
				-I ../Dependence/Headers

LIB_PATH =	-L ../Dependence/Lib

CXXFLAG = -lglfw -framework OpenGL

C_FILE = ../Dependence/Lib/glad.c

CXX_FILE = 	../Sources/main.cpp \
			../Sources/example.cpp

C_OBJS = glad.o

CXX_OBJS = main.o

TARGET = game

$(TARGET):$(CXX_OBJS) $(C_OBJS)
	g++ $(HEADER_PATH) $(LIB_PATH)  $(CXXFLAG) $^ -o $@
	mkdir -p ../Release
	mv $(TARGET) ../Release/$(TARGET)

$(C_OBJS):$(C_FILE)
	gcc -c $^

$(CXX_OBJS):$(CXX_FILE)
	g++ -c $^

.PHONY:clean
clean:
	-rm $(TARGET)
	-rm $(C_OBJS)
	-rm $(CXX_OBJS)