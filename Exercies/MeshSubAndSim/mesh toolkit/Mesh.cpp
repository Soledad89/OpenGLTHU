#include "Mesh.h"
#include <map>

using namespace std;

#define ITERATE_TIMES 1
#define	ALPHA_STRETCH 1.0

//#define  REMOVE_DUPLICATE_VERTEX

//////////////////////////////////////////////////////
//						CVertex						//
//////////////////////////////////////////////////////
CVertex::~CVertex()
{
	if(m_piEdge!=NULL)
		delete[] m_piEdge;
	m_lEdgeList.clear();
}

CVertex& CVertex::operator = (CVertex& v)
{
	m_vPosition = v.m_vPosition;
	m_nValence = v.m_nValence;
	m_piEdge = new UINT[m_nValence];
	for(short i=0;i<m_nValence;i++)
		m_piEdge[i] = v.m_piEdge[i];
	
	m_bIsBoundary = v.m_bIsBoundary;
	m_vNormal = v.m_vNormal;

	m_nCutValence = v.m_nCutValence;

	m_color = v.m_color;

	return *this;
}


//////////////////////////////////////////////////////
//						CEdge						//
//////////////////////////////////////////////////////

CEdge::~CEdge()
{
}

CEdge& CEdge::operator = (const CEdge& e)
{
	m_iVertex[0] = e.m_iVertex[0];
	m_iVertex[1] = e.m_iVertex[1];
	m_iTwinEdge = e.m_iTwinEdge;
	m_iNextEdge = e.m_iNextEdge;
	m_iFace = e.m_iFace;
	m_bCut = e.m_bCut;
	m_nCutTag = e.m_nCutTag;
	m_color = e.m_color;
	m_length = e.m_length;

	return *this;
}

//////////////////////////////////////////////////////
//						CFace						//
//////////////////////////////////////////////////////
CFace::CFace(short s)
{
	m_nType=s;
	m_vNormal=Vector3D(0.0,0.0,1.0);
	m_dArea=0.0;
	m_piEdge=new UINT[s];
	m_piVertex=new UINT[s];
}

CFace::~CFace()
{
	if(m_piEdge!=NULL)
		delete[] m_piEdge;
	if(m_piVertex!=NULL)
		delete[] m_piVertex;
}

void CFace::Create(short s)
{
	m_nType=s;
	m_vNormal=Vector3D(0.0,0.0,1.0);
	m_dArea=0.0;
	m_piEdge=new UINT[s];
	m_piVertex=new UINT[s];
}

CFace& CFace::operator =(const CFace& f)
{
	m_nType = f.m_nType;
	m_piVertex = new UINT[m_nType];
	m_piEdge = new UINT[m_nType];
	for(short i=0;i<m_nType;i++)
	{
		m_piVertex[i] = f.m_piVertex[i];
		m_piEdge[i] = f.m_piEdge[i];
	}
	
	m_vNormal = f.m_vNormal;
	m_vMassPoint = f.m_vMassPoint;
	m_dArea = f.m_dArea;
	m_color = f.m_color;
	
	return (*this);
}

//////////////////////////////////////////////////////
//						CMesh						//
//////////////////////////////////////////////////////
void CMesh::clear()
{
	if(m_pVertex!=NULL)	{delete[] m_pVertex;m_pVertex=NULL;}
	if(m_pEdge!=NULL)	{delete[] m_pEdge;m_pEdge=NULL;}
	if(m_pFace!=NULL)	{delete[] m_pFace;m_pFace=NULL;}
	if(m_pAngles!= NULL) {delete[] m_pAngles; m_pAngles = NULL;}
	m_nVertex=m_nEdge=m_nFace=0;
	m_lFocusEdge.clear();
	m_lFocusVertex.clear();
}

