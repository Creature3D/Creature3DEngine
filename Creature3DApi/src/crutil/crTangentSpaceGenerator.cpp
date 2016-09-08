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
#include <CRUtil/crTangentSpaceGenerator.h>
#include <CRCore/crNotify.h>

using namespace CRUtil;

crTangentSpaceGenerator::crTangentSpaceGenerator():	
    CRCore::Referenced(),
	m_T(new CRCore::Vec3Array),
	m_B(new CRCore::Vec3Array),
	m_N(new CRCore::Vec3Array)
{
}

crTangentSpaceGenerator::crTangentSpaceGenerator(const crTangentSpaceGenerator &copy, const CRCore::crCopyOp &copyop):	
    CRCore::Referenced(copy),
	m_T(static_cast<CRCore::Vec3Array *>(copyop(copy.m_T.get()))),
	m_B(static_cast<CRCore::Vec3Array *>(copyop(copy.m_B.get()))),
	m_N(static_cast<CRCore::Vec3Array *>(copyop(copy.m_N.get())))
{
}

bool crTangentSpaceGenerator::generate(CRCore::crGeometry *geo, int normal_map_tex_unit)
{
	const CRCore::Array *vx = geo->getVertexArray();
	const CRCore::Array *nx = geo->getNormalArray();
	const CRCore::Array *tx = geo->getTexCoordArray(normal_map_tex_unit);

	if (!vx || !nx || !tx) return false;

	unsigned int vertex_count = vx->getNumElements();

	m_T->assign(vertex_count, CRCore::crVector3());
	m_B->assign(vertex_count, CRCore::crVector3());
	m_N->assign(vertex_count, CRCore::crVector3());

	unsigned i; // VC6 doesn't like for-scoped variables

	for (unsigned pri=0; pri<geo->getNumPrimitives(); ++pri) 
	{
		CRCore::crPrimitive *pset = geo->getPrimitive(pri);

		unsigned N = pset->getNumIndices();

		switch (pset->getMode()) 
		{
			case CRCore::crPrimitive::PT_TRIANGLES:
				for (i=0; i<N; i+=3) 
				{
					compute_basis_vectors(pset, vx, nx, tx, i, i+1, i+2);
				}
				break;

			case CRCore::crPrimitive::PT_QUADS:
				for (i=0; i<N; i+=4) 
				{
					compute_basis_vectors(pset, vx, nx, tx, i, i+1, i+2);
					compute_basis_vectors(pset, vx, nx, tx, i+2, i+3, i);
				}
				break;

			case CRCore::crPrimitive::PT_TRIANGLE_STRIP:
				if (pset->getType() == CRCore::crPrimitive::DrawArrayLengthsPrimitiveType) 
				{
					CRCore::DrawArrayLengths *dal = static_cast<CRCore::DrawArrayLengths *>(pset);
					unsigned j = 0;
					for (CRCore::DrawArrayLengths::PT_IndexArray::const_iterator pi=dal->getIndexArray().begin(); 
						 pi!=dal->getIndexArray().end(); 
						 ++pi) 
					{
						unsigned iN = static_cast<unsigned>(*pi-2);
						for (i=0; i<iN; ++i, ++j) 
						{
							if ((i%2) == 0) 
							{
								compute_basis_vectors(pset, vx, nx, tx, j, j+1, j+2);
							} 
							else 
							{
								compute_basis_vectors(pset, vx, nx, tx, j+1, j, j+2);
							}
						}
						j += 2;
					}
				} 
				else 
				{
					for (i=0; i<N-2; ++i) 
					{
						if ((i%2) == 0) 
						{
							compute_basis_vectors(pset, vx, nx, tx, i, i+1, i+2);							
						} 
						else 
						{
							compute_basis_vectors(pset, vx, nx, tx, i+1, i, i+2);
						}
					}
				}
				break;

			case CRCore::crPrimitive::PT_TRIANGLE_FAN:
				if (pset->getType() == CRCore::crPrimitive::DrawArrayLengthsPrimitiveType) 
				{
					CRCore::DrawArrayLengths *dal = static_cast<CRCore::DrawArrayLengths *>(pset);
					unsigned j = 0;
					for (CRCore::DrawArrayLengths::PT_IndexArray::const_iterator pi=dal->getIndexArray().begin(); 
						 pi!=dal->getIndexArray().end(); 
						 ++pi) 
					{
						unsigned iN = static_cast<unsigned>(*pi-2);
						for (i=0; i<iN; ++i) 
						{
							compute_basis_vectors(pset, vx, nx, tx, 0, j+1, j+2);
						}
						j += 2;
					}
				} 
				else 
				{
					for (i=0; i<N-2; ++i) 
					{
						compute_basis_vectors(pset, vx, nx, tx, 2, i+1, i+2);
					}
				}
				break;

			case CRCore::crPrimitive::PT_POINTS:
			case CRCore::crPrimitive::PT_LINES:
			case CRCore::crPrimitive::PT_LINE_STRIP:
			case CRCore::crPrimitive::PT_LINE_LOOP:
				break;

			default: CRCore::notify(CRCore::WARN) << "Warning: crTangentSpaceGenerator: unknown primitive mode " << pset->getMode() << "\n";
		}
	}

	// normalize basis vectors and force the normal vector to match
	// the triangle normal's direction

	for (i=0; i<vertex_count; ++i) 
	{
		CRCore::crVector3 &vT = (*m_T)[i];
		CRCore::crVector3 &vB = (*m_B)[i];
		CRCore::crVector3 &vN = (*m_N)[i];
	
		CRCore::crVector3 txN = CRCore::crVector3(vT.x(), vT.y(), vT.z()) ^ CRCore::crVector3(vB.x(), vB.y(), vB.z());

		if (txN * CRCore::crVector3(vN.x(), vN.y(), vN.z()) >= 0) 
		{
			vN = txN;/*CRCore::crVector3(txN, 0);*/
		} 
		else 
		{
			vN = -txN;/*CRCore::crVector3(-txN, 0);*/
		}

		vT.normalize();
		vB.normalize();
		vN.normalize();
	}
    return true;
}

