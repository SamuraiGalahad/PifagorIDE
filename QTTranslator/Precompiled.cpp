#include "Precompiled.h"
void DW(char *s)
{
	int fc;
	char x[1000];

    std::ifstream in("x.myinit");
	in >> fc;
	in.close();
	
	fc++;

    std::ofstream out1("x.myinit");
	out1 << fc;
	out1.close();

    sprintf(x,"%d.mydeb",fc);
    std::ofstream out(x);
	out << s;
	out.close();
}
