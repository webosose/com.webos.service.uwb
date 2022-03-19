#ifndef _ANGLEMEASURE_H
#define _ANGLEMEASURE_H

class AngleMeasure {
private:
	double mRadians;
    double mErrorRadians;
    double mConfidenceLevel;
	
public:
	AngleMeasure();
	AngleMeasure(double radians, double errorRadians, double confidenceLevel);
	~AngleMeasure() = default;
	
	double getRadians() const;
	double getErrorRadians() const;
	double getConfidenceLevel() const;
	
	void setRadians(double radians);
	void setErrorRadians(double errorRadians);
	void setConfidenceLevel(double confidenceLevel);
};

#endif