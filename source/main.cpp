#include <Application.h>

int main(int argc, char* argv[])
{
	Application app("Hello OpenGL", argv[0]);
	app.mainLoop();

	return 0;
}