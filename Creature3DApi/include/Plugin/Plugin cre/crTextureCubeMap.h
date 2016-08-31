#ifndef CRE_TEXTURECUBEMAP
#define CRE_TEXTURECUBEMAP 1

#include <CRCore/crTextureCubeMap.h>
#include <Plugin cre/crReadWrite.h>

namespace cre
{

class crTextureCubeMap : public CRCore::crTextureCubeMap, public crReadWrite 
{
public:
	void write(crDataOutputStream* out);
        
        void writeImage(crDataOutputStream* out,bool includeImg,CRCore::crImage* image);

	void read(crDataInputStream* in);
        
	CRCore::crImage* readImage(crDataInputStream* in, bool includeImg);
        

};
}

#endif
