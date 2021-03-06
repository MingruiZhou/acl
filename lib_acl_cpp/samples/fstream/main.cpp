#include "lib_acl.h"
#include "acl_cpp/stream/ifstream.hpp"
#include "acl_cpp/stdlib/locker.hpp"
#include <stdio.h>
#include <stdlib.h>

static void test_stdio(void)
{
	acl::string buf;
	acl::ifstream in;
	in.open(0, O_RDONLY);

	if (in.gets(buf))
		printf(">>>>ok, gets: %s\n", buf.c_str());
	else
		printf(">>>gets error, %s\n", acl_last_serror());
}

int main(void)
{
	test_stdio();

	acl::string path("test.eml");

	acl::ifstream fp;

	if (fp.open_read(path) == false) {
	//if (fp.open(path.c_str(), O_RDWR, 0600) == false) {
		printf("open %s error(%s)\n", path.c_str(), acl_last_serror());
		getchar();
		return (-1);
	}

	acl::locker locker;
	if (locker.open(fp.file_handle()) == false) {
		printf("open %s's lock error\n", path.c_str());
		getchar();
		return (-1);
	}
	if (locker.lock() == true)
		printf("first lock %s ok\n", path.c_str());
	else
		printf("first lock %s error(%s)\n", path.c_str(), acl_last_serror());
	if (locker.lock() == true)
		printf("second lock %s ok\n", path.c_str());
	else
		printf("second lock %s error(%s)\n", path.c_str(), acl_last_serror());

	acl::string buf;
	while (1) {
		if (fp.gets(buf, false) == false)
			break;
		printf("%s", buf.c_str());
	}

	getchar();
	fp.close();

	return (0);
}
