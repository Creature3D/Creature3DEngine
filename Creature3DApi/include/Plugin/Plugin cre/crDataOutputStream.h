#ifndef CRE_DATAOUTPUTSTREAM
#define CRE_DATAOUTPUTSTREAM 1



#include <iostream>		// for ofstream
#include <string>
#include <CRCore/crVector2.h>
#include <CRCore/crVector3.h>
#include <CRCore/crVector4.h>
#include <CRCore/crQuat.h>
#include <CRCore/crArray.h>
#include <CRCore/crMatrix.h>
#include <CRCore/crGeometry.h>
#include <CRCore/crShape.h>
#include <CRCore/crUniform.h>
#include <CRCore/crEventGroup.h> 
#include <CRCore/crEventMsg.h>

#include <CRIOManager/crReaderWriter.h>

#include <Plugin cre/crCreVersion.h>
#include <Plugin cre/crDataTypeSize.h>
#include <Plugin cre/crException.h>

#include <CRCore/crStateSet.h>
#include <CRCore/ref_ptr.h>

#include <CRPhysics/crJoint.h>

//#include <CRAL/crSoundState.h>
//#include <CRAL/crOccludeCallback.h>
#include <CRCore/crCollideCallback.h>
#include <CREncapsulation/crTableIO.h>
namespace cre {					   

class crDataOutputStream{

public:
	crDataOutputStream(std::ostream* ostream,const std::string &filename);
	~crDataOutputStream();
        
	void setOptions(const CRIOManager::crReaderWriter::Options* options);
	const CRIOManager::crReaderWriter::Options* getOptions() const { return m_options.get(); }

	unsigned int getVersion() { return VERSION; }
        
	void writeBool(bool b);
	void writeChar(char c);
	void writeUChar(unsigned char c);
	void writeUShort(unsigned short s);
	void writeUInt(unsigned int s);
	void writeInt(int i);
	void writeFloat(float f);
	void writeLong(long l);
	void writeULong(unsigned long l);
	void writeInt64(_crInt64 i);

	void writeDouble(double d);
	void writeString(const std::string& s);
	void writeCharArray(const char* data, int size);
	void writeVec2(const CRCore::crVector2f& v);
	void writeVec3(const CRCore::crVector3f& v);
	void writeVec4(const CRCore::crVector4f& v);
	void writeVec2d(const CRCore::crVector2d& v);
	void writeVec3d(const CRCore::crVector3d& v);
	void writeVec4d(const CRCore::crVector4d& v);
	void writeVec2i(const CRCore::crVector2i& v);
	void writeVec3i(const CRCore::crVector3i& v);
	void writeVec4i(const CRCore::crVector4i& v);
	void writeVec2b(const CRCore::crVector2b& v);
	void writeVec3b(const CRCore::crVector3b& v);
	void writeVec4b(const CRCore::crVector4b& v);
	void writePlane(const CRCore::crPlane& v);
	void writeUByte4(const CRCore::crVector4ub& v);
	void writeQuat(const CRCore::crQuat& q);
	void writeBinding(CRCore::crGeometry::AttributeBinding b);
	void writeArray(const CRCore::Array* a);
	void writeIntArray(const CRCore::IntArray* a);
	void writeUByteArray(const CRCore::UByteArray* a);
	void writeUShortArray(const CRCore::UShortArray* a);
	void writeUIntArray(const CRCore::UIntArray* a);
	void writeUByte4Array(const CRCore::Vec4ubArray* a);
	void writeFloatArray(const CRCore::FloatArray* a);
	void writeVec2Array(const CRCore::Vec2Array* a);
	void writeVec3Array(const CRCore::Vec3Array* a);
	void writeVec4Array(const CRCore::Vec4Array* a);
	void writeMatrixf(const CRCore::crMatrixf& mat);
	void writeMatrixd(const CRCore::crMatrixd& mat);

	void writeStateSet(const CRCore::crStateSet* stateset);
	void writeStateAttribute(const CRCore::crStateAttribute* sa);
    void writeUniform(const CRCore::crUniform* uniform);
	void writeShader(const CRCore::crShader* shader);
	void writeDrawable(const CRCore::crDrawable* sa);
	void writeShape(const CRCore::crShape* sa);
	void writeNode(const CRCore::crNode* sa);

	void writeCondition(const CRCore::Condition*);
	void writeSendMsg(const CRCore::SendMsg*);
	void writeECondition(const CRCore::ECondition*);
	void writeEvent(const CRCore::crEvent*);
	void writeEventMsg(const CRCore::crEventMsg*);
//	void writePPC(const CRCore::ParseParamCallback*);

