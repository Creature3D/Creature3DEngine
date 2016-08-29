/********************************************************************
	      Creature3D - Copyright (C) 2005 
                      Îâ²Æ»ª
	
*********************************************************************/

#ifndef CRCORE_CRBLOCKDETECTTHREAD
#define CRCORE_CRBLOCKDETECTTHREAD 1

#include <CRCore/crExport.h>
#include <CRCore/ref_ptr.h>
#include <CRCore/Referenced.h>
#include <CRCore/thread/crThread.h>
namespace CRCore
{
	class CR_EXPORT crBlockDetectThread: public CRCore::crThread, public CRCore::Referenced
	{
	public:
		crBlockDetectThread();
		static crBlockDetectThread *getInstance();

		virtual void clear();

		virtual void run();
		void update();
		void done();
		void setTimeout(int timeout);
		void pause();
		void resume();
	protected:
		virtual ~crBlockDetectThread();
		static CRCore::ref_ptr<crBlockDetectThread> m_instance;
	private:
		bool m_done;
		int m_timer;
		int m_timeout;
		bool m_pause;
	};
}

#endif
