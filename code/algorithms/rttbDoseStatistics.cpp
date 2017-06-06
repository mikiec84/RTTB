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
// @version $Revision$ (last changed revision)
// @date    $Date$ (last change date)
// @author  $Author$ (last changed by)
*/

#include "rttbDoseStatistics.h"

#include "boost/make_shared.hpp"

#include "rttbDataNotAvailableException.h"
#include "rttbInvalidParameterException.h"

namespace rttb
{

	namespace algorithms
	{
		DoseStatistics::DoseStatistics(DoseStatisticType minimum, DoseStatisticType maximum,
		                               DoseStatisticType mean,
		                               DoseStatisticType stdDeviation, VoxelNumberType numVoxels, VolumeType volume,
		                               ResultListPointer maximumVoxelPositions /*= ResultListPointer()*/,
		                               ResultListPointer minimumVoxelPositions /*= ResultListPointer()*/,
									   VolumeToDoseMeasure Dx /*= VolumeToDoseMeasure(VolumeToDoseMeasure::complexStatistics::Dx)*/,
		                               DoseToVolumeFunctionType Vx /*= std::map<VolumeType, DoseTypeGy>()*/,
		                               VolumeToDoseFunctionType MOHx /*= std::map<VolumeType, DoseTypeGy>()*/,
		                               VolumeToDoseFunctionType MOCx /*= std::map<VolumeType, DoseTypeGy>()*/,
		                               VolumeToDoseFunctionType MaxOHx /*= std::map<VolumeType, DoseTypeGy>()*/,
		                               VolumeToDoseFunctionType MinOCx /*= std::map<VolumeType, DoseTypeGy>()*/,
		                               DoseTypeGy referenceDose /*=-1*/):
			_minimum(minimum), _maximum(maximum), _mean(mean), _stdDeviation(stdDeviation),
			_numVoxels(numVoxels), _volume(volume),
			_Dx(Dx), _Vx(Vx),
			_MOHx(MOHx),
			_MOCx(MOCx), _MaxOHx(MaxOHx), _MinOCx(MinOCx)
		{
			if (maximumVoxelPositions == NULL)
			{
				_maximumVoxelPositions = boost::make_shared<std::vector<std::pair<DoseTypeGy, VoxelGridID> > >
				                         (std::vector<std::pair<DoseTypeGy, VoxelGridID> >());
			}
			else
			{
				_maximumVoxelPositions = maximumVoxelPositions;
			}

			if (minimumVoxelPositions == NULL)
			{
				_minimumVoxelPositions = boost::make_shared<std::vector<std::pair<DoseTypeGy, VoxelGridID> > >
				                         (std::vector<std::pair<DoseTypeGy, VoxelGridID> >());
			}
			else
			{
				_minimumVoxelPositions = minimumVoxelPositions;
			}

			if (referenceDose <= 0)
			{
				_referenceDose = _maximum;
			}
			else
			{
				_referenceDose = referenceDose;
			}
		}


		DoseStatistics::~DoseStatistics()
		{

		}


		void DoseStatistics::setMinimumVoxelPositions(ResultListPointer minimumVoxelPositions)
		{
			_minimumVoxelPositions = minimumVoxelPositions;
		}

		void DoseStatistics::setMaximumVoxelPositions(ResultListPointer maximumVoxelPositions)
		{
			_maximumVoxelPositions = maximumVoxelPositions;
		}

		void DoseStatistics::setDx(const VolumeToDoseMeasure& DxValues)
		{
			_Dx = DxValues;
		}

		void DoseStatistics::setVx(const VolumeToDoseFunctionType& VxValues)
		{
			_Vx = VxValues;
		}

		void DoseStatistics::setMOHx(const VolumeToDoseFunctionType& MOHxValues)
		{
			_MOHx = MOHxValues;
		}

		void DoseStatistics::setMOCx(const VolumeToDoseFunctionType& MOCxValues)
		{
			_MOCx = MOCxValues;
		}

		void DoseStatistics::setMaxOHx(const VolumeToDoseFunctionType& MaxOHValues)
		{
			_MaxOHx = MaxOHValues;
		}

