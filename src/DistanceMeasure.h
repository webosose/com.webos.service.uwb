#ifndef _DISTANCEMEASURE_H
#define _DISTANCEMEASURE_H

class DistanceMeasure {
private:
	double mMeters;
	double mErrorMeters;
	double mConfidenceLevel;
	
public:
	DistanceMeasure();
	DistanceMeasure(double meters, double errorMeters, double confidenceLevel);
	~DistanceMeasure() = default;
	
	double getMeters() const;
	double getErrorMeters() const;
	double getConfidenceLevel() const;
	
	void setMeters(double meters);
	void setErrorMeters(double errorMeters);
	void setConfidenceLevel(double confidenceLevel);

};

#endif