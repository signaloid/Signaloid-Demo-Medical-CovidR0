/*
 *	Authored 2022, Angelos Plevris.
 *
 *	Copyright (c) 2022, Signaloid.
 *
 *	Permission is hereby granted, free of charge, to any person obtaining a copy
 *	of this software and associated documentation files (the "Software"), to deal
 *	in the Software without restriction, including without limitation the rights
 *	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *	copies of the Software, and to permit persons to whom the Software is
 *	furnished to do so, subject to the following conditions:
 *
 *	The above copyright notice and this permission notice shall be included in all
 *	copies or substantial portions of the Software.
 *
 *	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *	SOFTWARE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include<uncertain.h>

enum
{
	kMaxData = 20,
};

int
main(int argc, char const *argv[])
{
	/*
	 *	This mathematical model was taken from https://www.ncbi.nlm.nih.gov/pmc/articles/PMC7376536/.
	 */
	double b;		/* Birth rate of human population*/
	double mu;		/* Natural human death rate */
	double mp;		/* Natural death rate of pathogens in the environment */
	double b1;		/* Rate of transmission from S to E due to contact with P */
	double b2;		/* Rate of transmission from S to E due to contact with Ia and/or Is */
	double delta;		/* Proportion of symptomatic infectious people */
	double psi;		/* Progression rate from E back to S due to robust immune system */
	double omega;		/* Progression rate from E to either Ia or Is */
	double sigma;		/* Death rate due to the coronavirus */
	double gs;		/* Rate of recovery of the symptomatic population */
	double ga;		/* Rate of recovery of the asymptomatic human population */ 
	double hs;		/* Rate of virus spread to environment by symptomatic infectious individuals */
	double ha;		/* Rate of virus spread to environment by asymptomatic infectious individuals */

	double lifeExpectancy;
	double lifeExpectancyData[kMaxData];

	if (argc == 2)
	{
		FILE * fp = fopen(argv[1],"r+");
		if (fp == NULL)
		{
			printf("Could not open %s input file!\n", argv[1]);
			return 0;
		}
		int i;
		for (i = 0; i < kMaxData; i++)
		{
			/*
			 *	Data in years we need to transform in days^-1
			 */
			
			fscanf(fp, "%*d %lf\n", &lifeExpectancyData[i]);
			lifeExpectancyData[i] *= 365;
		}

		fclose(fp);

		lifeExpectancy = libUncertainDoubleDistFromSamples(lifeExpectancyData, kMaxData);
	}
	else
	{
		double lifeExpectancyMedian = 65 * 365;
		double lifeExpectancyDeviation = 40;
		lifeExpectancy = libUncertainDoubleGaussDist(lifeExpectancyMedian, lifeExpectancyDeviation);
	}
	
	/*
	 *	Mean values are taken from https://www.ncbi.nlm.nih.gov/pmc/articles/PMC7376536/.
	 *	Standard Deviation values chosen by us. Feel free to experiment with them.
	 */
	double humanBirthRateMean = 0.00018;
	double humanBirthRateDeviation = 0.000001;
	double pathogenBirthRateMean = 0.1724;
	double pathogenBirthRateDeviation = 0.001;
	double b1Mean = 0.00414;
	double b1Deviation = 0.00001;
	double b2Mean = 0.0115;
	double b2Deviation =  0.0001;
	double deltaMean = 0.7;
	double deltaDeviation = 0.001;
	double psiMean = 0.0051;
	double psiDeviation = 0.001;
	double omegaMean = 0.09;
	double omegaDeviation =  0.001;
	double sigmaMean = 0.0018;
	double sigmaDeviation =  0.0001;
	double gsMean = 0.06;
	double gsDeviation = 0.001;
	double gaMean = 0.0714;
	double hsMean = 0.1;
	double haMean = 0.05;


	
	b = libUncertainDoubleGaussDist(humanBirthRateMean, humanBirthRateDeviation);
	mu = 1 / lifeExpectancy;
	mp = libUncertainDoubleGaussDist(pathogenBirthRateMean, pathogenBirthRateDeviation);
	b1 = libUncertainDoubleGaussDist(b1Mean,b1Deviation);
	b2 = libUncertainDoubleGaussDist(b2Mean, b2Deviation);
	delta = libUncertainDoubleGaussDist(deltaMean, deltaDeviation);
	psi = libUncertainDoubleGaussDist(psiMean, psiDeviation);
	omega = libUncertainDoubleGaussDist (omegaMean, omegaDeviation);
	sigma = libUncertainDoubleGaussDist(sigmaMean, sigmaDeviation);
	gs = libUncertainDoubleGaussDist(gsMean, gsDeviation);
	ga =  gaMean;
	hs = hsMean;
	ha = haMean;

	double c1;
	double c2;
	double c3;
	
	c1 = psi + mu + omega;
	c2 = mu + sigma + gs;
	c3 = mu + sigma + ga;

	double rh;
	double rp;
	double r0;

	rh = (b2*b / (mu * c1)) * (delta * omega / c2 + (1 - delta) * omega / c3);
	rp = (b1 * b / (mu * mp * c1)) * (hs  * delta * omega / c2 + ha * (1 - delta) * omega / c3);

	r0 = (rh + sqrt(pow(rh, 2) + 4 * rp)) / 2;

	printf("R0 = %lf\n", r0);

	return 0;
}
