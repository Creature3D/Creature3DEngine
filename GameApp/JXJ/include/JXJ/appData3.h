/**********************************************************************
*
*	文件:	   appData3.h
*
*	描述:	   formation data
*
*	作者:	   陆宾礼
*					
*
**********************************************************************/
#ifndef JXJ_APPDATA3_H
#define JXJ_APPDATA3_H
#include <CRCore/crStreamBuf.h>
#include <CRCore/crHandleManager.h>
#include <CRCore/crDataManager.h>
#include <CRCore/crWchMsg.h>
#include <gnelib.h>
#include <CRNetApp/appDataParam.h>
#include <CRNetApp/appMsg.h>
#include <CRCore/thread/crMutex.h>
#include <CRCore/thread/crScopedLock.h>
#include <CRNetApp/appDataClass.h>
#include <CREncapsulation/crTableIO.h>
#include <JXJ/appDBQuery.h>
#include <JXJ/appData.h>
#include <JXJ/appData2.h>

namespace JXJ{
	//class crBagItemData;
	//typedef std::vector< CRCore::ref_ptr<crBagItemData> > JXJItemBagVec;
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	// Formation Data Class
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	typedef std::deque< int > ShopCardIDDeq;
	class crJXJUIFormationJKData :  public CRCore::crUIData
	{// client data
	public:
		crJXJUIFormationJKData();
		crJXJUIFormationJKData(const crJXJUIFormationJKData& data);
		DataClass(JXJ, JXJUIFormationJK)
		virtual void addParam(int i, const std::string& str);
		virtual void inputParam(int i, void *param);
		virtual void getParam(int i, void*& param);
		virtual void excHandle(CREPARAM msg);
	private:
		GNE::Mutex m_dataMutex;
		JXJItemBagVec m_cards; // 当前显示的将卡列表
		int m_show_type;	// 0 - 显示用户数据中的将卡列表
		// 1 - 显示编制编辑状态下的将卡列表
		JXJItemBagVec m_updateneedcardvec;
		int m_curTainkaRow;
		int m_curDikaRow;
		int m_tiankaCount;
		int m_dikaCount;
		CRCore::crVector3i m_cardExtraActiveParam;//composeid,itemid,itemcount
		///////crJXJCardShopCanvasData
		int m_curpage;
		int m_totalpage;
		int m_puechasecardid;
		ShopCardIDDeq m_cardiddeq;
		int m_recvupgradeflag;
		GongFangMaterialVec m_gongfangmaterialvec;
		int m_gongfangmaterialindex;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	// Formation Data Class
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

	typedef std::map<int,CRCore::ref_ptr<CRNetApp::crRole>> TempRoleMap;//abstractid,role

	class crJXJUIFormationData :  public CRCore::crUIData
	{// client data
	public:
		// 兵种
		enum ARMS
		{
			ALL = -1,	// 全部
			INFANTRY = 0,	// 步兵
			ARCHER = 1,			// 弓兵
			CAVALRY =2,		// 骑兵
			INSTRUMENT = 3		// 器械
		};

		crJXJUIFormationData();
		crJXJUIFormationData(const crJXJUIFormationData& data);
		DataClass(JXJ, JXJUIFormation)
		virtual void addParam(int i, const std::string& str);
		virtual void inputParam(int i, void *param);
		virtual void getParam(int i, void*& param);
		virtual void excHandle(CREPARAM msg);
	private:
		GNE::Mutex m_dataMutex;
		char m_selecting;
		FormationInfoVec m_formationInfoVec;
		int	 m_arms[8]; // left arms
		int firstcardindex;
		int secondcardindex;
		TempRoleMap m_tempRoleMap;
		//int  eqps[3]; // right troops
		//int  heds[9]; // right troop jks
		//bool m_is_move; // skip move show
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	// Battle Data Class
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