		void DoseStatistics::setMinOCx(const VolumeToDoseFunctionType& MinOCValues)
		{
			_MinOCx = MinOCValues;
		}

		void DoseStatistics::setReferenceDose(DoseTypeGy referenceDose)
		{
			if (referenceDose <= 0)
			{
				_referenceDose = _maximum;
			}
			else
			{
				_referenceDose = referenceDose;
			}
		}

		VoxelNumberType DoseStatistics::getNumberOfVoxels() const
		{
			return _numVoxels;
		}


		VolumeType DoseStatistics::getVolume() const
		{
			return _volume;
		}

		DoseTypeGy DoseStatistics::getReferenceDose() const
		{
			return _referenceDose;
		}


		DoseStatisticType DoseStatistics::getMaximum() const
		{
			return _maximum;
		}

		DoseStatisticType DoseStatistics::getMinimum() const
		{
			return _minimum;
		}

		DoseStatisticType DoseStatistics::getMean() const
		{
			return _mean;
		}

		DoseStatisticType DoseStatistics::getStdDeviation() const
		{
			return _stdDeviation;
		}

		DoseStatisticType DoseStatistics::getVariance() const
		{
			return _stdDeviation * _stdDeviation;
		}

		VolumeType DoseStatistics::getVx(DoseTypeGy xDoseAbsolute, bool findNearestValue,
		                                 DoseTypeGy& nearestXDose) const
		{
			return getValue(_Vx, xDoseAbsolute, findNearestValue, nearestXDose);
		}

		VolumeType DoseStatistics::getVx(DoseTypeGy xDoseAbsolute) const
		{
			DoseTypeGy dummy;
			return getValue(_Vx, xDoseAbsolute, false, dummy);
		}
		VolumeType DoseStatistics::getVxRelative(DoseTypeGy xDoseRelative)  const 
		{
			if (_referenceDose != -1 && xDoseRelative >=0 && xDoseRelative <=1){
				DoseTypeGy xDoseAbsolute = xDoseRelative * _referenceDose;
				DoseTypeGy dummy;
				return getValue(_Vx, xDoseAbsolute, false, dummy);
			}
			else {
				throw rttb::core::InvalidParameterException("Reference dose must be > 0 and 0 <= relative Dose <= 1");
			}
		}
		VolumeType DoseStatistics::getVxRelative(DoseTypeGy xDoseRelative, bool findNearestValue,
			DoseTypeGy& nearestXDose) const
		{
			if (_referenceDose != -1 && xDoseRelative >= 0 && xDoseRelative <= 1){
				DoseTypeGy xDoseAbsolute = xDoseRelative * _referenceDose;
				return getValue(_Vx, xDoseAbsolute, findNearestValue, nearestXDose);
			}
			else {
				throw rttb::core::InvalidParameterException("Reference dose must be > 0 and 0 <= relative Dose <= 1");
			}
		}

		DoseTypeGy DoseStatistics::getMOHx(VolumeType xVolumeAbsolute, bool findNearestValue,
		                                   VolumeType& nearestXVolume) const
		{
			return getValue(_MOHx, xVolumeAbsolute, findNearestValue, nearestXVolume);
		}

		DoseTypeGy DoseStatistics::getMOHx(VolumeType xVolumeAbsolute) const
		{
			VolumeType dummy;
			return getValue(_MOHx, xVolumeAbsolute, false, dummy);
		}

		DoseTypeGy DoseStatistics::getMOHxRelative(VolumeType xVolumeRelative, bool findNearestValue,
			VolumeType& nearestXVolume) const
		{
			if (xVolumeRelative >= 0 && xVolumeRelative <= 1){
			DoseTypeGy xVolumeAbsolute = xVolumeRelative*_volume;
			return getValue(_MOHx, xVolumeAbsolute, findNearestValue, nearestXVolume);
			}
			else {
				throw rttb::core::InvalidParameterException("Relative Volume must be >= 0 and <=1");
			}
		}

