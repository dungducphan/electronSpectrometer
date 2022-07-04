// Units used in this script
// - Length: m
// - Angle: rad
// - Energy: MeV
// - Time: ns

// Position of DRZ Cube
double x_center_0 = 0.0000;
double y_center_0 = 0.0000;
double z_center_0 = 1.7669;
double l_0 = 76.2E-3;
double h_0 = 76.2E-3;
double a_0 = -TMath::PiOver4();
double x_0 = x_center_0 - (l_0/2.)
std::vector<double> pset_0 = {x_0, y_0, z_0, l_0, h_0};

// Position of DRZ Plate 1
double x_center_1 = 0.0539;
double y_center_1 = 0.0000;
double z_center_1 = 0.6608;
double l_1 = 228.6E-3;
double h_1 63.5E-3;
double a_1 = TMath::PiOver4();
std::vector<double> pset_1 = {x_1, y_1, z_1, l_1, h_1};

// Position of IP
double x_center_2 = 0.1000;
double y_center_2 = 0.0000;
double z_center_2 = 0.1460;
double l_2 = 350E-3;
double h_2 = 150E-3;
double a_2 = TMath::PiOver2();
std::vector<double> pset_2 = {x_2, y_2, z_2, l_2, h_2};

// Position of DRZ Plate 2
double x_center_3 =  0.0539;
double y_center_3 =  0.0000;
double z_center_3 = -0.3522;
double l_3 = 228.6E-3;
double h_3 = 63.5E-3;
double a_3 TMath::PiOver4();
std::vector<double> pset_3 = {x_3, y_3, z_3, l_3, h_3};

// Selection
std::vector<std::vector<double>> psets = {pset_0, pset_1, pset_2, pset_3};
std::vector<std::string> pnames = {"DRZCube", "DRZPlate1", "ImagePlate", "DRZPlate2"};

// Resolution and saturation
double resolution = 1E-4;
double saturationValue = 2E6;

// x-ray response is generally lower than electron response
// and is a function of x-ray energy
double XRayResponse(double energy) {
  if (energy <= 0) return 0;

  if (energy > 1100) {
    return 0.8;
  } else {
    return 0.8 * TMath::Exp(energy - 1100);
  }
}

void ROOT2Img(int detectorID) {

  // Getting anatree
  TFile* inroot = new TFile("BeamImage.root", "READ");
  TTree* tree = (TTree*) inroot->Get("BeamImage");

  TBranch *bE      = tree->GetBranch("E");
  TBranch *bX      = tree->GetBranch("X");
  TBranch *bY      = tree->GetBranch("Y");
  TBranch *bZ      = tree->GetBranch("Z");
  TBranch *bT      = tree->GetBranch("T");
  TBranch *bdetID  = tree->GetBranch("detID");
  TBranch *bPID    = tree->GetBranch("PID");
  
  double E;
  double X;
  double Y;
  double Z;
  double T;
  double detID;
  double PID;

  bE->SetAddress(&E);
  bX->SetAddress(&X);
  bY->SetAddress(&Y);
  bZ->SetAddress(&Z);
  bT->SetAddress(&T);
  bdetID->SetAddress(&detID);
  bPID->SetAddress(&PID);

  Int_t nHits = tree->GetEntries();

  // Prepare histogram
  std::vector<double> detectorSpec = psets.at(detectorID);
  double length = detectorSpec.at(3);
  double height = detectorSpec.at(4);
  int nbin_x = length / resolution;
  int nbin_y = height / resolution;
  TH2D* hist = new TH2D(pnames.at(detectorID).c_str(), ";X;Y", nbin_x, 0, nbin_x, nbin_y, 0, nbin_y);

  // Event loop
  double x_origin = detectorSpec.at(0);
  double y_origin = detectorSpec.at(1);
  double z_origin = detectorSpec.at(2);
  for (Int_t i = 0; i < nHits; ++i) {
    tree->GetEntry(i);
    if (detID != detectorID) continue;

    int binx = TMath::Sqrt(TMath::Power(X - x_origin, 2) + TMath::Power(Z - z_origin, 2)) / resolution;
    int biny = (Y - y_origin) / resolution;

    if (PID == 1) {
      hist->Fill(binx, biny);
    } else if (PID == 2) {
      hist->Fill(binx, biny, XRayResponse(E));
    } else continue;
  }

  // Apply saturation
  for (unsigned long long int i = 0; i < hist->GetSize(); i++) {
    if (hist->GetBinContent(i) > saturationValue)
      hist->SetBinContent(i, saturationValue);
  }

  // Save the image format to a new root file
  TFile* outroot = new TFile("ImageFormat.root", "UPDATE");
  outroot->cd();
  hist->SetName(pnames.at(detectorID).c_str());
  hist->Write();
  outroot->Write();
  outroot->Close();

  inroot->Close();
}