	typedef std::map< unsigned short,CRCore::ref_ptr<CRCore::crWidgetNode> >ChengchiPlayFireMap;//城池ID
	class crJXJUIBattleData :  public CRCore::crUIData
	{// client data
	public:
		crJXJUIBattleData();
		crJXJUIBattleData(const crJXJUIBattleData& data);
		DataClass(JXJ, JXJUIBattle)
		virtual void addParam(int i, const std::string& str);
		virtual void inputParam(int i, void *param);
		virtual void getParam(int i, void*& param);
		virtual void excHandle(CREPARAM msg);
	private:
		GNE::Mutex m_dataMutex;
		//int m_selected;
		int m_cityliststartindexA;
		int m_cityliststartindexD;
		std::set<unsigned short> m_battlecities;
		std::set<unsigned short> m_cityinbattle;
		std::set<unsigned short> m_mycitiesset;
		ChengchiPlayFireMap m_playfiremap;
		unsigned char m_IsFirst;
		unsigned short m_battlecity;
		std::set<unsigned short> m_cityindeque;
	};

	class CantonInfo: public CRCore::Referenced
	{
	public:
		CantonInfo(int ID,std::string groupName,std::string groupLeader):m_groupID(ID),m_groupName(groupName),m_groupLeader(groupLeader){}
		int m_groupID;
		std::string m_groupName;
		std::string m_groupLeader;
	};

	typedef std::map<int,CRCore::ref_ptr<CantonInfo> > CantonInfoMap;

	typedef std::vector<CRCore::ref_ptr<CRCore::crWidgetNode>> RebelWidgetVec;
	class crJXJUICityMapData :  public CRCore::crUIData
	{
	public:
		crJXJUICityMapData();
		crJXJUICityMapData(const crJXJUICityMapData& data);
		DataClass(JXJ, JXJUICityMap)
			virtual void addParam(int i, const std::string& str);
		virtual void inputParam(int i, void *param);
		virtual void getParam(int i, void*& param);
	protected:
		GNE::Mutex m_dataMutex;
		CRCore::crVector2 m_mousePos;
		CRCore::crVector2 m_centerOffset;
		CRCore::crVector3 m_scale;
		CRCore::crVector3 m_scale2;
		CRCore::crVector2 m_offset;
		bool m_isCanDrag;
		RebelWidgetVec m_rebelCanvasVec;
		RebelCityVec m_rebelCityVec;
		unsigned short m_chooseRebelCity;
		float m_turnTimer;
		unsigned char m_rebelCount;
		CRCore::ref_ptr<CRCore::crWidgetNode> m_widgetNode;
		CantonInfoMap m_chengchiInfoMap;		// 用于国战地图显示城池太守显示
	};
	class crJXJUIDamageData :  public CRCore::crUIData
	{
	public:
		crJXJUIDamageData();
		crJXJUIDamageData(const crJXJUIDamageData& data);
		DataClass(JXJ, JXJUIDamage)
		virtual void addParam(int i, const std::string& str);
		virtual void inputParam(int i, void *param);
		virtual void getParam(int i, void*& param);
		virtual void excHandle(CREPARAM msg);
	protected:
		GNE::Mutex m_dataMutex;
		CRCore::ref_ptr<CRNetApp::crInstanceItem> m_role;
		CRCore::ref_ptr<CRPhysics::crViewMatterObject> m_object;
		CRCore::crVector3 m_pos;
		//bool m_isUsed;
	};

	class crJXJUITongGaoData :  public CRCore::crUIData
	{
	public:
		crJXJUITongGaoData();
		crJXJUITongGaoData(const crJXJUITongGaoData& data);
		DataClass(JXJ, JXJUITongGao)
			virtual void addParam(int i, const std::string& str);
		virtual void inputParam(int i, void *param);
		virtual void getParam(int i, void*& param);
		virtual void excHandle(CREPARAM msg);
	protected:
		GNE::Mutex m_dataMutex;
		std::deque<std::string> m_tonggaodeq;
		char m_time;
		//bool m_isUsed;
	};
	class crJXJBattleBehaveData :  public CRCore::crUIData
	{
	public:
		crJXJBattleBehaveData();
		crJXJBattleBehaveData(const crJXJBattleBehaveData& data);
		DataClass(JXJ, JXJBattleBehave)
			virtual void addParam(int i, const std::string& str);
		virtual void inputParam(int i, void *param);
		virtual void getParam(int i, void*& param);
		virtual void excHandle(CREPARAM msg);
	protected:
		GNE::Mutex m_dataMutex;
		char m_time;
		//bool m_isUsed;
	};