		DoseTypeGy DoseStatistics::getMOHxRelative(VolumeType xVolumeRelative) const
		{
			if (xVolumeRelative >= 0 && xVolumeRelative <= 1){
			DoseTypeGy xVolumeAbsolute = xVolumeRelative*_volume;
			VolumeType dummy;
			return getValue(_MOHx, xVolumeAbsolute, false, dummy);
			}
			else {
				throw rttb::core::InvalidParameterException("Relative Volume must be >= 0 and <=1");
			}
		}


		DoseTypeGy DoseStatistics::getMOCx(VolumeType xVolumeAbsolute, bool findNearestValue,
		                                   VolumeType& maximumDistanceFromOriginalVolume) const
		{
			return getValue(_MOCx, xVolumeAbsolute, findNearestValue, maximumDistanceFromOriginalVolume);
		}

		DoseTypeGy DoseStatistics::getMOCx(VolumeType xVolumeAbsolute) const
		{
			VolumeType dummy;
			return getValue(_MOCx, xVolumeAbsolute, false, dummy);
		}

		DoseTypeGy DoseStatistics::getMOCxRelative(VolumeType xVolumeRelative, bool findNearestValue,
			VolumeType& maximumDistanceFromOriginalVolume) const
		{
			if (xVolumeRelative >= 0 && xVolumeRelative <= 1){
			DoseTypeGy xVolumeAbsolute = xVolumeRelative*_volume;
			return getValue(_MOCx, xVolumeAbsolute, findNearestValue, maximumDistanceFromOriginalVolume);
			}
			else {
				throw rttb::core::InvalidParameterException("Relative Volume must be >= 0 and <=1");
			}
		}

		DoseTypeGy DoseStatistics::getMOCxRelative(VolumeType xVolumeRelative) const
		{
			if (xVolumeRelative >= 0 && xVolumeRelative <= 1){
			DoseTypeGy xVolumeAbsolute = xVolumeRelative*_volume;
			VolumeType dummy;
			return getValue(_MOCx, xVolumeAbsolute, false, dummy);
			}
			else {
				throw rttb::core::InvalidParameterException("Relative Volume must be >= 0 and <=1");
			}
		}

		DoseTypeGy DoseStatistics::getMaxOHx(VolumeType xVolumeAbsolute, bool findNearestValue,
		                                     VolumeType& maximumDistanceFromOriginalVolume) const
		{
			return getValue(_MaxOHx, xVolumeAbsolute, findNearestValue, maximumDistanceFromOriginalVolume);
		}

		DoseTypeGy DoseStatistics::getMaxOHx(VolumeType xVolumeAbsolute) const
		{
			VolumeType dummy;
			return getValue(_MaxOHx, xVolumeAbsolute, false, dummy);
		}

		DoseTypeGy DoseStatistics::getMaxOHxRelative(VolumeType xVolumeRelative, bool findNearestValue,
			VolumeType& maximumDistanceFromOriginalVolume) const
		{
			if (xVolumeRelative >= 0 && xVolumeRelative <= 1){
			DoseTypeGy xVolumeAbsolute = xVolumeRelative*_volume;
			return getValue(_MaxOHx, xVolumeAbsolute, findNearestValue, maximumDistanceFromOriginalVolume);
			}
			else {
				throw rttb::core::InvalidParameterException("Relative Volume must be >= 0 and <=1");
			}
		}

		DoseTypeGy DoseStatistics::getMaxOHxRelative(VolumeType xVolumeRelative) const
		{
			if (xVolumeRelative >= 0 && xVolumeRelative <= 1){
			DoseTypeGy xVolumeAbsolute = xVolumeRelative*_volume;
			VolumeType dummy;
			return getValue(_MaxOHx, xVolumeAbsolute, false, dummy);
			}
			else {
				throw rttb::core::InvalidParameterException("Relative Volume must be >= 0 and <=1");
			}
		}

		DoseTypeGy DoseStatistics::getMinOCx(VolumeType xVolumeAbsolute, bool findNearestValue,
		                                     VolumeType& maximumDistanceFromOriginalVolume) const
		{
			return getValue(_MinOCx, xVolumeAbsolute, findNearestValue, maximumDistanceFromOriginalVolume);
		}

