#ifndef IPCNAME_H
#define IPCNAME_H

#include <string>

struct IPCName
{
	std::string path;
	char  index;

	IPCName () { }

	IPCName (const char *path, const char index)
		: path (path), index (index) { }
};

#endif
