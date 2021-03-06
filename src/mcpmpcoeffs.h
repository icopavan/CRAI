//
// MuDiSP2
// Multirate Digital Signal Processor 2
// By Luca Simone Ronga (C) Apr 1999
// 
//

#ifndef __MUDISP2_MCPMPCOEFFS_H 
#define __MUDISP2_MCPMPCOEFFS_H

////////
////////   Here put your personal includes
////////

#include "mudisp.h"
#include "gmccdma.h"
#include "gsl/gsl_vector.h"
#include "gsl/gsl_matrix.h"
#include "gsl/gsl_rng.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <time.h>


#define EARTH_RADIUS 6371.009 // km
#define M_PI_OVER_180 M_PI/180.0


// vx (km/h), vy (km/h)
// deltalon deltalat (deg/s)
// deltalon = vx / 3.6 / 111111 Cos[lat]
// deltalat = vy / 3.6 / 111111 


// Accuracy versus decimal places at the equator
// decimal
// places	degrees		distance
// 0		1.0    		111 km
// 1		0.1		11.1 km
// 2		0.01		1.11 km
// 3		0.001		111 m
// 4		0.0001		11.1 m
// 5		0.00001		1.11 m
// 6		0.000001	0.111 m
// 7		0.0000001	1.11 cm
// 8		0.00000001	1.11 mm
//

//
// Quick and dirty
//
// 111.111 km = 1 deg LAT
// 111.111 cos(LAT) km = 1 deg LON
//


enum geo_init_type {uniform, center, belt};

class MCPMPCoeffs : public Block {
 private:

  ////////   Parameters instances

  IntParam N,L,M;
  FloatParam PTau;
  FloatParam Ricean;
  FloatParam SOSa, SOSP, SOSsigma;
  IntParam SOSM;
  StringParam GeoFn;

  ////////   Local Attributes

  double gain, gainrice, sosc;
  gsl_matrix_complex *ch;
  gsl_matrix *pathLoss, *sostheta;
  gsl_vector *sosfrn, *sosfxn,*sosfyn;
  gsl_rng * ran; 
  unsigned int _M, runCount, SOSN;

  // Geo Model
  gsl_vector_complex *geoPositions, *geoVelocities; // node coordinates tx + rx (2xM)
  unsigned int geoRenderEnabled;
  // Geo Rendering
  ostringstream kmlhead,kmlobject,kmlheadend;
  ofstream ofs;
  time_t tlast, tnow;

 public:

////////   InPorts and OutPorts

  OutPort < gsl_matrix_complex > mout1;  // complex mat (MxM)xN



  MCPMPCoeffs():Block("MCPMPCoeffs")
    ////////  parameters initializazion
    ,M("NumUsers",2,"number of users")
    ,N("Carriers",16,"number of carriers")
    ,L("ChanTaps",2,"number of channel taps")
    ,PTau("PowerTau",1.0,"decay factor of delay spread")
    ,Ricean("RiceFact",0.9,"Ricean factor of channel")
    ,GeoFn("GeoFName","/home/ronga/public_html/track.kml","kml for geo-rendering (none to disable)")
	,SOSa("SOSa",0.1,"SOS Spatial channel a parameter")
	,SOSP("SOSP",0.8,"SOS Spatial channel P parameter")
	,SOSsigma("SOSsigma",3,"SOS Spatial channel sigma parameter")
	,SOSM("SOSM",5,"SOS Spatial channel M parameter")
    {

      //////// local parameter registration
      AddParameter(N);
      AddParameter(M);
      AddParameter(L);
      AddParameter(PTau);
      AddParameter(Ricean);
      AddParameter(GeoFn);
      AddParameter(SOSa);
      AddParameter(SOSP);
      AddParameter(SOSsigma);
      AddParameter(SOSM);
    }

  ~MCPMPCoeffs() {
  }

  void Setup();
  void Run();
  void Finish();

 protected:

  void GeoInit(geo_init_type t=uniform);
  void GeoUpdate(double seconds=1.0);
  void GeoRender();
  double GeoDistance(unsigned int,unsigned int);
  void SpatialChannelUpdate();
  
  double latdeg(double lat); // lenghth in m of a degree of latitude at lat(itude)
  double londeg(double lat);

};

#endif /* __MUDISP_MYBLOCK_H  */

