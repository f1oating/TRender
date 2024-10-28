#ifndef TLOGMANAGER_H_INCLUDED
#define TLOGMANAGER_H_INCLUDED

//------------------------------------------------------------------------

#include <string>
#include <sstream>
#include <iostream>
#include <chrono>
#include <iomanip>
#include <cstdio>
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
    std::string m_ErrorDesc;
    std::string m_SrcFileName;
    int m_LineNumber;
    std::string m_ErrText;

    // Override std::exception::what
    const char* what();

    TException(int ErrorNumber, std::string ErrorDesc, std::string SrcFileName, int LineNumber);
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
    virtual ~TLogManager() {}
    static TLogManager m_LogManager;

public:
    std::stringstream m_LogBuffer;
    void create(const std::string& Filename);
    void flush();
    void close();
    void logException(TException e);
    std::string getTimeString();

private:
    FILE* m_LogFile;
};

//------------------------------------------------------------------------

#endif // TLOGMANAGER_H_INCLUDED
