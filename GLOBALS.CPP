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

BovineHerd bovineHerd;

int		numMonths;
double	probDeadRendered;
int		outputInterval;
bool	reportDetailedFoodData;
bool	treatNonAmbAsDead;
FunctInterStep<int, double> importBolus;
FunctTable<int, std::string> dataFileList;
FunctTable<int, std::string> animalImportList;

GenesisVisitor genesisVisitor;
BirthVisitor birthVisitor;
DeathVisitor deathVisitor;

AMInspector amInspector;
Materializer materializer;
Stunner stunner;
Splitter splitter;
PMInspector pmInspector;
FoodInspector foodInspector;
Renderer renderer;
Renderer_Dead renderer_Dead;
MBMTransporter mbmTransporter;
MBMTransporter_Dead mbmTransporter_Dead;
FeedProducer feedProducer;
FeedTransporter feedTransporter;
Feeder feeder;

BloodInfector bloodInfector;
ProteinInfector proteinInfector;
RandomInfector randomInfector;
SpontInfector spontInfector;

SRMInspector srmInspector;
SRMInspector_Dead srmInspector_Dead;

FunctTable<Bovine::Type, FunctTable<Bovine::Gender, FunctInterStep<int, double  > > > rateSlaughter;