		DoseTypeGy DoseStatistics::getMinOCx(VolumeType xVolumeAbsolute) const
		{
			VolumeType dummy;
			return getValue(_MinOCx, xVolumeAbsolute, false, dummy);
		}
		DoseTypeGy DoseStatistics::getMinOCxRelative(VolumeType xVolumeRelative, bool findNearestValue,
			VolumeType& maximumDistanceFromOriginalVolume) const
		{
			if (xVolumeRelative >= 0 && xVolumeRelative <= 1){
			DoseTypeGy xVolumeAbsolute = xVolumeRelative*_volume;
			return getValue(_MinOCx, xVolumeAbsolute, findNearestValue, maximumDistanceFromOriginalVolume);
			}
			else {
				throw rttb::core::InvalidParameterException("Relative Volume must be >= 0 and <=1");
			}
		}

		DoseTypeGy DoseStatistics::getMinOCxRelative(VolumeType xVolumeRelative) const
		{
			if (xVolumeRelative >= 0 && xVolumeRelative <= 1){
			DoseTypeGy xVolumeAbsolute = xVolumeRelative*_volume;
			VolumeType dummy;
			return getValue(_MinOCx, xVolumeAbsolute, false, dummy);
			}
			else {
				throw rttb::core::InvalidParameterException("Relative Volume must be >= 0 and <=1");
			}
		}

		double DoseStatistics::getValue(const std::map<double, double>& aMap, double key,
		                                bool findNearestValueInstead, double& storedKey) const
		{
			if (aMap.find(key) != std::end(aMap))
			{
				return aMap.find(key)->second;
			}
			else
			{
				//value not in map. We have to find the nearest value
				if (aMap.empty())
				{
					throw core::DataNotAvailableException("No Vx values are defined");
				}
				else
				{
					if (findNearestValueInstead)
					{
						auto iterator = findNearestKeyInMap(aMap, key);
						storedKey = iterator->first;
						return iterator->second;
					}
					else
					{
						throw core::DataNotAvailableException("No Vx value with required dose is defined");
					}
				}
			}
		}

		std::map<double, double>::const_iterator DoseStatistics::findNearestKeyInMap(
		    const std::map<double, double>& aMap,
		    double key) const
		{
			double minDistance = 1e19;
			double minDistanceLast = 1e20;

			auto iterator = std::begin(aMap);

			while (iterator != std::end(aMap))
			{
				minDistanceLast = minDistance;
				minDistance = fabs(iterator->first - key);

				if (minDistanceLast > minDistance)
				{
					++iterator;
				}
				else
				{
					if (iterator != std::begin(aMap))
					{
						--iterator;
						return iterator;
					}
					else
					{
						return std::begin(aMap);
					}
				}
			}

			--iterator;
			return iterator;
		}

		DoseStatistics::ResultListPointer DoseStatistics::getMaximumVoxelPositions() const
		{
			return _maximumVoxelPositions;
		}

		DoseStatistics::ResultListPointer DoseStatistics::getMinimumVoxelPositions() const
		{
			return _minimumVoxelPositions;
		}

		DoseStatistics::DoseToVolumeFunctionType DoseStatistics::getAllVx() const
		{
			return _Vx;
		}

		VolumeToDoseMeasure DoseStatistics::getDx() const
		{
			return _Dx;
		}

		DoseStatistics::VolumeToDoseFunctionType DoseStatistics::getAllMOHx() const
		{
			return _MOHx;
		}

		DoseStatistics::VolumeToDoseFunctionType DoseStatistics::getAllMOCx() const
		{
			return _MOCx;
		}

		DoseStatistics::VolumeToDoseFunctionType DoseStatistics::getAllMaxOHx() const
		{
			return _MaxOHx;
		}

		DoseStatistics::VolumeToDoseFunctionType DoseStatistics::getAllMinOCx() const
		{
			return _MinOCx;
		}



	}//end namespace algorithms
}//end namespace rttb

