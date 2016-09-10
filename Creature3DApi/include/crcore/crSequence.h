/* -*-c++-*- OpenSceneGraph - Copyright (C) 1998-2006 Robert Osfield 
 *
 * This library is open source and may be redistributed and/or modified under  
 * the terms of the OpenSceneGraph Public License (OSGPL) version 0.0 or 
 * (at your option) any later version.  The full license is in LICENSE file
 * included with this distribution, and on the openscenegraph.org website.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * OpenSceneGraph Public License for more details.
*/
//Modified by Wucaihua
#ifndef CRCORE_CRSEQUENCE_H
#define CRCORE_CRSEQUENCE_H

#include <CRCore/crSwitch.h>

namespace CRCore {

	/** crSequence is a crGroup node which allows automatic, time based
	switching between children.
	*/
	//crSequenceStateSet的要求: crSequence的子节点是crObject类型，crObject的Drawables使用共同地址，并且Drawables没有stateset。
	class CR_EXPORT crSequence : public crGroup
	{
	public :

		crSequence();

		/** Copy constructor using crCopyOp to manage deep vs shallow copy.*/
		crSequence(const crSequence&, const crCopyOp& copyop=crCopyOp::SHALLOW_COPY);

		META_Node(CRCore, crSequence);

		virtual void traverse(crNodeVisitor& nv);


		void setValue(int value) { m_nValue = value; }
		int getValue() const { return m_nValue; }

		/** Set time in seconds for child */
		void setTime(int frame, float t);

		/** Get time for child */
		float getTime(int frame) const;

		/** Interval modes */
		enum LoopMode
		{
			LOOP,
			SWING
		};

		/** Set sequence mode & interval. */
		void setInterval(LoopMode mode, int begin, int end);
		void setInterval(LoopMode mode, int begin, int end, int frameCount);

		/** Get sequence mode & interval. */
		inline void getInterval(LoopMode& mode, int& begin, int& end) const
		{
			mode = m_loopMode;
			begin = m_nBegin;
			end = m_nEnd;
		}
		/** Get sequence mode & interval. */
		void getInterval(LoopMode& mode, int& begin, int& end, int& frameCount) const;
		/** Set duration: speed-up & number of repeats */
		void setDuration(float speed, int nreps = -1);

		/** Get duration */
		inline void getDuration(float& speed, int& nreps) const
		{
			speed = m_fSpeed;
			nreps = m_nNreps;
		}

		/** crSequence modes */
		enum SequenceMode
		{
			START,
			STOP,
			PAUSE,
			RESUME
		};

		/** Set sequence mode. Start/stop & pause/resume. */
		void setMode(SequenceMode mode);

		/** Get sequence mode. */
		inline SequenceMode getMode() const { return m_mode; }

		virtual void setEffectPosition(const crVector3 &position);
		inline virtual void setEffectMatrix(const crMatrix& mat){}

		inline virtual bool isEffectIdle() const { return m_mode != START;}
		inline virtual void startEffect(){ setMode(crSequence::START); }
		inline virtual void stopEffect(){ setMode(crSequence::STOP); }
		void setVanishWhenStop(bool vanish);
		bool getVanishWhenStop();

		void lockEffect();
		void unlockEffect();

		void update(/*const crFrameStamp* framestamp*/);
		virtual void addParam(int i, const std::string &str);
	protected :

		virtual ~crSequence() {}

		int        m_nValue;

		float      m_fLast;
		std::vector<float> m_vecFrameTime;

		int m_nStep;

		LoopMode m_loopMode;
		int m_nBegin, m_nEnd;

		float m_fSpeed;
		int   m_nNreps, m_nNrepsremain;

		SequenceMode m_mode;
		bool m_vanishWhenStop;
		crMutex  m_effectMutex;

		unsigned int m_frameCount;
	};
//crMatrixSequence，crMatrixSequenceStateSet
	class CR_EXPORT crMatrixSequence : public crSequence
	{
	public :

		crMatrixSequence();

		/** Copy constructor using crCopyOp to manage deep vs shallow copy.*/
		crMatrixSequence(const crMatrixSequence&, const crCopyOp& copyop=crCopyOp::SHALLOW_COPY);

		META_Node(CRCore, crMatrixSequence);

		enum ReferenceFrame
		{
			RELATIVE_TO_PARENTS,
			RELATIVE_TO_ABSOLUTE
		};

		void setReferenceFrame(ReferenceFrame rf);
		ReferenceFrame getReferenceFrame() const { return m_referenceFrame; }

		virtual void dirtyBound(){ m_inverseDirty=true; crSequence::dirtyBound(); }

		inline virtual void setEffectPosition(const crVector3 &position) 
		{
			m_matrix.setTrans(position); dirtyBound(); 
		}
		inline virtual void setEffectMatrix(const crMatrix& mat) 
		{
			m_matrix = mat; dirtyBound(); 
		}

		//virtual void setMatrix(const crMatrix& mat) { m_matrix = mat; dirtyBound(); }

		/** Get the matrix. */
		inline const crMatrix& getMatrix() const { return m_matrix; }
		inline const crMatrix& getInverseMatrix() const
		{
			if (m_inverseDirty)
			{
				m_inverse.invert(m_matrix);
				m_inverseDirty = false;
			}
			return m_inverse;
		}
		virtual bool computeLocalToWorldMatrix(crMatrix& matrix, crNodeVisitor*) const;
		virtual bool computeWorldToLocalMatrix(crMatrix& matrix, crNodeVisitor*) const;
	protected :
		virtual ~crMatrixSequence();
		virtual bool computeBound() const;
		ReferenceFrame                      m_referenceFrame;

		crMatrix                              m_matrix;
		mutable crMatrix                      m_inverse;
		mutable bool                          m_inverseDirty;
	};

}

#endif
