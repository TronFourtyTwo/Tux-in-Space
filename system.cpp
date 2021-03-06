/*
#################################################################################
#    Tux in Space - space exploration game										#
#    Copyright (C) 2016-2017  emanuele.sorce@hotmail.com						#
#																				#
#    This program is free software; you can redistribute it and/or modify		#
#    it under the terms of the GNU General Public License as published by		#
#    the Free Software Foundation, version 3 or compatibles.					#
#																				#
#    This program is distributed in the hope that it will be useful,			#
#    but WITHOUT ANY WARRANTY; without even the implied warranty of				#
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the				#
#    GNU General Public License for more details.								#
#																				#
#    You should have received a copy of the GNU General Public License			#
#    along with this program; if not, write to the Free Software				#
#    Foundation, Inc.															#
#################################################################################
 *
 * Here we have functions that manage objects
 * 
 */

#include "system.h"

using namespace std;

/***
 * 	The object editor
 */

void system_c::EditObj (const setting& set, object& obj) {
	
	int input;
	string comment;				// This is a buffer that contein comment of what is just been done
	string mass_irregularity;	// assume the value IRREGULARITY if the mass is out of range
	string color_irregularity;	// assume the value IRREGULARITY if the color is out of range
	string name_irregularity;	// assume the value IRREGULARITY if there is no name
	string radius_irregularity;	// assume the value IRREGULARITY if the radius is 0
	string touch_irregularity;	// assume the value IRREGULARITY if it touch some other object
	
	// the object after the editing
	object newobj(obj);
	
	// the loop
	while(1) {
		// check for IRREGALARITY
		// irregularity: NAME
		name_irregularity.clear();
		if (!newobj.name.length()) {
			name_irregularity = IRREGULARITY;
			comment += "\n" IRREGULARITY " name not given yet";
		}
		// Two object can't have the same name
		for(unsigned int i=0; i!=o.size(); i++){
			if (!newobj.name.compare(o[i].name)){
				if(o[i] == obj)
					continue;
				name_irregularity = IRREGULARITY;
				comment += "\n" IRREGULARITY " the name '";
				comment += obj.name;
				comment += "' already names another object";
				break;
			}
		}
		// irregularity: MASS
		if ((newobj.Mass() > newobj.typ->mass_min) && (newobj.Mass() < newobj.typ->mass_max)) {
			mass_irregularity.clear();
		}
		else {
			mass_irregularity = IRREGULARITY;
			comment += "\n" IRREGULARITY " mass out of range";
		}
		// irregularity: RADIUS
		if(newobj.Radius() > tlength(0) )
			radius_irregularity.clear();
		else if (newobj.Radius() == tlength(0)) {
			radius_irregularity = IRREGULARITY;
			comment += "\n" IRREGULARITY " radius is zero";
		}
		else {
			radius_irregularity = IRREGULARITY;
			comment += "\n" IRREGULARITY " radius is below zero";
		}
		// irregularity: COLOR
		if (newobj.colour.CheckRange(newobj.typ->color_min, newobj.typ->color_max) == true)
			color_irregularity.clear();
		else {
			color_irregularity = IRREGULARITY;
			comment += "\n" IRREGULARITY " color out of range";
		}
		// irregularity: TOUCH
		touch_irregularity.clear();
		for(unsigned int i=0; i!=o.size(); i++){
			if (newobj.touch(o[i])){
				if(o[i] == obj)
					continue;
				touch_irregularity = IRREGULARITY;
				comment += "\n" IRREGULARITY " the object would overlap the '";
				comment += o[i].name;
				comment += "' object";
				break;
			}
		}
		
		// Print the actual state and scan the desire of the user
		tposition p (newobj.Pos());
		tvelocity v (newobj.Vel());
		stringstream ss;
		ss << "CREATE A NEW OBJECT\n\n%r-1) name:         "
			<< newobj.name << "   " << name_irregularity
			<< "\n%r-2) type:         " << newobj.typ->name
			<< "\n&ti7" << newobj.typ->description
			<< "&t0\n%r-3) color:        red: " << newobj.colour.red
			<< "   " << color_irregularity << "&ti7\ngreen: "
			<< newobj.colour.green << "\nblue: " << newobj.colour.blue
			<< "&t0\n%r-4) mass:         " << newobj.Mass().value() << "   "
			<< mass_irregularity << "\n%r-5) radius:       "
			<< newobj.Radius().value() << "   " << radius_irregularity
			<< "\n%r-6) coordinates:  x: "
			<< p.x().value() << "   " << touch_irregularity << "&ti7\ny: " << p.y().value() 
			<< "\nz: " << p.z().value()	<< "&t0\n%r-7) velocity:     x: " << v.x().value()
			<< "&ti7\ny: " << v.y().value() << "\nz: " << v.z().value()
			<< "&t0\n%r-8) LOAD  the object from a file\n%r-9) SAVE  "
			<< "this object to a file\n%r-10) DONE\n11) EXIT without saving\n\n"
			<< comment;
			
		OPS(set, ss.str());
		input = in_i();
		
		// reset previous comment
		comment.clear();
	
		switch(input) {
			// Name
			case 1:
				OPS (set, "INITIALIZE A NEW OBJECT\n\nInsert the name of the new object:");
				in_s(newobj.name);
				// apply the new name
				comment += "\nNew name assigned succefully!";
				break;
		// Type
			case 2:
				newobj.typ = &stype->Browse(set, "Choose a new type for your new object");
				comment += "\nNew type assigned succefully!";
				break;
		// Color
			case 3:
				newobj.colour.Scan(set, newobj.typ->color_min, newobj.typ->color_max);
				comment += "\nNew color assigned succefully!";
				break;
		// Mass
			case 4:
				ss.clear();
				ss.str("");
				ss << "Create A NEW OBJECT\n\nInsert the mass of the new object: (Kg)\n&tdThe mass's legal values are between " << newobj.typ->mass_min << " and " << newobj.typ->mass_max;
				OPS (set, ss.str());
				newobj.SetMass(in_ld());
				comment += "\nNew mass assigned succefully!";
				
				// TODO: user should customize tolerance values, or they should somehow defined by type
				// tolerance for now is just related to mass
				newobj.SetFrcSingleTolerance((newobj.Mass()/600)*tacceleration_scalar(1));
				newobj.SetFrcSumTolerance((newobj.Mass()/400)*tacceleration_scalar(1));
				break;
		// Radius
			case 5:
				OPS (set, "Create A NEW OBJECT\n\nInsert the radius of the new object: (m)");
				newobj.SetRadius(in_ld());
				comment += "\nNew radius assigned succefully!";
				break;
		// Coordiates
			case 6: {
					vec3<long double> p = newobj.Pos().value();
					OPS (set, "Create A NEW OBJECT\n\nInsert the position in the x axis of the new object: (m)\n&t2Now is " + to_string(p.x) + " m");
					p.x = in_ld();
					OPS (set, "Create A NEW OBJECT\n\nInsert the position in the y axis of the new object: (m)\n&t2Now is " + to_string(p.y) + " m");
					p.y = in_ld();
					OPS (set, "Create A NEW OBJECT\n\nInsert the position in the z axis of the new object: (m)\n&t2Now is " + to_string(p.z) + " m");
					p.z = in_ld();
					newobj.SetPos(p);
					comment += "\nNew coordinates assigned succefully!";
					break;
				}
		// Velocity
			case 7: {
					vec3<long double> v = newobj.Vel().value();
					OPS (set, "Create A NEW OBJECT\n\nInsert the velocity in the x axis of the new object: (m/s)\n&t2Now is "  + to_string(v.x) + " m/s");
					v.x = in_ld();
					OPS (set, "Create A NEW OBJECT\n\nInsert the velocity in the y axis of the new object: (m/s)\n&t2Now is " + to_string(v.y) + " m/s");
					v.y = in_ld();
					OPS (set, "Create A NEW OBJECT\n\nInsert the velocity in the z axis of the new object: (m/s)\n&t2Now is " + to_string(v.z) + " m/s");
					v.z = in_ld();
					newobj.SetVel(v);
					comment += "\nNew velocity assigned succefully!";
					break;
				}
		// LOAD
			case 8: {
					// load the object in a temporany variable
					signal result;
					string _name;
					
					stringstream ss;
					
					ss << "Create A NEW OBJECT\n\nInsert the name of the object you want to load:\n";
					ss << "%r-";
					for(auto& p: experimental::filesystem::directory_iterator(OBJECT_PATH))
						ss << p << '\n';
					ss << "%r-type 'n' to cancel";

					string sout = ss.str();
					// remove stuff not nice to see
					// search for the '"'s
					while(1) {
						auto pos = sout.find("\"");
						if(pos == string::npos)
							break;
						sout.replace(pos, 1, "");
					}
					// search for ".obj"
					while(1) {
						auto pos = sout.find(".object");
						if(pos == string::npos)
							break;
						sout.replace(pos, 7, "");
					}
					// search for "Objects/"
					while(1) {
						auto pos = sout.find("Objects/");
						if(pos == string::npos)
							break;
						sout.replace(pos, 8, "");
					}
					
					// out and in
					OPS(set, sout);
					in_s(_name);
					
					if(!_name.compare("n"))
						break;
					
					object temp(*stype, _name, result);
					if (result == signal::good) {
						// move kinematic stats
						newobj.MoveKinematic(temp);
						// move the temp(the new) object in this
						newobj = temp;
						comment += "\nNew object loaded succefully!";
					}
					// negative comment if fail
					else {
						comment += "\nCan't load the object! ";
						if (result == signal::file_err)
							comment += "No object whit that name found!";
						else if (result == signal::corrupted)
							comment += "File corrupted or outdated!";
					}
				}
				break;
		// SAVE
			case 9:
				newobj.Save(set);
				comment += "\nNew object saved succefully!";
				break;
		// DONE
			case 10:
				if ( mass_irregularity.compare(IRREGULARITY) &&
					color_irregularity.compare(IRREGULARITY) &&
					name_irregularity.compare(IRREGULARITY)  &&
					radius_irregularity.compare(IRREGULARITY) &&
					touch_irregularity.compare(IRREGULARITY) ){

					obj = newobj;
					
					return;
				}
				else
					comment += "\nCannot exit! Fix irregularities first";
				break;
		// EXIT WHITOUT SAVE
			case 11:
				return;
		}
	}
	
}

