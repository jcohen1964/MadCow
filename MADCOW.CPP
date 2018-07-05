#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "organ.h"
#include "mealGroup.h"
#include "feedGroup.h"
#include "deathVisitor.h"
#include "totalNumVisitor.h"
#include "totalInfectedVisitor.h"
#include "totalClinicalVisitor.h"
#include "reporter.h"
#include "globals.h"
#include "random.h"
#include "utilities.h"
#include <ctime>
#include "bovine.h"
#include "SickBovine.h"

#include <time.h>                      // define time()
#include "randomc.h"                   // define classes for random number generators
// #include "userintf.cpp"                // define system specific user interface

// define which random number generator to base random library on:
// #include "mersenne.cpp"                // members of class TRandomMersenne
#define RANDOM_GENERATOR TRandomMersenne

#include "stocc.h"                     // define random library classes
// #include "stoc1.cpp"   

using KHD_UTIL::operator<<;

using namespace std;

void readFiles(istream&);
void readAnimalImports(istream&);
void readData(istream&);
void usageMsg();
void cleanMsg();

/////////////////////////////////////////////////////////////////////////////////////////////////
// Reads in single user-provided argument (name of file listing all the input parameter files. //
// Initializes the herd.  Loops through all months of the simulation.  For each month, main    //
// checks if new parameter values must be read in, processes animals that die on the farm, and //
// processes animals sent to slaughter.                                                        //
/////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argCount, char* argValue[]) {

	///////////////////////////////////////////////////////////////////////////////////
	//Get user input                                                                 //
	///////////////////////////////////////////////////////////////////////////////////
	if(argCount != 2) {					//If user provided wrong number of input args
		cerr << "Wrong number of arguments" << endl; usageMsg(); exit(0);}

	ifstream inputFiles(argValue[1]);	// file containing list of input files
	if(!inputFiles) {cerr << "Cannot open input file list" << endl; usageMsg(); exit(0);}

	readFiles(inputFiles); 				//Read in file names listed in file inputFiles
	inputFiles.close();					//Close inputFiles

	time_t theTime;						//Time simulation execution
	time(&theTime);

	int32 seed = time(0);
	StochasticLib1 sto(seed);

	//////////////////////////////////////////////////////////////////////////////////
	//Initialize the BovineHerd                                                     //
	//////////////////////////////////////////////////////////////////////////////////
	bovineHerd.accept(genesisVisitor);	//Populate the herd
	BovineHerd::iterator iterator;		//iterator over bovineGroups in bovineHerd

	TotalNumVisitor totalnum;			//Object stores total number of animals in herd
	TotalInfectedVisitor totalsick;     //Object stores total number of sick animals in herd
	TotalClinicalVisitor totalclinical;	//Object stores total number of clinical animals in herd

	MealGroup mealGroup;			    //Object stores list of meal packets (MBM and BLOOD)
	FeedGroup feedGroup;                //Object stores list of feed packets (FEED, BLOOD)

	Reporter& theReporter = *Reporter::instance(); //Alias the reporter

	//////////////////////////////////////////////////////////////////////////////////
	//Loop through months to be simulated                                           //
	//////////////////////////////////////////////////////////////////////////////////
	for(int month = 0; month <= numMonths; ++month) {	//Conduct simulation for numMonths months

		//////////////////////////////////////////////////////////////////////////////
		//Revise parameter values if necessary                                      //
		//////////////////////////////////////////////////////////////////////////////
		if((dataFileList(month) != "NOCHANGE") &&			//If the parameters changed this month
			(dataFileList(month) != "")) {					
			ifstream fileList(dataFileList(month).c_str());	//Open file with new parameters
			if(!fileList) {cerr << "Cannot open input file "//If cannot open, provide error msg and exit
				<< dataFileList(month) << endl; exit(0);};
			readFiles(fileList);							//Read in the files listed in file fileList
			fileList.close();								//Close the file
			cout << "read files" << endl;
		}

		//////////////////////////////////////////////////////////////////////////////
		//Check for animal imports                                                  //
		//////////////////////////////////////////////////////////////////////////////
		if((animalImportList(month) != "NOIMPORTS") &&			//If the imports changed this month
			(animalImportList(month) != "")) {					
			ifstream fileList(animalImportList(month).c_str());	//Open file with new import list
			if(!fileList) {cerr << "Cannot open animal imports "//If cannot open, provide error msg and exit
				<< animalImportList(month) << endl; exit(0);};
			readAnimalImports(fileList);						//Read in the animal imports specified
			fileList.close();									//Close the file
			cout << "read animal import files" << endl;
		}
		
		//////////////////////////////////////////////////////////////////////////////
		//Simulate births, deaths, aging of herd                                    //
		//////////////////////////////////////////////////////////////////////////////
		bovineHerd.accept(birthVisitor);    //New births in herd
		bovineHerd.accept(deathVisitor);    //Natural deaths in herd
		bovineHerd.live();                  //Age cattle 1 month

		//////////////////////////////////////////////////////////////////////////////
		//Update stock counts for reporter                                          //
		//////////////////////////////////////////////////////////////////////////////
		totalnum.reset();                   //Reset the total herd size counter
		totalsick.reset();                  //Reset the total sick counter
		totalclinical.reset();              //Reset the total number clinical counter
		bovineHerd.accept(totalnum);        //Count total number of animals in herd
		bovineHerd.accept(totalsick);       //Count total number sick in herd
		bovineHerd.accept(totalclinical);   //Count total number clinical in herd
		Reporter::instance()->updateStocks(totalnum);		// Send totalnum to reporter
		Reporter::instance()->updateStocks(totalsick);		// Send totalsick to reporter
		Reporter::instance()->updateStocks(totalclinical);	// Send totalclinical to reporter

	
		////////////////////////////////////////////////////////////////////////////////////////////////
		// Process dead on farm bovines																  //
		// Animal is first turned into a carcass (vector of organs)                                   //
		// and then into meal packets.  The mbmTransporter determines the destination of each meal    //
		// packet based on its type (P or NP) and the type of renderer that produced it (P, NP, or    //
		// mixed.  The meal packets are processed by the feed producer, which returns a list of feed  //
		// packets.  The feedTransporter determines the destination of each feed pkt based on its     //
		// type (P or NP) and the type of feedProducer (P, NP, or mixed) that produced it.  The       //
		// feeder marks any P feed packets destined for misfeeding by converting them to NP packets.  //
		// Note, the bloodInfector and proteinInfector administor the appropriate pkts as feed.       //
		// Finally, at this time, the simulation automatically assumes that animals that die on the   //
		// farm can be used in feed (okForFeed = true) but cannot be used in food (okForFood = false).//
		// A new processor should be created to handle this issue and to specify the on-farm rendering//
		// rate.  Note also, these flags are not actually referenced.  The animal is sent to rendering//
		// and not to the slaughterhouse.
		////////////////////////////////////////////////////////////////////////////////////////////////
		std::vector<Organ> carcass;					// Holds ID50 counts for dead animals
		DeathVisitor::deadBovineCont::iterator it;	//iterator over list of dead bovines
		DeathVisitor::deadBovineCont::iterator end	//end of dead bovine list marker
			= deathVisitor.deadBovines.end();

		for(it = deathVisitor.deadBovines.begin(); it != end; ++it) { //Loop through dead bovines

			if(Random::instance()->nextFloat() <= probDeadRendered) {			//If animal is to be rendered

				Reporter::instance()->reportDeath(Reporter::ONFARM_RENDER); // notify reporter: dead on farm
				bovineHerd(it->getType(), it->getGender(),					// remove bovine from group
					it->getAge()).removeBovine(*it);
				it->okForFeed = true;										// Animal can be used in feed;
				it->okForFood = false;										// Animal cannot be used in food;
				carcass = materializer.process(*it);						// convert to carcass
				Reporter::instance()->reportid50Track(Reporter::ID50_DIEONFARM, 
					carcass);												// Notify reporter of id50s
				srmInspector_Dead.process(carcass);								//Send carcass to SRM inspector
				mealGroup = renderer_Dead.process(carcass);						// Convert into meal pkt list
				mbmTransporter_Dead.transport(mealGroup);						// Determine destination of pkts
				feedGroup = feedProducer.process(mealGroup);				// Convert into feed pkt list
				feedTransporter.transport(feedGroup);						// Determine destination of pkts
				feeder.markForMisfeed(feedGroup);							// Mark P pkts for misfeeding
				bloodInfector.distributeInfectivity(feedGroup);				// Queue id50s for admin to cattle.
				proteinInfector.distributeInfectivity(feedGroup);			// Queue id50s for admin to cattle

			//Else -- If animal is to be buried on farm
			} else { 

				Reporter::instance()->reportDeath(Reporter::ONFARM_BURY);	// Notify reporter
				bovineHerd(it->getType(), it->getGender(),					// Remove bovine from group
					it->getAge()).removeBovine(*it);
			}
		}
		deathVisitor.deadBovines.clear();		//Clear the list of dead bovines

		////////////////////////////////////////////////////////////////////////////////////
		// Process bovines to be slaughtered                                              //
		// Processing is the same as for dead on farm bovines except that slaughtered     //
		// animals are sent to splitter, PM inspector, SRM inspector, and foodInspector.  //
		////////////////////////////////////////////////////////////////////////////////////
		BovineHerd::iterator endherd = bovineHerd.end();		//Alias the last bovineGroup in the herd
		Bovine bovine;											//Points to a representative bovine

		for(iterator = bovineHerd.begin();						// Loop through bovine groups
			iterator != endherd; ++iterator) { 

			if(iterator->getNumSick() > 0) {					//If this group has at least 1 sick bovine

				bovine = iterator->getBovine();					//Get bovine template for group
				int population = iterator->getNumTotal();		//Get num bovines in group
				double fraction =								//Get the slaughter rate for this group
					rateSlaughter(bovine.getType())(bovine.getGender())(bovine.getAge());
				BovineGroup::SickBovineIterator 
					sickone,									//sickBovine iterator
					end;										//marks end of sickBovine list
				BovineGroup group(iterator->					//Create list of bovines to be slaughtered
					getBovinesSlaughtered(fraction));
				end = group.getSickEnd();						//End of slaughter list

				for(sickone = group.getSickBegin(); sickone != end; ++sickone) {//Loop thru slaughter list

					Reporter::instance()->
						reportDeath(Reporter::SLAUGHTER);			//Notify reporter
					carcass =	materializer.process(*sickone);		//Convert bovine to carcass
					Reporter::instance()->							//Notify reporter of id50s in carcass
							reportid50Track(Reporter::ID50_SLAUGHTER, carcass);

					amInspector.process(*sickone);					//Send animal to AM inspector

					if (sickone->isNonAmbulatory && treatNonAmbAsDead) {				// Nonambulatory handled like deads
																						
						if(Random::instance()->nextFloat() <= probDeadRendered) {		// If animal is to be rendered
							if (sickone->okForFeed) {									// If ok for feed
								stunner.process(carcass);								// Send carcass to stunner
								srmInspector_Dead.process(carcass);						// Send carcass to SRM inspector
								mealGroup = renderer_Dead.process(carcass);				// Convert into meal pkt list
								mbmTransporter_Dead.transport(mealGroup);				// Determine destination of pkts
								feedGroup = feedProducer.process(mealGroup);			// Convert into feed pkt list
								feedTransporter.transport(feedGroup);					// Determine destination of pkts
								feeder.markForMisfeed(feedGroup);						// Mark P pkts for misfeeding
								bloodInfector.distributeInfectivity(feedGroup);			// Queue id50s for admin to cattle.
								proteinInfector.distributeInfectivity(feedGroup);		// Queue id50s for admin to cattle
							} else {													// Animal not ok for feed
								amInspector.incinerate(carcass);
							}
						} else {														//Else -- If animal is to be buried on farm
							Reporter::instance()->reportDeath(Reporter::ONFARM_BURY);	// Notify reporter
						}
					}


					if (sickone->okForFood && sickone->okForFeed  && 
						(!sickone->isNonAmbulatory || !treatNonAmbAsDead)) {
																		//CASE 1 -- OK FOR FOOD AND OK FOR FEED
						stunner.process(carcass);						//Send carcass to stunner
						splitter.process(carcass);						//Send carcass to splitter
						pmInspector.process(carcass);					//Send carcass to PM inspector
						foodInspector.process(carcass);					//Identify organs for human food
						srmInspector.process(carcass);					//Send carcass to SRM inspector
						mealGroup = renderer.process(carcass);			//Convert into meal pkt list
						mbmTransporter.transport(mealGroup);			//Determine destination of pkts
						feedGroup =										//Convert into feed pkts
							feedProducer.process(mealGroup);
						feedTransporter.transport(feedGroup);			//Determine dest of feed pkts
						feeder.markForMisfeed(feedGroup);				//Mark P pkts for misfeeding
						bloodInfector.distributeInfectivity(feedGroup);	//Queue id50s for admin to cattle
						proteinInfector.distributeInfectivity(feedGroup);	//Queue id50s for admin to cattle
					}
					if (sickone->okForFeed && !sickone->okForFood && 
						(!sickone->isNonAmbulatory || !treatNonAmbAsDead))	{
																		//CASE 2 -- OK FOR FEED ONLY
						stunner.process(carcass);						//Send carcass to stunner
						srmInspector.process(carcass);					//Send carcass to SRM inspector
						mealGroup = renderer.process(carcass);			//Convert to meal pkt list
						mbmTransporter.transport(mealGroup);			//Determine destination of meal pkt
						feedGroup = feedProducer.process(mealGroup);	//Convert to feed pkts
						feedTransporter.transport(feedGroup);			//Determine destination of pkts
						feeder.markForMisfeed(feedGroup);				//Mark P pkts for misfeeding
						bloodInfector.distributeInfectivity(feedGroup);	//Queue id50s for admin to cattle
						proteinInfector.distributeInfectivity(feedGroup);//Queue id50s for admin to cattle
					}
					if (!sickone->okForFeed && !sickone->okForFood && 
						(!sickone->isNonAmbulatory || !treatNonAmbAsDead))	{
																		//CASE 3 -- NOT OK FOR ANYTHING
						amInspector.incinerate(carcass);
					}
					if (!sickone->okForFeed && sickone->okForFood && 
						(!sickone->isNonAmbulatory || !treatNonAmbAsDead))	{
																		//CASE 4 -- OK FOR FOOD BUT NOT FEED
						cerr << "Error AM inspector" << endl;
						cerr << "Animal ok for food but not feed" <<endl;//Case is impossible.
					}
				}

			} else {												//If no infected bovines in this group
				bovine = iterator->getBovine();						//Get age, gender, type
				int population = iterator->getNumTotal();			//Number bovines in group
				double fraction =									//Slaughter rate for this group
					rateSlaughter(bovine.getType())(bovine.getGender())(bovine.getAge());
				iterator->removeBovine(int(population*fraction));	//Remove slaughtered animals
			}
		}

		///////////////////////////////////////////////////////////////////////////////////
		//Distribute infectivity                                                         //
		///////////////////////////////////////////////////////////////////////////////////
		bovineHerd.accept(bloodInfector);					//Admin blood meal id50s to cattle
		bovineHerd.accept(proteinInfector);					//Admin feed id50s to cattle
		bovineHerd.accept(spontInfector);					//Carry out spontaneous infection
		if(importBolus(month) != 0.0) {						//If infectivity is imported this month
			randomInfector.addBolus(importBolus(month));	//Queue id50s for admin to cattle
			bovineHerd.accept(randomInfector);				//Admin imported id50s to cattle
		}

		////////////////////////////////////////////////////////////////////////
		//Output reporter statistics and reset reporter counters              //
		////////////////////////////////////////////////////////////////////////
		Reporter::instance()->monthlyOutput(month, outputInterval, numMonths);	//Monthly output
		Reporter::instance()->monthlyReset(month, outputInterval);				//Reset reporter inc. counters
	}
	time(&theTime);
	return 0;
  }

