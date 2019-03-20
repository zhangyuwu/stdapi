
/*-----------------------------------------------------------------------------
 *                          Embedded System SDK 2.00                          *
 *                               Copyright (c)                                *
 *                             2005-12-15 by Ken                              *
 *                               Beijing.China                                *
 *                            cpp.china@gmail.com                             *
 -----------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <system.h>
#include <ucos_ii.h>
#include <errno.h>
#include <memory.h>
#include <magic.h>
#include <serial.h>
#include <ucos_ii.h>

/*
 * --- Update history ---
 *
 * MOD: 改信号量的创建与释放为静态内存，以前需要动态的支持，如果频繁的创建释放信号量会有损效率。
 *      Modified by Ken, 2006-02-23 15:14:07
 *
 */
 
/* ------------------------------------------------------------------------
 *  函数名：
 *      int GetLastError()
 *  功能：
 *      返回最后一次的错误码
 *  参数：
 *      无
 *  返回值：
 *      错误码
 *  说明：
 *      无
 * -----------------------------------------------------------------------*/

int GetLastError()
{
    return errno;
}

/* ------------------------------------------------------------------------
 *  函数名：
 *      void SetLastError(int code)
 *  功能：
 *      设置最后一次的错误码
 *  参数：
 *      code            ----    错误码
 *  返回值：
 *      无
 *  说明：
 *      无
 * -----------------------------------------------------------------------*/

void SetLastError(int code)
{
    errno = code;
}

/* ------------------------------------------------------------------------
 *  函数名：
 *      const char *GetErrorMessage(int ErrorCode)
 *  功能：
 *      获取错误信息
 *  参数：
 *      level           ----    错误级别
 *      ErrorCode       ----    错误码
 *  返回值：
 *      错误信息字符串
 *  说明：
 *      无
 * -----------------------------------------------------------------------*/