CMesh::CMesh(CMesh* pMesh)
{
	m_nVertexCapacity = pMesh->m_nVertexCapacity;
	m_nEdgeCapacity = pMesh->m_nEdgeCapacity;
	m_nFaceCapacity = pMesh->m_nFaceCapacity;

	m_nVertex = pMesh->m_nVertex;
	m_pVertex = new CVertex[m_nVertexCapacity];
	UINT i;
	for(i=0;i<m_nVertex;i++)
		m_pVertex[i] = pMesh->m_pVertex[i];

	m_nEdge = pMesh->m_nEdge;
	m_pEdge = new CEdge[m_nEdgeCapacity];
	for(i=0;i<m_nEdge;i++)
		m_pEdge[i] = pMesh->m_pEdge[i];

	m_nFace = pMesh->m_nFace;
	m_pFace = new CFace[m_nFaceCapacity];
	for(i=0;i<m_nFace;i++)
		m_pFace[i] = pMesh->m_pFace[i];

	m_pAngles = new double[m_nVertex];
	for (i = 0; i < m_nVertex; ++i)
		m_pAngles[i] = pMesh->m_pAngles[i];

	m_lFocusEdge = pMesh->m_lFocusEdge;
	m_lFocusVertex = pMesh->m_lFocusVertex;
	m_lFocusFace = pMesh->m_lFocusFace;
//	_UINTLIST m_lPickedVertex;

	m_iPickedVertex = pMesh->m_iPickedVertex;
	m_iPickedEdge = pMesh->m_iPickedEdge;
	m_iPickedFace = pMesh->m_iPickedFace;

	filename = pMesh->filename;
}

CMesh* CMesh::clone()
{
	return NULL;
}

CMesh::~CMesh()
{
	clear();	
}

bool CMesh::Load(const char* sFileName)
{
	clear();
	filename = sFileName;
	return loadFromSMF(sFileName);
}

bool CMesh::loadFromSMF(const char* sFileName)
// -----  format: smf,obj,dat -----
//vertex:
//      v x y z,
//face(triangle):
//      f v1 v2 v3  (the vertex index is 1-based)
{
	
	//open the file
	filename = sFileName;
	ifstream fin;
	fin.open(sFileName);
	if(!fin) return false;
	
	_VECTORLIST VertexList;
	_UINTLIST FaceList;

	Vector3D vector;
	Vector2D vector2d;
	Vector3D vMin(1e30, 1e30, 1e30);
	Vector3D vMax(-1e30, -1e30, -1e30);
	bool bFirst=true;

	maxTex = CTexture(0, 0);

	UINT l[3];
	char tmpC;
	short j;

	std::map<Vector3D, UINT> redunVertex;
	std::vector<UINT> realIndex;
	UINT vertexIndex = 0;

	string buf;
	while (getline(fin,buf))
	{
		if (buf.size()>0) //non-empty line
		{
			if (buf[0]=='v' && buf[1] == ' ')//A line starting with a 'v' designating a vertex position information
			{
				//%S: S same as ls. A wide character string is expected; the corresponding argument should be a wide character pointer pointing to an array of wide characters large enough to accept the wide character string and a terminating \0
				istringstream sin(buf);
				sin>>tmpC>>vector.x>>vector.y>>vector.z;
				
#ifdef REMOVE_DUPLICATE_VERTEX
				map<Vector3D, UINT>::iterator iter = redunVertex.find(vector);
				if (iter == redunVertex.end())
				{
					redunVertex[vector] = vertexIndex;
#endif
					realIndex.push_back(vertexIndex++);
					VertexList.push_back(Vector3D(vector));
#ifdef REMOVE_DUPLICATE_VERTEX
				}
				else
					realIndex.push_back(iter->second);
#endif

				if(vector.x<vMin.x)
					vMin.x=vector.x;
				if(vector.x>vMax.x)
					vMax.x=vector.x;
				if(vector.y<vMin.y)
					vMin.y=vector.y;
				if(vector.y>vMax.y)
					vMax.y=vector.y;
				if(vector.z<vMin.z)
					vMin.z=vector.z;
				if(vector.z>vMax.z)
					vMax.z=vector.z;
			}
			else if (buf[0] == 'v' && buf[1] == 't')
			{
				string buftmp = buf.substr(3, buf.length()-3);
				istringstream sin(buftmp);
				sin>>vector2d.x>>vector2d.y;
				CTexture curTexture(vector2d);
				m_pTexture.push_back(curTexture);

				if(vector2d.x > maxTex.m_vPosition.x) maxTex.m_vPosition.x = vector2d.x;
				if(vector2d.y > maxTex.m_vPosition.y) maxTex.m_vPosition.y = vector2d.y;
			}
			else if (buf[0]=='f') 
			{
				istringstream sin(buf);
				int tx, ty, tz;
				if(m_pTexture.empty()) sscanf(buf.c_str(), "f %d %d %d", &l[0], &l[1], &l[2]);
				else sscanf(buf.c_str(), "f %d/%d %d/%d %d/%d", &l[0], &tx, &l[1], &ty, &l[2], &tz);
				if (realIndex[l[0]-1] == realIndex[l[1]-1] || realIndex[l[1]-1] == realIndex[l[2]-1] || realIndex[l[2]-1] == realIndex[l[0]-1]) continue;
				//sin>>tmpC>>l[0]>>l[1]>>l[2];
				for(j=0;j<3;j++)
					FaceList.push_back(realIndex[l[j]-1]);
			}		
			else if (buf[0] == 'p')
			{
				istringstream sin(buf);
				sin>>tmpC>>vector.x>>vector.y>>vector.z;
				isolatedPoints.push_back(vector);
			}
		}
	}
	fin.close();
	vector=vMax-vMin;
	double d=__max(vector.x,vector.y);
	d=0.5*__max(d,vector.z);
	vector=0.5*(vMax+vMin);
	scaleD = d;
	origin = vector;
	
	m_nVertex=(UINT) VertexList.size();
	m_nFace=(UINT) FaceList.size()/3;
	m_nEdge=(UINT) 3*m_nFace;

	m_nVertexCapacity = m_nVertex * 2;
	m_nEdgeCapacity = m_nEdge * 2;
	m_nFaceCapacity = m_nFace * 2;

	//read vertices and faces
	m_pVertex = new CVertex[m_nVertexCapacity];
	if (m_pVertex==NULL) {clear(); return false;}//out of memory
	m_pFace = new CFace[m_nFaceCapacity];
	if (m_pFace == NULL) {clear(); return false;}//out of memory

	UINT i;
	_VECTORLIST::iterator iVertex = VertexList.begin();
	_UINTLIST::iterator iFace = FaceList.begin();

	for(i=0;i<m_nVertex;i++)
		m_pVertex[i].m_vPosition=(*(iVertex++)-vector)/d;
	for(i = 0; i < isolatedPoints.size(); ++i)
		isolatedPoints[i] = (isolatedPoints[i]-vector)/d;
	for(i=0;i<m_nFace;i++)
	{
		m_pFace[i].Create(3);
		for(j=0;j<3;j++)
			m_pFace[i].m_piVertex[j]=*iFace++;
	}

	VertexList.clear();
	FaceList.clear();
	return construct();
}

