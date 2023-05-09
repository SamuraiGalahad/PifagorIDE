#include "Precompiled.h"
extern void DW(char *s)
{
	int fc;
	char x[1000];

	ifstream in("x.myinit");
	in >> fc;
	in.close();
	
	fc++;

	ofstream out1("x.myinit");
	out1 << fc;
	out1.close();

	sprintf(x,"%d.mydeb",fc);
	ofstream out(x);
	out << s;
	out.close();
}