////////////////////////////////////////////////////////////////////////////////
// Read in contents of file                                                   //
////////////////////////////////////////////////////////////////////////////////
void readFiles(istream& stream) {
	string fileName;							//String to hold name of file to be read
	stream >> fileName;							//Get name of first file
	while(fileName != "") {						//Until next file name is empty
		ifstream currentFile(fileName.c_str());	//Get file with name fileName
		if(!currentFile) {						//If unable to open file, report error, exit
			cerr << "Cannot open input file " << fileName << endl; exit(0);};
		readData(currentFile);					//Read contents of file
		currentFile.close();					//Close file
		stream >> fileName;						//Get name of next file
	}
}

///////////////////////////////////////////////////////////////////////////////
// Process contents of input file                                            //
///////////////////////////////////////////////////////////////////////////////

void readData(istream& stream) {
	string token;
	stream >> token;
	char dump[256];

	while(!stream.eof()) {					//Until end of file is reached

	//Input elements with no sub-elements
	if     (token == "<numMonths>")        stream >> numMonths     >> token;
	else if(token == "<outputInterval>")   stream >> outputInterval >> token;
	else if(token == "<probDeadRendered>") stream >> probDeadRendered >> token;
	else if(token == "<reportDetailedFoodData>")
										  stream >> reportDetailedFoodData >> token;
	else if(token == "<treatNonAmbAsDead>")
										  stream >> treatNonAmbAsDead >> token;
	else if(token == "<importBolus>")     stream.getline(dump, 256).getline(dump, 256) >> importBolus  >> token;
	else if(token == "<dataFileList>")    stream.getline(dump, 256).getline(dump, 256) >> dataFileList  >> token;
	else if(token == "<animalImportList>")
										  stream.getline(dump, 256).getline(dump, 256) >> animalImportList  >> token;
	else if(token == "<rateSlaughter>")   stream.getline(dump, 256).getline(dump, 256) >> rateSlaughter >> token;

	//Input elements with sub-elements
	else if(token == "<genesisVisitor>")  stream >> genesisVisitor;
	else if(token == "<birthVisitor>")    stream >> birthVisitor;
	else if(token == "<deathVisitor>")    stream >> deathVisitor;
	else if(token == "<AMInspector>")     stream >> amInspector;
	else if(token == "<stunner>")         stream >> stunner;
	else if(token == "<splitter>")        stream >> splitter; 	
	else if(token == "<PMInspector>")     stream >> pmInspector;
	else if(token == "<foodInspector>")   stream >> foodInspector;
	else if(token == "<renderer>")        stream >> renderer;
	else if(token == "<MBMTransporter>")  stream >> mbmTransporter;
	else if(token == "<feedProducer>")    stream >> feedProducer;
	else if(token == "<feedTransporter>") stream >> feedTransporter;
	else if(token == "<feeder>")          stream >> feeder;
	else if(token == "<bloodInfector>")   stream >> bloodInfector;
	else if(token == "<proteinInfector>") stream >> proteinInfector;
	else if(token == "<SRMInspector>")    stream >> srmInspector;
	else if(token == "<materializer>")    stream >> materializer;
	else if(token == "<randomInfector>")  stream >> randomInfector;
	else if(token == "<spontInfector>")   stream >> spontInfector;
	else if(token == "<sickBovine>")      stream >> SickBovine();
	else if(token == "<MBMTransporter_Dead>")  stream >> mbmTransporter_Dead;
	else if(token == "<renderer_Dead>")        stream >> renderer_Dead;
	else if(token == "<SRMInspector_Dead>")    stream >> srmInspector_Dead;
	else if(token == "");
	else {cerr << "Token unknown : " << token << "\n"; exit(0);}
	stream >> token;
	}
}