const char *GetErrorMessage(int level, int ErrorCode)
{
    typedef struct {
        int         ErrorCode;
        const char  *Message;
    } TErrorInfo;
    
    const TErrorInfo c_errors[] = {
		// ----------------------------------------------------------------------------
		// C语言错误
        { EPERM                         , "Not super-user"                      },
        { ENOENT                        , "No such file or directory"           },
        { ESRCH                         , "No such process"                     },
        { EINTR                         , "Interrupted system call"             },
        { EIO                           , "I/O error"                           },
        { ENXIO                         , "No such device or address"           },
        { E2BIG                         , "Arg list too long"                   },
        { ENOEXEC                       , "Exec format error"                   },
        { EBADF                         , "Bad file number"                     },
        { ECHILD                        , "No children"                         },
        { EAGAIN                        , "No more processes"                   },
        { ENOMEM                        , "Not enough core"                     },
        { EACCES                        , "Permission denied"                   },
        { EFAULT                        , "Bad address"                         },
        { ENOTBLK                       , "Block device required"               },
        { EBUSY                         , "Mount device busy"                   },
        { EEXIST                        , "File exists"                         },
        { EXDEV                         , "Cross-device link"                   },
        { ENODEV                        , "No such device"                      },
        { ENOTDIR                       , "Not a directory"                     },
        { EISDIR                        , "Is a directory"                      },
        { EINVAL                        , "Invalid argument"                    },
        { ENFILE                        , "Too many open files in system"       },
        { EMFILE                        , "Too many open files"                 },
        { ENOTTY                        , "Not a typewriter"                    },
        { ETXTBSY                       , "Text file busy"                      },
        { EFBIG                         , "File too large"                      },
        { ENOSPC                        , "No space left on device"             },
        { ESPIPE                        , "Illegal seek"                        },
        { EROFS                         , "Read only file system"               },
        { EMLINK                        , "Too many links"                      },
        { EPIPE                         , "Broken pipe"                         },
        { EDOM                          , "Math arg out of domain of func"      },
        { ERANGE                        , "Math result not representable"       },
        { ENOMSG                        , "No message of desired type"          },
        { EIDRM                         , "Identifier removed"                  },
        { ECHRNG                        , "Channel number out of range"         },
        { EL2NSYNC                      , "Level 2 not synchronized"            },
        { EL3HLT                        , "Level 3 halted"                      },
        { EL3RST                        , "Level 3 reset"                       },
        { ELNRNG                        , "Link number out of range"            },
        { EUNATCH                       , "Protocol driver not attached"        },
        { ENOCSI                        , "No CSI structure available"          },
        { EL2HLT                        , "Level 2 halted"                      },
        { EDEADLK                       , "Deadlock condition"                  },
        { ENOLCK                        , "No record locks available"           },
        { EBADE                         , "Invalid exchange"                    },
        { EBADR                         , "Invalid request descriptor"          },
        { EXFULL                        , "Exchange full"                       },
        { ENOANO                        , "No anode"                            },
        { EBADRQC                       , "Invalid request code"                },
        { EBADSLT                       , "Invalid slot"                        },
        { EDEADLOCK                     , "File locking deadlock error"         },
        { EBFONT                        , "Bad font file fmt"                   },
        { ENOSTR                        , "Device not a stream"                 },
        { ENODATA                       , "No data (for no delay io)"           },
        { ETIME                         , "Timer expired"                       },
        { ENOSR                         , "Out of streams resources"            },
        { ENONET                        , "Machine is not on the network"       },
        { ENOPKG                        , "Package not installed"               },
        { EREMOTE                       , "The object is remote"                },
        { ENOLINK                       , "The link has been severed"           },
        { EADV                          , "Advertise error"                     },
        { ESRMNT                        , "Srmount error"                       },
        { ECOMM                         , "Communication error on send"         },
        { EPROTO                        , "Protocol error"                      },
        { EMULTIHOP                     , "Multihop attempted"                  },
        { ELBIN                         , "Inode is remote (not really)"        },
        { EDOTDOT                       , "Cross mount point (not really)"      },
        { EBADMSG                       , "Trying to read unreadable message"   },
        { ENOTUNIQ                      , "Given log. name not unique"          },
        { EBADFD                        , "f.d. invalid for this operation"     },
        { EREMCHG                       , "Remote address changed"              },
        { ELIBACC                       , "Can't access a needed shared lib"    },
        { ELIBBAD                       , "Access a corrupted shared lib"       },
        { ELIBSCN                       , ".lib section in a.out corrupted"     },
        { ELIBMAX                       , "Attempt to link in too many libs"    },
        { ELIBEXEC                      , "Attempt to exec a shared library"    },
        { ENOSYS                        , "Function not implemented"            },
        { ENMFILE                       , "No more files"                       },
        { ENOTEMPTY                     , "Directory not empty"                 },
        { ENAMETOOLONG                  , "File or path name too long"          },
        { ELOOP                         , "Too many symbolic links"             },
        { EOPNOTSUPP                    , "Operation not supported on transport endpoint" },
        { EPFNOSUPPORT                  , "Protocol family not supported"       },
        { ECONNRESET                    , "Connection reset by peer"            },
        { ENOBUFS                       , "No buffer space available"           },
        { EAFNOSUPPORT                  , "Address family not supported by protocol family" },
        { EPROTOTYPE                    , "Protocol wrong type for socket"      },
        { ENOTSOCK                      , "Socket operation on non-socket"      },
        { ENOPROTOOPT                   , "Protocol not available"              },
        { ESHUTDOWN                     , "Can't send after socket shutdown"    },
        { ECONNREFUSED                  , "Connection refused"                  },
        { EADDRINUSE                    , "Address already in use"              },
        { ECONNABORTED                  , "Connection aborted"                  },
        { ENETUNREACH                   , "Network is unreachable"              },
        { ENETDOWN                      , "Network interface is not configured" },
        { ETIMEDOUT                     , "Connection timed out"                },
        { EHOSTDOWN                     , "Host is down"                        },
        { EHOSTUNREACH                  , "Host is unreachable"                 },
        { EINPROGRESS                   , "Connection already in progress"      },
        { EALREADY                      , "Socket already connected"            },
        { EDESTADDRREQ                  , "Destination address required"        },
        { EMSGSIZE                      , "Message too long"                    },
        { EPROTONOSUPPORT               , "Unknown protocol"                    },
        { ESOCKTNOSUPPORT               , "Socket type not supported"           },
        { EADDRNOTAVAIL                 , "Address not available"               },
        { ENETRESET                     , "Error network reset"                 },
        { EISCONN                       , "Socket is already connected"         },
        { ENOTCONN                      , "Socket is not connected"             },
        { ETOOMANYREFS                  , "ETOOMANYREFS"                        },
        { EPROCLIM                      , "EPROCLIM"                            },
        { EUSERS                        , "EUSERS"                              },
        { EDQUOT                        , "EDQUOT"                              },
        { ESTALE                        , "ESTALE"                              },
        { ENOTSUP                       , "Not supported"                       },
        { ENOMEDIUM                     , "No medium (in tape drive)"           },
        { ENOSHARE                      , "No such host or network path"        },
	};
	
    const TErrorInfo sys_errors[] = {
		// ----------------------------------------------------------------------------
		// 系统错误
        { NO_ERROR                      , "Successful"                          },
        { ERROR_INVALID_FUNCTION        , "Invalid Function"                    },
        { ERROR_FILE_NOT_FOUND          , "File not found"                      },
        { ERROR_ACCESS_DENIED           , "Access denied"                       },
        { ERROR_INVALID_HANDLE          , "Invalid handle"                      },
        { ERROR_NO_ENOUGH_MEMORY        , "No enough memory"                    },
        { ERROR_INVALID_BLOCK           , "Invalid block"                       },
        { ERROR_BAD_ENVIRONMENT         , "Bad environment"                     },
        { ERROR_BAD_FORMAT              , "Bad format"                          },
        { ERROR_INVALID_ACCESS          , "Invalid access"                      },
        { ERROR_INVALID_DATA            , "Invalid data"                        },
        { ERROR_WRITE_PROTECT           , "Write protect"                       },
        { ERROR_NOT_READY               , "Not ready"                           },
        { ERROR_BAD_COMMAND             , "Bad command"                         },
        { ERROR_CRC                     , "Wrong CRC"                           },
        { ERROR_BAD_LENGTH              , "Bad length"                          },
        { ERROR_WRITE_FAULT             , "Write fault"                         },
        { ERROR_READ_FAULT              , "Read fault"                          },
        { ERROR_FILE_EXISTS             , "File exists already"                 },
        { ERROR_INVALID_PASSWORD        , "Invalid password"                    },
        { ERROR_OPEN_FAILED             , "Open failed"                         },
        { ERROR_FLASH_FULL              , "Flash memory full"                   },
        { ERROR_INVALID_NAME            , "Invalid name"                        }, 
        { ERROR_LOCKED                  , "Resource locked"                     },
        { ERROR_DEVICE_BUSY             , "Device is busy"                      },
        { ERROR_DEVICE_NOT_OPEN         , "Device not open"                     },
        { ERROR_NO_FREE_SPACE           , "No more free space"                  },
        { ERROR_NOT_REGISTERED          , "Not registered"                      },
        { ERROR_OUT_OF_RANGE            , "Out of range"                        },
        { ERROR_BAD_PARAMETER           , "Bad parameter"                       },
        { ERROR_DEVICE_SEM              , "Sem error while open device"         }, 
        { ERROR_NO_FONT_SELECTED        , "No font selected"                    },
        { ERROR_THREAD_PRIORITY         , "Thread priority not valid"           },
        { ERROR_THREAD_STACK            , "Thread stack size not valid"         },
        { ERROR_THREAD_OS_CREATE        , "OS create thread failed"             },
        { ERROR_HOTKEY_LIST_FULL        , "Hotkey list full"                    },
        { ERROR_HOTKEY_NOT_REGISTERED   , "Hotkey not registered"               },
        { ERROR_TIMER_CREATE_PARAM      , "Timer create with invalid param"     },
        { ERROR_TIMER_LIST_FULL         , "No more timer can create: Full"      },
        { ERROR_MEM_POINTER             , "Invalid memory pointer"              },
        { ERROR_MEM_NO_PARTITION        , "No more memory partition"            },
        { ERROR_NULL_POINTER            , "Null pointer"                        },
        { ERROR_NODE_NOT_FOUND          , "Node not found"                      },
	};

    const TErrorInfo os_errors[] = {
		// ----------------------------------------------------------------------------
		// OS错误
        { OS_NO_ERR                     , "OS no err"                           },
        { OS_ERR_EVENT_TYPE             , "OS err event type"                   },
        { OS_ERR_PEND_ISR               , "OS err pend isr"                     },
        { OS_ERR_POST_NULL_PTR          , "OS err post null ptr"                },
        { OS_ERR_PEVENT_NULL            , "OS err pevent null"                  },
        { OS_ERR_POST_ISR               , "OS err post isr"                     },
        { OS_ERR_QUERY_ISR              , "OS err query isr"                    },
        { OS_ERR_INVALID_OPT            , "OS err invalid opt"                  },
        { OS_ERR_TASK_WAITING           , "OS err task waiting"                 },
        { OS_TIMEOUT                    , "OS timeout"                          },
        { OS_TASK_NOT_EXIST             , "OS task not exist"                   },
        { OS_ERR_EVENT_NAME_TOO_LONG    , "OS err event name too long"          },
        { OS_ERR_FLAG_NAME_TOO_LONG     , "OS err flag name too long"           },
        { OS_ERR_TASK_NAME_TOO_LONG     , "OS err task name too long"           },
        { OS_ERR_PNAME_NULL             , "OS err pname null"                   },
        { OS_ERR_TASK_CREATE_ISR        , "OS err task create isr"              },
        { OS_MBOX_FULL                  , "OS mbox full"                        },
        { OS_Q_FULL                     , "OS queue full"                       },
        { OS_Q_EMPTY                    , "OS queue empty"                      },
        { OS_PRIO_EXIST                 , "OS prio exist"                       },
        { OS_PRIO_ERR                   , "OS prio err"                         },
        { OS_PRIO_INVALID               , "OS prio invalid"                     },
        { OS_SEM_OVF                    , "OS sem ovf"                          },
        { OS_TASK_DEL_ERR               , "OS task del err"                     },
        { OS_TASK_DEL_IDLE              , "OS task del idle"                    },
        { OS_TASK_DEL_REQ               , "OS task del req"                     },
        { OS_TASK_DEL_ISR               , "OS task del isr"                     },
        { OS_NO_MORE_TCB                , "OS no more tcb"                      },
        { OS_TIME_NOT_DLY               , "OS time not dly"                     },
        { OS_TIME_INVALID_MINUTES       , "OS time invalid minutes"             },
        { OS_TIME_INVALID_SECONDS       , "OS time invalid seconds"             },
        { OS_TIME_INVALID_MILLI         , "OS time invalid milli"               },
        { OS_TIME_ZERO_DLY              , "OS time zero dly"                    },
        { OS_TASK_SUSPEND_PRIO          , "OS task suspend prio"                },
        { OS_TASK_SUSPEND_PRIO          , "OS task suspend prio"                },
        { OS_TASK_RESUME_PRIO           , "OS task resume prio"                 },
        { OS_TASK_NOT_SUSPENDED         , "OS task not suspended"               },
        { OS_MEM_INVALID_PART           , "OS mem invalid part"                 },
        { OS_MEM_INVALID_BLKS           , "OS mem invalid blks"                 },
        { OS_MEM_INVALID_SIZE           , "OS mem invalid size"                 },
        { OS_MEM_NO_FREE_BLKS           , "OS mem no free blks"                 },
        { OS_MEM_FULL                   , "OS mem full"                         },
        { OS_MEM_INVALID_PBLK           , "OS mem invalid pblk"                 },
        { OS_MEM_INVALID_PMEM           , "OS mem invalid pmem"                 },
        { OS_MEM_INVALID_PDATA          , "OS mem invalid pdata"                },
        { OS_MEM_INVALID_ADDR           , "OS mem invalid addr"                 },
        { OS_MEM_NAME_TOO_LONG          , "OS mem name too long"                },
        { OS_ERR_NOT_MUTEX_OWNER        , "OS err not mutex owner"              },
        { OS_TASK_OPT_ERR               , "OS task opt err"                     },
        { OS_ERR_DEL_ISR                , "OS err del isr"                      },
        { OS_ERR_CREATE_ISR             , "OS err create isr"                   },
        { OS_FLAG_INVALID_PGRP          , "OS flag invalid pgrp"                },
        { OS_FLAG_ERR_WAIT_TYPE         , "OS flag err wait type"               },
        { OS_FLAG_ERR_NOT_RDY           , "OS flag err not rdy"                 },
        { OS_FLAG_INVALID_OPT           , "OS flag invalid opt"                 },
        { OS_FLAG_GRP_DEPLETED          , "OS flag grp depleted"                },
    };
    const char * UnknowError = "Unknow Error Code";
    
    int i, size;
    TErrorInfo *errors;
	
	switch (level) {
	case ERROR_LEVEL_C:
		errors = (TErrorInfo *)c_errors;
		size = sizeof(c_errors) / sizeof(TErrorInfo);
		break;
	case ERROR_LEVEL_SYS:
		errors = (TErrorInfo *)sys_errors;
		size = sizeof(sys_errors) / sizeof(TErrorInfo);
		break;
	case ERROR_LEVEL_OS:
		errors = (TErrorInfo *)os_errors;
		size = sizeof(os_errors) / sizeof(TErrorInfo);
		break;
	default:
		return UnknowError; 
	}
	
    for (i = 0; i < size; i++) {
        if (errors[i].ErrorCode == ErrorCode)
            return errors[i].Message;
    }
    return UnknowError;
}

