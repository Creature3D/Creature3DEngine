#ifndef CRE_DATAINPUTSTREAM
#define CRE_DATAINPUTSTREAM 1


#include <iostream>        // for ifstream
#include <string>
#include <map>
#include <vector>
#include <CRCore/crVector2.h>
#include <CRCore/crVector3.h>
#include <CRCore/crVector4.h>
#include <CRCore/crQuat.h>
#include <CRCore/crArray.h>
#include <CRCore/crMatrix.h>
#include <CRCore/crGeometry.h>                       
#include <CRCore/crImage.h>
#include <CRCore/crStateSet.h>
#include <CRCore/crShape.h>
#include <CRCore/crUniform.h>
#include <CRCore/crEventGroup.h> 
#include <CRCore/crEventMsg.h>
#include <CRPhysics/crJoint.h>
#include <CRCore/ref_ptr.h>
//#include <CRAL/crSoundState.h>
//#include <CRAL/crOccludeCallback.h>
#include <CRCore/crCollideCallback.h>

#include <CRIOManager/crReaderWriter.h>


#include <Plugin cre/crCreVersion.h>
#include <Plugin cre/crDataTypeSize.h>    
#include <Plugin cre/crException.h>

namespace cre{

class crDataInputStream{

public:
    crDataInputStream(std::istream* istream);
    ~crDataInputStream();

        void setOptions(const CRIOManager::crReaderWriter::Options* options);
        const CRIOManager::crReaderWriter::Options* getOptions() const { return m_options.get(); }

    unsigned int getVersion();
    bool readBool();
    char readChar();
    unsigned char readUChar();
    unsigned short readUShort();
    unsigned int readUInt();
    int readInt();
    int peekInt();
    float readFloat();
    long readLong();
    unsigned long readULong();
	_crInt64 readInt64();
    double readDouble();
    std::string readString();
    void readCharArray(char* data, int size);
        
    CRCore::crVector2f readVec2();
    CRCore::crVector3f readVec3();
    CRCore::crVector4f readVec4();
    CRCore::crVector2d readVec2d();
    CRCore::crVector3d readVec3d();
    CRCore::crVector4d readVec4d();
	CRCore::crVector2i readVec2i();
	CRCore::crVector3i readVec3i();
	CRCore::crVector4i readVec4i();
	CRCore::crVector2b readVec2b();
	CRCore::crVector3b readVec3b();
	CRCore::crVector4b readVec4b();
    CRCore::crPlane readPlane();
    CRCore::crVector4ub readUByte4();
    CRCore::crQuat readQuat();
    CRCore::crMatrixf readMatrixf();
    CRCore::crMatrixd readMatrixd();
    CRCore::crGeometry::AttributeBinding readBinding();
    CRCore::Array* readArray();
    CRCore::IntArray* readIntArray();
    CRCore::UByteArray* readUByteArray();
    CRCore::UShortArray* readUShortArray();
    CRCore::UIntArray* readUIntArray();
    CRCore::Vec4ubArray* readUByte4Array();
    CRCore::FloatArray* readFloatArray();
    CRCore::Vec2Array* readVec2Array();
    CRCore::Vec3Array* readVec3Array();
    CRCore::Vec4Array* readVec4Array();

    CRCore::crImage* readImage(std::string s);
    CRCore::crStateSet* readStateSet();
    CRCore::crStateAttribute* readStateAttribute();
    CRCore::crUniform* readUniform();
    CRCore::crShader* readShader();
    CRCore::crDrawable* readDrawable();
    CRCore::crShape* readShape();
    CRCore::crNode* readNode();

    CRCore::Condition* readCondition();
	CRCore::SendMsg* readSendMsg();
	CRCore::ECondition* readECondition();
	CRCore::crEvent* readEvent();
	CRCore::crEventMsg* readEventMsg();
	//CRCore::ParseParamCallback* readPPC();

