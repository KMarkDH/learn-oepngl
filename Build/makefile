HEADER_PATH = 	-I ../src \
				-I ../src/vendor

LIB_PATH =	-L ../src/vendor/Lib \
			-lglfw -framework OpenGL

CXXFLAG = -g

OBJECTS = 	main.o \
			ShaderFile.o \
			glad.o \
			VertexBuffer.o \
			IndexBuffer.o \
			VertexArrayBuffer.o \
			Application.o \
			Shader.o \
			stb_image.o

TARGET = game

$(TARGET):$(OBJECTS)
	echo "\033[44;30m	::linking::    \033[0m"
	g++ $(HEADER_PATH) $(LIB_PATH)  $(CXXFLAG) $^ -o $@

	echo "\033[44;30m	::installing::	\033[0m"
	mkdir -p ../Release
	cp $(TARGET) ../Release/$(TARGET)

	rm -rf ../Release/res
	cp -rf ../res ../Release
	echo "\033[44;30m	::build done::	\033[0m"

#compile c++ file in lib
%.o:../src/vendor/%.cpp
	echo "\033[44;30m	::build lib cpp file::	\033[0m"
	g++ $(HEADER_PATH) $(CXXFLAG) $^ -c $^

#compile c file in lib
%.o:../src/vendor/%.c
	echo "\033[44;30m	::build lib c file::	\033[0m"
	gcc $(HEADER_PATH) $(CXXFLAG) $^ -c $^

#compile all source code
%.o:../src/%.cpp
	echo "\033[44;30m	::build source file::	\033[0m"
	g++ $(HEADER_PATH) $(CXXFLAG) $^ -c $^

.PHONY:clean
clean:
	echo "\033[44;30m	::cleaning::	\033[0m"
	-rm -f $(TARGET)
	-rm -f *.o