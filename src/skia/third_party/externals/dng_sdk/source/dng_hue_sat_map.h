/*****************************************************************************/
// Copyright 2007 Adobe Systems Incorporated
// All Rights Reserved.
//
// NOTICE:  Adobe permits you to use, modify, and distribute this file in
// accordance with the terms of the Adobe license agreement accompanying it.
/*****************************************************************************/

/* $Id: //mondo/dng_sdk_1_4/dng_sdk/source/dng_hue_sat_map.h#2 $ */ 
/* $DateTime: 2012/07/31 22:04:34 $ */
/* $Change: 840853 $ */
/* $Author: tknoll $ */

/** \file
 * Table-based color correction data structure.
 */

/*****************************************************************************/

#ifndef __dng_hue_sat_map__
#define __dng_hue_sat_map__

/*****************************************************************************/

#include "dng_classes.h"
#include "dng_exceptions.h"
#include "dng_ref_counted_block.h"
#include "dng_safe_arithmetic.h"
#include "dng_types.h"

/*****************************************************************************/

/// \brief A 3D table that maps HSV (hue, saturation, and value) floating-point
/// input coordinates in the range [0,1] to delta signals. The table must have at
/// least 1 sample in the hue dimension, at least 2 samples in the saturation
/// dimension, and at least 1 sample in the value dimension. Tables are stored in
/// value-hue-saturation order.

class dng_hue_sat_map
	{

	public:

		/// HSV delta signal. \param fHueShift is a delta value specified in degrees.
		/// This parameter, added to the original hue, determines the output hue. A
		/// value of 0 means no change. \param fSatScale and \param fValScale are
		/// scale factors that are applied to saturation and value components,
		/// respectively. These scale factors, multiplied by the original saturation
		/// and value, determine the output saturation and value. A scale factor of
		/// 1.0 means no change.

		struct HSBModify
			{
			real32 fHueShift;
			real32 fSatScale;
			real32 fValScale;
			};

	private:

		uint32 fHueDivisions;
		uint32 fSatDivisions;
		uint32 fValDivisions;
		
		uint32 fHueStep;
		uint32 fValStep;

		dng_ref_counted_block fDeltas;

		HSBModify *SafeGetDeltas ()
			{
			return (HSBModify *) fDeltas.Buffer_real32 ();
			}

	public:

		/// Construct an empty (and invalid) hue sat map.

		dng_hue_sat_map ();

		/// Copy an existing hue sat map.

		dng_hue_sat_map (const dng_hue_sat_map &src);

		/// Copy an existing hue sat map.

		dng_hue_sat_map & operator= (const dng_hue_sat_map &rhs);

		/// Destructor.

		virtual ~dng_hue_sat_map ();

		/// Is this hue sat map invalid?

		bool IsNull () const
			{
			return !IsValid ();
			}

		/// Is this hue sat map valid?

		bool IsValid () const
			{
			
			return fHueDivisions > 0 &&
				   fSatDivisions > 1 &&
				   fValDivisions > 0 &&
				   fDeltas.Buffer ();
				   
			}

		/// Clear the hue sat map, making it invalid.

		void SetInvalid ()
			{

			fHueDivisions = 0;
			fSatDivisions = 0;
			fValDivisions = 0;
			
			fHueStep = 0;
			fValStep = 0;

			fDeltas.Clear ();

			}

		/// Get the table dimensions (number of samples in each dimension).

		void GetDivisions (uint32 &hueDivisions,
						   uint32 &satDivisions,
						   uint32 &valDivisions) const
			{
			hueDivisions = fHueDivisions;
			satDivisions = fSatDivisions;
			valDivisions = fValDivisions;
			}
			
		/// Set the table dimensions (number of samples in each dimension). This
		/// erases any existing table data.

		void SetDivisions (uint32 hueDivisions,
						   uint32 satDivisions,
						   uint32 valDivisions = 1);	

		/// Get a specific table entry, specified by table indices.

		void GetDelta (uint32 hueDiv,
					   uint32 satDiv,
					   uint32 valDiv,
					   HSBModify &modify) const;

		/// Make sure the table is writeable.

		void EnsureWriteable ()
			{
			fDeltas.EnsureWriteable ();
			}
		
		/// Set a specific table entry, specified by table indices.

		void SetDelta (uint32 hueDiv,
					   uint32 satDiv,
					   uint32 valDiv,
					   const HSBModify &modify)
			{
			
			EnsureWriteable ();
			
			SetDeltaKnownWriteable (hueDiv,
									satDiv,
									valDiv,
									modify);
			
			}
		
		/// Same as SetDelta, without checking that the table is writeable.

		void SetDeltaKnownWriteable (uint32 hueDiv,
									 uint32 satDiv,
									 uint32 valDiv,
									 const HSBModify &modify);
		
		/// Get the total number of samples (across all dimensions).

		uint32 DeltasCount () const
			{
			uint32 deltaCount;
			if (!SafeUint32Mult(fValDivisions, fHueDivisions, &deltaCount) ||
	    			   !SafeUint32Mult(deltaCount, fSatDivisions, &deltaCount))
				{
				ThrowMemoryFull("Arithmetic overflow computing delta count");
				}
	
			return deltaCount;
			}
		
		/// Direct read/write access to table entries. The entries are stored in
		/// value-hue-saturation order (outer to inner).

		HSBModify *GetDeltas ()
			{
				
			EnsureWriteable ();

			return (HSBModify *) fDeltas.Buffer_real32 ();

			}

		/// Direct read-only access to table entries. The entries are stored in
		/// value-hue-saturation order (outer to inner).

		const HSBModify *GetConstDeltas () const
			{
			return (const HSBModify *) fDeltas.Buffer_real32 ();
			}

		/// Equality test.

		bool operator== (const dng_hue_sat_map &rhs) const;
		
		/// Compute a linearly-interpolated hue sat map (i.e., delta and scale factors)
		/// from the specified tables, with the specified weight. map1 and map2 must
		/// have the same dimensions.

		static dng_hue_sat_map * Interpolate (const dng_hue_sat_map &map1,
											  const dng_hue_sat_map &map2,
											  real64 weight1);

	};

/*****************************************************************************/

#endif

/*****************************************************************************/
