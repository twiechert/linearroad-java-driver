
#include <stdio.h>
#include <pthread.h>
#include "LRDataProvider.h"

using namespace std;

CMemTuples::CMemTuples(int nBufferSize, int nMaxLineSize)
{
	m_nBufferSize	= nBufferSize;
	m_nMaxLineSize	= nMaxLineSize;

	m_lpzBuf		= new char[DEFAULT_MAX_LINE_SIZE];
	m_lpTuples		= new Tuple[m_nBufferSize];
	//m_lpTuples	= malloc((size_t)(m_nBufferSize*sizeof(Tuple));

	m_nFirstTuple		= -1;
	m_nCurrentTuple		= -1;
	m_nLastTuple		= -1; 
	m_nTuplesInBuffer	= 0; 
	m_nAvailableTuples	= 0;
	m_bIsEof			= false;

}

CMemTuples::~CMemTuples(void)
{
	if ( m_lpzBuf != NULL )
	{
		delete m_lpzBuf;
	}
	if ( m_lpTuples != NULL )
	{
		delete[] m_lpTuples;
	}
}

// Convert the buffer content to string
char* CMemTuples::ToString(void)
{
	char* str = new char[m_nBufferSize*m_nMaxLineSize + 1];
	memset(str, '\0', m_nBufferSize*m_nMaxLineSize + 1);

	if ( (m_nFirstTuple == -1) || (m_nLastTuple == -1) )
	{
		return str;		
	}
	
	for(int i = m_nFirstTuple; i <= m_nLastTuple; i++ )
	{
		char* temp = m_lpTuples[i].ToString();
		str = strcat(str, temp);
	}

	return str;
}

// Fill tuples until the buffer is full or eof is reached.
int CMemTuples::FillTuples(ifstream* file)
{
	pthread_mutex_lock(m_lock);

	//Check if buffer is full
	if ( m_nTuplesInBuffer == m_nBufferSize )
	{
		pthread_mutex_unlock(m_lock);
		return SUCCESS;
	}

	//Check for valid file hanler
	if ( file == NULL )
	{
		pthread_mutex_unlock(m_lock);
		return ERROR_INVALID_FILE;
	}
	
	while ( !(m_bIsEof = file->eof()) )
	{
		//Advance one slot
		if ( ++m_nLastTuple == m_nBufferSize )
		{
			m_nLastTuple = 0; 
		}
				
		//Gets data into the buffer
		file->getline(m_lpzBuf, sizeof(char[DEFAULT_MAX_LINE_SIZE]) );
		//Initialize tuple with string
		m_lpTuples[m_nLastTuple].Initialize(m_lpzBuf);

		if ( m_nFirstTuple == -1 )
		{
			m_nFirstTuple = 0;
		}
		
		//Check for buffer is full
		++m_nTuplesInBuffer;

		if (m_nTuplesInBuffer  == m_nBufferSize )
		{
			break;
		}
	}

	pthread_mutex_unlock(m_lock);

	return SUCCESS;
}

int CMemTuples::GetTuples(LPTuple tuples, int nMaxTuples, int& nTuplesRet)
{
	pthread_mutex_lock(m_lock);
	
	//Number of return tuples
	nTuplesRet = 0;

	//Check for available of tuples 
	//Check for number of tuples in buffer 
	//Check for available tuples in buffer 
	if ( (m_nAvailableTuples == 0) || (m_nTuplesInBuffer == 0) || (m_nCurrentTuple == -1) )
	{
		if ( (m_nTuplesInBuffer == 0) && m_bIsEof )
		{
			pthread_mutex_unlock(m_lock);
			return END_OF_FILE;
		}
		pthread_mutex_unlock(m_lock);
		return nTuplesRet;
	}

	
	//Calculate number of return tuples 
	nTuplesRet = m_nAvailableTuples;

	//Number of tuples return should not bigger than nMaxTuple
	nTuplesRet = (nTuplesRet > nMaxTuples) ? nMaxTuples : nTuplesRet;
		
	if ( nTuplesRet == 0 )
	{
		pthread_mutex_unlock(m_lock);
		return (nTuplesRet);
	}

    if ( (m_nCurrentTuple >= m_nFirstTuple) )
	{
		memcpy(&tuples[0], &m_lpTuples[m_nFirstTuple], (size_t)(sizeof(Tuple)*nTuplesRet));
        m_nFirstTuple += nTuplesRet;
	}
	else
	{
		if ( nTuplesRet < (m_nBufferSize - m_nFirstTuple) )
		{
			 memcpy(&tuples[0], &m_lpTuples[m_nFirstTuple], (size_t)(sizeof(Tuple)*(nTuplesRet)));
			 m_nFirstTuple += nTuplesRet;
		}
		else
		{
			 int nFirstRead = (m_nBufferSize - m_nFirstTuple);
			 memcpy(&tuples[0], &m_lpTuples[m_nFirstTuple], (size_t)(sizeof(Tuple)*(nFirstRead)));
			 m_nFirstTuple = 0;

			 if ( (nTuplesRet - nFirstRead) > 0 )
			 {
				memcpy(&tuples[nFirstRead], &m_lpTuples[m_nFirstTuple], (size_t)(sizeof(Tuple)*(nTuplesRet - nFirstRead)));
			 }
		}
	}
	m_nTuplesInBuffer -= nTuplesRet;
	m_nAvailableTuples -= nTuplesRet;

	pthread_mutex_unlock(m_lock);

	return (nTuplesRet); 
}

//Make buffer availbe to ts seconds
void CMemTuples::AdvanceTo(int ts)
{
	pthread_mutex_lock(m_lock);
	
	if ( m_nTuplesInBuffer == 0 )
	{
		pthread_mutex_unlock(m_lock);
		return;
	}

	for( ; m_nAvailableTuples < m_nTuplesInBuffer  ; )
	{
		if (m_lpTuples[m_nCurrentTuple+1].m_iTime > ts + 1 )
		{
			break;
		}
		++m_nCurrentTuple;
		++m_nAvailableTuples;

		if ( m_nCurrentTuple == m_nBufferSize )
		{
			m_nCurrentTuple = 0;
		}
	}

	pthread_mutex_unlock(m_lock);
}

