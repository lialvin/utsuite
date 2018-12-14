// LoopBuf.h: interface for the CLoopBuf class.
#if !defined(_LOOPBUF_H__INCLUDED_)
#define _LOOPBUF_H__INCLUDED_

#ifndef  BYTE
#define    BYTE   unsigned char
#define  TRUE 1
#define  FALSE 0

#endif

#ifndef  DWORD
#define    DWORD   unsigned int
#endif

#ifndef  MAX_PATH
#define    MAX_PATH   128
#endif

#ifndef  SLEEP_MS
#ifdef WIN32
#define    SLEEP_MS   ::Sleep
#else
#define    SLEEP_MS   usleep
#endif
#endif

enum {
    LOOP_SHMMEM_MODE = 1,
    LOOP_MEM_MODE = 0,
	NO_MEM_MODE = -1,
};

class  CLoopBuf
{
public:

typedef	struct{
		int  nHeadPos;
		int  nTailPos;
		int  nSize;
		int  nQueueNo;//2M ������  /*512*4096-20*/ �����ڴ�ӳ��ָ��
		short  wReadUseFlag; //��ʼ������1 ����0 
		short  wWriteUseFlag;//��ʼ������1 
	}MemQueueHeader; // �����ڴ�

	BYTE * m_pShmBuf;
	CLoopBuf();
	virtual ~CLoopBuf();

	void  WritePushLog();
// ��ӡ��־
	void  WritePopLog();

//���������ڴ�
//���ٹ����ڴ�
	int	  PushIn(BYTE * lpData,  int nDataSize, BYTE * packet, int nPkgLen);
	int   PushIn(BYTE* lpData,  int nDataSize);

	int		PopOut(BYTE*  lpData, int nDataBufSize);
/**************************************************************************************************
	�������� : GetSize
	�������� : �õ����е�Ԫ�ĸ���
	�� �� ֵ : ���е�Ԫ�ĸ���
**************************************************************************************************/
	int		GetSize();
	/*bSetZero  == TRUE Ϊ���� �����ڴ�   ==FALSE Ϊʹ�ù����ڴ�*/
	// nSize �����ڴ��С Ϊ4096�������� ��shmname Ϊ������
	bool    Init(int nSize, char * shmname,int nMemMapMode,bool bSetZero );
	void    SetZero();
	// �Ƿ�ռ��
	static bool   IsUsed(char * shmname );
	bool          IsUsed();
	// ���ռ��
	void      SetUnused();
	void      SetUsed();
	// ��־��¼����¼���ͳɹ������߷���ʧ��
	DWORD     m_dwLogPushSucc;
    DWORD     m_dwLogPushZero;

	DWORD     m_dwLogPopSucc;
    DWORD     m_dwLogPopZero;
	char      m_szShaName[MAX_PATH];
	char      m_szLogstr[256];
	int       m_nQueNo;// ���б��
private:
	bool    OpenShmMap(int memsize,char * shmname, bool bSetZero);
    int     m_nMemMapMode;
    void    CloseShmMap(int memsize);
	int     OpenShm(int memsize,char * shmname, bool bSetZero);
    bool    OpenMem(int memsize, bool bSetZero);
    void    CloseMem();
	// �رչ����ڴ�
	void    CloseShm();
	int     PushInNoblock(BYTE* lpData,  int nDataSize,BYTE * packet , int nPkgLen);
	int     PushInNoblock(BYTE* lpData,  int nDataSize);
	int		m_nLeftSize;// ��ʱ����
	int		m_nBufSize; // m_pMem->pBuf ���л������ߴ�
	int		m_nDataSize;// ���ݿ�ĳߴ�

    BYTE    m_PktHead[8] ;    // push
    BYTE    m_PopPktHead[8] ; // pop
    int     LOOP_PKT_HEAD_LEN  ; // total len 4 byte + 4 sync byte
	int       m_shm_id;
	int 	m_hMapFile;
	MemQueueHeader *    m_pmap; // ����ָ��
};

#endif // !defined(_LOOPBUF_H__INCLUDED_)