/* ------------------------------------------------------------------------
 *  函数名：
 *      void sleep(unsigned long ms)
 *  功能：
 *      系统延时等待
 *  参数：
 *      ms              ----    延时的时间（毫秒）
 *  返回值：
 *      无
 *  说明：
 *      利用 OS 挂起当前任务
 * -----------------------------------------------------------------------*/

void sleep(ulong ms)
{
    ulong timeout = (ms * OS_TICKS_PER_SEC / 1000);

    while (timeout > 0xffff) {
        OSTimeDly(0xffff);
        timeout -= 0xffff;
    }
    
    OSTimeDly(timeout < 1 ? 1 : timeout);
}

/* ------------------------------------------------------------------------
 *  函数名：
 *      void udelay(int us)
 *  功能：
 *      系统延时等待
 *  参数：
 *      us              ----    延时的时间（微妙）
 *  返回值：
 *      无
 *  说明：
 *      靠CPU循环等待，没有低功耗处理
 * -----------------------------------------------------------------------*/
void udelay(ulong us)
{
    while (us) {
        us--;
    }
}

/* ------------------------------------------------------------------------
 *  函数名:
 *      void halt(const char *msg)
 *  功能:
 *      挂起系统,进入死循环
 *  参数:
 *      msg             ----    要显示的挂起消息
 *  返回值：
 *      无
 *  说明：
 *      
 * -----------------------------------------------------------------------*/

