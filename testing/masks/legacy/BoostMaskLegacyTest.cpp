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

#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>

#include "litCheckMacros.h"

#include "rttbBaseType.h"
#include "rttbMaskVoxel.h"

#include "../../core/DummyStructure.h"
#include "../../core/DummyDoseAccessor.h"
#include "rttbBoostMask_LEGACY.h"
#include "rttbBoostMaskAccessor_LEGACY.h"


namespace rttb
{
	namespace testing
	{

		/*! @brief BoostMaskAccessorTest.
			1) test constructors
			2) test getRelevantVoxelVector
			3) test getMaskAt
		*/
		int BoostMaskLegacyTest(int argc, char* argv[])
		{
			PREPARE_DEFAULT_TEST_REPORTING;

			typedef core::Structure::StructTypePointer StructTypePointer;
			typedef masks::boostLegacy::BoostMaskAccessor::MaskVoxelListPointer MaskVoxelListPointer;
            typedef masks::boostLegacy::BoostMaskAccessor::MaskVoxelList MaskVoxelList;

			// generate test structure set
			boost::shared_ptr<DummyDoseAccessor> spTestDoseAccessor =
			    boost::make_shared<DummyDoseAccessor>();

			DummyStructure myStructGenerator(spTestDoseAccessor->getGeometricInfo());

			GridIndexType zPlane = 4;
			core::Structure myTestStruct = myStructGenerator.CreateRectangularStructureCentered(zPlane);
			StructTypePointer spMyStruct = boost::make_shared<core::Structure>(myTestStruct);
			boost::shared_ptr<core::GeometricInfo> geometricPtr = boost::make_shared<core::GeometricInfo>
			        (spTestDoseAccessor->getGeometricInfo());

			//1) test BoostMask and BoostMaskAccessor constructor
            CHECK_NO_THROW(rttb::masks::boostLegacy::BoostMask(geometricPtr, spMyStruct));
            rttb::masks::boostLegacy::BoostMask boostMask = rttb::masks::boostLegacy::BoostMask(geometricPtr, spMyStruct);
            CHECK_NO_THROW(rttb::masks::boostLegacy::BoostMaskAccessor(spMyStruct,
			               spTestDoseAccessor->getGeometricInfo()));
            rttb::masks::boostLegacy::BoostMaskAccessor boostMaskAccessor(spMyStruct,
			        spTestDoseAccessor->getGeometricInfo());

			//2) test getRelevantVoxelVector
			CHECK_NO_THROW(boostMask.getRelevantVoxelVector());
			CHECK_NO_THROW(boostMaskAccessor.getRelevantVoxelVector());


			//3) test getMaskAt
			const VoxelGridIndex3D inMask1(2, 1, 4); //corner -> volumeFraction = 0.25
			const VoxelGridIndex3D inMask2(3, 4, 4); //inside ->volumeFraction = 1
			const VoxelGridIndex3D inMask3(4, 5, 4); //side -> volumeFraction = 0.5
			const VoxelGridIndex3D outMask1(7, 5, 4);
			const VoxelGridIndex3D outMask2(2, 1, 2);
			VoxelGridID testId;
			double errorConstant = 1e-7;

			core::MaskVoxel tmpMV1(0), tmpMV2(0);
			CHECK(boostMaskAccessor.getMaskAt(inMask1, tmpMV1));
			geometricPtr->convert(inMask1, testId);
			CHECK(boostMaskAccessor.getMaskAt(testId, tmpMV2));
			CHECK_EQUAL(tmpMV1, tmpMV2);
			CHECK_CLOSE(0.25, tmpMV1.getRelevantVolumeFraction(), errorConstant);
			CHECK_EQUAL(testId, tmpMV1.getVoxelGridID());

			CHECK(boostMaskAccessor.getMaskAt(inMask2, tmpMV1));
			CHECK(geometricPtr->convert(inMask2, testId));
			CHECK(boostMaskAccessor.getMaskAt(testId, tmpMV2));
			CHECK_EQUAL(tmpMV1, tmpMV2);
			CHECK_EQUAL(1, tmpMV1.getRelevantVolumeFraction());
			CHECK_EQUAL(testId, tmpMV1.getVoxelGridID());

			CHECK(boostMaskAccessor.getMaskAt(inMask3, tmpMV1));
			CHECK(geometricPtr->convert(inMask3, testId));
			CHECK(boostMaskAccessor.getMaskAt(testId, tmpMV2));
			CHECK_EQUAL(tmpMV1, tmpMV2);
			CHECK_CLOSE(0.5, tmpMV1.getRelevantVolumeFraction(), errorConstant);
			CHECK_EQUAL(testId, tmpMV1.getVoxelGridID());

			CHECK(!boostMaskAccessor.getMaskAt(outMask1, tmpMV1));
			CHECK(geometricPtr->convert(outMask1, testId));
			CHECK(!boostMaskAccessor.getMaskAt(testId, tmpMV2));
			CHECK_EQUAL(tmpMV1, tmpMV2);
			CHECK_EQUAL(0, tmpMV1.getRelevantVolumeFraction());
			//CHECK_EQUAL(testId,tmpMV1.getVoxelGridID()); -> return value will not be valid outside the mask

			CHECK(!boostMaskAccessor.getMaskAt(outMask2, tmpMV1));
			CHECK(geometricPtr->convert(outMask2, testId));
			CHECK(!boostMaskAccessor.getMaskAt(testId, tmpMV2));
			CHECK_EQUAL(tmpMV1, tmpMV2);
			CHECK_EQUAL(0, tmpMV1.getRelevantVolumeFraction());
			//CHECK_EQUAL(testId,tmpMV1.getVoxelGridID()); -> return value will not be valid outside the mask



			RETURN_AND_REPORT_TEST_SUCCESS;
		}
	}//testing
}//rttb
