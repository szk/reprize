#include "Common.hpp"

#include "resource/Mesh.hpp"
#include "MakeStrip.hpp"
#include "util/nvtristrip/NvTriStrip.h"

using namespace reprize;
using namespace vid;
using namespace res;
using namespace std;

MakeStrip::MakeStrip(void)
{
    SetCacheSize(CACHESIZE_GEFORCE3);
    SetStitchStrips(true);
    SetMinStripSize(0);
    SetListsOnly(false);
}

MakeStrip::~MakeStrip(void)
{
}

// void MakeStrip::gen_strips(vector<Vertex*>* vertex, Mesh* mesh)
// {
//     uInt32 mesh_size = mesh->vertex_idx_size();
//     // XXX
//     uInt16 indexPtr[65536];

//     for (Size32 i = 0; mesh_size > i; ++i)
//     {
// 	indexPtr[i] = (uInt16) mesh->CurrentVertex();
//     }

//     PrimitiveGroup* primGroups;
//     uInt16 numGroups;
//     GenerateStrips(indexPtr, mesh_size, &primGroups, (unsigned short*)&numGroups);
////	GenerateStrips(indexPtr, tempMesh->GetNumFaces() * 3, &pOldPG, (unsigned short*)&m_dwNumSections);
// }

// void MakeStrip::remap_indices(vector<Vertex*>* vertex_v, vector<Mesh*>* mesh_v)
// {
////	RemapIndices(pOldPG, m_dwNumSections, m_NumVerts, &m_pPrimitiveGroups);
// }

/*
  void GenerateStrips(const unsigned short* in_indices, const unsigned int in_numIndices,
  PrimitiveGroup** primGroups, unsigned short* numGroups)
  {
  //put data in format that the stripifier likes
  WordVec tempIndices;
  tempIndices.resize(in_numIndices);
  unsigned short maxIndex = 0;
  for(int i = 0; i < in_numIndices; i++)
  {
  tempIndices[i] = in_indices[i];
  if(in_indices[i] > maxIndex)
  maxIndex = in_indices[i];
  }
  NvStripInfoVec tempStrips;
  NvFaceInfoVec tempFaces;

  NvStripifier stripifier;
	
  //do actual stripification
  stripifier.Stripify(tempIndices, cacheSize, minStripSize, maxIndex, tempStrips, tempFaces);

  //stitch strips together
  IntVec stripIndices;
  unsigned int numSeparateStrips = 0;

  if(bListsOnly)
  {
  //if we're outputting only lists, we're done
  *numGroups = 1;
  (*primGroups) = new PrimitiveGroup[*numGroups];
  PrimitiveGroup* primGroupArray = *primGroups;

  //count the total number of indices
  unsigned int numIndices = 0;
  for(int i = 0; i < tempStrips.size(); i++)
  {
  numIndices += tempStrips[i]->m_faces.size() * 3;
  }

  //add in the list
  numIndices += tempFaces.size() * 3;

  primGroupArray[0].type       = PT_LIST;
  primGroupArray[0].numIndices = numIndices;
  primGroupArray[0].indices    = new unsigned short[numIndices];

  //do strips
  unsigned int indexCtr = 0;
  for(i = 0; i < tempStrips.size(); i++)
  {
  for(int j = 0; j < tempStrips[i]->m_faces.size(); j++)
  {
  //degenerates are of no use with lists
  if(!NvStripifier::IsDegenerate(tempStrips[i]->m_faces[j]))
  {
  primGroupArray[0].indices[indexCtr++] = tempStrips[i]->m_faces[j]->m_v0;
  primGroupArray[0].indices[indexCtr++] = tempStrips[i]->m_faces[j]->m_v1;
  primGroupArray[0].indices[indexCtr++] = tempStrips[i]->m_faces[j]->m_v2;
  }
  else
  {
  //we've removed a tri, reduce the number of indices
  primGroupArray[0].numIndices -= 3;
  }
  }
  }

  //do lists
  for(i = 0; i < tempFaces.size(); i++)
  {
  primGroupArray[0].indices[indexCtr++] = tempFaces[i]->m_v0;
  primGroupArray[0].indices[indexCtr++] = tempFaces[i]->m_v1;
  primGroupArray[0].indices[indexCtr++] = tempFaces[i]->m_v2;
  }
  }
  else
  {
  stripifier.CreateStrips(tempStrips, stripIndices, bStitchStrips, numSeparateStrips);

  //if we're stitching strips together, we better get back only one strip from CreateStrips()
  assert( (bStitchStrips && (numSeparateStrips == 1)) || !bStitchStrips);
		
  //convert to output format
  *numGroups = numSeparateStrips; //for the strips
  if(tempFaces.size() != 0)
  (*numGroups)++;  //we've got a list as well, increment
  (*primGroups) = new PrimitiveGroup[*numGroups];
		
  PrimitiveGroup* primGroupArray = *primGroups;
		
  //first, the strips
  int startingLoc = 0;
  for(int stripCtr = 0; stripCtr < numSeparateStrips; stripCtr++)
  {
  int stripLength = 0;

  if(!bStitchStrips)
  {
  //if we've got multiple strips, we need to figure out the correct length
  for(int i = startingLoc; i < stripIndices.size(); i++)
  {
  if(stripIndices[i] == -1)
  break;
  }
				
  stripLength = i - startingLoc;
  }
  else
  stripLength = stripIndices.size();
			
  primGroupArray[stripCtr].type       = PT_STRIP;
  primGroupArray[stripCtr].indices    = new unsigned short[stripLength];
  primGroupArray[stripCtr].numIndices = stripLength;
			
  int indexCtr = 0;
  for(int i = startingLoc; i < stripLength + startingLoc; i++)
  primGroupArray[stripCtr].indices[indexCtr++] = stripIndices[i];

  //we add 1 to account for the -1 separating strips
  //this doesn't break the stitched case since we'll exit the loop
  startingLoc += stripLength + 1; 
  }
		
  //next, the list
  if(tempFaces.size() != 0)
  {
  int faceGroupLoc = (*numGroups) - 1;    //the face group is the last one
  primGroupArray[faceGroupLoc].type       = PT_LIST;
  primGroupArray[faceGroupLoc].indices    = new unsigned short[tempFaces.size() * 3];
  primGroupArray[faceGroupLoc].numIndices = tempFaces.size() * 3;
  int indexCtr = 0;
  for(int i = 0; i < tempFaces.size(); i++)
  {
  primGroupArray[faceGroupLoc].indices[indexCtr++] = tempFaces[i]->m_v0;
  primGroupArray[faceGroupLoc].indices[indexCtr++] = tempFaces[i]->m_v1;
  primGroupArray[faceGroupLoc].indices[indexCtr++] = tempFaces[i]->m_v2;
  }
  }
  }

  //clean up everything

  //delete strips
  for(i = 0; i < tempStrips.size(); i++)
  {
  for(int j = 0; j < tempStrips[i]->m_faces.size(); j++)
  {
  delete tempStrips[i]->m_faces[j];
  tempStrips[i]->m_faces[j] = NULL;
  }
  delete tempStrips[i];
  tempStrips[i] = NULL;
  }

  //delete faces
  for(i = 0; i < tempFaces.size(); i++)
  {
  delete tempFaces[i];
  tempFaces[i] = NULL;
  }
  }
*/