void halt(const char *msg)
{
    RawDebugOutputString("System halt: ");
    RawDebugOutputString(msg);
    while (true);
}

// ----------------------------------------------------------------------------
// OS 线程封装，需要动态内存支持

/* ------------------------------------------------------------------------
 *  函数名:
 *      THandle CreateThread(void (* routine)(void *), void *arg, UINT stacksize, UINT priority)
 *  功能:
 *      创建线程
 *  参数:
 *      routine         ----    线程入口地址
 *      arg             ----    传递给线程的参数
 *      stacksize       ----    线程堆栈大小（OS_STK 单位）
 *      priority        ----    线程优先级（0 ~ 63）
 *  返回值:
 *      是否成功
 *  说明:
 *      
 * -----------------------------------------------------------------------*/

typedef struct {
    UINT    MagicNumber;
    UINT    ID;
    UINT    Priority;
	void 	(*Routine)(void *);
	void *	Arg;
    void *  Stack;
	UINT	StackSize;
} TThread;

// 统一线程入口，线程代码可以退出
void ThreadEntry(void *arg)
{
	TThread *handle = (TThread *)arg;
	
	if (handle->MagicNumber == MAGIC_NUMBER_THREAD) {
		handle->Routine(handle->Arg);
		RawDebugPrintf("Thread %p (priority is %d) now exiting...\n", handle->Routine, handle->ID);
		KillThread(handle);
	}
	else {
		halt("ThreadEntry passed an invalid thread info.\n");
	}
}

THandle CreateThread(void (* routine)(void *), void *arg, UINT stacksize, UINT priority)
{
    OS_STK *stack;
    TThread *handle;
    byte err;
    
    if (priority >= OS_LOWEST_PRIO || OSTCBPrioTbl[priority] != NULL) {
        SetLastError(ERROR_THREAD_PRIORITY);
		halt("CreateThread failed, bad thread priority!");
        return NULL;
    }
    
    if (stacksize < MIN_THREAD_STACK_SIZE) {
        SetLastError(ERROR_THREAD_STACK);
		halt("CreateThread failed, stack size too small!");
        return NULL;
    }
    
    // 创建句柄
    handle = (TThread *)malloc(sizeof(TThread));
    if (handle == NULL) {
        SetLastError(ERROR_NO_ENOUGH_MEMORY);
		halt("CreateThread failed, no more memory!");
        return NULL;
    }

    // 创建堆栈
    stack = (OS_STK *)malloc(stacksize * sizeof(OS_STK));
    if (stack == NULL) {
        free(handle);
        halt("CreateThread failed, no more memory!");
        SetLastError(ERROR_NO_ENOUGH_MEMORY);
        return NULL;
    }
    else {
        RawDebugPrintf("CreateThread: routine: %p, stack = %p, stack size: %d, priority: %d.\n", routine, stack, stacksize * sizeof(OS_STK), priority);
    }

    // 句柄内容填充
    handle->MagicNumber = MAGIC_NUMBER_THREAD;
    handle->ID = priority;
    handle->Priority = priority;
	handle->Routine = routine;
	handle->Arg = arg;
    handle->Stack = stack;
	handle->StackSize = stacksize;

    // 创建线程
    err = OSTaskCreate(ThreadEntry, handle, &stack[stacksize - 1], priority);
    if (err != OS_NO_ERR) {
        free(handle);
        free(stack);
        SetLastError(ERROR_THREAD_OS_CREATE);
		halt("CreateThread failed, OS error!");
        return NULL;
    }
    
    SetLastError(NO_ERROR);
    return handle;
}

