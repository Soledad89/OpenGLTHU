#ifndef _MODEL_H_
#define _MODEL_H_

#include "geometry.h"

class CModel
{
public:
	CModel() {}
public:
	Vector3D m_vTranslation;
	Vector3D m_vRotation;
	double m_color[3];

};

#endif