bool CMesh::Save(const char* sFileName)
{
	this->filename = sFileName;
	ofstream fout(sFileName);
	fout<<"# vertices: "<<m_nVertex<<endl;
	fout<<"# faces: "<<m_nFace<<endl;
	for (UINT i = 0; i < m_nVertex; ++i) {
		Vector3D v = m_pVertex[i].m_vPosition;
		fout<<"v "<<v.x<<" "<<v.y<<" "<<v.z<<endl;
	}
	for (UINT i = 0; i < m_nFace; ++i) {
		UINT* v = m_pFace[i].m_piVertex;
		fout<<"f "<<v[0]+1<<" "<<v[1]+1<<" "<<v[2]+1<<endl;
	}
	fout.close();
	return true;
}

void CMesh::calcAllEdgeLength(){
	avgEdgeLength = 0.0;
	for (UINT i = 0; i < m_nEdge; ++i) {
		UINT nodeA = m_pEdge[i].m_iVertex[0];
		UINT nodeB = m_pEdge[i].m_iVertex[1];
		double len = Vector3D(m_pVertex[nodeA].m_vPosition
							- m_pVertex[nodeB].m_vPosition).length();
		m_pEdge[i].m_length = len;
		avgEdgeLength += len;
	}
	avgEdgeLength /= m_nEdge;
}

bool CMesh::saveToSMF(const char* sFileName)
{
	return true;
}

bool CMesh::reConstruct()
{
	_VECTORLIST VertexList;
	_UINTLIST FaceList;

	Vector3D vector;
	Vector3D vMin;
	Vector3D vMax;
	bool bFirst=true;

	UINT i;
	UINT* old2new = new UINT[m_nVertex];
	UINT iNewIndex=0;
	for(i=0;i<m_nVertex;i++)
	{
		VertexList.push_back(m_pVertex[i].m_vPosition);
		old2new[i]=iNewIndex++;
	}

	for(i=0;i<m_nFace;i++)
		for(short j=0;j<3;j++)
			FaceList.push_back(old2new[m_pFace[i].m_piVertex[j]]);

	clear();

	m_nVertex=(UINT) VertexList.size();
	m_nFace=(UINT) FaceList.size()/3;
	m_nEdge=(UINT) 3*m_nFace;
	//read vertices and faces

	m_nVertexCapacity = m_nVertex * 2;
	m_nFaceCapacity = m_nFace * 2;

	m_pVertex = new CVertex[m_nVertexCapacity];
	if (m_pVertex==NULL) {clear(); return false;}//out of memory
	m_pFace = new CFace[m_nFaceCapacity];
	if (m_pFace == NULL) {clear(); return false;}//out of memory

	_VECTORLIST::iterator iVertex = VertexList.begin();
	_UINTLIST::iterator iFace = FaceList.begin();

	for(i=0;i<m_nVertex;i++)
		m_pVertex[i].m_vPosition=*iVertex++;
	for(i=0;i<m_nFace;i++)
	{
		m_pFace[i].Create(3);
		for(short j=0;j<3;j++)
			m_pFace[i].m_piVertex[j]=*iFace++;
	}

	delete[] old2new;
	VertexList.clear();
	FaceList.clear();
	return construct();
}

