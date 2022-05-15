#include "Main.hpp"
#include "Application.hpp"
#include "System.hpp"


#ifndef TEST

int main(int argc, char* const argv[])
{
	System filesystem;
	Application app(filesystem);
	return app.Run(argc, argv);
}

#endif
