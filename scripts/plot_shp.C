void plot_shp(char* fname="shp.txt"){
	TGraph2D* gLims=new TGraph2D(fname,"LIM %lf %lf %lf");
	TGraph2D* g0=new TGraph2D(fname,"... %lf %lf %lf");
	TGraph2D* gH=new TGraph2D(fname,"HIT %lf %lf %lf");
	gH->SetMarkerStyle(7);
	gH->SetMarkerColor(kBlue);
	gLims->Draw("P");
	g0->Draw("P same");
	gH->Draw("P same");
	// g0->GetXaxis()->SetLabel("X");
	// g0->GetYaxis()->SetTitle("Y");
	// g0->GetZaxis()->SetTitle("Z");
	
}