	struct PlayerData
	{
		int playerid;
		std::string name;
	};
	class crJXJWorldFuBenData :  public CRCore::crUIData
	{// client data
	public:
		crJXJWorldFuBenData();
		crJXJWorldFuBenData(const crJXJWorldFuBenData& data);
		DataClass(JXJ, JXJWorldFuBen)
			virtual void addParam(int i, const std::string& str);
		virtual void inputParam(int i, void *param);
		virtual void getParam(int i, void*& param);
		virtual void excHandle(CREPARAM msg);
	private:
		GNE::Mutex m_dataMutex;
		CRCore::crVector3i m_countrypoints;
		int m_myinfomation[6];
		bool m_iscanshow;
		unsigned char m_huodongid;//当前活动号
		std::set<int> m_praisedid;
		std::multimap<int,PlayerData,std::greater<int>> m_pointsrank;
		std::multimap<int,PlayerData,std::greater<int>> m_deadrank;
		std::multimap<int,PlayerData,std::greater<int>> m_killrank;
		unsigned char m_temactiom;
		int m_openday;
		int m_tempcountrypoints;
	};

	class crJXJWorldStatusData :  public CRCore::crUIData
	{// client data
	public:
		crJXJWorldStatusData();
		crJXJWorldStatusData(const crJXJWorldStatusData& data);
		DataClass(JXJ, JXJWorldStatus)
			virtual void addParam(int i, const std::string& str);
		virtual void inputParam(int i, void *param);
		virtual void getParam(int i, void*& param);
		virtual void excHandle(CREPARAM msg);
	private:
		GNE::Mutex m_dataMutex;
		int m_remaintime;
		int m_time;
		std::string m_event;
		//unsigned char m_temactiom;
	};

