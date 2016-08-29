/**********************************************************************
*
*	文件:	   crStatistics.h
*
*	描述:
*
*	作者:	   吴财华
*					
*
*	Copyright 2005 吴财华
**********************************************************************/
#ifndef CRCORE_STATISTICS
#define CRCORE_STATISTICS 1

#include <CRCore/Referenced.h>
#include <CRCore/crDrawable.h>
#include <CRCore/crExport.h>

#include <map>

namespace CRCore {

class CR_EXPORT crStatistics : public CRCore::Referenced
{
    public:
        crStatistics()
        {
			m_drawTime = 0.0f;
			m_updateTime = 0.0f;
			m_cullTime = 0.0f;
			m_phyUpdTime = 0.0f;
			m_characterUpdTime = 0.0f;
			m_particleUpdTime = 0.0f;
			m_keyboardMouseTime = 0.0f;
			m_swapTime = 0.0f;
			m_numStates = 0;
			m_numDrawables = 0;
			m_vertexCount = 0;
			m_primCount = 0;
			m_particleCount = 0;
			m_numStates_rd = 0;
			m_numDrawables_rd = 0;
			m_vertexCount_rd = 0;
			m_primCount_rd = 0;
			m_particleCount_rd = 0;

			m_statistics = this;
			m_stat = false;
        };
        void reset()
        {
			m_numStates_rd = m_numStates;
			m_numDrawables_rd = m_numDrawables;
			m_vertexCount_rd = m_vertexCount;
			m_primCount_rd = m_primCount;
			m_particleCount_rd = m_particleCount;
			m_numStates = 0;
			m_numDrawables = 0;
			m_vertexCount = 0;
			m_primCount = 0;
			m_particleCount = 0;
        }

		void addState() {  m_numStates++;}
        void addDrawable() { m_numDrawables++;}
		void addVertexCount( int numVertex ){ m_vertexCount += numVertex; }
		void addPrimCount( int numPrim ){ m_primCount += numPrim; }
		void addParticleCount( int num ){ m_particleCount += num; }
        //void addMatrix() { nummat++;}
        //void addLight(int np) { nlights+=np;}
        //void addImpostor(int np) { nimpostor+= np; }
        //void setDepth(int d) { depth=d; }
        //void addBins(int np) { nbins+= np; }

        //void setBinNo(int n) { m_binNo=n;}    

		int getState() {  return m_numStates_rd;}
		int getDrawable() { return m_numDrawables_rd;}
		int getVertexCount(){ return m_vertexCount_rd; }
		int getPrimCount(){ return m_primCount_rd; }
		int getParticleCount(){ return m_particleCount_rd; }
		//int getMatrix() { return nummat;}
		//int getLight() { return nlights;}
		//int getImpostor() { return nimpostor; }
		//inline int getBins() { return nbins;}
		//int getDepth() { return depth; }

		//int getBinNo() { return m_binNo;}    

		static crStatistics *getInstance();
		void setStat(bool stat){ m_stat = stat; }
		bool getStat(){ return m_stat; }
		
		void setDrawTime(float ms){ m_drawTime = ms; }
		void setUpdTime(float ms){ m_updateTime = ms; }
		void setCullTime(float ms){ m_cullTime = ms; }
		void setPhyUpdTime(float ms){ m_phyUpdTime = ms; }
		void setCharacterUpdTime(float ms){ m_characterUpdTime = ms; }
		void setParticleUpdTime(float ms){ m_particleUpdTime = ms; }
		void setKeyboardMouseTime(float ms){ m_keyboardMouseTime = ms; }
		void setSwapTime(float ms){ m_swapTime = ms; }
		float getDrawTime(){ return m_drawTime; }
		float getUpdTime(){ return m_updateTime; }
		float getCullTime(){ return m_cullTime; }
		float getPhyUpdTime(){ return m_phyUpdTime; }
		float getCharacterUpdTime(){ return m_characterUpdTime; }
		float getParticleUpdTime(){ return m_particleUpdTime; }
		float getKeyboardMouseTime(){ return m_keyboardMouseTime; }
		float getSwapTime(){ return m_swapTime; }
	protected:
		float m_drawTime;
		float m_updateTime;
		float m_cullTime;
		float m_phyUpdTime;
		float m_characterUpdTime;
		float m_particleUpdTime;
		float m_keyboardMouseTime;
		float m_swapTime;
		int m_numStates;
		int m_numDrawables;
		int m_vertexCount;
		int m_primCount;
		int m_particleCount;
		int m_numStates_rd;
		int m_numDrawables_rd;
		int m_vertexCount_rd;
		int m_primCount_rd;
		int m_particleCount_rd;
    private:
		static CRCore::ref_ptr<crStatistics> m_statistics;
        bool                                 m_stat;
};

}

#endif
