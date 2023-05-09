#ifndef __FISHKA_H_HEADER_INCLUDED__
#define __FISHKA_H_HEADER_INCLUDED__

#include "Scaner.h"

#define ERRCOUNT 10

enum ELexTypeEnum;

enum ERuntimeErrorEnum
{
	NOERR=0,
	UNKNOWN_ERROR,	// неидентифицируемая ошибка;
	REAL_ERROR,		// некорректное преобразование  действительного числа;
	INT_ERROR,		// некорректное преобразование целого числа;
	ZERO_DIVIDE,	// деление на ноль;
	INTERPR_ERROR,	// ошибка операции интерпретации;
	BOUND_ERROR,	// ошибка выхода за границы диапазона
	NOEXPRESSION_ERROR, // отсутствие неименованного выражения в функции или в блоке
	TYPE_ERROR,		// ошибка преобразования типа
	VALUE_ERROR,    // ошибка выделения значения фишки пользовательского типа
};

class CGeneralObject;

class CFishka
{
public:
	CFishka() { };
	virtual ~CFishka() { };
	ELexTypeEnum Type;
	virtual void OutConsole() {};
	virtual CGeneralObject* GetObject() { return (CGeneralObject*)NULL;};
};

CFishka* GetCopyFishka(CFishka* Fishka);

#endif