/***
 * 	The function initialize a new object and ask information about it
 * 
 */
void system_c::NewObj (const setting& set) {
	
	// new empty object
	object newobj(*stype);
	
	// edit
	EditObj(set, newobj);
	
	// if the object is not empty anymore push it
	if(newobj != object(*stype))
		o.push_back(newobj);
	
}

/***
 * This function Delete an object from the system
 */
void system_c::RemoveObj (object& rem) {
	rem = o.back();
	o.pop_back();
}

/***
 * Initialize a new system
 */
system_c::system_c (const setting& set, typeSTR& s) {

	// set the type struct pointer
	stype = &s;
	// set the constant of gravitation
	G = tguc(6.67408e-11);
	
	// Ask for the name of the new system
	OPS (set, "NEW SYSTEM INITIALIZATION\n\nname of the system:");
	// the name
	in_s(name);
	
	precision = time_raw(0.1);
}

/***
 * Save a system in the directory Systems/
 */
void system_c::Save(const setting& set){
	
	// the path where the system must be saved, an input variable and the destination file pointer
	std::string path;
	std::string input;
	
	// Write the path
	path = SYSTEM_PATH;
	path += name;
	path += ".sys";
	
	// open the file to read
	ifstream idest(path);
	if(idest) {
		OPS(set, "While saving: The system you want to save alredy exist.\nDo you want to delete the previous system and save this? [n = no | something else = y]");
		in_s(input);
		if(!input.compare("n"))
			return;
	}
	// open the file to write
	ofstream odest(path);
	if(!odest) {
		OPS(set, "FAILED TO SAVE THE SYSTEM!\n\nCan't access file to write on\nPress something to continue");
		in_s();
	}
	
	// Write information about the system
	Write(odest);
	
	OPS(set, "SYSTEM SAVED WHIT SUCCESS!\n\nPress something to continue");
	in_s();
}

