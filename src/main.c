#include "vulkan/MKTAppEngine.h"

int main(int argc, char* argv[])
{
	_MKTAE_init();

	_MKTAE_run();

	_MKTAE_cleanup();

	return 0;
}