bool CMesh::construct()
{
	if ((m_pVertex==NULL)||(m_pFace==NULL)) return false;//empty

	if(m_pEdge!=NULL){delete[] m_pEdge;m_pEdge=NULL;}//delete old edgelist

	m_bClosed = true;

	UINT i;
	if(m_nEdge==0)
		for(i=0;i<m_nFace;i++)
			m_nEdge+=m_pFace[i].m_nType;

	m_nEdgeCapacity = m_nEdge * 2;
	m_pEdge = new CEdge[m_nEdgeCapacity];
	
	short j,nType;

	UINT iEdge=0;
	UINT iVertex;

	for(i=0;i<m_nFace;i++)
	{
		calFaceNormal(i);

		nType=m_pFace[i].m_nType;
		for(j=0;j<nType;j++)
		{
			m_pFace[i].m_piEdge[j]=iEdge;
			iVertex=m_pFace[i].m_piVertex[j];
			m_pVertex[iVertex].m_nValence++;
			m_pVertex[iVertex].m_lEdgeList.push_back(iEdge);
			m_pEdge[iEdge].m_iFace=i;
			m_pEdge[iEdge].m_iVertex[0]=iVertex;
			m_pEdge[iEdge].m_iVertex[1]=m_pFace[i].m_piVertex[(j+1)%nType];
			m_pEdge[iEdge].m_iNextEdge=iEdge+1;
			iEdge++;

		}
		m_pEdge[iEdge-1].m_iNextEdge-=nType;

	}

	_UINTLIST::iterator iv;
	UINT iSrcVertex,iDesVertex;
	bool bFlag;
	for(i=0;i<m_nEdge;i++)
	{
		if(m_pEdge[i].m_iTwinEdge!=-1)
			continue;
		iSrcVertex=m_pEdge[i].m_iVertex[0];
		iDesVertex=m_pEdge[i].m_iVertex[1];
		bFlag=true;
		for (iv = m_pVertex[iDesVertex].m_lEdgeList.begin(); iv != m_pVertex[iDesVertex].m_lEdgeList.end(); iv++)
		{
			iEdge=*iv;
			if(m_pEdge[iEdge].m_iVertex[1]==iSrcVertex)
			{
				m_pEdge[i].m_iTwinEdge=iEdge;
				m_pEdge[iEdge].m_iTwinEdge=i;
				bFlag=false;
				break;
			}
		}
		if(bFlag)
		{
			m_pVertex[iSrcVertex].m_bIsBoundary = true;
			m_bClosed = false;
		}

	}
	short nValence;
	UINT iTwinEdge;
	

	for(i=0;i<m_nVertex;i++)
	{
		nValence=m_pVertex[i].m_nValence;
		if(nValence<2)
			m_pVertex[i].m_bIsBoundary=true;

		if(nValence==0)
			continue;

		if(m_pVertex[i].m_piEdge!=NULL)
			delete[] m_pVertex[i].m_piEdge;
		m_pVertex[i].m_piEdge=new UINT[nValence];


//		if(m_pVertex[i].m_piEdge[0]==-1)//temp
//			return false;

		if(m_pVertex[i].m_bIsBoundary)
		{
			for (iv = m_pVertex[i].m_lEdgeList.begin(); iv != m_pVertex[i].m_lEdgeList.end(); iv++)
			{
				bFlag=true;
				m_pVertex[i].m_piEdge[0]=*iv;
				for(j=1;j<nValence;j++)
				{
					iTwinEdge=m_pEdge[m_pVertex[i].m_piEdge[j-1]].m_iTwinEdge;
					if(iTwinEdge==-1)
					{
						bFlag=false;
						break;
					}
					m_pVertex[i].m_piEdge[j]=m_pEdge[iTwinEdge].m_iNextEdge;
				}
				if(bFlag)//success
					break;
			}
		
		}
		else
		{
			iv = m_pVertex[i].m_lEdgeList.begin();
			m_pVertex[i].m_piEdge[0]=*iv;
			for(j=1;j<nValence;j++)
			{
				iTwinEdge=m_pEdge[m_pVertex[i].m_piEdge[j-1]].m_iTwinEdge;
//				if(iTwinEdge==-1)//temp
//					return false;
				m_pVertex[i].m_piEdge[j]=m_pEdge[iTwinEdge].m_iNextEdge;
			}
		}
		m_pVertex[i].m_lEdgeList.clear();
		for(j=0;j<nValence;j++)
			m_pVertex[i].m_lEdgeList.push_back(m_pVertex[i].m_piEdge[j]);

		calVertexNormal(i);
//		calVertexMeanCurvature(i);
	}

	calcAllEdgeLength();
	calcVertexAngle();
	return true;
}