	typedef std::vector< std::pair< int,std::pair< int,/*unsigned short*/int > > > BattleAddArmyInfoVec; //abstractid,roleid,count
	class crJXJUIAddArmyCanvasData :  public CRCore::crUIData
	{
	public:
		crJXJUIAddArmyCanvasData();
		crJXJUIAddArmyCanvasData(const crJXJUIAddArmyCanvasData& data);
		DataClass(JXJ, JXJUIAddArmyCanvas)
			virtual void addParam(int i, const std::string& str);
		virtual void inputParam(int i, void *param);
		virtual void getParam(int i, void*& param);
		virtual void excHandle(CREPARAM msg);
	protected:
		GNE::Mutex m_dataMutex;
		char m_case;
		int m_gold;
		BattleAddArmyInfoVec m_armyMap;
		int m_copper;
		int m_food;
		int m_wood;
		int m_iron;
		int m_horse;
		CRNetApp::crInstanceItem * m_touchCamp;
	};
	class crJXJFubenRewardCanvasData :  public CRCore::crUIData
	{
	public:
		crJXJFubenRewardCanvasData();
		crJXJFubenRewardCanvasData(const crJXJFubenRewardCanvasData& data);
		DataClass(JXJ, JXJFubenRewardCanvas)
			virtual void addParam(int i, const std::string& str);
		virtual void inputParam(int i, void *param);
		virtual void getParam(int i, void*& param);
		virtual void excHandle(CREPARAM msg);
	protected:
		GNE::Mutex m_dataMutex;
		int m_exp;
		int m_gold;
		int m_exploit;
		int m_copper;
		bool m_isWin;
		FubenDropRewardVec m_fubenDropVec;
		unsigned char m_index;
		unsigned char m_overTimes;
		int m_timer;
		unsigned char m_canReliveCount;
		int m_fubenDropID;
		unsigned char m_starCount;
	};
	typedef  std::map<_crInt64,int> FubenRushItemMap;
	class crJXJSweepCanvasData :  public CRCore::crUIData
	{
	public:
		crJXJSweepCanvasData();
		crJXJSweepCanvasData(const crJXJSweepCanvasData& data);
		DataClass(JXJ, JXJSweepCanvas)
			virtual void addParam(int i, const std::string& str);
		virtual void inputParam(int i, void *param);
		virtual void getParam(int i, void*& param);
		virtual void excHandle(CREPARAM msg);
	protected:
		GNE::Mutex m_dataMutex;
		unsigned int m_count;
		FubenRushItemMap m_fubenRushMap;
	};
	class crJXJOnlineRewardCanvasData :  public CRCore::crUIData
	{
	public:
		crJXJOnlineRewardCanvasData();
		crJXJOnlineRewardCanvasData(const crJXJOnlineRewardCanvasData& data);
		DataClass(JXJ, JXJOnlineRewardCanvas)
			virtual void addParam(int i, const std::string& str);
		virtual void inputParam(int i, void *param);
		virtual void getParam(int i, void*& param);
		virtual void excHandle(CREPARAM msg);
	protected:
		GNE::Mutex m_dataMutex;
		unsigned char m_count;
		int m_timer;
	};
	class crJXJMoveCanvasData :  public CRCore::crUIData
	{
	public:
		crJXJMoveCanvasData();
		crJXJMoveCanvasData(const crJXJMoveCanvasData& data);
		DataClass(JXJ, JXJMoveCanvas)
			virtual void addParam(int i, const std::string& str);
		virtual void inputParam(int i, void *param);
		virtual void getParam(int i, void*& param);
		virtual void excHandle(CREPARAM msg);
	protected:
		GNE::Mutex m_dataMutex;
		//float m_showTimer;
		CRCore::crVector3 m_beginPos;
		CRCore::crVector3 m_targetPos;
		CRCore::crVector3 m_speed;
		float m_distance;
	};
	class crJXJRoomMemberInfo : public CRCore::Referenced
	{
	public:
		crJXJRoomMemberInfo():m_playerIconId(0),m_playerLevel(0),m_bingfa(0),m_zhenfa(0),m_dongcha(0),m_bIsValid(false),m_shiliID(0),m_govPost(0),m_exploit(0),m_leaderPower(0)/*,m_fightPower(0)*/{}
		void setPlayerIconID(unsigned char playerIconId){m_playerIconId = playerIconId;}  
		unsigned char getPlayerIconID(){return m_playerIconId;}
		void setPlayerLv(unsigned char playerLv){m_playerLevel = playerLv;}
		unsigned char getPlayerLv(){return m_playerLevel;}
		void setBingFa(unsigned short bingfa){m_bingfa = bingfa;}
		unsigned short getBingFa(){return m_bingfa;}
		void setZhenFa(unsigned short zhenfa){m_zhenfa = zhenfa;}
		unsigned short getZhenFa(){return m_zhenfa;}
		void setDongCha(unsigned short dongcha){m_dongcha = dongcha;}
		unsigned short getDongCha(){return m_dongcha;}
		void setValid(bool isValid){m_bIsValid = isValid;}
		double getValid(){return m_bIsValid;}
		void setCharacterName(std::string strName){m_characterName = strName;}
		std::string & getCharacterName(){return m_characterName;}
		JXJLordEquipVec & getEquipVec(){return m_loardEquipVec;}
		void setPlayerShiliID(unsigned char shiliID){m_shiliID = shiliID;}
		unsigned char getPlayerShiliID(){return m_shiliID;}
		void setPlayerGovPos(unsigned char govPos){m_govPost = govPos;}
		unsigned char getPlayerGovPos(){return m_govPost;}
		void setPlayerExploit(int playerExp){m_exploit = playerExp;}
		int getPlayerExploit(){return m_exploit;}
		void setPlayerLeaderPower(short leaderPower){m_leaderPower = leaderPower;}
		short getPlayerLeaderPower(){return m_leaderPower;}
		//void setFightPower(int fightPower){m_fightPower = fightPower;}
		//int getFightPower(){return m_fightPower;}
	protected:
		unsigned char m_playerIconId;
		unsigned char m_playerLevel;
		unsigned short m_bingfa;
		unsigned short m_zhenfa;
		unsigned short m_dongcha;
		bool m_bIsValid;
		std::string m_characterName;
		JXJLordEquipVec m_loardEquipVec;
		unsigned char m_shiliID;
		unsigned char m_govPost;
		int m_exploit;
		short m_leaderPower;
		//int m_fightPower;
	};