// --------------------------------------------------------------------------
// bool KillThread(THandle hThread)
//
// BUG : should free stack and handle before OSTaskDel
//       because when call OSTaskDel, the code after OSTaskDel will never be executed
//       this will cause memory leaks.
//       Fixed by Ken, 2006-02-15 17:12:43
// --------------------------------------------------------------------------

bool KillThread(THandle hThread)
{
    TThread *handle = (TThread *)hThread;
	byte err;
	UINT prio;

	ARMDisableInt();
    if (handle->MagicNumber == MAGIC_NUMBER_THREAD) {
		// 释放堆栈
		free(handle->Stack);
		
		// 修改幻数，防止下次重复删除
		handle->MagicNumber = ~MAGIC_NUMBER_THREAD;
		prio = handle->Priority;
		free(handle);
		
		// 删除任务
		err = OSTaskDel(prio);
        if (err != OS_NO_ERR) {            
			halt("KillThread failed!\n");
		}
		
		return true;
    }
	ARMEnableInt();
    
    SetLastError(ERROR_INVALID_HANDLE);
    return false;
}

// --------------------------------------------------------------------------
UINT GetCurrentThreadId()
{
    return (UINT)OSPrioCur;
}

// --------------------------------------------------------------------------
// OS 信号量封装,不需要动态内存支持
typedef struct TSemaphore {
	UINT		MagicNumber;
	OS_EVENT *	Object;
} TSemaphore;

TSemaphore sem_list[OS_MAX_EVENTS];

/* ------------------------------------------------------------------------
 *  函数名:
 *      THandle CreateSemaphore(UINT count)
 *  功能:
 *      创建信号量
 *  参数:
 *      count           ----    信号量最大容量
 *  返回值:
 *      信号量句柄
 *  说明:
 *      
 * -----------------------------------------------------------------------*/
THandle CreateSemaphore(UINT count)
{
	static bool sem_inited = false;
	TSemaphore *sem = NULL;
	int i;
	
	if (!sem_inited) {
		memset(sem_list, 0, sizeof(sem_list));
		sem_inited = true;
	}
	
	for (i = 0; i < sizeof(sem_list) / sizeof(TSemaphore); i++) {
		if (sem_list[i].MagicNumber != MAGIC_NUMBER_SEMAPHORE) {
			sem = &sem_list[i];
			sem->Object = OSSemCreate((unsigned short)count);
			if (sem->Object == NULL) {
				halt("CreateSemaphore failed: OS error!\n");
			}
			else {
				sem->MagicNumber = MAGIC_NUMBER_SEMAPHORE;
			}
		
			SetLastError(NO_ERROR);
			return (THandle)sem;
		}
	}
	
	return (THandle)sem;
}

/* ------------------------------------------------------------------------
 *  函数名:
 *      void CloseSemaphore(THandle hSemaphore)
 *  功能:
 *      关闭信号量
 *  参数:
 *      hSemaphore      ----    信号量句柄
 *  返回值:
 *      无
 *  说明:
 *      
 * -----------------------------------------------------------------------*/
void CloseSemaphore(THandle hSemaphore)
{
	TSemaphore *sem = (TSemaphore *)hSemaphore;
	byte err;
	
	if (sem->MagicNumber == MAGIC_NUMBER_SEMAPHORE) {
		do {
			OSSemDel(sem->Object, OS_DEL_NO_PEND, &err );
			if (err != OS_NO_ERR) {
				RawDebugPrintf("CloseSemaphore failed: %s. retry!\n", GetErrorMessage(ERROR_LEVEL_OS, err));
				OSTimeDly(1);
			}
		} while (err != OS_NO_ERR);

		// mark bad magic number and free memory
		sem->MagicNumber = ~MAGIC_NUMBER_SEMAPHORE;

		SetLastError(NO_ERROR);
	}
	else {
		halt("CloseSemaphore failed: bad param!\n");
		SetLastError(ERROR_BAD_PARAMETER);
	}
}

/* ------------------------------------------------------------------------
 *  函数名:
 *      TWaitResult WaitSemaphore(THandle hSemaphore, UINT timeout)
 *  功能:
 *      等待信号
 *  参数:
 *      hSemaphore      ----    信号量句柄
 *      timeout         ----    超时时间
 *  返回值:
 *      等待状态
 *      WAIT_OBJECT     ----    获取成功
 *      WAIT_TIMEOUT    ----    超时
 *      WAIT_FAILED     ----    失败
 *  说明:
 *      
 * -----------------------------------------------------------------------*/
