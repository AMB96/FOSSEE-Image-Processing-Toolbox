/*
This is the .cpp gateway file for the 'applyTransformer' scilab function.

OpenCV classes :
1. Ptr< HistogramCostExtractor > cv::createChiHistogramCostExtractor (int nDummies=25, float defaultCost=0.2f)
2  Ptr< HistogramCostExtractor > cv::createEMDL1HistogramCostExtractor (int nDummies=25, float defaultCost=0.2f)
3. Ptr< HistogramCostExtractor > cv::createNormHistogramCostExtractor (int flag=DIST_L2, int nDummies=25, float defaultCost=0.2f)

It includes the following OpenCV functions, belonging to the Shape Distance and Matching module of OpenCV 3.0.0 : 
1. void buildCostMatrix (InputArray descriptors1, InputArray descriptors2, OutputArray costMatrix)


*/

#include<numeric>
#include"opencv2/core/core.hpp"
#include"opencv2/highgui/highgui.hpp"
#include"opencv2/opencv.hpp"
#include"opencv2/shape.hpp"
#include"opencv2/imgcodecs.hpp"
#include"opencv2/imgproc/imgproc.hpp"
#include"opencv2/features2d/features2d.hpp"
#include"opencv2/xfeatures2d.hpp"
#include"opencv2/core/utility.hpp"
#include<string>
#include<iostream>
#include<cstdlib>

using namespace cv;
using namespace std;
using namespace cv::xfeatures2d;

extern "C"
{
	#include"api_scilab.h"
	#include"Scierror.h"
	#include"BOOL.h"
	#include<localization.h>
	#include"sciprint.h"
	#include"../common.h"

	int opencv_histogramCostExtractor(char *fname, unsigned long fname_len)
	{
		// Error management variable
		SciErr sciErr;
		int i;

		// variables required to read argument #3
		int *piAddr3 = NULL;
		int intErr3 = 0;
		double typeOfMethod = 0;

		// variables required to read argument #4
		int *piAddr4 = NULL;
		int intErr4 = 0;
		double nDummies = 0;

		// variables required to read argument #5
		int *piAddr5 = NULL;
		int intErr5 = 0;
		double defaultCost = 0;

		// variables required to read argument #6
		int *piAddr6 = NULL;
		int intErr6 = 0;
		double hessianThreshold = 0;


		// Checking number of input and output arguments (enviromnet variable, min arguments, max arguments) 
		CheckInputArgument(pvApiCtx, 4, 6);
		CheckOutputArgument(pvApiCtx, 1, 1);
		
		Mat img1, img2;

		int a = retrieveImage(img1, 1);
		if(a == 0)
		{
			sciprint("Error while retrieving the image1.");
			return 0;
		}
		int b = retrieveImage(img2, 2);
		if(b == 0)
		{
			sciprint("Error while retrieving the image2.");
		}


		// to get argument #3
		sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddr3);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}
		intErr3 = getScalarDouble(pvApiCtx, piAddr3, &typeOfMethod);
		if(intErr3)
		{
			Scierror(999, "gateway crashed abruptly while reading input argument #%d.", 3);
			return -1;
		}

		// to get argument #4
		sciErr = getVarAddressFromPosition(pvApiCtx, 4, &piAddr6);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}
		intErr6 = getScalarDouble(pvApiCtx, piAddr6, &hessianThreshold);
		if(intErr6)
		{
			Scierror(999, "gateway crashed abruptly while reading input argument #%d.", 4);
			return -1;
		}


		// output costMatrix
		Mat costMatrix;
		Mat image1, image2;

		try
		{
			// OpenCV functionalitites
			
			img1.convertTo(img1, CV_8U);
			cvtColor(img1, image1, CV_BGR2GRAY);

			img2.convertTo(img2, CV_8U);
			cvtColor(img2, image2, CV_BGR2GRAY);

			// detecting keypoints & computing descriptors
        		Ptr<SURF> surf = SURF::create(hessianThreshold);
        		vector<KeyPoint> keypoints1, keypoints2;
        		Mat descriptors1, descriptors2;
        		surf->detectAndCompute(image1, Mat(), keypoints1, descriptors1);
        		surf->detectAndCompute(image2, Mat(), keypoints2, descriptors2);

			if(typeOfMethod == 1)
			{
				
				// to get the argument #4
				sciErr = getVarAddressFromPosition(pvApiCtx, 5, &piAddr4);
				if(sciErr.iErr)
				{
					printError(&sciErr, 0);
					return 0;
				}
				intErr4 = getScalarDouble(pvApiCtx, piAddr4, &nDummies);
				if(intErr4)
				{
					return intErr4;
				}
		
				// to get the argument #5
					sciErr = getVarAddressFromPosition(pvApiCtx, 6, &piAddr5);
				if(sciErr.iErr)
				{
					printError(&sciErr, 0);
					return 0;
				}
				intErr5 = getScalarDouble(pvApiCtx, piAddr5, &defaultCost);
				if(intErr5)
				{
					return intErr5;
				}
				
				// Chi Histogram Cost Extractor
        			Ptr<HistogramCostExtractor> model = createChiHistogramCostExtractor(nDummies, defaultCost);

        			model->buildCostMatrix(descriptors1, descriptors2, costMatrix);
			}
			else if(typeOfMethod == 2)
			{
				
				// to get the argument #4
				sciErr = getVarAddressFromPosition(pvApiCtx, 5, &piAddr4);
				if(sciErr.iErr)
				{
					printError(&sciErr, 0);
					return 0;
				}
				intErr4 = getScalarDouble(pvApiCtx, piAddr4, &nDummies);
				if(intErr4)
				{
					return intErr4;
				}

				// to get the argument #5
				sciErr = getVarAddressFromPosition(pvApiCtx, 6, &piAddr5);
				if(sciErr.iErr)
				{
					printError(&sciErr, 0);
					return 0;
				}
				intErr5 = getScalarDouble(pvApiCtx, piAddr5, &defaultCost);
				if(intErr5)
				{
					return intErr5;
				}

				// EMDL1 Histogram Cost Extractor
				Ptr<HistogramCostExtractor> model = createEMDL1HistogramCostExtractor(nDummies, defaultCost);

				model->buildCostMatrix(descriptors1, descriptors2, costMatrix);
			}
			else if(typeOfMethod == 3)
			{
				// Norm Histogram Cost Extractor
				Ptr<HistogramCostExtractor> model =  createNormHistogramCostExtractor();

				model->buildCostMatrix(descriptors1, descriptors2, costMatrix);
			}	
			else
			{
				Scierror(999,"Wrong input for Argument #3. Use '1' for 'Chi', '2' for 'EMDL1', and '3' for 'Norm'.\n", 1);
			}
		
		}catch(Exception& e)
		{
			const char* err = e.what();
			Scierror(999, "%s", err);
		} 

		// to return the output transformed image
		string tempstring1 = type2str(costMatrix.type());
		char *checker1;
		checker1 = (char *)malloc(tempstring1.size() + 1);
		memcpy(checker1, tempstring1.c_str(), tempstring1.size() + 1);
		returnImage(checker1, costMatrix, 1);
		free(checker1);		

//		sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, costMatrix.rows, costMatrix.cols, costMatrix);
			
		AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;

		ReturnArguments(pvApiCtx);

		return 0;
	}
}