	typedef std::map< int,CRCore::ref_ptr<crJXJRoomMemberInfo> > RoomMemberInfoMap;//playerid,crJXJRoomMemberInfo
	class crJXJBattleShowCanvasData :  public CRCore::crUIData
	{
	public:
		crJXJBattleShowCanvasData();
		crJXJBattleShowCanvasData(const crJXJBattleShowCanvasData& data);
		DataClass(JXJ, JXJBattleShowCanvas)
			virtual void addParam(int i, const std::string& str);
		virtual void inputParam(int i, void *param);
		virtual void getParam(int i, void*& param);
		virtual void excHandle(CREPARAM msg);
	protected:
		GNE::Mutex m_dataMutex;
		RoomMemberInfoMap m_roomInfoMap;
		CRCore::ref_ptr<CRNetApp::crInstanceItem> m_curItem;
	};

	class crJXJPersuadeCanvasData :  public CRCore::crUIData
	{
	public:
		crJXJPersuadeCanvasData();
		crJXJPersuadeCanvasData(const crJXJPersuadeCanvasData& data);
		DataClass(JXJ, JXJPersuadeCanvas)
			virtual void addParam(int i, const std::string& str);
		virtual void inputParam(int i, void *param);
		virtual void getParam(int i, void*& param);
		virtual void excHandle(CREPARAM msg);
	protected:
		GNE::Mutex m_dataMutex;
		unsigned short m_fubenid;
		int m_jkid;
		CRCore::crCanvasNode * m_canvas;
		CRCore::crVector3i m_countrystrength;
	};
	typedef std::multimap<int,std::pair<int,int>> LandRewardMap;//type,id,count
	class crJXJLandRewardCanvasData :  public CRCore::crUIData
	{
	public:
		crJXJLandRewardCanvasData();
		crJXJLandRewardCanvasData(const crJXJLandRewardCanvasData& data);
		DataClass(JXJ, JXJLandRewardCanvas)
			virtual void addParam(int i, const std::string& str);
		virtual void inputParam(int i, void *param);
		virtual void getParam(int i, void*& param);
		virtual void excHandle(CREPARAM msg);
	protected:
		GNE::Mutex m_dataMutex;
		LandRewardMap m_lanRewadMap;
		unsigned char m_nMulti;
		bool m_bHas;
	};

	typedef std::map<char,CRCore::ref_ptr<crBagItemData>> ItemOrCardMap;//index;itemorcardid
	typedef std::map< char,CRCore::ref_ptr<crStoreBagItemData> > ItemSwitchMap;//将卡背包中，playerStore中
	class crJXJLunHuiCanvasData :  public CRCore::crUIData
	{
	public:
		crJXJLunHuiCanvasData();
		crJXJLunHuiCanvasData(const crJXJLunHuiCanvasData & data);
		DataClass(JXJ, JXJLunHuiCanvas)
			virtual void addParam(int i, const std::string& str);
		virtual void inputParam(int i, void *param);
		virtual void getParam(int i, void*& param);
		virtual void excHandle(CREPARAM msg);
	protected:
		GNE::Mutex m_dataMutex;
		ItemOrCardMap m_itemMap;
		ItemOrCardMap m_cardMap;
		ItemSwitchMap m_itemSwitchMap;
		ItemSwitchMap m_cardSwitchMap;
		char m_type;
		char m_replacecount;
	};

	typedef std::pair<int, float> BufferInfo;//id,timer
	typedef std::vector<BufferInfo> BufferInfoVec;
	//typedef std::map<int, BufferInfoVec> BufferInfoMap;
	
	typedef std::map<short, std::pair<short,float> > BufferInfoMap;//extraid,//secondValue

