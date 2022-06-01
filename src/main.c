#include "vulkan/MKTAppEngine.h"

int main(int argc, char* argv[])
{
	_MKTGE_init();

	_MKTGE_run();

	_MKTGE_cleanup();

	return 0;
}
