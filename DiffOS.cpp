
#include "Daemon/Daemon.h"
#include <ctype.h>
//#include <statbuf.h>

#ifdef __unix__
int CreateDirectory(char*q,void* r)
{
  mkdir(q,0);
return 0;
}

int MoveFile(char*f,char*t)
{
  if (!link(f,t))
    if (!unlink(f))
      return 0;
  return -1;
}

unsigned long GetTickCount()
{
  timeval t;
  gettimeofday(&t,NULL);
  return t.tv_sec*1000+t.tv_usec;
}

#endif // unix


unsigned long GetFSize(FILE*fp)
{
#ifdef _WIN32
  return _filelength(_fileno(fp));
#endif //_WIN32
#ifdef __unix__
	struct  stat st;
	fstat(fileno(fp),&st);
	return st.st_size;
return 0;
#endif // __unix__
}

#ifdef __linux__
char *_strlwr(char*s)
{
  char*tmp=s;
  while (*tmp)
    {
      *tmp=tolower(*tmp);
      tmp++;
    }
  return s;
}
#endif


bool basicdl::ConnectErr(LPARAM lParam)
{
  char str[1024];
#ifdef _WIN32
  if (WSAGETSELECTERROR(lParam))
    {
      if (WSAGETSELECTERROR(lParam)==WSAECONNREFUSED)
#endif //win32
#ifdef __unix__
	if (lParam)
	  {
	    if (lParam==ECONNREFUSED)
#endif 
	      {
		if (firectrl)
	    {
	      strcpy(str,GetSTR(12,"FireWall refused to connect."));
	      wb(str,3);
	      if (type==1) log_wr(str,true);
	    }
	  else
	    {
	      strcpy(str,GetSTR(13,"Host refused to connect."));
	      wb(str,3);
	      if (type==1) log_wr(str,true);
	    }
	}
      else
	{
	  strcpy(str,GetSTR(14,"Error connecting."));
	  wb(str,3);
	  if (type==1) log_wr(str,true);
	}
      dds();
      return true;
    }
  return false;
}

//find funcs
#ifdef _WIN32
 bool FFirstFile(char *ffn,char *match)
   {
     if ((findhandle=FindFirstFile(ffn,&findstruct))==INVALID_HANDLE_VALUE)
       return false;
     strcpy(match,findstruct.cFileName);
     return true;
   }
 bool FNextFile(char *match)
   {
     if (FindNextFile(findhandle,&findstruct))
       {
	 strcpy(match,findstruct.cFileName);
	 return true;
       }
     else
       return false;
   }
#endif
#ifdef __unix__
 bool FFirstFile(char *ffn,char *match)
   {
     findcount=0;
     if (findstruct.gl_pathc)
       {
	 globfree(&findstruct);
	 findstruct.gl_pathc=0;
       }
     findcount=0;
     if (glob(ffn,0,NULL,&findstruct)) return false;
     printf("he\n");
     for (int k=0;k<findstruct.gl_pathc;k++)
       printf("%s\n",findstruct.gl_pathv[k]);
	 if (strrchr(findstruct.gl_pathv[0],'/'))
		strcpy(match,strrchr(findstruct.gl_pathv[0],'/')+1);
	 else
		 strcpy(match,findstruct.gl_pathv[0]);
     return true;
   }
 bool FNextFile(char *match)
   {
     findcount++;
     if (findstruct.gl_pathv[findcount]==NULL) return false;
	 if (strrchr(findstruct.gl_pathv[findcount],'/'))
		strcpy(match,strrchr(findstruct.gl_pathv[findcount],'/')+1);
	 else
		 strcpy(match,findstruct.gl_pathv[findcount]);
     //strcpy(match,findstruct.gl_pathv[findcount]);
     return true;
   }
#endif

