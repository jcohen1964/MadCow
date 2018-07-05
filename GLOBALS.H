#ifndef  GLOBALS_H
#define  GLOBALS_H
#include "bovineHerd.h"
#include "genesisVisitor.h"
#include "birthVisitor.h"
#include "deathVisitor.h"
#include "amInspector.h"
#include "stunner.h"
#include "splitter.h"
#include "pmInspector.h"
#include "foodInspector.h"
#include "renderer.h"
#include "MBMTransporter.h"
#include "feedProducer.h"
#include "feedTransporter.h"
#include "feeder.h"
#include "bloodInfector.h"
#include "proteinInfector.h"
#include "srmInspector.h"
#include "FunctTable.h"
#include "FunctInterStep.h"
#include "materialize.h"
#include "randomInfector.h"
#include "spontInfector.h"

#include "MBMTransporter_Dead.h"
#include "renderer_Dead.h"
#include "srmInspector_Dead.h"

#include <string>

extern BovineHerd bovineHerd;

extern int    numMonths;
extern int	  outputInterval;
extern bool	  reportDetailedFoodData;
extern bool	  treatNonAmbAsDead;
extern double probDeadRendered;
extern FunctInterStep<int, double> importBolus;
extern FunctTable<int, std::string> dataFileList;
extern FunctTable<int, std::string> animalImportList;

extern GenesisVisitor genesisVisitor;
extern BirthVisitor birthVisitor;
extern DeathVisitor deathVisitor;

extern AMInspector amInspector;
extern Materializer materializer;
extern Stunner stunner;
extern Splitter splitter;
extern PMInspector pmInspector;
extern FoodInspector foodInspector;
extern Renderer renderer;
extern MBMTransporter mbmTransporter;
extern FeedProducer feedProducer;
extern FeedTransporter feedTransporter;
extern Feeder feeder;

extern BloodInfector bloodInfector;
extern ProteinInfector proteinInfector;
extern RandomInfector randomInfector;
extern SpontInfector spontInfector;

extern SRMInspector srmInspector;

extern MBMTransporter_Dead mbmTransporter_Dead;
extern Renderer_Dead renderer_Dead;
extern SRMInspector_Dead srmInspector_Dead;


extern FunctTable<Bovine::Type, FunctTable<Bovine::Gender, FunctInterStep<int, double  > > > rateSlaughter;

#endif