void CMesh::expandCapacity(){
	if ((m_nVertex + 8 >= m_nVertexCapacity) ||
		(m_nEdge + 8 >= m_nEdgeCapacity)	 ||
		(m_nFace + 8 >= m_nFaceCapacity)){

		m_nVertexCapacity *= 2;
		m_nEdgeCapacity *= 2;
		m_nFaceCapacity *= 2;

		CVertex* m_pVertexNew = new CVertex[m_nVertexCapacity];
		for (UINT i = 0; i < m_nVertex; ++i) {
			m_pVertexNew[i] = m_pVertex[i];
		}

		CEdge* m_pEdgeNew = new CEdge[m_nEdgeCapacity];
		for (UINT i = 0; i < m_nEdge; ++i) {
			m_pEdgeNew[i] = m_pEdge[i];
		}

		CFace* m_pFaceNew = new CFace[m_nFaceCapacity];
		for (UINT i = 0; i < m_nFace; ++i) {
			m_pFaceNew[i] = m_pFace[i];
		}
		
		if (m_pVertex != NULL) delete[] m_pVertex;
		if (m_pEdge != NULL) delete[] m_pEdge;
		//if (m_pFace != NULL) delete[] m_pFace;
		m_pVertex = m_pVertexNew;
		m_pEdge = m_pEdgeNew;
		m_pFace = m_pFaceNew;
	}
}

void CMesh::calcVertexAngle()
{
	m_pAngles = new double[m_nVertex];

	for (int i = 0; i < m_nVertex; ++i)
	{
		m_pAngles[i] = 0;
		for (int j = 0; j < m_pVertex[i].m_nValence; ++j)
		{
			UINT edge0 = m_pVertex[i].m_piEdge[j];
			UINT edge1 = m_pEdge[edge0].m_iNextEdge;
			UINT edge2 = m_pEdge[edge1].m_iNextEdge;
			double len0 = m_pEdge[edge0].m_length;
			double len1 = m_pEdge[edge1].m_length;
			double len2 = m_pEdge[edge2].m_length;

			m_pAngles[i] += acos((len0*len0 + len2*len2 - len1*len1)/(2*len0*len2));
		}
	}
}

