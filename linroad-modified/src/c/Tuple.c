
#include<string.h>
#include <stdio.h>
#include <cstdlib>
#include <string.h>
#include "LRDataProvider.h"

Tuple::Tuple()
{
	Initialize();
}
	
Tuple::Tuple(char* str)
{
	Tuple(str, (char*)DEFAULT_DELIMS);
}

Tuple::Tuple(char* str, char* delims)
{
	Initialize(str, delims);
}
	
void Tuple::Initialize()
{
	m_iType		= 0;	
	m_iTime		= 0;   // 0...10799 (second), timestamp position report emitted
	m_iVid		= 0;	   // 0...MAXINT, vehicle identifier
	m_iSpeed	= 0;  // 0...100, speed of the vehicle
	m_iXway		= 0;	// 0...L-1, express way
	m_iLane		= 0;   // 0...4, lane
	m_iDir		= 0;    // 0..1, direction
	m_iSeg		= 0;    // 0...99, segment
	m_iPos		= 0;    // 0...527999, position of the vehicle
	m_iQid		= 0;    // query identifier
	m_iSinit	= 0;  // start segment
	m_iSend		= 0;   // end segment
	m_iDow		= 0;    // 1..7, day of week
	m_iTod		= 0;    // 1...1440, minute number in the day
	m_iDay		= 0;    // 1..69, 1: yesterday, 69: 10 weeks ago
}

void Tuple::Initialize(char* str)
{
	char* delims = new char[256]; 
	strcpy(delims, DEFAULT_DELIMS); 
	Initialize(str, delims);
	delete delims;
}

void Tuple::Initialize(char* str, char* delims)
{
	Initialize();

	char *token = NULL;     

	token = strtok( str, delims);
	if (token != NULL) {m_iType = atoi(token); }

	token = strtok(NULL, delims);
	if (token != NULL) {m_iTime = atoi(token); }

	token = strtok(NULL, delims);
	if (token != NULL) {m_iVid = atoi(token);	}

	token = strtok(NULL, delims);
	if (token != NULL) {m_iSpeed = atoi(token); }

	token = strtok(NULL, delims);
	if (token != NULL) {m_iXway = atoi(token); }

	token = strtok(NULL, delims);
	if (token != NULL) {m_iLane = atoi(token); }

	token = strtok(NULL, delims);
	if (token != NULL) {m_iDir = atoi(token); }

	token = strtok(NULL, delims);
	if (token != NULL) {m_iSeg = atoi(token); }

	token = strtok(NULL, delims);
	if (token != NULL) {m_iPos = atoi(token); }

	token = strtok(NULL, delims);
	if (token != NULL) {m_iQid = atoi(token); }

	token = strtok(NULL, delims);
	if (token != NULL) {m_iSinit = atoi(token); }

	token = strtok(NULL, delims);
	if (token != NULL) {m_iSend = atoi(token); }

	token = strtok(NULL, delims);
	if (token != NULL) {m_iDow = atoi(token); }

	token = strtok(NULL, delims);
	if (token != NULL) {m_iTod = atoi(token); }

	token = strtok(NULL, delims);
	if (token != NULL) {m_iDay = atoi(token); }

}
	
char* Tuple::ToString()
{
	char* str = new char[1024];
	sprintf(str, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",
		m_iType, m_iTime, m_iVid, 
		m_iSpeed, m_iXway, m_iLane,
		m_iDir, m_iSeg, m_iPos,
		m_iQid, m_iSinit, m_iSend,
		m_iDow, m_iTod, m_iDay);

	return str;
}