	CRPhysics::crJointParameter* readJointParam();
	CRPhysics::crJoint* readJoint();

	//CRAL::crSoundState* readSoundState();
	//CRAL::crOccludeCallback* readSoundOccludeCallback();

	CRCore::crCollideCallback* readCollideCallback();

    // Set and get if must be generated external reference cre files
    void setLoadExternalReferenceFiles(bool b) {m_loadExternalReferenceFiles=b;};
    bool getLoadExternalReferenceFiles() {return m_loadExternalReferenceFiles;};

    typedef std::map<std::string, CRCore::ref_ptr<CRCore::crImage> >    ImageMap;
    typedef std::map<int,CRCore::ref_ptr<CRCore::crStateSet> >          StateSetMap;
    typedef std::map<int,CRCore::ref_ptr<CRCore::crStateAttribute> >    StateAttributeMap;
    typedef std::map<int,CRCore::ref_ptr<CRCore::crUniform> >           UniformMap;
    typedef std::map<int,CRCore::ref_ptr<CRCore::crShader> >            ShaderMap;
    typedef std::map<int,CRCore::ref_ptr<CRCore::crDrawable> >          DrawableMap;
    typedef std::map<int,CRCore::ref_ptr<CRCore::crShape> >             ShapeMap;
    typedef std::map<int,CRCore::ref_ptr<CRCore::crNode> >              NodeMap;

	typedef std::map<int,CRCore::ref_ptr<CRCore::Condition> >           ConditionMap;
	typedef std::map<int,CRCore::ref_ptr<CRCore::SendMsg> >             SendMsgMap;
	typedef std::map<int,CRCore::ref_ptr<CRCore::ECondition> >          EConditionMap;
	typedef std::map<int,CRCore::ref_ptr<CRCore::crEvent> >             EventMap;
	typedef std::map<int,CRCore::ref_ptr<CRCore::crEventMsg> >          EventMsgMap;
	typedef std::map<int,CRCore::ref_ptr<CRCore::ParseParamCallback> >  PPCMap;

	typedef std::map<int,CRCore::ref_ptr<CRPhysics::crJointParameter> >    JointParamMap;
	typedef std::map<int,CRCore::ref_ptr<CRPhysics::crJoint> >             JointMap;

	//typedef std::map<int, CRCore::ref_ptr<CRAL::crSoundState> >         SoundStateMap;
	//typedef std::map<int, CRCore::ref_ptr<CRAL::crOccludeCallback> >    SoundOccludeCallbackMap;

	typedef std::map<int, CRCore::ref_ptr<CRCore::crCollideCallback> >   CollideCallbackMap;

    bool                m_verboseOutput;
    std::istream*       m_istream;
    int                 m_byteswap;

private:
    int                 m_version;
    bool                m_peeking;
    int                 m_peekValue; 
    ImageMap            m_imageMap;
    StateSetMap         m_statesetMap;
    StateAttributeMap   m_stateAttributeMap;
    UniformMap          m_uniformMap;
    ShaderMap           m_shaderMap;
    DrawableMap         m_drawableMap;
    ShapeMap            m_shapeMap;
    NodeMap             m_nodeMap;

    ConditionMap        m_conditionMap;
	SendMsgMap          m_sendMsgMap;
	EConditionMap       m_econditionMap;
	EventMap            m_eventMap;
    EventMsgMap         m_eventMsgMap;
	PPCMap              m_ppcMap;

	JointParamMap       m_jointParamMap;
	JointMap            m_jointMap;
    
	//SoundStateMap       m_soundStateMap;
    //SoundOccludeCallbackMap m_soundOccludeCallbackMap;
 
	CollideCallbackMap  m_collideCallbackMap;

	bool m_loadExternalReferenceFiles;
        
    CRCore::ref_ptr<const CRIOManager::crReaderWriter::Options> m_options;
   
};

}
#endif // CRE_DATAINPUTSTREAM