void crTangentSpaceGenerator::compute_basis_vectors(CRCore::crPrimitive *pset, const CRCore::Array *vx, const CRCore::Array *nx, const CRCore::Array *tx, int iA, int iB, int iC)
{
	iA = pset->index(iA);
	iB = pset->index(iB);
	iC = pset->index(iC);

	CRCore::crVector3 P1;
	CRCore::crVector3 P2;
	CRCore::crVector3 P3;

	int i; // VC6 doesn't like for-scoped variables

	switch (vx->getType())
	{
		case CRCore::Array::Vec2ArrayType:
			for (i=0; i<2; ++i) 
			{
				P1.ptr()[i] = static_cast<const CRCore::Vec2Array&>(*vx)[iA].ptr()[i];
				P2.ptr()[i] = static_cast<const CRCore::Vec2Array&>(*vx)[iB].ptr()[i];
				P3.ptr()[i] = static_cast<const CRCore::Vec2Array&>(*vx)[iC].ptr()[i];
			}
			break;

		case CRCore::Array::Vec3ArrayType:
			P1 = static_cast<const CRCore::Vec3Array&>(*vx)[iA];
			P2 = static_cast<const CRCore::Vec3Array&>(*vx)[iB];
			P3 = static_cast<const CRCore::Vec3Array&>(*vx)[iC];
			break;

		case CRCore::Array::Vec4ArrayType:
			for (i=0; i<3; ++i) 
			{
				P1.ptr()[i] = static_cast<const CRCore::Vec3Array&>(*vx)[iA].ptr()[i];
				P2.ptr()[i] = static_cast<const CRCore::Vec3Array&>(*vx)[iB].ptr()[i];
				P3.ptr()[i] = static_cast<const CRCore::Vec3Array&>(*vx)[iC].ptr()[i];
			}
			break;

		default:
			CRCore::notify(CRCore::WARN) << "Warning: crTangentSpaceGenerator: vertex array must be Vec2Array, Vec3Array or Vec3Array" << std::endl;
	}

	CRCore::crVector3 N1;
	CRCore::crVector3 N2;
	CRCore::crVector3 N3;

	if(nx)
	{
		switch (nx->getType())
		{
			case CRCore::Array::Vec2ArrayType:
				for (i=0; i<2; ++i) 
				{
					N1.ptr()[i] = static_cast<const CRCore::Vec2Array&>(*nx)[iA].ptr()[i];
					N2.ptr()[i] = static_cast<const CRCore::Vec2Array&>(*nx)[iB].ptr()[i];
					N3.ptr()[i] = static_cast<const CRCore::Vec2Array&>(*nx)[iC].ptr()[i];
				}
				break;

			case CRCore::Array::Vec3ArrayType:
				N1 = static_cast<const CRCore::Vec3Array&>(*nx)[iA];
				N2 = static_cast<const CRCore::Vec3Array&>(*nx)[iB];
				N3 = static_cast<const CRCore::Vec3Array&>(*nx)[iC];
				break;

			case CRCore::Array::Vec4ArrayType:
				for (i=0; i<3; ++i) 
				{
					N1.ptr()[i] = static_cast<const CRCore::Vec3Array&>(*nx)[iA].ptr()[i];
					N2.ptr()[i] = static_cast<const CRCore::Vec3Array&>(*nx)[iB].ptr()[i];
					N3.ptr()[i] = static_cast<const CRCore::Vec3Array&>(*nx)[iC].ptr()[i];
				}
				break;

			default:
				CRCore::notify(CRCore::WARN) << "Warning: crTangentSpaceGenerator: normal array must be Vec2Array, Vec3Array or Vec3Array" << std::endl;
		}
	}

	CRCore::crVector2 uv1;
	CRCore::crVector2 uv2;
	CRCore::crVector2 uv3;

	switch (tx->getType())
	{
		case CRCore::Array::Vec2ArrayType:
			uv1 = static_cast<const CRCore::Vec2Array&>(*tx)[iA];
			uv2 = static_cast<const CRCore::Vec2Array&>(*tx)[iB];
			uv3 = static_cast<const CRCore::Vec2Array&>(*tx)[iC];
			break;

		case CRCore::Array::Vec3ArrayType:
			for (i=0; i<2; ++i) 
			{
				uv1.ptr()[i] = static_cast<const CRCore::Vec3Array&>(*tx)[iA].ptr()[i];
				uv2.ptr()[i] = static_cast<const CRCore::Vec3Array&>(*tx)[iB].ptr()[i];
				uv3.ptr()[i] = static_cast<const CRCore::Vec3Array&>(*tx)[iC].ptr()[i];
			}
			break;

		case CRCore::Array::Vec4ArrayType:
			for (i=0; i<2; ++i) 
			{
				uv1.ptr()[i] = static_cast<const CRCore::Vec3Array&>(*tx)[iA].ptr()[i];
				uv2.ptr()[i] = static_cast<const CRCore::Vec3Array&>(*tx)[iB].ptr()[i];
				uv3.ptr()[i] = static_cast<const CRCore::Vec3Array&>(*tx)[iC].ptr()[i];
			}
			break;

		default:
			CRCore::notify(CRCore::WARN) << "Warning: crTangentSpaceGenerator: texture coord array must be Vec2Array, Vec3Array or Vec3Array" << std::endl;
	}

	if(nx)
	{
		CRCore::crVector3 V, T1, T2, T3, B1, B2, B3;

		V = CRCore::crVector3(P2.x() - P1.x(), uv2.x() - uv1.x(), uv2.y() - uv1.y()) ^ 
			CRCore::crVector3(P3.x() - P1.x(), uv3.x() - uv1.x(), uv3.y() - uv1.y());
		if (V.x() != 0) 
		{
			V.normalize();
			T1.x() += -V.y() / V.x();
			B1.x() += -V.z() / V.x();
			T2.x() += -V.y() / V.x();
			B2.x() += -V.z() / V.x();
			T3.x() += -V.y() / V.x();
			B3.x() += -V.z() / V.x();			
		}

		V = CRCore::crVector3(P2.y() - P1.y(), uv2.x() - uv1.x(), uv2.y() - uv1.y()) ^ 
			CRCore::crVector3(P3.y() - P1.y(), uv3.x() - uv1.x(), uv3.y() - uv1.y());
		if (V.x() != 0) 
		{
			V.normalize();
			T1.y() += -V.y() / V.x();
			B1.y() += -V.z() / V.x();
			T2.y() += -V.y() / V.x();
			B2.y() += -V.z() / V.x();
			T3.y() += -V.y() / V.x();
			B3.y() += -V.z() / V.x();			
		}

		V = CRCore::crVector3(P2.z() - P1.z(), uv2.x() - uv1.x(), uv2.y() - uv1.y()) ^ 
			CRCore::crVector3(P3.z() - P1.z(), uv3.x() - uv1.x(), uv3.y() - uv1.y());
		if (V.x() != 0) 
		{
			V.normalize();
			T1.z() += -V.y() / V.x();
			B1.z() += -V.z() / V.x();
			T2.z() += -V.y() / V.x();
			B2.z() += -V.z() / V.x();
			T3.z() += -V.y() / V.x();
			B3.z() += -V.z() / V.x();			
		}

		CRCore::crVector3 tempvec;
		tempvec = N1 ^ T1;
		(*m_T)[iA] = tempvec ^ N1;//CRCore::crVector4(tempvec ^ N1, 0);
		tempvec = B1 ^ N1;
		(*m_B)[iA] = N1 ^ tempvec;//CRCore::crVector4(N1 ^ tempvec, 0);
		tempvec = N2 ^ T2;
		(*m_T)[iB] = tempvec ^ N2;//CRCore::crVector4(tempvec ^ N2, 0);
		tempvec = B2 ^ N2;
		(*m_B)[iB] = N2 ^ tempvec;//CRCore::crVector4(N2 ^ tempvec, 0);
		tempvec = N3 ^ T3;
		(*m_T)[iC] = tempvec ^ N3;//CRCore::crVector4(tempvec ^ N3, 0);
		tempvec = B3 ^ N3;
		(*m_B)[iC] = N3 ^ tempvec;//CRCore::crVector4(N3 ^ tempvec, 0);

		(*m_N)[iA] += N1;//CRCore::crVector4(N1, 0);
		(*m_N)[iB] += N2;//CRCore::crVector4(N2, 0);
		(*m_N)[iC] += N3;//CRCore::crVector4(N3, 0);
	}
	else
	{
		CRCore::crVector3 face_normal = (P2 - P1) ^ (P3 - P1);

		CRCore::crVector3 V;

		V = CRCore::crVector3(P2.x() - P1.x(), uv2.x() - uv1.x(), uv2.y() - uv1.y()) ^ 
			CRCore::crVector3(P3.x() - P1.x(), uv3.x() - uv1.x(), uv3.y() - uv1.y());
		if (V.x() != 0) 
		{
			V.normalize();
			(*m_T)[iA].x() += -V.y() / V.x();
			(*m_B)[iA].x() += -V.z() / V.x();
			(*m_T)[iB].x() += -V.y() / V.x();
			(*m_B)[iB].x() += -V.z() / V.x();
			(*m_T)[iC].x() += -V.y() / V.x();
			(*m_B)[iC].x() += -V.z() / V.x();			
		}

		V = CRCore::crVector3(P2.y() - P1.y(), uv2.x() - uv1.x(), uv2.y() - uv1.y()) ^ 
			CRCore::crVector3(P3.y() - P1.y(), uv3.x() - uv1.x(), uv3.y() - uv1.y());
		if (V.x() != 0) 
		{
			V.normalize();
			(*m_T)[iA].y() += -V.y() / V.x();
			(*m_B)[iA].y() += -V.z() / V.x();
			(*m_T)[iB].y() += -V.y() / V.x();
			(*m_B)[iB].y() += -V.z() / V.x();
			(*m_T)[iC].y() += -V.y() / V.x();
			(*m_B)[iC].y() += -V.z() / V.x();			
		}

		V = CRCore::crVector3(P2.z() - P1.z(), uv2.x() - uv1.x(), uv2.y() - uv1.y()) ^ 
			CRCore::crVector3(P3.z() - P1.z(), uv3.x() - uv1.x(), uv3.y() - uv1.y());
		if (V.x() != 0) 
		{
			V.normalize();
			(*m_T)[iA].z() += -V.y() / V.x();
			(*m_B)[iA].z() += -V.z() / V.x();
			(*m_T)[iB].z() += -V.y() / V.x();
			(*m_B)[iB].z() += -V.z() / V.x();
			(*m_T)[iC].z() += -V.y() / V.x();
			(*m_B)[iC].z() += -V.z() / V.x();			
		}

		(*m_N)[iA] += face_normal;//CRCore::crVector4(face_normal, 0);
		(*m_N)[iB] += face_normal;//CRCore::crVector4(face_normal, 0);
		(*m_N)[iC] += face_normal;//CRCore::crVector4(face_normal, 0);
	}
}
