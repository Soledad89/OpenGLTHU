///////////////////////class CMesh////////////////////////////////////
//////////////////////////////////////////////////////////////////////
#ifndef MESH_H
#define MESH_H

#include "model.h"
#include <vector>
#include <list>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

typedef unsigned int UINT;
typedef std::list<Vector3D>	_VECTORLIST;
typedef std::list<UINT>		_UINTLIST;

#define MAX_VERTEX_PER_FACE 20

class CVertex
{
public:
	Vector3D	m_vPosition;
	UINT*		m_piEdge;
	_UINTLIST	m_lEdgeList;
	short		m_nValence;
	Vector3D	m_vNormal;
	bool		m_bIsBoundary;
	int			m_nCutValence;
	UINT		m_color;

public:
	//constructions
	CVertex() { m_piEdge=NULL; m_nValence=0; m_nCutValence = 0; m_bIsBoundary = false; m_color = 0;}
	CVertex(double x,double y,double z) {m_vPosition=Vector3D(x,y,z);m_piEdge=NULL; m_nValence=0; m_bIsBoundary = false; m_nCutValence = 0;}
	CVertex(Vector3D v) {m_vPosition=v;m_piEdge=NULL; m_nValence=0; m_bIsBoundary = false; m_nCutValence = 0;}
	virtual ~CVertex();

	//operations
	CVertex& operator = (CVertex& v);

};

class CTexture
{
public:
	Vector2D m_vPosition;
public:
	CTexture() {m_vPosition = Vector2D(0, 0);}
	CTexture(double x, double y) {m_vPosition = Vector2D(x, y);}
	CTexture(Vector2D v) {m_vPosition = v;}
};

class CEdge
{
public:
	UINT	m_iVertex[2];

	UINT	m_iTwinEdge;
	UINT	m_iNextEdge;
	UINT	m_iFace;
	UINT	m_color;
	double  m_length;

public:		
	bool	m_bCut;
	int		m_nCutTag;

public:
	//constructions
	CEdge() {
		m_iVertex[0]=m_iVertex[1]=m_iTwinEdge=m_iNextEdge=m_iFace=-1; 
		m_bCut = false; m_nCutTag = 0;m_color = 0;m_length = 0;
	}
	CEdge(UINT iV0, UINT iV1) { m_iVertex[0]=iV0; m_iVertex[1]=iV1;m_iTwinEdge=m_iNextEdge=m_iFace=-1; m_bCut = false; m_nCutTag = 0;}
	virtual ~CEdge();

	//operations
	CEdge& operator = (const CEdge& e);
};

class CFace
{
public:
	short	m_nType;
	UINT*	m_piVertex;
	UINT*	m_piEdge;
	Vector3D m_vNormal;
	Vector3D m_vMassPoint;
	double	m_dArea;
	UINT	m_color;

public:
	//constructions
	CFace() {m_nType=0;m_piVertex=m_piEdge=NULL;m_vNormal=Vector3D(0.0,0.0,1.0);m_dArea=0.0;m_color = 0;}
	CFace(short s);
	virtual ~CFace();

	//operations
	void Create(short s);
	CFace& operator = (const CFace& f);
};

class CMesh :public CModel
{
public:
	UINT		m_nVertex;
	CVertex*	m_pVertex;

	std::vector<CTexture> m_pTexture;
	CTexture maxTex;

	UINT		m_nEdge;
	CEdge*		m_pEdge;
	UINT		m_nFace;
	CFace*		m_pFace;
	UINT		m_nVertexCapacity;
	UINT		m_nEdgeCapacity;
	UINT		m_nFaceCapacity;

	std::vector<Vector3D> isolatedPoints;

	double *m_pAngles;

	std::string		filename;
	double scaleD;
	Vector3D origin;

	double avgEdgeLength;

	//temp
	_UINTLIST m_lFocusEdge;
	_UINTLIST m_lFocusVertex;
	_UINTLIST m_lFocusFace;
	UINT	m_iPickedFace;
	UINT	m_iPickedEdge;
	UINT	m_iPickedVertex;

	bool	m_bClosed;

public:
	CMesh() {
		m_nVertex=m_nEdge=m_nFace=0;
		m_pVertex=NULL;m_pEdge=NULL;m_pFace=NULL;m_pAngles=NULL;
		m_iPickedFace=m_iPickedEdge=m_iPickedVertex=-1;
	}
	CMesh(CMesh* pMesh);
	virtual ~CMesh();

public:
	bool	Load(const char* sFileName);	// load from file
	bool	Save(const char* sFileName);	// save to file

	bool	construct();// construct connectivity

	CMesh*	clone();

	UINT	split(UINT iEdge);

	void	calcAllEdgeLength();
private:
	void	clear();
	bool	reConstruct();// construct connectivity from current mesh
	bool	loadFromSMF(const char* sFileName);
	bool	saveToSMF(const char* sFileName);

	void	calFaceNormal(UINT i);
	void	calVertexNormal(UINT i);
	void	expandCapacity();
	void calcVertexAngle();
};

#endif //MESH_H
