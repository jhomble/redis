#pragma once
#include "acl_cpp/acl_cpp_define.hpp"
#include <stdlib.h>
#ifndef WIN32
#include <pthread.h>
#endif

#ifdef WIN32
struct acl_pthread_mutex_t;
#else
# ifndef	acl_pthread_mutex_t
#  define	acl_pthread_mutex_t	pthread_mutex_t
# endif
#endif

namespace acl {

/**
 * ������������ͬʱ�����ļ������߳�����Ҳ����ֻ����һ����
 */
class ACL_CPP_API locker
{
public:
	/**
	 * ���캯��
	 * @param use_mutex {bool} �Ƿ񴴽��߳���
	 * @param use_spinlock {bool} �ڲ���ʹ���߳���ʱ�Ƿ���Ҫ������
	 */
	locker(bool use_mutex = true, bool use_spinlock = false);
	virtual ~locker();

	/**
	 * �����ļ�·�������ļ���
	 * @param file_path {const char*} �ļ�·�����ǿ�
	 * @return {bool} �Ƿ�ɹ�
	 * ע���˺���������� open ��������ͬʱ����һ��
	 */
	bool open(const char* file_path);

	/**
	 * �����ļ���������ļ���
	 * @param fh {int} �ļ����
	 * @return {bool} �Ƿ�ɹ�
	 */
#ifdef WIN32
	bool open(void* fh);
#else
	bool open(int fh);
#endif

	/**
	 * ����Ѿ��򿪵���(�����߳������ļ���)���м���
	 * @return {bool} �����Ƿ�ɹ�
	 */
	bool lock();

	/**
	 * ���Զ��Ѿ��򿪵���(�����߳������ļ���)���м���
	 * @return {bool} �����Ƿ�ɹ�
	 */
	bool try_lock();

	/**
	 * ����Ѿ��򿪵���(�����߳������ļ���)���н���
	 * @return {bool} �����Ƿ�ɹ�
	 */
	bool unlock();

private:
	acl_pthread_mutex_t* mutex_;
	char* pFile_;
#ifdef WIN32
	void* fHandle_;
#else
	int   fHandle_;
	pthread_mutexattr_t  mutex_attr_;
# if !defined(MINGW) && !defined(MACOSX)
	pthread_spinlock_t*  spinlock_;
# endif
#endif
	bool  myFHandle_;

	void init_mutex(bool use_spinlock);
};

}  // namespace acl
