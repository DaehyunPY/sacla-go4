////////////////////////////////////////////
//
// A C++ interface to gnuplot.
//
// This is a direct translation from the C interface
// written by N. Devillard (which is available from
// http://ndevilla.free.fr/gnuplot/).
//
// As in the C interface this uses pipes and so wont
// run on a system that doesn't have POSIX pipe
// support
//
// Rajarshi Guha
// <rajarshi@presidency.com>
//
// 07/03/03
//
////////////////////////////////////////////
//
// A little correction for Win32 compatibility
// and MS VC 6.0 done by V.Chyzhdzenka
//
// Notes:
// 1. Added private method Gnuplot::init().
// 2. Temporary file is created in th current
//    folder but not in /tmp.
// 3. Added #indef WIN32 e.t.c. where is needed.
// 4. Added private member m_sGNUPlotFileName is
//    a name of executed GNUPlot file.
//
// Viktor Chyzhdzenka
// e-mail: chyzhdzenka@mail.ru
//
// 20/05/03
//
////////////////////////////////////////////
//
// corrections for Win32 and Linux compatibility
//
// some member functions added:
//  set_GNUPlotPath
//  create_tmpfile, get_program_path, file_exists
//  operator<<, plotfile_*, plot_xy_err
//  plot_equation3d, savetops, showonscreen
// set, unset: pointsize, grid, *logscale, samples,
//  isosamples, hidden3d, legend, title, cbrange
//
// Markus Burgis
//
// 22/01/08
//
////////////////////////////////////////////


#include <fstream>              // for std::ifstream
#include <sstream>              // for std::ostringstream
#include <list>                 // for std::list
#include <cstdio>               // for FILE, fputs(), fflush(), popen()
#include <cstdlib>              // for getenv()
#include "Gnuplot.h"


#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__) //defined for 32 and 64-bit environments
#include <io.h>                // for _access(), _mktemp()
#define GP_MAX_TMP_FILES    27 // 27 temporary files it's Microsoft restriction
#elif defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__) //all UNIX-like OSs (Linux, *BSD, MacOSX, Solaris, ...)
#include <unistd.h>            // for access(), mkstemp()
#define GP_MAX_TMP_FILES    64
#else
#error unsupported or unknown operating system
#endif



//----------------------------------------------------------------------------------
//
// initialize static data
//
int Gnuplot::tmpfile_num = 0;

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__)
std::string Gnuplot::m_sGNUPlotFileName = "pgnuplot.exe";
std::string Gnuplot::m_sGNUPlotPath = "C:/program files/gnuplot/bin/";
#elif defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
std::string Gnuplot::m_sGNUPlotFileName = "gnuplot";
std::string Gnuplot::m_sGNUPlotPath = "/usr/bin/";
#endif


//----------------------------------------------------------------------------------
//
// define static member function
//
bool Gnuplot::set_GNUPlotPath(const std::string &path)
{

	std::string tmp = path + "/" + Gnuplot::m_sGNUPlotFileName;


#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__)
	if ( Gnuplot::file_exists(tmp,0) ) // check existence
#elif defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
		if ( Gnuplot::file_exists(tmp,1) ) // check existence and execution permission
#endif
	{
		Gnuplot::m_sGNUPlotPath = path;
		return true;
	}else{
		Gnuplot::m_sGNUPlotPath.clear();
		return false;
	}
}


//----------------------------------------------------------------------------------
//
// A string tokenizer taken from http://www.sunsite.ualberta.ca/Documentation/
// /Gnu/libstdc++-2.90.8/html/21_strings/stringtok_std_h.txt
//
template <typename Container>
		void stringtok (Container &container,
						std::string const &in,
	  const char * const delimiters = " \t\n")
{
	const std::string::size_type len = in.length();
	std::string::size_type i = 0;

	while ( i < len )
	{
        // eat leading whitespace
		i = in.find_first_not_of (delimiters, i);

		if (i == std::string::npos)
			return;   // nothing left but white space

        // find the end of the token
		std::string::size_type j = in.find_first_of (delimiters, i);

        // push token
		if (j == std::string::npos)
		{
			container.push_back (in.substr(i));
			return;
		}
		else
			container.push_back (in.substr(i, j-i));

        // set up for next loop
		i = j + 1;
	}

	return;
}


