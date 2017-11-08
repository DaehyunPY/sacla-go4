
Bool_t BackupSettings(TString name="conpar_online", TString folder="")
{
    TString filext = "C";
    TString bakdir = "conpar_backups/";

    gSystem->mkdir(bakdir, kTRUE);

    TTimeStamp ts = TTimeStamp();
    TString now = TString::Format("%08d_%06d", ts.GetDate(kFALSE), ts.GetTime(kFALSE));

    TString from = folder + name + "." + filext;
    TString to   = bakdir + name + "_" + now + "." + filext;
//    cout << "Backed up \"" << from << "\" to \"" << to << "\"" << endl;

    int status = gSystem->CopyFile(from, to);
    return (status == 0) ? kTRUE : kFALSE;
}


