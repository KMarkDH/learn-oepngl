HEADER_PATH = 	-I ../Headers \
				-I ../Dependence/Headers

LIB_PATH =	-L ../Dependence/Lib \
			-lglfw -framework OpenGL

CXXFLAG = -g

OBJECTS = 	main.o \
			ShaderFile.o \
			glad.o \
			VAO.o \
			Application.o \
			Shader.o

TARGET = game

$(TARGET):$(OBJECTS)
	echo "\033[44;30m	::linking::    \033[0m"
	g++ $(HEADER_PATH) $(LIB_PATH)  $(CXXFLAG) $^ -o $@

	echo "\033[44;30m	::installing::	\033[0m"
	mkdir -p ../Release
	mv $(TARGET) ../Release/$(TARGET)
	cp -rf ../res ../Release
	-rm -f *.o
	echo "\033[44;30m	::build done::	\033[0m"

#compile c++ file in lib
%.o:../Dependence/Lib/%.cpp
	echo "\033[44;30m	::build lib cpp file::	\033[0m"
	g++ $(HEADER_PATH) $(CXXFLAG) $^ -c $^

#compile c file in lib
%.o:../Dependence/Lib/%.c
	echo "\033[44;30m	::build lib c file::	\033[0m"
	gcc $(HEADER_PATH) $(CXXFLAG) $^ -c $^

#compile all source code
%.o:../Sources/%.cpp
	echo "\033[44;30m	::build source file::	\033[0m"
	g++ $(HEADER_PATH) $(CXXFLAG) $^ -c $^

.PHONY:clean
clean:
	echo "\033[44;30m	::cleaning::	\033[0m"
	-rm -f $(TARGET)
	-rm -f *.o