	class crBattleJKCanvasData : public CRCore::crUIData
	{
	public:
		crBattleJKCanvasData();
		crBattleJKCanvasData(const crBattleJKCanvasData & data);
		DataClass(JXJ, BattleJKCanvas)
			virtual void addParam(int i, const std::string& str);
		virtual void inputParam(int i, void *param);
		virtual void getParam(int i, void*& param);
		virtual void excHandle(CREPARAM msg);
	protected:
		GNE::Mutex m_dataMutex;
		BufferInfoMap m_bufferInfoMap[3];//
		int m_timer;
	};

	class crBufferWidgetData : public CRCore::crUIData
	{
	public:
		crBufferWidgetData();
		crBufferWidgetData(const crBufferWidgetData & data);
		DataClass(JXJ, BufferWidget)
			virtual void addParam(int i, const std::string& str);
		virtual void inputParam(int i, void *param);
		virtual void getParam(int i, void*& param);
		virtual void excHandle(CREPARAM msg);
	protected:
		GNE::Mutex m_dataMutex;
		bool m_isFlash;
	};

	class crJXJChatChannelCanvasData :  public CRCore::crUIData
	{
	public:
		crJXJChatChannelCanvasData();
		crJXJChatChannelCanvasData(const crJXJChatChannelCanvasData & data);
		DataClass(JXJ, JXJChatChannelCanvas)
			virtual void addParam(int i, const std::string& str);
		virtual void inputParam(int i, void *param);
		virtual void getParam(int i, void*& param);
		virtual void excHandle(CREPARAM msg);
	protected:
		GNE::Mutex m_dataMutex;
		BitSet64 m_chatChannelSet;
		BitSet64 m_chatChannelTempSet;
	};

	class crJXJJuQingCanvasData : public CRCore::crUIData
	{
	public:
		crJXJJuQingCanvasData();
		crJXJJuQingCanvasData(const crJXJJuQingCanvasData & data);
		DataClass(JXJ, JXJJuQingCanvas)
			virtual void addParam(int i, const std::string& str);
		virtual void inputParam(int i, void *param);
		virtual void getParam(int i, void*& param);
		virtual void excHandle(CREPARAM msg);
	protected:
		GNE::Mutex m_dataMutex;
		std::string m_strJuQingName;
		std::string m_strImgName;
		std::string m_strInfo;
		float m_time;
	};

	class crBattleReportCanvasData : public CRCore::crUIData
	{
	public:
		crBattleReportCanvasData();
		crBattleReportCanvasData(const crBattleReportCanvasData & data);
		DataClass(JXJ, BattleReportCanvas)
			virtual void addParam(int i, const std::string& str);
		virtual void inputParam(int i, void *param);
		virtual void getParam(int i, void*& param);
		virtual void excHandle(CREPARAM msg);
	protected:
		GNE::Mutex m_dataMutex;
		std::string m_needQueryPlayerName;
	};

	class crJXJStarCanvasData : public CRCore::crUIData
	{
	public:
		crJXJStarCanvasData();
		crJXJStarCanvasData(const crJXJStarCanvasData & data);
		DataClass(JXJ, JXJStarCanvas)
			virtual void addParam(int i, const std::string& str);
		virtual void inputParam(int i, void *param);
		virtual void getParam(int i, void*& param);
		virtual void excHandle(CREPARAM msg);
	protected:
		GNE::Mutex m_dataMutex;
		int m_npcKillCount;
		int m_bossKillCount;
		CRCore::crVector3i m_starInfoVec[3];
	};
	class crHeadCanvasData : public CRCore::crUIData
	{
	public:
		crHeadCanvasData();
		crHeadCanvasData(const crHeadCanvasData & data);
		DataClass(JXJ, HeadCanvas)
			virtual void addParam(int i, const std::string& str);
		virtual void inputParam(int i, void *param);
		virtual void getParam(int i, void*& param);
		virtual void excHandle(CREPARAM msg);
	protected:
		GNE::Mutex m_dataMutex;
		CRNetApp::crInstanceItem * m_instance;
	};
}

#endif