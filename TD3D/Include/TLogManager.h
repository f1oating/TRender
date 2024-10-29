#ifndef TLOGMANAGER_H_INCLUDED
#define TLOGMANAGER_H_INCLUDED

//------------------------------------------------------------------------

#include <cstdio>
#include <cstring>
#include <chrono>
#include <exception>

#ifndef THROW_EXCEPTION
#define THROW_EXCEPTION(ErrorNum, ErrorDesc) throw TException(ErrorNum, ErrorDesc, __FILE__, __LINE__);
#endif

//------------------------------------------------------------------------

// Custom exception class

class TException : public std::exception
{
private:
protected:
public:
    int m_ErrorNumber;
    char m_ErrorDesc[256];
    char m_SrcFileName[256];
    int m_LineNumber;
    char m_ErrText[1024];

    // Override std::exception::what
    const char* what();

    TException(int ErrorNumber, const char* ErrorDesc, const char* SrcFileName, int LineNumber);
    ~TException() throw() {}
};

//------------------------------------------------------------------------

// Singleton Logging Object

class TLogManager
{
public:
    static TLogManager* GetLogManager();

protected:
    TLogManager();
    ~TLogManager();
    static TLogManager m_LogManager;

public:
    void logException(TException e);
    void logMessage(const char* head, const char* message);  // New function for logging a regular message
    

private:
    void create(const char* Filename);
    void flush();
    void close();

    const char* getTimeString();

    char m_LogBuffer[1024];
    FILE* m_LogFile;

};

//------------------------------------------------------------------------

#endif // TLOGMANAGER_H_INCLUDED
