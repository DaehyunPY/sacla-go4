////////////////////////////////////////////////////////////////////////////////////////
//
// A C++ interface to gnuplot.
//
//
// The interface uses pipes and so wont run on a system that doesn't have POSIX pipe support
// Tested on Windows and Linux
//
// Version history:
// 0. C interface written by N. Devillard (27/01/03)
//    http://ndevilla.free.fr/gnuplot/
// 1. C++ interface: direct translation from the C interface by Rajarshi Guha (07/03/03)
//    http://cheminfo.informatics.indiana.edu/~rguha/code/cc++/
// 2. corrections for Win32 compatibility by V.Chyzhdzenka (20/05/03)
//    chyzhdzenka@mail.ru
// 3. corrections for Win32 and Linux compatibility, some member functions added
//    by M. Burgis (22/01/08)
//
// Requirements:
// * gnuplot has to be installed (http://www.gnuplot.info/download.html)
// * set Path-Variable for Gnuplot path (e.g. C:/program files/gnuplot/bin; /usr/bin)
//     or set Gnuplot path with: Gnuplot::set_GNUPlotPath(const std::string &path)
//
////////////////////////////////////////////////////////////////////////////////////////


#ifndef _GNUPLOT_PIPES_H_
#define _GNUPLOT_PIPES_H_

#include "TObject.h"

#include <string>
#include <vector>
#include <stdexcept>  // for std::runtime_error class in GnuplotException
#include <cstdio>     // for FILE


//declare classes in global namespace

/**
 * \brief handles exceptions within the Gnuplot class
 * 
 * This is a light version of the last version of Gnuplot++. It consists only of the basic constructor, the init() routine and the command-passing function cmd()
 */
class GnuplotException : public std::runtime_error
{
	public:
		GnuplotException(const std::string &msg) : std::runtime_error(msg){}
		
 	ClassDef(GnuplotException, 1);
};


/**
 * \brief Provides a interface to pipe commands to the plotting tool Gnuplot
 * 
 * 
 */
class Gnuplot
{
	private:

  // member data
		FILE                    *gnucmd;
		bool                     valid;        // validation of gnuplot session
		int                      nplots;       // number of plots in session
		std::vector<std::string> tmpfile_list; // list of created tmpfiles

  // static data
		static int               tmpfile_num;        // number of all tmpfiles (number of tmpfiles restricted)
		static std::string       m_sGNUPlotFileName; // Name of executed GNUPlot file
		static std::string       m_sGNUPlotPath;     // gnuplot path

  // member functions (auxiliary functions)
		void           init(bool checkForX11);              // get_program_path(); and popen();
		std::string    create_tmpfile(std::ofstream &tmp);  // creates tmpfile and returns its name

  //static functions
		static bool    get_program_path();                                   // gnuplot path found?
		static bool    file_exists(const std::string &filename, int mode=0); // checks if file exists


	public:

  // optional: set Gnuplot path manual
  //   for windows: path with slash "/" not backslash "\"
		static bool set_GNUPlotPath(const std::string &path);

		Gnuplot(bool checkForX11 = false);

  // destructor: needed to delete temporary files
		virtual ~Gnuplot();

  // send a command to gnuplot
		Gnuplot& cmd(const std::string &cmdstr);
		Gnuplot& operator<<(const std::string &cmdstr);

  // validation of gnuplot session
		bool is_valid();

	ClassDef(Gnuplot, 1);
};


#endif
