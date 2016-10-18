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
// @version $Revision: 1221 $ (last changed revision)
// @date    $Date: 2015-12-01 13:43:31 +0100 (Di, 01 Dez 2015) $ (last change date)
// @author  $Author: hentsch $ (last changed by)
*/


#ifndef __BOOST_MASK_L_H
#define __BOOST_MASK_L_H

#include "rttbBaseType.h"
#include "rttbStructure.h"
#include "rttbGeometricInfo.h"
#include "rttbMaskVoxel.h"
#include "rttbMaskAccessorInterface.h"

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/geometries/ring.hpp>
#include <boost/shared_ptr.hpp>

namespace rttb
{
	namespace masks
	{
        namespace boostLegacy
		{
			/*! @class BoostMask
			*   @brief Implementation of voxelization using boost::geometry.
			*   @attention If "strict" is set to true, an exception will be thrown when the given structure has self intersection.
			*   (A structure without self interseciton means all contours of the structure have no self intersection, and
			*   the polygons on the same slice have no intersection between each other, unless the case of a donut. A donut is accepted.)
			*   If "strict" is set to false, debug information will be displayed when the given structure has self intersection. Self intersections will be ignored
			*   and the mask will be calculated, however, it may cause errors in the mask results.
			*/
			class BoostMask
			{

			public:
				typedef ::boost::shared_ptr<rttb::core::GeometricInfo> GeometricInfoPointer;
				typedef core::Structure::StructTypePointer StructPointer;
				typedef core::MaskAccessorInterface::MaskVoxelList MaskVoxelList;
				typedef core::MaskAccessorInterface::MaskVoxelListPointer MaskVoxelListPointer;

				/*! @brief Constructor
				* @exception rttb::core::NullPointerException thrown if aDoseGeoInfo or aStructure is NULL
				* @param strict indicates whether to allow self intersection in the structure. If it is set to true, an exception will be thrown when the given structure has self intersection.
				* @exception InvalidParameterException thrown if strict is true and the structure has self intersections
				*/
				BoostMask(GeometricInfoPointer aDoseGeoInfo, StructPointer aStructure, bool strict = true);

				/*! @brief Generate mask and return the voxels in the mask
				* @exception rttb::core::InvalidParameterException thrown if the structure has self intersections
				*/
				MaskVoxelListPointer getRelevantVoxelVector();

			private:
				typedef ::boost::geometry::model::d2::point_xy<double> BoostPoint2D;
				typedef ::boost::geometry::model::polygon< ::boost::geometry::model::d2::point_xy<double> >
				BoostPolygon2D;
				typedef ::boost::geometry::model::ring< ::boost::geometry::model::d2::point_xy<double> >
				BoostRing2D;
				typedef std::deque<BoostPolygon2D> BoostPolygonDeque;
				typedef std::vector<BoostRing2D> BoostRingVector;//polygon without holes
				typedef std::vector<BoostPolygon2D> BoostPolygonVector;//polygon with or without holes
				typedef std::vector<rttb::VoxelGridIndex3D> VoxelIndexVector;

				GeometricInfoPointer _geometricInfo;

				StructPointer _structure;

				bool _strict;

				//vector of the MaskVoxel inside the structure
				MaskVoxelListPointer _voxelInStructure;

				/*! @brief If the mask is up to date
				*/
				bool _isUpToDate;

				/*! @brief Voxelization and generate mask
				*/
				void calcMask();

				/*! @brief Check if the structure has self intersections*/
				bool hasSelfIntersections();

				/*! @brief Get the min/max voxel index of the bounding box of the polygon in the slice with sliceNumber
				 * @param sliceNumber slice number between 0 and _geometricInfo->getNumSlices()
				 * @param intersectionSlicePolygons Get the polygons intersecting the slice
				 * @exception InvalidParameterException thrown if sliceNumber < 0 or sliceNumber >=  _geometricInfo->getNumSlices()
				 * @return Return the 4 voxel index of the bounding box
				*/
				VoxelIndexVector getBoundingBox(unsigned int sliceNumber,
				                                const BoostPolygonVector& intersectionSlicePolygons);

				/*! @brief Get intersection polygons of the contour and a voxel polygon
				 * @param aVoxelIndex3D The 3d grid index of the voxel
				 * @param intersectionSlicePolygons The polygons of the slice intersecting the voxel
				 * @return Return all intersetion polygons of the structure and the voxel
				*/
				BoostPolygonDeque getIntersections(const rttb::VoxelGridIndex3D& aVoxelIndex3D,
				                                   const BoostPolygonVector& intersectionSlicePolygons);

				/*! @brief Calculate the area of all polygons
				 * @param aPolygonDeque The deque of polygons
				 * @return Return the area of all polygons
				*/
				double calcArea(const BoostPolygonDeque& aPolygonDeque);

				/*! @brief Get grid index of a mask voxel
				 * @param aMaskVoxel A mask voxel
				 * @return Return the 3d grid index of the mask voxel
				*/
				VoxelGridIndex3D getGridIndex3D(const core::MaskVoxel& aMaskVoxel);

				/*! @brief Convert RTTB polygon to boost polygon*/
				BoostRing2D convert(const rttb::PolygonType& aRTTBPolygon);

				/*! @brief Get the intersection slice of the voxel, return the polygons (with (donut) or without holes) in the slice
				 * @param aVoxelGridIndexZ The z grid index (slice number) of the voxel
				 * @return Return the structure polygons intersecting the slice
				*/
				BoostPolygonVector getIntersectionSlicePolygons(const rttb::GridIndexType aVoxelGridIndexZ);

				/*! @brief Get the voxel 2d contour polygon*/
				BoostRing2D get2DContour(const rttb::VoxelGridIndex3D& aVoxelGrid3D);

				/*! @brief If 2 rings in the vector build a donut, convert the 2 rings to a donut polygon, other rings unchanged*/
				BoostPolygonVector checkDonutAndConvert(const BoostRingVector& aRingVector);

			};

		}


	}
}

#endif