TWaitResult WaitSemaphore(THandle hSemaphore, UINT timeout)
{
	TSemaphore *sem = (TSemaphore *)hSemaphore;
	byte err;
	
	if (sem->MagicNumber == MAGIC_NUMBER_SEMAPHORE) {
		// timeout = 0 means wait infinite
		if (timeout != 0) {
			timeout = (timeout * OS_TICKS_PER_SEC) / 1000;
		
			if (timeout < 1) {
				timeout = 1;
			}
			if (timeout > 0xffff) {
				timeout = 0xffff;
			}
		}
		
		OSSemPend(sem->Object, timeout, &err);
		SetLastError(NO_ERROR);
	
		switch (err) {
		case OS_TIMEOUT:
			return WAIT_TIMEOUT;
		case OS_NO_ERR:
			return WAIT_OBJECT;
		default:
			RawDebugPrintf("WaitSemaphore failed: %s.\n", GetErrorMessage(ERROR_LEVEL_OS, err));
			halt("WaitSemaphore failed: OS error!\n");
			return WAIT_FAILED;
		}
	}
	else {
		halt("WaitSemaphore failed: bad param!\n");
		SetLastError(ERROR_BAD_PARAMETER);
		return WAIT_FAILED;
	}
}

/* ------------------------------------------------------------------------
 *  函数名:
 *      bool SignalSemaphore(THandle hSemaphore)
 *  功能:
 *      释放信号
 *  参数:
 *      hSemaphore      ----    信号量句柄
 *  返回值:
 *      无
 *  说明:
 *      
 * -----------------------------------------------------------------------*/
bool SignalSemaphore(THandle hSemaphore)
{
	TSemaphore *sem = (TSemaphore *)hSemaphore;
	byte err;
	
	if (sem->MagicNumber == MAGIC_NUMBER_SEMAPHORE) {
		err = OSSemPost(sem->Object, &err);
		if (err != OS_NO_ERR) {
			RawDebugPrintf("SignalSemaphore failed: %s.\n", GetErrorMessage(ERROR_LEVEL_OS, err));
			RawDebugPrintf("SignalSemaphore failed: sem = %p, cnt = %d, err = %d!\n", sem->Object, sem->Object->OSEventCnt, err);
			return false;
		}
		SetLastError(NO_ERROR);
		return true;
	}
	else {
		SetLastError(ERROR_BAD_PARAMETER);
		return false;
	}
}

bool SetSemaphore(THandle hSemaphore, UINT count)
{
	TSemaphore *sem = (TSemaphore *)hSemaphore;
	byte err;
	
	if (sem->MagicNumber == MAGIC_NUMBER_SEMAPHORE) {
		OSSemSet(sem->Object, count, &err);
		if (err != OS_NO_ERR) {
			RawDebugPrintf("SetSemaphore failed: %s.\n", GetErrorMessage(ERROR_LEVEL_OS, err));
			RawDebugPrintf("SetSemaphore failed: sem = %p, cnt = %d, err = %d!\n", sem->Object, sem->Object->OSEventCnt, err);
			return false;
		}
		SetLastError(NO_ERROR);
		return true;
	}
	else {
		SetLastError(ERROR_BAD_PARAMETER);
		return false;
	}
}

// --------------------------------------------------------------------------
// OS 邮箱封装，需要动态内存支持

typedef struct TMailBox {
	UINT		MagicNumber;
	OS_EVENT *	Object;
	void **		Entries;
} TMailBox;

const void * MAIL_BOX_NULL_DATA;

/* ------------------------------------------------------------------------
 *  函数名:
 *      THandle CreateMailBox(UINT size)
 *  功能:
 *      创建邮箱
 *  参数:
 *      size            ----    邮箱的最大容量
 *  返回值:
 *      邮箱句柄
 *  说明:
 *      
 * -----------------------------------------------------------------------*/
THandle CreateMailBox(UINT size)
{
	TMailBox *mbox;
	
	if (size == 0) {
		SetLastError(ERROR_BAD_PARAMETER);
		return NULL;
	}
	
	mbox = (TMailBox *)malloc(sizeof(TMailBox));
	if (mbox == NULL) {
		SetLastError(ERROR_NO_ENOUGH_MEMORY);
		halt("CreateMailBox failed, out of memory!\n");
	}
	
	mbox->Entries = (void **)malloc(size * sizeof(void *));
	if (mbox->Entries == NULL) {
		free(mbox);
		SetLastError(ERROR_NO_ENOUGH_MEMORY);
		halt("CreateMailBox failed: out of memory!\n");
	}
	
	mbox->Object = OSQCreate(mbox->Entries, size);
	if (mbox->Object != NULL) {
		mbox->MagicNumber = MAGIC_NUMBER_MAILBOX;
		SetLastError(NO_ERROR);
		return (THandle)mbox;
	}
	else {
		halt("CreateMailBox failed: OS error!\n");
		
		free(mbox->Entries);
		free(mbox);
		
		SetLastError(ERROR_OS_MBOX_CREATE);
		return NULL;
	}
}

/* ------------------------------------------------------------------------
 *  函数名:
 *      void CloseMailBox(THandle hMailBox)
 *  功能:
 *      关闭邮箱
 *  参数:
 *      hMailBox        ----    邮箱句柄
 *  返回值:
 *      无
 *  说明:
 *      
 * -----------------------------------------------------------------------*/
void CloseMailBox(THandle hMailBox)
{
	TMailBox *mbox = (TMailBox *)hMailBox;
	byte err;
	
	if (mbox->MagicNumber == MAGIC_NUMBER_MAILBOX) {
		do {
			OSQDel(mbox->Object, OS_DEL_NO_PEND, &err);
			if (err != OS_NO_ERR) {
				RawDebugPrintf("CloseMailBox failed: %s. retry!\n", GetErrorMessage(ERROR_LEVEL_OS, err));
				OSTimeDly(1);
			}
		} while (err != OS_NO_ERR);

		// mark bad magic number
		mbox->MagicNumber = ~MAGIC_NUMBER_MAILBOX;
		free(mbox->Entries);
		free(mbox);
		
		SetLastError(NO_ERROR);
	}
	else {
		SetLastError(ERROR_BAD_PARAMETER);
	}
}