void system_c::Write(ofstream& dest){
	dest << precision.time() << "\n";
	dest << o.size() << "\n";
	dest << G.value() << "\n";
	dest << stime.Year() << "\n";
	dest << stime.Day() << "\n";
	dest << stime.Hour() << "\n";
	dest << stime.Minute() << "\n";
	dest << stime.Second() << "\n";
	// write the system objects
	for(unsigned int i=0; i!=o.size(); i++)
		o[i].WriteComplete(dest);
}

signal system_c::Read(ifstream& file, string& _name) {
	
	name = _name;
	
	precision = time_raw(in_ff(file));
	
	unsigned int num_obj = in_fi(file);
	
	G = tguc(in_ff(file));
	
	int y, d, h, m;
	float s;
	y = in_fi(file);
	d = in_fi(file);
	h = in_fi(file);
	m = in_fi(file);
	s = in_ff(file);
	stime = time_sim(y, d, h, m, s);
	
	// alloc memory
	o.resize(num_obj);
	// fscanf for objects datas
	for(unsigned int i=0; i!=o.size(); i++) {
		if(o[i].ReadComplete(file, *stype) == signal::corrupted) {
			debug_Printf("(!) system::Read: the system to load seems corrupted or outdated! While loading the object (read below):");
			debug_Printf(o[i].name);
			return signal::corrupted;
		}
	}
	return signal::good;
}

/**
 * Load a system from a file
 */

