#include <stdlib.h>

#include "config.h"
#include "common.h"
#include "coreinstall.hpp"
#include "log.h"

bool self_install()
{
	DEBUG(LOG_DEFAULT, "installation init");

	Installer inst(getprogname());

	inst.install("/tmp/");

	return true;
}

Installer::Installer(char *app)
{
	DEBUG(LOG_VERBOSE, "Try to install %s\n", app);

}

Installer::~Installer()
{
	DEBUG(LOG_VERBOSE, "Installer finalize\n");

}

bool Installer::install(char *todir)
{
	DEBUG(LOG_VERBOSE, "Install to %s\n", todir);

	return true;
}


bool Installer::uninstall(char *fromdir)
{
	DEBUG(LOG_VERBOSE, "Uninstall from %s\n", fromdir);

	return true;
}