////////////////////////////////////////////////////////////////////////////////
// Read in contents of file                                                   //
// Each line of the file is a separate request                                //
//     1. Bovine type                                                         //
//     2. Bovine gender                                                       //
//     3. Bovine age (int)                                                    //
//     4. Import rate (int)                                                   //
//     5. Distribution type (0 Constant 1 Poisson)                            //
//     5. Age at infection                                                    //
//     6. Age at slaughter (int, negative if no value specified)              //
////////////////////////////////////////////////////////////////////////////////

void readAnimalImports(istream& stream) {
	string token;
	char dump[256];
	Bovine myBovine;
	double importRate;
	int distType;
	int ageInfected;
	int ageSlaughter;
	stream.getline(dump,256);
	while (!stream.eof()) {
		stream >> myBovine;
		stream >> importRate;
		stream >> distType;
		stream >> ageInfected;
		stream >> ageSlaughter;
		bovineHerd.import(myBovine,importRate,distType,ageInfected,ageSlaughter);
	}
}

void usageMsg() {
	std::cout <<
		"---------------------------------------------------------------------------" << endl <<
		"MadCow Simulator                                                           " << endl <<
	    "Version: 2001.03                                                           " << endl <<
		"---------------------------------------------------------------------------" << endl <<
		"                                                                           " << endl <<
		"usage:                                                                     " << endl <<
		"madcow [working directory] [startup file list]                             " << endl <<
		"                                                                           " << endl <<
		"[working directory] : This is an operating specific directory path.  It    " << endl <<
		"                      shall become the root directory for the duration of  " << endl <<
		"                      program execution.  In other words, all file paths   " << endl <<
		"                      specified one the command line and in the data files " << endl <<
		"                      shall be assumed relative to this directory.         " << endl <<
		"                                                                           " << endl <<
		"                      RESTRICTIONS                                         " << endl <<
		"                      This program does not support spaces in path names.  " << endl <<
		"                                                                           " << endl <<
		"[startup file list] : This is a path relative to the working directory.  It" << endl <<
		"                      must be the path of a text file.  This text file must" << endl <<
		"                      contain a list of paths.  These paths must contain no" << endl <<
		"                      spaces (note restriction above) and must be separated" << endl <<
		"                      from each other by whitespace.  There must be at     " << endl <<
		"                      least one such path.  The paths must themselves      " << endl <<
		"                      be text files.  Their contents shall be concatenated " << endl <<
		"                      upon program startup.  The resulting character data  " << endl <<
		"                      shall be assumed to contain the simulation startup   " << endl <<
		"                      data.                                                " << endl <<
		"                                                                           " << endl <<
		"EXAMPLE (PC):                                                              " << endl <<
		"                                                                           " << endl <<
		"    madcow .\\Scenarios\\Baseline\\ fileList000.data                       " << endl <<
		"                                "
		                               << '\030'
									   << " -- note the space                       " << endl <<
		"                                                                           " << endl <<
		"    CONTENTS OF .\\Scenarios\\Baseline\\ :                                 " << endl <<
		"    OtherData\\                                                            " << endl <<
		"    fileList000.data                                                       " << endl <<
		"    dataFile000.data                                                       " << endl <<
		"    dataFile001.data                                                       " << endl <<
		"                                                                           " << endl <<
		"    CONTENTS OF .\\Scenarios\\Baseline\\OtherData\\ :                      " << endl <<
		"    dataFile002.data                                                       " << endl <<
		"    dataFile003.data                                                       " << endl <<
		"                                                                           " << endl <<
		"    CONTENTS OF .\\Scenarios\\Baseline\\fileList000.data :                 " << endl <<
		"    dataFile000.data                                                       " << endl <<
		"    dataFile001.data                                                       " << endl <<
		"    OtherData\\dataFile002.data                                            " << endl <<
		"    OtherData\\dataFile003.data                                            " << endl ; cleanMsg();
}

void cleanMsg() {
	int buffer = Random::instance()->nextFloat() * 12;
	switch(buffer) {
	    case 0 :  cout << "\104\157\145\163\40\141\156\171\157\156\145\40\150\141\166\145\40\141\40\144\157\156\165\164\77\40\120\154\145\141\163\145\77" << endl;
		break;
        case 1  : cout << "\103\150\151\157\156\157\40\151\163\40\144\141\40\155\141\156\41" << endl;
		break;
		case 2  : cout << "\111\40\150\157\160\145\40\156\157\142\157\144\171\40\150\141\163\40\141\40\142\162\141\151\156\40\144\162\151\160\56" << endl;
		break;
		case 3  : cout << "\123\171\154\166\151\141\40\141\163\153\145\144\40\155\145\40\164\157\40\141\144\144\40\164\150\151\163\40\146\145\141\164\165\162\145\56" << endl;
		break;
		default : ;
	}
}