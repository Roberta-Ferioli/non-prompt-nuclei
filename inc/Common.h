#ifndef COMMON_H
#define COMMON_H

#include "ROOT/RDataFrame.hxx"

#include <iostream>
#include <map>
#include <string>
#include <vector>

#include <TList.h>
#include <TF1.h>
#include <TObject.h>
#include <TMath.h>
#include <TSystem.h>

using std::map;
using std::string;
using std::vector;

const char   kLetter[2] = {'M','A'}; // M -> Matter, A -> Anti-matter
const string kNames[2] = {"he3","antihe3"};
const string kLabels[2] = {"^{3}He","^{3}#bar{He}"};

// const string kMCproduction = "LHC24b2";
const string kMCproduction = "LHC23j6b";
const string kRecoPass = "apass4";
const string kPeriod = "LHC22";
const string kVariant = "giovanni";

const string kBaseOutputDir = "$NUCLEI_OUTPUT/" + kPeriod + "/" + kRecoPass + "/";
const string kBaseInputDir = "$NUCLEI_INPUT/";

const string kDataTreeFilename = kBaseInputDir + "data/" + kPeriod + "/" + kRecoPass + "/MergedAO2D.root";
const string kDataFilename = kBaseInputDir + "data/" + kPeriod + "/" + kRecoPass + "/DataHistos" + kVariant + ".root";
const string kDataFilenameHe4 = kBaseInputDir + "data/" + kPeriod + "/" + kRecoPass + "/DataHistosHe4" + kVariant + ".root";
const string kDataAnalysisResults = kBaseInputDir + "data/" + kPeriod + "/" + kRecoPass + "/AnalysisResults.root";
const string kMCAnalysisResults = kBaseInputDir + "MC/" + kMCproduction + "/AnalysisResults.root";
const string kMCtreeFilename = kBaseInputDir + "MC/" + kMCproduction + "/MergedAO2D.root";
const string kMCfilename = kBaseInputDir + "MC/" + kMCproduction + "/MChistos"  + kVariant + ".root";
const string kMCfilenameHe4 = kBaseInputDir + "MC/" + kMCproduction + "/MChistosHe4"  + kVariant + ".root";

const string kFilterListNames = "nuclei";

const string kBaseRecSelections = "fTPCnCls >= 110 && nITScls >= 5 && std::abs(fEta) < 0.9 && std::abs(fDCAxy) < 0.7 && pt > 0.8 && pt < 9.0";
const string kDefaultRecSelections = "fTPCnCls > 120 && nITScls >= 6 && std::abs(nsigmaDCAz) < 7 && std::abs(fDCAxy) < 0.2";

const string kSignalOutput = kBaseOutputDir + "signal" + kVariant + ".root";
const string kSystematicsOutput = kBaseOutputDir + "systematics" + kVariant + ".root";

// constexpr int    kNPtBins = 11;
// constexpr double  kPtBins[kNPtBins+1] = {1.0, 1.2, 1.4, 1.8, 2.2, 2.6, 3.2,4.0,4.8,5.8,7,9};

constexpr int    kNPtBins = 12;
constexpr double  kPtBins[kNPtBins+1] = {1.5, 1.75, 2.0, 2.25, 2.5, 2.75, 3.0, 3.25, 3.5, 3.75, 4.0, 4.5, 5.0};

const int    kCentLength = 1;
const int    kCentBinsArray[kCentLength][2] = {{2,2}};
const float  kCentPtLimits[kCentLength] = {7};
const float  kCentLabels[kCentLength][2] = {{0.,100.}};

const float  kTPCmaxPt = 7.0f;
const float  kTOFminPt = 1.f;
const float  kPtRange[2] = {1.4,7};

const bool   kUseBarlow{true};
const float  kAbsSyst[2] = {0.08,0.1f};

constexpr int kNTPCfunctions = 3;
const std::string kTPCfunctName[4]{"GausGaus", "ExpGaus", "ExpTailGaus", "LognormalLognormal"};


std::map<string,vector<float> > kCutNames {{"nsigmaDCAz", {6, 7, 8}},{"fTPCnCls", {110, 120, 130}},{"nITScls", {5, 6, 7}}, {"nsigmaTPC", {3, 4, 5}}};
size_t nTrials{kCutNames["fDCAz"].size() * kCutNames["fTPCnCls"].size() * kCutNames["nITScls"].size()};

double bb(double bg, double kp1, double kp2, double kp3, double kp4, double kp5)
{
  double beta = bg / std::sqrt(1. + bg * bg);
  double aa = std::pow(beta, kp4);
  double bb = std::pow(1. / bg, kp5);
  bb = std::log(kp3 + bb);
  return (kp2 - aa - bb) * kp1 / aa;
}

float bbHe3(float mom) { return bb(mom / 2.80839, -321.34, 0.6539, 1.591, 0.8225, 2.363); }
float nsigmaHe3(float mom, float sig) { return (sig / bbHe3(mom * 2) - 1. + 2.20376e-02) / 0.055; }

float bbH3(float mom) { return bb(mom / 2.80892f, -136.71, 0.441, 0.2269, 1.347, 0.8035); }
float nsigmaH3(float mom, float sig) { return (sig / bbH3(mom) - 1.) / 0.07; }

float bbHe4(float mom) { return bb(mom / 3.72738f, -321.34, 0.6539, 1.591, 0.8225, 2.363); }
float nsigmaHe4(float mom, float sig) { return (sig / bbHe4(mom * 2) - 1.) / 0.07; }

float DCAxyCut(float pt, float nsigma)
{
  float invPt = 1.f / pt;
  return (7.62783e-04 + 4.59326e-03 * invPt + 6.89163e-03 * invPt * invPt) * nsigma;
}
float nSigmaDCAxy(double pt, float dcaxy) {
  return dcaxy / DCAxyCut(pt, 1);
}

float DCAzCut(float pt, float nsigma)
{
  float invPt = 1.f / pt;
  return (5.00000e-04 + 8.73690e-03 * invPt + 9.62329e-04 * invPt * invPt) * nsigma;
}

float averageClusterSize(uint32_t itsClusterSizes)
{
  float sum = 0;
  int nclusters = 0;
  int max = 0;
  for (int layer = 0; layer < 7; layer++) {
    int clsize = (itsClusterSizes >> (layer * 4)) & 0xf;
    if (clsize > 0) {
      nclusters++;
      sum += clsize;
      if (clsize > max) {
        max = clsize;
      }
    }
  }
  if (nclusters == 0) {
    return 0;
  }
  // truncated mean
  return (sum - max) / (nclusters - 1);
};

float nSigmaDCAz(double pt, float dcaz) {
  return dcaz / DCAzCut(pt, 1);
}

#endif