/* ------------------------------------------------------------------------
 *  函数名:
 *      TWaitResult FetchMailBox(THandle hMailBox, void **data, UINT timeout)
 *  功能:
 *      从邮箱获取数据
 *  参数:
 *      hMailBox        ----    邮箱句柄
 *      data            ----    存放数据指针的指针
 *      timeout         ----    超时时间（毫秒）,0表示无限等待
 *  返回值:
 *      等待状态
 *      WAIT_OBJECT     ----    获取成功
 *      WAIT_TIMEOUT    ----    超时
 *      WAIT_FAILED     ----    失败
 *  说明:
 *      
 * -----------------------------------------------------------------------*/
TWaitResult FetchMailBox(THandle hMailBox, void **data, UINT timeout)
{
	TMailBox *mbox = (TMailBox *)hMailBox;
	byte err;
	
	if (mbox->MagicNumber == MAGIC_NUMBER_MAILBOX) {
		// timeout = 0 means wait infinite
		if (timeout != 0) {
			timeout = (timeout * OS_TICKS_PER_SEC) / 1000;
		
			if (timeout < 1) {
				timeout = 1;
			}
			
			if (timeout > 0xffff) {
				timeout = 0xffff;
			}
		}

		if (data == NULL) {
			OSQPend(mbox->Object, timeout, &err);
		}
		else {
			*data = OSQPend(mbox->Object, timeout, &err);
		}

		switch (err) {
		case OS_TIMEOUT:
			return WAIT_TIMEOUT;
		case OS_NO_ERR:
			if (*data == &MAIL_BOX_NULL_DATA) {
				*data = NULL;
			}
			return WAIT_OBJECT;
		default:
			RawDebugPrintf("FetchMailBox failed: %s.\n", GetErrorMessage(ERROR_LEVEL_OS, err));
			halt("FetchMailBox failed: OS error!\n");
			return WAIT_FAILED;
		}
	}
	else {
		halt("FetchMailBox failed: bad param!\n");
		SetLastError(ERROR_BAD_PARAMETER);
		return WAIT_FAILED;
	}	
}

/* ------------------------------------------------------------------------
 *  函数名:
 *      bool PostMailBox(THandle hMailBox, void *data)
 *  功能:
 *      往邮箱发送数据
 *  参数:
 *      hMailBox        ----    邮箱句柄
 *      data            ----    要发送的数据指针
 *  返回值:
 *      无
 *  说明:
 *      
 * -----------------------------------------------------------------------*/

bool PostMailBox(THandle hMailBox, void *data)
{
	TMailBox *mbox = (TMailBox *)hMailBox;
	byte err;

	if (mbox->MagicNumber == MAGIC_NUMBER_MAILBOX) {
		if (data == NULL) {
			data = &MAIL_BOX_NULL_DATA;
		}

		do {
			err = OSQPost(mbox->Object, data);
			if (err != OS_NO_ERR) {
				// if OS Queue is full then wait (OS_Q_FULL)
				RawDebugPrintf("PostMailBox failed: OS error(%s), retry!\n", GetErrorMessage(ERROR_LEVEL_OS, err));
				OSTimeDly(1);
			}
		} while (err != OS_NO_ERR);
		
		return true;
	}
	else {
		SetLastError(ERROR_BAD_PARAMETER);
		return false;
	}
}

/* ------------------------------------------------------------------------
 *  函数名：
 *      char * TimeToString(char *buffer, const TTime *time, int language)
 *  功能：
 *      把时间转换成为字符串
 *  参数：
 *      time            ----    待转换的时间指针
 *      language        ----    语言类型
 *                                ◎ LAN_CHINESE		 中文
 *                                ◎ LAN_ENGLISH		 英文
 *  返回值：
 *      转换后的字符串值（例如"11:31:20"，或者"11时31分20秒")
 *  说明：
 *      不检查时间结构是否合理
 * -----------------------------------------------------------------------*/

char * TimeToString(char *buffer, const TTime *time, int language)
{
	char *fmt;
	
	switch (language) {
	case LAN_CHINESE:
		fmt = "%02d时%02d分%02d秒";
		break;
		
	default:
		fmt = "%02d:%02d:%02d";
		break;
	}
	
	sprintf(buffer, fmt, time->hour, time->minute, time->second);		
	return buffer;
}

/* ------------------------------------------------------------------------
 *  函数名：
 *      char * DateToString(char *buffer, const TDate *date, int language)
 *  功能：
 *      把日期转换成为字符串
 *  参数：
 *      data            ----    待转换的日期指针
 *      language        ----    语言类型
 *                                ◎ LAN_CHINESE		 中文
 *                                ◎ LAN_ENGLISH		 英文
 *  返回值：
 *      转换后的字符串值（例如"04-06-01"，或者"04年06月01日")
 *  说明：
 *      不检查日期结构是否合理
 * -----------------------------------------------------------------------*/

char * DateToString(char *buffer, const TDate *date, int language)
{
	char *fmt;
	
	switch (language) {
	case LAN_CHINESE:
		fmt = "%02d年%02d月%02d日 %02d时%02d分%02d秒";
		break;
		
	default:
		fmt = "%02d-%02d-%02d %02d:%02d:%02d";
		break;
	}
	
	sprintf(buffer, fmt, date->year, date->month, date->day);	
	return buffer;
}

