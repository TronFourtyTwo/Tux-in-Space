/*
#############################################################################################
#    CSpace - space simulator																#
#    Copyright (C) 2016  emanuele.sorce@hotmail.com											#
#																							#
#    This program is free software; you can redistribute it and/or modify					#
#    it under the terms of the GNU General Public License as published by					#
#    the Free Software Foundation, version 3 or compatibles.								#
#																							#
#    This program is distributed in the hope that it will be useful,						#
#    but WITHOUT ANY WARRANTY; without even the implied warranty of							#
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the							#
#    GNU General Public License for more details.											#
#																							#
#    You should have received a copy of the GNU General Public License						#
#    along with this program; if not, write to the Free Software							#
#    Foundation, Inc.																		#
#############################################################################################


#########################################################################################################################################################################################################
# This is the Main function of the program but doesn't do much, is a launchpad for compiling and esecution, is a collection of structures for the other functions and contein file's initialization		#
# Questa è la funzione principale del programma e non ha un vero scopo ma fa da piattaforma per compilazione ed esecuzione e raggruppa le strutture e inizializza i file di configurazione				#
# 																																																		#
#########################################################################################################################################################################################################
*/

	// include definition of constants
	#include "constants.h"

	// the structure infostruct is a structure that contein information about the options and other tecnical things 
	struct info {
		// Visual mode (0 = Only OPS)
		int vmode;
		// Maxoutput is the maximum number of char that can be printed (not consider the frames)
		int maxoutput;
		// The number of columns and lines that Rmotor's function must use.
		int column;
		int line;
		// number of character used for printing the deciamal of a long double
		int numprecision;
	};
	typedef struct info tinf;

	// definition of the type of the object's structures
	// structure of one object
	struct object {
		char name [NAMELUN];// the name of the object (es.: Earth, My Planet, Moon)
		int type;	 		// the type of object. see information.txt
		long double mass;	// the mass
		long double x;   	// the coordinate x
		long double y;		// the coordinate y
		long double z;		// the coordinate z
		long double velx;	// the fast of the movement in x
		long double vely;	// the fast of the movement in y
		long double velz;	// the fast of the movement in z
	};
	typedef struct object tobj;
	
	// one system's structure: include the objects, the name and the number, the active object's position, the time of the system and the G constant of universal gravitation
	struct system {
		char name [NAMELUN];
		// Precision of the simulation (in second). [Pmotor needs it]
		float precision;
		int year;
		int day;
		int hour;
		int min;
		int sec;
		int millisec;
		int active[NUMOGG];		//This variable contein the position of the active (type!=0) object
		int nactive;			//the number of them
		tobj o[NUMOGG];
		long double G;
	};
	typedef struct system tsys;


	// Include the header file
	#include "CS_header.h"

	// Funzione principale

	int main (int argc, char *argv[]) {
		
		// definition of the structures
		tinf inf;
		tsys sys;

		// The return value from shell function
		int staterec;
		
		// default things for struct inf are setted:
		// line and column are respectively the number of line and column that Rmotor can use for printing
		inf.line = 40;
		inf.column = 100;
	
		// maxoutput is the max number of character that Rmotor can effectevely use (there are four column and five lines occupied by the frame)
		Setmaxoutput(&inf);
		
		// the vmode is setted 0 (onlyOPS)
		inf.vmode = 0;
	
		// the precision is setted 1 calcolation every 2 second
		sys.precision = 2;
		
		// set num precision to 7
		inf.numprecision = 4;

		// call the shell and she returns a state
		staterec = Shell (&sys, &inf);
		
		//The program finish
		if(staterec != 0) {
			Rmotor (0, &inf, 0, "main: The shell function exited whit %i (This is an error or, in the best case, an unexpected event)", &staterec, 0, 0);
		}
		
		return 0;
	}
