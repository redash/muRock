void plot_shp(char* fname="shp.txt"){
	TGraph2D* g0=new TGraph2D(fname,"... %lf %lf %lf");
	TGraph2D* gH=new TGraph2D(fname,"HIT %lf %lf %lf");
	gH->SetMarkerStyle(7);
	gH->SetMarkerColor(kBlue);
	g0->Draw("P");
	// g0->GetXaxis()->SetLabel("X");
	// g0->GetYaxis()->SetTitle("Y");
	// g0->GetZaxis()->SetTitle("Z");
	gH->Draw("P same");
}