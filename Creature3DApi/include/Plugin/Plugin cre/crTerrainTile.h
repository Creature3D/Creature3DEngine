
#ifndef CRE_CRTERRAINTILE_H
#define CRE_CRTERRAINTILE_H 1

#include <CRTerrain/crTerrainTile.h>
#include <Plugin cre/crReadWrite.h>

namespace cre
{
class crTerrainTile : public CRTerrain::crTerrainTile, public crReadWrite
{
public:
	void write(crDataOutputStream* out);
	void read(crDataInputStream* in);
};
}

#endif