//----------------------------------------------------------------------------------
//
// constructor: set a style during construction
//
Gnuplot::Gnuplot(bool checkForX11)
{
	this->init(checkForX11);
}


//----------------------------------------------------------------------------------
//
// Destructor: needed to delete temporary files
//
Gnuplot::~Gnuplot()
{
	if ((this->tmpfile_list).size() > 0)
	{
		for (unsigned int i = 0; i < this->tmpfile_list.size(); i++)
			remove( this->tmpfile_list[i].c_str() );

		Gnuplot::tmpfile_num -= this->tmpfile_list.size();
	}

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__)
	if (_pclose(this->gnucmd) == -1)
#elif defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
		if (pclose(this->gnucmd) == -1)
#endif
			throw GnuplotException("Problem closing communication to gnuplot");
}


//----------------------------------------------------------------------------------
//
// Find out if valid is true
//
bool Gnuplot::is_valid()
{
	return(this->valid);
}

//----------------------------------------------------------------------------------
//
// Sends a command to an active gnuplot session
//
Gnuplot& Gnuplot::cmd(const std::string &cmdstr)
{
	if( !(this->valid) )
	{
		return *this;
	}

    // writes the string cmdstr to the stream gnucmd
	fputs( (cmdstr+"\n").c_str(), this->gnucmd );

    // flushes the stream, the stream remains open after this call
	fflush(this->gnucmd);

	return *this;
}



//----------------------------------------------------------------------------------
//
// Sends a command to an active gnuplot session, overloads <<
//
Gnuplot& Gnuplot::operator<<(const std::string &cmdstr)
{
	this->cmd(cmdstr);
	return *this;
}



//----------------------------------------------------------------------------------
//
// Opens up a gnuplot session, ready to receive commands
//
void Gnuplot::init(bool checkForX11)
{
#if defined(unix) || defined(__unix) || defined(__unix__)  // check DISPLAY variable
	if (getenv("DISPLAY") == NULL)   {
		this->valid = false;
		throw GnuplotException("Can't find DISPLAY variable");
	}
#elif defined(__APPLE__)
	bool didCheck;
	if (checkForX11) {
		if (getenv("DISPLAY") == NULL)   {
			this->valid = false;
			throw GnuplotException("Can't find DISPLAY variable");
		}   
		didCheck = true;
	} else {
		this->valid = true;
		didCheck = false;
	}
#endif

    // if gnuplot not available
	if (!Gnuplot::get_program_path())
	{
		this->valid = false;
		throw GnuplotException("Can't find gnuplot");
	}

	//
    // open pipe
	//
	std::string tmp = Gnuplot::m_sGNUPlotPath + "/" + m_sGNUPlotFileName;

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__)
	this->gnucmd = _popen(tmp.c_str(),"w");
#elif defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
	this->gnucmd = popen(tmp.c_str(),"w");
#endif

	if (!this->gnucmd)
	{
		this->valid = false;
		throw GnuplotException("Couldn't open connection to gnuplot");
	}

	this->nplots = 0;
	this->valid = true;
    
    // set terminal type
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__)
	this->cmd("set terminal windows");
#elif defined(unix) || defined(__unix) || defined(__unix__)
	this->cmd("set terminal x11");
#elif defined(__APPLE__)
	if (didCheck) this->cmd("set terminal x11");
	else this->cmd("set terminal aqua");
#endif


	return;
}


//----------------------------------------------------------------------------------
//
// Find out if a command lives in m_sGNUPlotPath or in PATH
//
bool Gnuplot::get_program_path()
{
	//
    // first look in m_sGNUPlotPath for Gnuplot
	//
	std::string tmp = Gnuplot::m_sGNUPlotPath + "/" + Gnuplot::m_sGNUPlotFileName;

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__)
	if ( Gnuplot::file_exists(tmp,0) ) // check existence