/* ------------------------------------------------------------------------
 *  函数名：
 *      char *DateTimeToString(char *buffer, const TDateTime *datetime, int language)
 *  功能：
 *      把日期时间转换成为字符串
 *  参数：
 *      datetime        ----    待转换的日期指针
 *      language        ----    语言类型
 *                                ◎ LAN_CHINESE		 中文
 *                                ◎ LAN_ENGLISH		 英文
 *  返回值：
 *      转换后的字符串值
 *  说明：
 *      不检查日期结构是否合理
 * -----------------------------------------------------------------------*/

char *DateTimeToString(char *buffer, const TDateTime *datetime, int language)
{
	char *fmt;
	
	switch (language) {
	case LAN_CHINESE:
		fmt = "%02d年%02d月%02d日";
		break;
		
	default:
		fmt = "%02d-%02d-%02d";
		break;
	}
	
	sprintf(buffer, fmt, datetime->year, datetime->month, datetime->day, datetime->hour, datetime->minute, datetime->second);	
	return buffer;
}

/* ------------------------------------------------------------------------
 *  函数名：
 *      TDateTime * GetCurrentDateTime(TDateTime *datetime)
 *  功能：
 *      获取当前的日期和时间
 *  参数：
 *      datetime        ----    存放日期时间的指针
 *  返回值：
 *      传入的日期时间指针(datetime)
 *  说明：
 *      无
 * -----------------------------------------------------------------------*/
TDateTime * GetCurrentDateTime(TDateTime *datetime)
{
	TDate date;
	TTime time;
	
	GetCurrentDate(&date);
	GetCurrentTime(&time);
	
	datetime->day = date.day;
	datetime->month = date.month;
	datetime->week = date.week;
	datetime->year = date.year;

	datetime->second = time.second;
	datetime->minute = time.minute;
	datetime->hour = time.hour;
	
	return datetime;
}

/* ------------------------------------------------------------------------
 *  函数名：
 *      bool SetCurrentDateTime(const TDateTime *datetime)
 *  功能：
 *      设置当前日期和时间
 *  参数：
 *      datetime        ----    待设置的日期时间指针
 *  返回值：
 *      设置成功true，失败false
 *  说明：
 *      月1～12，日1～31，时0～23，分0～59，秒0～59
 * -----------------------------------------------------------------------*/
bool SetCurrentDateTime(const TDateTime *datetime)
{
	TDate date;
	TTime time;

	date.day = datetime->day;
	date.month = datetime->month;
	date.week = datetime->week;
	date.year = datetime->year;

	time.second = datetime->second;
	time.minute = datetime->minute;
	time.hour = datetime->hour;

	if (!SetCurrentDate(&date)) {
		return false;
	}
	
	if (!SetCurrentTime(&time)) {
		return false;
	}
	
	return true;
}

/* ------------------------------------------------------------------------
 *  函数名：
 *      int CompareDate(const TDate *date1, const TDate *date2)
 *  功能：
 *      比较两个日期
 *  参数：
 *      date1           ----    日期指针1
 *      date2           ----    日期指针2
 *  返回值：
 *      如果相等返回 0，如果 date1 大于 date2 返回大于零，否则小于零
 *  说明：
 *      
 * -----------------------------------------------------------------------*/

int CompareDate(const TDate *date1, const TDate *date2)
{
	UINT value1, value2;
	
	value1 = MAKE_DATETIME_UINT(date1->year, date1->month, date1->day);
	value2 = MAKE_DATETIME_UINT(date2->year, date2->month, date2->day);
	
	if (value1 == value2) return 0;
	return (value1 > value2 ? 1 : -1);
}

/* ------------------------------------------------------------------------
 *  函数名：
 *      int CompareTime(const TTime *time1, const TTime *time2)
 *  功能：
 *      比较两个时间
 *  参数：
 *      time1           ----    时间指针1
 *      time2           ----    时间指针2
 *  返回值：
 *      如果相等返回 0，如果 time1 大于 time2 返回大于零，否则小于零
 *  说明：
 *      
 * -----------------------------------------------------------------------*/

int CompareTime(const TTime *time1, const TTime *time2)
{
	UINT value1, value2;
	
	value1 = MAKE_DATETIME_UINT(time1->hour, time1->minute, time1->second);
	value2 = MAKE_DATETIME_UINT(time2->hour, time2->minute, time2->second);
	
	if (value1 == value2) return 0;
	return (value1 > value2 ? 1 : -1);
}

/* ------------------------------------------------------------------------
 *  函数名：
 *      int CompareDateTime(const TDateTime *datetime1, const TDateTime *datetime2)
 *  功能：
 *      比较两个时间
 *  参数：
 *      datetime1       ----    日期时间指针1
 *      datetime2       ----    日期时间指针2
 *  返回值：
 *      如果相等返回 0，如果 datetime1 大于 datetime2 返回大于零，否则小于零
 *  说明：
 *      
 * -----------------------------------------------------------------------*/

int CompareDateTime(const TDateTime *datetime1, const TDateTime *datetime2)
{
	UINT value1, value2;
	
	value1 = MAKE_DATETIME_UINT(datetime1->year, datetime1->month, datetime1->day);
	value2 = MAKE_DATETIME_UINT(datetime2->year, datetime2->month, datetime2->day);
	
	if (value1 == value2) {
		// 如果日期一样，再比较时间
		value1 = MAKE_DATETIME_UINT(datetime1->hour, datetime1->minute, datetime1->second);
		value2 = MAKE_DATETIME_UINT(datetime2->hour, datetime2->minute, datetime2->second);
		
		if (value1 == value2) return 0;
		return (value1 > value2 ? 1 : -1);
	}
	else {
		// 直接比较日期
		return (value1 > value2 ? 1 : -1);
	}
}

// ----------------------------------------------------------------------------
// E N D
