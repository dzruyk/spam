#ifndef __COREINSTALL_H__
#define __COREINSTALL_H__

bool self_install();

/*
 * Install/unistall application to/from selected dir
 *
 */

class Installer
{
public:
	Installer(char *app);
	~Installer();

	bool install(char *todir);
	bool uninstall(char *fromdir);
private:
	char *pname;
};

#endif