	void writeJointParam(const CRPhysics::crJointParameter*);
	void writeJoint(const CRPhysics::crJoint*);

	//void writeSoundState(const CRAL::crSoundState*);
	//void writeSoundOccludeCallback(const CRAL::crOccludeCallback*);

	void writeCollideCallback(const CRCore::crCollideCallback*);

	// Set and get include image data in stream
	void setIncludeImageData(bool b) {m_includeImageData=b;};
	bool getIncludeImageData() {return m_includeImageData;};

	// Set and get include external references in stream
	void setIncludeExternalReferences(bool b) {m_includeExternalReferences=b;};
	bool getIncludeExternalReferences() {return m_includeExternalReferences;};

	// Set and get if must be generated external reference cre files
	void setWriteExternalReferenceFiles(bool b) {m_writeExternalReferenceFiles=b;};
	bool getWriteExternalReferenceFiles() {return m_writeExternalReferenceFiles;};

	// Set and get if must be used original external reference files
	void setUseOriginalExternalReferences(bool b) {m_useOriginalExternalReferences=b;};
	bool getUseOriginalExternalReferences() {return m_useOriginalExternalReferences;};

	const std::string& getWriteImagePath(){ return m_writeImagePath; }

	bool                m_verboseOutput;

	const std::string& getWriteFileName(){ return m_writeFileName; }
	void pushDataFile(const std::string &datafile);
	void getCurrentDataFile(std::string &datafile);
	void popDataFile();
	//CREncapsulation::crTableIO *getFilesTab(){ return m_filesTab.get(); }
	//bool addFiles(const std::string &filename);
	//void addImageFiles(const std::string &imageid);
	//void addCubeImageFiles(const std::string &cubeImageNameID);
	void addTabFiles(const std::string &filename);
	void saveFilesTab();
private:
	std::ostream* m_ostream;

 	// Container to map stateset uniques to their respective stateset.
	typedef std::map<const CRCore::crStateSet*,int>        StateSetMap;
	typedef std::map<const CRCore::crStateAttribute*,int>  StateAttributeMap;
	typedef std::map<const CRCore::crUniform*,int>         UniformMap;
	typedef std::map<const CRCore::crShader*,int>          ShaderMap;
	typedef std::map<const CRCore::crDrawable*,int>        DrawableMap;
	typedef std::map<const CRCore::crShape*,int>           ShapeMap;
	typedef std::map<const CRCore::crNode*,int>            NodeMap;
        
	typedef std::map<const CRCore::Condition*,int>           ConditionMap;
	typedef std::map<const CRCore::SendMsg*,int>             SendMsgMap;
	typedef std::map<const CRCore::ECondition*,int>          EConditionMap;
	typedef std::map<const CRCore::crEvent*,int>             EventMap;
	typedef std::map<const CRCore::crEventMsg*,int>          EventMsgMap;
	typedef std::map<const CRCore::ParseParamCallback*,int>  PPCMap;

	typedef std::map<const CRPhysics::crJointParameter*,int> JointParamMap;
    typedef std::map<const CRPhysics::crJoint*,int>          JointMap;
	
	//typedef std::map<const CRAL::crSoundState*,int>          SoundStateMap;
	//typedef std::map<const CRAL::crOccludeCallback*,int>     SoundOccludeCallbackMap;

	typedef std::map<const CRCore::crCollideCallback*, int>  CollideCallbackMap;

    StateSetMap         m_stateSetMap;
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
   // SoundOccludeCallbackMap m_soundOccludeCallbackMap;

	CollideCallbackMap  m_collideCallbackMap;

	bool                m_includeImageData;
	bool                m_includeExternalReferences;
	bool                m_writeExternalReferenceFiles;
	bool                m_useOriginalExternalReferences;
	std::string         m_writeImagePath;
	
	CRCore::ref_ptr<const CRIOManager::crReaderWriter::Options> m_options;

	typedef std::stack< CRCore::ref_ptr<CRCore::crNode> > NodeStack;
	NodeStack m_tempNodeStack;

	std::string m_writeFileName;
	typedef std::stack<std::string> DataFileStack;
    DataFileStack m_dataFileStack;

	CRCore::ref_ptr<CREncapsulation::crTableIO> m_filesTab;
	bool acceptsExtension(const std::string& extension);
};

}
#endif // CRE_DATAOUTPUTSTREAM