/******************************************************
       O                         O
	  / \                       /|\
	 /   \                     / | \
	O-----O   =========>      O--O--O
	 \   /                     \ | /
	  \ /                       \|/
	   O                         O
*******************************************************/
UINT CMesh::split(UINT iEdge){	//split at the middle of the edge
	if (iEdge < 0 || iEdge >= m_nEdge) return -1;
	
	double originEdgeLen = m_pEdge[iEdge].m_length;
	UINT e2 = iEdge;
	UINT e6 = m_pEdge[e2].m_iTwinEdge;
	UINT e3 = m_pEdge[e2].m_iNextEdge;
	UINT e1 = m_pEdge[e3].m_iNextEdge;

	UINT f1 = m_pEdge[e2].m_iFace;

	UINT v2 = m_pEdge[e2].m_iVertex[0];
	UINT v4 = m_pEdge[e2].m_iVertex[1];
	UINT v1 = m_pEdge[e1].m_iVertex[0];

	double newEdgeLen = 
		(m_pVertex[v1].m_vPosition - (m_pVertex[v4].m_vPosition+m_pVertex[v2].m_vPosition)/2).length();

	if (e6 == -1) {
		UINT v5 = (m_nVertex++);
		UINT e8 = (m_nEdge++);
		UINT e9 = (m_nEdge++);
		UINT e10 = (m_nEdge++);
		UINT f3 = (m_nFace++);

		//Changes on v5
		m_pVertex[v5].m_vPosition = (m_pVertex[v2].m_vPosition + m_pVertex[v4].m_vPosition) / 2;
		m_pVertex[v5].m_nValence = 2;
		m_pVertex[v5].m_piEdge = new UINT[2];
		m_pVertex[v5].m_piEdge[0] = e8;
		m_pVertex[v5].m_piEdge[1] = e9;
		m_pVertex[v5].m_bIsBoundary = true;

		//No changes on v2 and v4

		//Changes on v1
		m_pVertex[v1].m_nValence ++;
		UINT* v1piEdge = new UINT[m_pVertex[v1].m_nValence];
		for (short i = 0; i < m_pVertex[v1].m_nValence - 1; ++i) {
			v1piEdge[i] = m_pVertex[v1].m_piEdge[i];
		}
		v1piEdge[m_pVertex[v1].m_nValence - 1] = e10;
		delete[] m_pVertex[v1].m_piEdge;
		m_pVertex[v1].m_piEdge = v1piEdge;

		//No changes on e1

		//Changes on e2
		m_pEdge[e2].m_iNextEdge = e9;
		m_pEdge[e2].m_iVertex[1] = v5;
		m_pEdge[e2].m_length = originEdgeLen / 2;
		
		//Changes on e3
		m_pEdge[e3].m_iFace = f3;
		m_pEdge[e3].m_iNextEdge = e10;

		//Changes on e8
		m_pEdge[e8].m_iFace = f3;
		m_pEdge[e8].m_iNextEdge = e3;
		m_pEdge[e8].m_iTwinEdge = -1;
		m_pEdge[e8].m_iVertex[0] = v5;
		m_pEdge[e8].m_iVertex[1] = v4;
		m_pEdge[e8].m_length = originEdgeLen / 2;
	
		//Changes on e9
		m_pEdge[e9].m_iFace = f1;
		m_pEdge[e9].m_iNextEdge = e1;
		m_pEdge[e9].m_iTwinEdge = e10;
		m_pEdge[e9].m_iVertex[0] = v5;
		m_pEdge[e9].m_iVertex[1] = v1;
		m_pEdge[e9].m_length = newEdgeLen;

		//Changes on e10
		m_pEdge[e10].m_iFace = f3;
		m_pEdge[e10].m_iNextEdge = e8;
		m_pEdge[e10].m_iTwinEdge = e9;
		m_pEdge[e10].m_iVertex[0] = v1;
		m_pEdge[e10].m_iVertex[1] = v5;
		m_pEdge[e10].m_length = newEdgeLen;

		//Changes on f1
		for (int i = 0; i < 3; ++i) {
			if (m_pFace[f1].m_piEdge[i] == e3) {
				m_pFace[f1].m_piEdge[i] = e9;
			}
			if (m_pFace[f1].m_piVertex[i] == v4) {
				m_pFace[f1].m_piVertex[i] = v5;
			}
		}
		calFaceNormal(f1);

		//Changes on f3
		m_pFace[f3].Create(3);
		m_pFace[f3].m_piEdge[0] = e10;
		m_pFace[f3].m_piEdge[1] = e8;
		m_pFace[f3].m_piEdge[2] = e3;
		m_pFace[f3].m_piVertex[0] = v1;
		m_pFace[f3].m_piVertex[1] = v5;
		m_pFace[f3].m_piVertex[2] = v4;
		m_pFace[f3].m_color = m_pFace[f1].m_color;
		calFaceNormal(f3);

		calVertexNormal(v5);
		expandCapacity();

		return v5;
	} else {
		UINT f2 = m_pEdge[e6].m_iFace;
		UINT e4 = m_pEdge[e6].m_iNextEdge;
		UINT e5 = m_pEdge[e4].m_iNextEdge;
		UINT v3 = m_pEdge[e4].m_iVertex[1];
		double newEdgeLen2 = 
			(m_pVertex[v3].m_vPosition - (m_pVertex[v4].m_vPosition+m_pVertex[v2].m_vPosition)/2).length();

		UINT e7 = (m_nEdge++);
		UINT e8 = (m_nEdge++);
		UINT e9 = (m_nEdge++);
		UINT e10 = (m_nEdge++);
		UINT e11 = (m_nEdge++);
		UINT e12 = (m_nEdge++);
		UINT v5 = (m_nVertex++);
		UINT f3 = (m_nFace++);
		UINT f4 = (m_nFace++);

		//Changes on v5
		m_pVertex[v5].m_vPosition = (m_pVertex[v2].m_vPosition + m_pVertex[v4].m_vPosition) / 2;
		m_pVertex[v5].m_nValence = 4;
		m_pVertex[v5].m_piEdge = new UINT[4];
		m_pVertex[v5].m_piEdge[0] = e7;
		m_pVertex[v5].m_piEdge[1] = e12;
		m_pVertex[v5].m_piEdge[2] = e8;
		m_pVertex[v5].m_piEdge[3] = e9;
		m_pVertex[v5].m_bIsBoundary = false;

		//No Changes on v2 and v4

		//Changes on v1
		m_pVertex[v1].m_nValence ++;
		UINT* v1piEdge = new UINT[m_pVertex[v1].m_nValence];
		for (short i = 0; i < m_pVertex[v1].m_nValence - 1; ++i) {
			v1piEdge[i] = m_pVertex[v1].m_piEdge[i];
		}
		v1piEdge[m_pVertex[v1].m_nValence - 1] = e10;
		delete[] m_pVertex[v1].m_piEdge;
		m_pVertex[v1].m_piEdge = v1piEdge;

		//Changes on v3
		m_pVertex[v3].m_nValence ++;
		UINT* v3piEdge = new UINT[m_pVertex[v3].m_nValence];
		for (short i = 0; i < m_pVertex[v3].m_nValence - 1; ++i) {
			v3piEdge[i] = m_pVertex[v3].m_piEdge[i];
		}
		v3piEdge[m_pVertex[v3].m_nValence - 1] = e11;
		delete[] m_pVertex[v3].m_piEdge;
		m_pVertex[v3].m_piEdge = v3piEdge;
		
		//No Changes on e1 and e5

		//Changes on e2
		m_pEdge[e2].m_iNextEdge = e9;
		m_pEdge[e2].m_iTwinEdge = e7;
		m_pEdge[e2].m_iVertex[1] = v5;
		m_pEdge[e2].m_length = originEdgeLen / 2;

		//Changes on e3
		m_pEdge[e3].m_iFace = f3;
		m_pEdge[e3].m_iNextEdge = e10;

		//Changes on e4
		m_pEdge[e4].m_iFace = f4;
		m_pEdge[e4].m_iNextEdge = e11;

		//Changes on e6
		m_pEdge[e6].m_iNextEdge = e12;
		m_pEdge[e6].m_iTwinEdge = e8;
		m_pEdge[e6].m_iVertex[1] = v5;
		m_pEdge[e6].m_length = originEdgeLen / 2;
		
		//Changes on e7
		m_pEdge[e7].m_iFace = f4;
		m_pEdge[e7].m_iNextEdge = e4;
		m_pEdge[e7].m_iTwinEdge = e2;
		m_pEdge[e7].m_iVertex[0] = v5;
		m_pEdge[e7].m_iVertex[1] = v2;
		m_pEdge[e7].m_length = originEdgeLen / 2;

		//Changes on e8
		m_pEdge[e8].m_iFace = f3;
		m_pEdge[e8].m_iNextEdge = e3;
		m_pEdge[e8].m_iTwinEdge = e6;
		m_pEdge[e8].m_iVertex[0] = v5;
		m_pEdge[e8].m_iVertex[1] = v4;
		m_pEdge[e8].m_length = originEdgeLen / 2;

		//Changes on e9
		m_pEdge[e9].m_iFace = f1;
		m_pEdge[e9].m_iNextEdge = e1;
		m_pEdge[e9].m_iTwinEdge = e10;
		m_pEdge[e9].m_iVertex[0] = v5;
		m_pEdge[e9].m_iVertex[1] = v1;	
		m_pEdge[e9].m_length = newEdgeLen;

		//Changes on e10
		m_pEdge[e10].m_iFace = f3;
		m_pEdge[e10].m_iNextEdge = e8;
		m_pEdge[e10].m_iTwinEdge = e9;
		m_pEdge[e10].m_iVertex[0] = v1;
		m_pEdge[e10].m_iVertex[1] = v5;
		m_pEdge[e10].m_length = newEdgeLen;

		//Changes on e11
		m_pEdge[e11].m_iFace = f4;
		m_pEdge[e11].m_iNextEdge = e7;
		m_pEdge[e11].m_iTwinEdge = e12;
		m_pEdge[e11].m_iVertex[0] = v3;
		m_pEdge[e11].m_iVertex[1] = v5;
		m_pEdge[e11].m_length = newEdgeLen2;

		//Changes on e12
		m_pEdge[e12].m_iFace = f2;
		m_pEdge[e12].m_iNextEdge = e5;
		m_pEdge[e12].m_iTwinEdge = e11;
		m_pEdge[e12].m_iVertex[0] = v5;
		m_pEdge[e12].m_iVertex[1] = v3;
		m_pEdge[e12].m_length = newEdgeLen2;

		//Changes on f1
		for (int i = 0; i < 3; ++i) {
			if (m_pFace[f1].m_piEdge[i] == e3) {
				m_pFace[f1].m_piEdge[i] = e9;
			}
			if (m_pFace[f1].m_piVertex[i] == v4) {
				m_pFace[f1].m_piVertex[i] = v5;
			}
		}
		calFaceNormal(f1);

		//Changes on f2
		for (int i = 0; i < 3; ++i) {
			if (m_pFace[f2].m_piEdge[i] == e4) {
				m_pFace[f2].m_piEdge[i] = e12;
			}
			if (m_pFace[f2].m_piVertex[i] == v2) {
				m_pFace[f2].m_piVertex[i] = v5;
			}
		}
		calFaceNormal(f2);

		//Changes on f3
		m_pFace[f3].Create(3);
		m_pFace[f3].m_piEdge[0] = e10;
		m_pFace[f3].m_piEdge[1] = e8;
		m_pFace[f3].m_piEdge[2] = e3;
		m_pFace[f3].m_piVertex[0] = v1;
		m_pFace[f3].m_piVertex[1] = v5;
		m_pFace[f3].m_piVertex[2] = v4;
		m_pFace[f3].m_color = m_pFace[f1].m_color;
		calFaceNormal(f3);

		//Changes on f4
		m_pFace[f4].Create(3);
		m_pFace[f4].m_piEdge[0] = e11;
		m_pFace[f4].m_piEdge[1] = e7;
		m_pFace[f4].m_piEdge[2] = e4;
		m_pFace[f4].m_piVertex[0] = v3;
		m_pFace[f4].m_piVertex[1] = v5;
		m_pFace[f4].m_piVertex[2] = v2;
		m_pFace[f4].m_color = m_pFace[f2].m_color;
		calFaceNormal(f4);

		calVertexNormal(v5);
		expandCapacity();
		return v5;
	}
	return -1;
}


