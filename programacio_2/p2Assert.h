#ifndef __P2SSERT_H__
  #define __P2ASSERT_H__

#include "p2Defs.h"
#include "p2Log.h"

// Assert only in debug
#ifdef _DEBUG
#define ASSERT_MSG_LEN 4096

// Returns 1 if we want to break
static int _p2assert( const char* expr, const char* msg, const char* file, const char* func, const unsigned long line )
{
  static char   _assert_msg[ASSERT_MSG_LEN];
  static char   _assert_title[MAX_PATH];
  static DWORD  _lastErr = GetLastError();
  static char*  _file = NULL;
  static int    r = 1;
  static void*  _last_error_msg = NULL;

  /*$1- Cut to show only filename (no dirs) ----------------------------------*/
  _file = (char *) strrchr( file, '\\' );
  _file = _file ? _file + 1 : (char*) file;

  /*$1- Create a message suitable for the Output Debug  ----------------------*/
  if( msg != NULL )
	_snprintf_s( _assert_msg, ASSERT_MSG_LEN, 2000, "\n%s(%u) : ASSERT (%s) \"%s\"\n", file, line, expr, msg );
  else
	_snprintf_s( _assert_msg, ASSERT_MSG_LEN, 2000, "\n%s(%u) : ASSERT (%s)\n", file, line, expr );

  OutputDebugString( _assert_msg );

  /*$1- Now a message for the MessageBox Title -------------------------------*/
  char  modulePath[MAX_PATH];
  GetModuleFileName( NULL, modulePath, MAX_PATH );

  const char*   moduleName = strrchr( modulePath, '\\' );
  moduleName = moduleName ? moduleName + 1 : modulePath;

  char  title[MAX_PATH + 20];
  sprintf_s( title, MAX_PATH + 20, "%s ASSERT!", moduleName );

  /*$1- Message box contents -------------------------------------------------*/
  _snprintf_s( _assert_msg, ASSERT_MSG_LEN, 2000, "%s(%u) : %s()\n\nExpr: (%s)", _file, line, func, expr );

  if( msg != NULL )
	_snprintf_s( _assert_msg, ASSERT_MSG_LEN, 2000, "%s(%u) : %s()\n\nASSERT (%s) \"%s\"", _file, line, func, expr, msg );
  else
	_snprintf_s( _assert_msg, ASSERT_MSG_LEN, 2000, "%s(%u) : %s()\n\nASSERT (%s)", _file, line, func, expr );

  int res = MessageBox( NULL, _assert_msg, title, MB_ICONERROR | MB_TASKMODAL | MB_YESNOCANCEL );
  if( IDYES == res )
  {
    r = 0;
  }
  else if( IDCANCEL == res )
  {
    _exit( EXIT_FAILURE );
  }

  return( r );
}

/*$1- Assert macro -----------------------------------------------------------*/
    #define ASSERT( expr )                                              \
  do                                                                        \
  {                                                                         \
    if( !(expr) )                                                           \
    {                                                                       \
      if( _p2assert(#expr, NULL, __FILE__, __FUNCTION__, __LINE__) == 0 ) \
      {                                                                     \
        _asm                                                                \
        {                                                                   \
          int 3                                                             \
        }                                                                   \
      }                                                                     \
    }                                                                       \
  } while( 0 )

/*$1- Assert macro with message ----------------------------------------------*/
    #define ASSERT_MSG( expr, msg )                                    \
  do                                                                       \
  {                                                                        \
    if( !(expr) )                                                          \
    {                                                                      \
      if( _p2assert(#expr, msg, __FILE__, __FUNCTION__, __LINE__) == 0 ) \
      {                                                                    \
        _asm                                                               \
        {                                                                  \
          int 3                                                            \
        }                                                                  \
      }                                                                    \
    }                                                                      \
  } while( 0 )
  
#else // _DEBUG

	#define ASSERT( expr )          __noop
	#define ASSERT_MSG( expr, msg ) __noop

#endif

#endif // __P2ASSERT_H__
    