system_c::system_c (const setting& set, typeSTR& str, signal& result) {

	string _name;	// the name of the system
	string path;	// the system file (path)
	
	stringstream output;

	output << "LOAD SYSTEM\n\nWhat is the name of the system you want to load?\n%r-";
	for(auto& p: experimental::filesystem::directory_iterator(SYSTEM_PATH))
        output << p << '\n';
	output << "%r-type 'n' to cancel";
	
	string sout = output.str();
	// remove stuff not nice to see
	// search for the '"'s
	while(1) {
		auto pos = sout.find("\"");
		if(pos == string::npos)
			break;
		sout.replace(pos, 1, "");
	}
	// search for ".sys"
	while(1) {
		auto pos = sout.find(".sys");
		if(pos == string::npos)
			break;
		sout.replace(pos, 4, "");
	}
	// search for "Systems/"
	while(1) {
		auto pos = sout.find("Systems/");
		if(pos == string::npos)
			break;
		sout.replace(pos, 8, "");
	}
	// ask which system
	OPS(set, sout);
	in_s(_name);
	if(!_name.compare("n")){
		result = signal::aborted;
		return;
	}
	// write the path
	path = SYSTEM_PATH;
	path += _name;
	path += ".sys";
	// open the file
	ifstream sysf(path);
	if (!sysf) {
		result = signal::file_err;
		return;
	}
	
	// set the type structure
	stype = &str;
	// Read the system
	result = Read(sysf, _name);
}

/***
 * the function search object search a object in a system whit a name and return his pointer or nullptr if there isn't any object whit that name
 */
object *system_c::SearchObj(const string& tofind) {
	for (unsigned int i=0; i != o.size(); i++)
		if(!o[i].name.compare(tofind))
			return &o[i];
	return nullptr;
}

#define COLOR_PREDOMINANCE 1.35

/****
 * This function will put in the system all the physic laws implemented
 * as long as the time given
 */
void system_c::Physic (time_sim& dest) {
	
	while (dest.Compare (stime) == comparison::major) {
		
		// Apply gravity
		Physic_Gravity();
		// TODO: AI should think only sometimes
		// AI will make their move
		Hunter_AI();
		// Run the simulation
		for (unsigned int i=0; i!=o.size(); i++)
			o[i].Sim(precision);
		// IMPACTS
		physic_Impacts();
		// TIME
		stime.Add(precision.time());
	}
}

/***
 * GRAVITY
 * 
 * WARNING: this function is heavily optimized! readability compromised! Refer to the comments
 */
void system_c::Physic_Gravity() {
	
	// the force
	tforce f;
	
	for(unsigned int i=0; i < o.size(); i++) {
		for (unsigned int l=i+1; l < o.size(); l++) {
			
			// the force is f = G * m1 * m2 / dist^2	
			f = G.newton(o[i].Mass(), o[l].Mass(), o[i].Distance(o[l]));
			
			o[i].AddForce(f);
			o[l].AddForce(-f);
		}
	}
}

/***
 * HunterIA
 * The monster search for human buildt things and destroy them
 * 
 * 		THERE IS MUCH WORK TO DO HERE TO MAKE MONSTERS SMARTER
 */
void system_c::Hunter_AI() {
	// Search hunters
	for(unsigned int i=0; i!=o.size(); i++)
		if(o[i].typ->hunter == true)
			o[i].AI_Hunter(*this);
}

/***
 * Impacts between object
 * HERE THERE IS MUCH MUCH MUCH WORK TO DO IT MORE REALISTIC AND ACCURATE
 * for example:
 * 	- elastics hits (WIP)
 * 	- hit that take time
 * 	- partial hit
 * 	- creation of moon, asteroids, debris and other objects from hits
 * 	- special hit mechanics depending on the types of the objects (WIP)
 */
void system_c::physic_Impacts() {
	
	for(unsigned int i=0; i < o.size(); i++) {
		for (unsigned int l=0; l < o.size(); l++) {
			// if are the same object continue
			if(l == i)
				continue;
			// if doesn't hit continue
			if (!o[i].touch(o[l]))
				continue;
			
			// Call the appropriate impact simulator
			// If is an hunter that hunts an hunted (if the first is an hunter and the second an hunted or viceversa)
			// HUNTING IMPACT
			if((o[i].typ->hunter == true) && (o[l].typ->hunted == true))
				o[i].Impact_Hunting(o[l], precision);
			else if((o[i].typ->hunted == true) && (o[l].typ->hunter == true))
				o[l].Impact_Hunting(o[i], precision);
		
			// TODO: refine this part of code!
			// ELASTIC/ANAELASTIC IMPACT
			if ((o[i].typ->merge == false) && (o[l].typ->merge == false)){
				// pure elastic impact
				// THEY ARE STILL UNSTABLE!!! USING ANAELASTIC INSTEAD
				//o[i].Impact_Elastic(o[l], precision);
				o[i].Impact_Anaelastic(o[l]);
				RemoveObj(o[l]);
			}
			else {
				o[i].Impact_Anaelastic(o[l]);
				RemoveObj(o[l]);
			}
		}
	}
}
