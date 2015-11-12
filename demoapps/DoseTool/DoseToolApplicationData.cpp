// -----------------------------------------------------------------------
// RTToolbox - DKFZ radiotherapy quantitative evaluation library
//
// Copyright (c) German Cancer Research Center (DKFZ),
// Software development for Integrated Diagnostics and Therapy (SIDT).
// ALL RIGHTS RESERVED.
// See rttbCopyright.txt or
// http://www.dkfz.de/en/sidt/projects/rttb/copyright.html
//
// This software is distributed WITHOUT ANY WARRANTY; without even
// the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
// PURPOSE.  See the above copyright notices for more information.
//
//------------------------------------------------------------------------
/*!
// @file
// @version $Revision: 1066 $ (last changed revision)
// @date    $Date: 2015-08-19 11:47:07 +0200 (Mi, 19 Aug 2015) $ (last change date)
// @author  $Author: floca $ (last changed by)
*/

#include "DoseToolApplicationData.h"

namespace rttb
{
	namespace apps
	{
		namespace doseTool
		{
			ApplicationData::
			ApplicationData()
			{
				this->reset();
			}

			void
			ApplicationData::
			reset()
			{
				_doseFileName = "";
				_structFileName = "";
				_structNameRegex = "";
				_structNameActual = "";
				_doseStatisticOutputFileName = "";
				_computeComplexDoseStatistics = false;
				_allowSelfIntersection = false;
				_computeDVH = false;
				_computeDoseStatistics = false;
			}

			void populateAppData(boost::shared_ptr<DoseToolCmdLineParser> argParser, ApplicationData& appData)
			{
				appData._doseFileName = argParser->get<std::string>(argParser->OPTION_DOSE_FILE);
				appData._doseLoadStyle = argParser->get<std::vector<std::string> >(argParser->OPTION_DOSE_LOAD_STYLE);
				appData._structFileName = argParser->get<std::string>(argParser->OPTION_STRUCT_FILE);
				appData._structLoadStyle = argParser->get<std::vector<std::string> >(argParser->OPTION_STRUCT_LOAD_STYLE);
				appData._structNameRegex = argParser->get<std::string >(argParser->OPTION_STRUCT_NAME);

				if (argParser->isSet(argParser->OPTION_DOSE_STATISTICS))
				{
					appData._computeDoseStatistics = true;
					appData._doseStatisticOutputFileName = argParser->get<std::string >(argParser->OPTION_DOSE_STATISTICS);
					appData._computeComplexDoseStatistics = argParser->isSet(argParser->OPTION_COMPLEX_STATISTICS);
					appData._allowSelfIntersection = argParser->isSet(argParser->OPTION_ALLOW_SELF_INTERSECTION_STRUCT);
				}

				if (argParser->isSet(argParser->OPTION_DVH))
				{
					appData._computeDVH = true;
					appData._dvhOutputFilename = argParser->get<std::string >(argParser->OPTION_DVH);
				}


				if (argParser->isSet(argParser->OPTION_DOSE_STATISTICS) && argParser->isSet(argParser->OPTION_PRESCRIBED_DOSE))
				{
					appData._prescribedDose = argParser->get<DoseTypeGy>(argParser->OPTION_PRESCRIBED_DOSE);
				}
				else
				{
					appData._prescribedDose = 1.0;
				}
			}

		}
	}
}