#elif defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
		if ( Gnuplot::file_exists(tmp,1) ) // check existence and execution permission
#endif
	{
		return true;
	}


	//
    // second look in PATH for Gnuplot
	//
	char *path;
    // Retrieves a C string containing the value of the environment variable PATH
	path = getenv("PATH");


	if (path == NULL)
	{
		throw GnuplotException("Path is not set");
		return false;
	}
	else
	{
		std::list<std::string> ls;

        //split path into list ls
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__)
		stringtok(ls,path,";");
#elif defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
		stringtok(ls,path,":");
#endif

        // scan list for Gnuplot program files
		for (std::list<std::string>::const_iterator i = ls.begin(); i != ls.end(); ++i)
		{
			tmp = (*i) + "/" + Gnuplot::m_sGNUPlotFileName;
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__)
			if ( Gnuplot::file_exists(tmp,0) ) // check existence
#elif defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
				if ( Gnuplot::file_exists(tmp,1) ) // check existence and execution permission
#endif
			{
				Gnuplot::m_sGNUPlotPath = *i; // set m_sGNUPlotPath
				return true;
			}
		}

		tmp = "Can't find gnuplot neither in PATH nor in \"" + Gnuplot::m_sGNUPlotPath + "\"";
		throw GnuplotException(tmp);

		Gnuplot::m_sGNUPlotPath = "";
		return false;
	}
}



//----------------------------------------------------------------------------------
//
// check if file exists
//
bool Gnuplot::file_exists(const std::string &filename, int mode)
{
	if ( mode < 0 || mode > 7)
	{
		throw std::runtime_error("In function \"Gnuplot::file_exists\": mode has to be an integer between 0 and 7");
		return false;
	}

    // _access(const char *path, int mode) returns 0 if the file has the given mode,
    // it returns -1 if the named file does not exist or is not accessible in the given mode
    // mode = 0 (F_OK) (default): checks file for existence only
    // mode = 1 (X_OK): execution permission
    // mode = 2 (W_OK): write permission
    // mode = 4 (R_OK): read permission
    // mode = 6       : read and write permission
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__)
	if (_access(filename.c_str(), mode) == 0)
#elif defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
		if (access(filename.c_str(), mode) == 0)
#endif
	{
		return true;
	}
	else
	{
		return false;
	}

}



//----------------------------------------------------------------------------------
//
// Opens a temporary file
//
std::string Gnuplot::create_tmpfile(std::ofstream &tmp)
{

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__)
	char name[] = "gnuplotiXXXXXX"; //tmp file in working directory
#elif defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
	char name[] = "/tmp/gnuplotiXXXXXX"; // tmp file in /tmp
#endif

	//
    // check if maximum number of temporary files reached
	//
	if (Gnuplot::tmpfile_num == GP_MAX_TMP_FILES - 1)
	{
		std::ostringstream except;
		except << "Maximum number of temporary files reached (" << GP_MAX_TMP_FILES
				<< "): cannot open more files" << std::endl;

		throw GnuplotException( except.str() );
		return "";
	}

	//
    // open temporary files for output
	//
#if defined(_MSC_VER) && ( defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__) )
	if (_mktemp_s(name) == NULL)
#elif defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__)
		if (_mktemp(name) == NULL)
#elif defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
			if (mkstemp(name) == -1)
#endif
	{
		std::ostringstream except;
		except << "Cannot create temporary file \"" << name << "\"";
		throw GnuplotException(except.str());
		return "";
	}

	tmp.open(name);
	if (tmp.bad())
	{
		std::ostringstream except;
		except << "Cannot create temporary file \"" << name << "\"";
		throw GnuplotException(except.str());
		return "";
	}

	//
    // Save the temporary filename
	//
	this->tmpfile_list.push_back(name);
	Gnuplot::tmpfile_num++;

	return name;
}

ClassImp(Gnuplot);
