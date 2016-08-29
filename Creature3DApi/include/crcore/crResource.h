
#ifndef CRCORE_CRRESOURCE_H
#define CRCORE_CRRESOURCE_H

#include <CRCore\crTexture.h>
#include <CRCore\crMaterial.h>
#include <CRCore\crLight.h>
#include <CRCore\ref_ptr.h>
#include <openalpp/sample.h>
#include <openalpp/ref_ptr.h>

namespace CRCore{

class CR_EXPORT crResource: public crBase
{
public:
  virtual crBase* cloneType() const{ return NULL; }
  virtual crBase* clone(const crCopyOp&) const{ return NULL; }
  virtual const char* libraryName() const { return "CRCore"; }
  virtual const char* className() const { return "crResource"; }

  typedef std::pair< ref_ptr<crTexture>, std::string >  TexturePair;
  typedef std::map< int, TexturePair >                  TexturePalette;
  typedef std::map< int, ref_ptr<crMaterial> >          MaterialPalette;
  typedef std::map< int, ref_ptr<crLight> >             LightPalette;

  typedef std::pair< openalpp::ref_ptr<openalpp::Sample>, std::string >  SoundSamplePair;
  typedef std::map< int, SoundSamplePair >  SoundSamplePalette;
/*  std::map<int, crColorARec>    ColorAPalette;
  std::map<int, crSoundRec>     SoundPalette; 
  std::map<int, crAviRec>       AviPalette; 
  std::map<int, crLightSourceRec>   LightSourcePalette; 
  std::map<int, crCharacterRec> CharacterPalette;
  std::map<int, crTextRec>      TextPalette;
  std::map<int, crEffectRec>    EffectPalette;
*/
	crResource(){};
	virtual ~crResource(){};

	void saveResource();
	void loadResource();

/*	int addTexture( crTexture *rec );
	int addMaterial( crMaterial *rec );
	int addColorA( crColorA *rec );
	int addSound( crSound *rec );
	int addAvi( crAvi *rec );
	int adLightSource( crLightSource *rec );
	int addCharacter( crCharacter *rec );
	int addText( crText *rec );
	int addEffect( crEffect *rec );
*/
  int findOrInsertMaterial( int matid, crMaterial *mat );
  int findOrInsertTexture( int texid, const std::string &texName, crTexture *tex );
  int findOrInsertLight( int lightid, crLight *light );
  int insertTexture( int texid, const std::string &texName, crTexture *tex );
  int findOrInsertSoundSample( int id, const std::string &fileName, openalpp::Sample *sample );
  int insertSoundSample( int id, const std::string &fileName, openalpp::Sample *sample );

  crMaterial *findOrLoadMaterial( int matid );
  crTexture *findOrLoadTexture( int texid );
  crLight *findOrLoadLight( int lightid );
  openalpp::Sample *findOrLoadSoundSample( int id );

  int getMaterialID( const crMaterial *mat );
  int getTextureID( const std::string &texName );
  int getTextureID( const crTexture *tex );
  int getLightID( const crLight *light );
  int getSoundSampleID( const openalpp::Sample *sample );
  int getSoundSampleID( const std::string &fileName );

  TexturePalette &getTexturePalette(){ return m_texturePalette; }
  MaterialPalette &getMaterialPalette(){ return m_materialPalette; }
  LightPalette &getLightPalette(){ return m_lightPalette; }
  SoundSamplePalette &getSoundSamplePalette(){ return m_soundSamplePalette; }
 
  void clear();
protected:
  TexturePalette            m_texturePalette;
  MaterialPalette           m_materialPalette;
  LightPalette              m_lightPalette;
  SoundSamplePalette        m_soundSamplePalette;
};

}
#endif