void CMesh::calFaceNormal(UINT i)
{
	Vector3D v[2];

	//get the vector
	v[0] = m_pVertex[m_pFace[i].m_piVertex[2]].m_vPosition-m_pVertex[m_pFace[i].m_piVertex[0]].m_vPosition;

	if(m_pFace[i].m_nType==3)
		v[1] = m_pVertex[m_pFace[i].m_piVertex[2]].m_vPosition-m_pVertex[m_pFace[i].m_piVertex[1]].m_vPosition;
	else
		v[1] = m_pVertex[m_pFace[i].m_piVertex[3]].m_vPosition-m_pVertex[m_pFace[i].m_piVertex[1]].m_vPosition;
	m_pFace[i].m_vNormal=v[0]^v[1];

	if(m_pFace[i].m_nType==3)
		m_pFace[i].m_dArea=0.5*m_pFace[i].m_vNormal.normalize();
	else if(m_pFace[i].m_nType==4)
		m_pFace[i].m_dArea=m_pFace[i].m_vNormal.normalize();
	else
		m_pFace[i].m_vNormal.normalize();

	Vector3D vMassPoint;

	for(short j = 0; j < m_pFace[i].m_nType; j++)
		vMassPoint+=m_pVertex[m_pFace[i].m_piVertex[j]].m_vPosition;
	vMassPoint/=(double)(m_pFace[i].m_nType);
	m_pFace[i].m_vMassPoint=vMassPoint;

}

void CMesh::calVertexNormal(UINT i)
{
	Vector3D v;
	UINT iFace;
	short valence=m_pVertex[i].m_nValence;
	if(valence<1)
		return;
	for(short j=0;j<valence;j++)
	{
		iFace=m_pEdge[m_pVertex[i].m_piEdge[j]].m_iFace;
		v+=m_pFace[iFace].m_vNormal;
	}
	v/=(double)valence;
	m_pVertex[i].m_vNormal=v;

}
