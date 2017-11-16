#ifndef CAPTEURINFRA_H__
#define CAPTEURINFRA_H__
class CapteurInfra
{

public:
	float CalculDeDistance(float fTensionDistance);


private:
	float voltage(int EntreeAnalogique);
	float DistanceCapteurChoisi(int numCapteur);

};


#endif //CAPTEURINFRA_H__
