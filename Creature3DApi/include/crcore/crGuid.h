/* Creature3D - Online Game Engine, Copyright (C) 2005 Wucaihua(26756325@qq.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
*/
#ifndef CRCORE_CRGUID_H
#define CRCORE_CRGUID_H 1

#include <CRCore/ref_ptr.h>
#include <CRCore/Referenced.h>
#include <CRCore/crWchMsg.h>
#include <CRCore/crExport.h>
#include <string>

namespace CRCore {

class CR_EXPORT crGuid
{
public :
	crGuid()
		: A(0)
		, B(0)
		, C(0)
		, D(0)
	{ }
	crGuid(_crInt32 inA,_crInt32 inB, _crInt32 inC, _crInt32 inD);
	virtual ~crGuid();
	static crGuid newGuid();
	friend bool operator==(const crGuid& X, const crGuid& Y)
	{
		return ((X.A ^ Y.A) | (X.B ^ Y.B) | (X.C ^ Y.C) | (X.D ^ Y.D)) == 0;
	}
	friend bool operator!=(const crGuid& X, const crGuid& Y)
	{
		return ((X.A ^ Y.A) | (X.B ^ Y.B) | (X.C ^ Y.C) | (X.D ^ Y.D)) != 0;
	}
	friend bool operator<(const crGuid& X, const crGuid& Y)
	{
		return	((X.A < Y.A) ? true : ((X.A > Y.A) ? false :
			((X.B < Y.B) ? true : ((X.B > Y.B) ? false :
			((X.C < Y.C) ? true : ((X.C > Y.C) ? false :
				((X.D < Y.D) ? true : ((X.D > Y.D) ? false : false))))))));
	}
	unsigned int& operator[](int Index)
	{
		switch (Index)
		{
		case 0: return A;
		case 1: return B;
		case 2: return C;
		case 3: return D;
		}

		return A;
	}
	const unsigned int& operator[](int Index) const
	{
		switch (Index)
		{
		case 0: return A;
		case 1: return B;
		case 2: return C;
		case 3: return D;
		}

		return A;
	}
	void invalidate()
	{
		A = B = C = D = 0;
	}
	bool isvalid() const
	{
		return ((A | B | C | D) != 0);
	}
	unsigned short GetTypeHash();
	enum GuidFormats
	{
		/**
		* 32 digits.
		*
		* For example: "00000000000000000000000000000000"
		*/
		Digits,

		/**
		* 32 digits separated by hyphens.
		*
		* For example: 00000000-0000-0000-0000-000000000000
		*/
		DigitsWithHyphens,

		/**
		* 32 digits separated by hyphens and enclosed in braces.
		*
		* For example: {00000000-0000-0000-0000-000000000000}
		*/
		DigitsWithHyphensInBraces,

		/**
		* 32 digits separated by hyphens and enclosed in parentheses.
		*
		* For example: (00000000-0000-0000-0000-000000000000)
		*/
		DigitsWithHyphensInParentheses,

		/**
		* Comma-separated hexadecimal values enclosed in braces.
		*
		* For example: {0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}}
		*/
		HexValuesInBraces,

		/**
		* This format is currently used by the FUniqueObjectGuid class.
		*
		* For example: 00000000-00000000-00000000-00000000
		*/
		UniqueObjectGuid,
	};
	std::string toString() const
	{
		return toString(Digits);
	}
	std::string toString(GuidFormats format) const;
private:
	unsigned int A;
	unsigned int B;
	unsigned int C;
	unsigned int D;
};
}

#endif
