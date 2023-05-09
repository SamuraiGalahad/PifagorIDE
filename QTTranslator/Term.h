#ifndef __TERM_H_HEADER_INCLUDED__
#define __TERM_H_HEADER_INCLUDED__

#include "GeneralObject.h"

class CTerm : public CGeneralObject
{
public:
	CTerm();
	virtual ~CTerm();

	CTerm* ElseTerm;
};

#endif
