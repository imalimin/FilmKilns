/*****************************************************************************/
// Copyright 2006-2007 Adobe Systems Incorporated
// All Rights Reserved.
//
// NOTICE:  Adobe permits you to use, modify, and distribute this file in
// accordance with the terms of the Adobe license agreement accompanying it.
/*****************************************************************************/

/* $Id: //mondo/dng_sdk_1_4/dng_sdk/source/dng_string.cpp#2 $ */ 
/* $DateTime: 2012/07/31 22:04:34 $ */
/* $Change: 840853 $ */
/* $Author: tknoll $ */

/*****************************************************************************/

#include "dng_string.h"

#include "dng_assertions.h"
#include "dng_exceptions.h"
#include "dng_flags.h"
#include "dng_mutex.h"
#include "dng_utils.h"
#include "dng_safe_arithmetic.h"

#if qMacOS
#include <TargetConditionals.h>
#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
#include <MobileCoreServices/MobileCoreServices.h>
#else
#include <CoreServices/CoreServices.h>
#endif  // TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
#endif  // qMacOS

#if qWinOS
#include <windows.h>
#endif

#if qiPhone || qAndroid || qLinux
#include <ctype.h> // for isdigit
#endif

/*****************************************************************************/

const uint32 kREPLACEMENT_CHARACTER	= 0x0000FFFD;

/*****************************************************************************/

// Returns the length of the zero-terminated string 's'. Throws a dng_exception
// if the length of 's' is too large to be represented as a uint32_t.
static uint32 strlenAsUint32(const char *s)
	{
	
	uint32 lengthAsUint32 = 0;
	ConvertUnsigned(strlen(s), &lengthAsUint32);
	
	return lengthAsUint32;
	
	}

// Checks whether there is enough space left in the buffer pointed to by
// 'currentPos' to write at least 'space' elements of type T (to positions
// currentPos[0] through currentPos[space - 1]. Throws a dng_exception if there
// is not enough space left in the buffer.
// 'bufferEnd' should point one element beyond the end of the buffer. For
// example, if the buffer is "T buffer[3];", then bufferEnd should point to
// T + 3.
template <class T>
static void CheckSpaceLeftInBuffer(const T *currentPos,
								   const T *bufferEnd,
								   size_t space)
	{
	
	if (bufferEnd < currentPos || static_cast<size_t>(bufferEnd - currentPos) < space)
		{
		ThrowMemoryFull ("Buffer overrun");
		}
	
	}

/*****************************************************************************/

// Throws an exception to notify the user of code that has not been security
// hardened and prevent execution of that code.
//
// Though the DNG SDK in general has been security-hardened, this does not apply
// to the following Mac-OS- and Windows-specific functions. Calls to
// ThrowNotHardened() have been added to these functions to alert callers of
// this fact.
//
// If you're trying to use a function that calls ThrowNotHardened(), you need to
// fix the security issues noted in the comment next to the ThrowNotHardened()
// call. Once you have fixed these issues, obtain a security review for the
// fixes. This may require fuzzing of the modified code on the target platform.
static void ThrowNotHardened()
	{
	ThrowProgramError ("This function has not been security-hardened");
	}

#if qMacOS
#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR

static uint32 Extract_SystemEncoding (const dng_string &dngString,
							   		  dng_memory_data &buffer)
	{
		// TODO: Needs implementation.
		ThrowProgramError ("Extract_SystemEncoding() not implemented on iOS");
		return 0;
	}

static void Assign_SystemEncoding (dng_string &dngString,
							       const char *otherString)
	{
		// TODO: Needs implementation.
		ThrowProgramError ("Assign_SystemEncoding() not implemented on iOS");

	}

static void Assign_JIS_X208_1990 (dng_string &dngString,
							      const char *otherString)
	{
		// TODO: Needs implementation.
		ThrowProgramError ("Assign_JIS_X208_1990() not implemented on iOS");
	}

#else

static void Assign_Multibyte (dng_string &dngString,
							  const char *otherString,
							  TextEncoding encoding)
	{
	
	// This function contains security-vulnerable code. Do not use.
	// The particular vulnerabilities are:
	// - Casting the result of strlen() to a uint32 may case truncation. (Use
	//   strlenAsUint32() instead.)
	// - The computation of aBufSize and the subsequent addition of 1 in the
	//   call to the dng_memory_data constructor may wrap around.
	ThrowNotHardened();
	
	uint32 aSize = (uint32) strlen (otherString);
	
	if (aSize > 0)
		{
		
		uint32 aBufSize = aSize * 6 + 256;
		
		dng_memory_data aBuf (aBufSize + 1);
		
		UnicodeMapping aMapping;
		
		aMapping.unicodeEncoding = ::CreateTextEncoding (kTextEncodingUnicodeV3_0,
														 kUnicodeNoSubset,
														 kUnicodeUTF8Format);
														 
		aMapping.otherEncoding   = encoding;
		aMapping.mappingVersion  = kUnicodeUseLatestMapping;

		TextToUnicodeInfo aInfo = NULL;
		
		if (::CreateTextToUnicodeInfo (&aMapping, &aInfo) == noErr)
			{
		
			ByteCount aInput  = 0;
			ByteCount aOutput = 0;
			
			::ConvertFromTextToUnicode (aInfo,
										aSize,
									    otherString,
									    kUnicodeUseFallbacksMask |
									    kUnicodeLooseMappingsMask,
									    0,
									    NULL,
									    NULL,
									    NULL,
									    aBufSize,
									    &aInput,
									    &aOutput,
									    (UniChar *) aBuf.Buffer ());
									   
			::DisposeTextToUnicodeInfo (&aInfo);
			
			if (aOutput > 0 && aOutput <= aBufSize)
				{
			
				char *aBufChar = aBuf.Buffer_char ();
			
				aBufChar [aOutput] = 0;
				
				dngString.Set (aBufChar);
				
				return;
				
				}
							
			}

		}
		
	dngString.Clear ();

	}

static uint32 Extract_Multibyte (const dng_string &dngString,
							     dng_memory_data &buffer,
							     TextEncoding encoding)
	{
	
	// This function contains security-vulnerable code. Do not use.
	// The particular vulnerabilities are:
	// - The computation of aBufSize may wrap around.
	// - The computation of the argument to buffer.Allocate() may overflow; the
	//   conversion to uint32 is also problematic.
	// - The signed-to-unsigned conversion in the return statement "
	//   return (uint32) aOutput;" may be problematic.
	ThrowNotHardened();
	
	uint32 aSize = dngString.Length ();
	
	if (aSize > 0)
		{
	
		uint32 aBufSize = (aSize * 2) + 256;
		
		dng_memory_data tempBuffer (aBufSize);
		
		UnicodeMapping aMapping;
		
		aMapping.unicodeEncoding = ::CreateTextEncoding (kTextEncodingUnicodeV3_0,
														 kUnicodeNoSubset,
														 kUnicodeUTF8Format);
														 
		aMapping.otherEncoding   = encoding;
		aMapping.mappingVersion  = kUnicodeUseLatestMapping;

		UnicodeToTextInfo aInfo = NULL;
		
		if (::CreateUnicodeToTextInfo (&aMapping, &aInfo) == noErr)
			{
			
			ByteCount aInput  = 0;
			ByteCount aOutput = 0;
			
			::ConvertFromUnicodeToText (aInfo,
										aSize,
										(const UniChar *) dngString.Get (),
									    kUnicodeUseFallbacksMask  |
									    kUnicodeLooseMappingsMask |
									    kUnicodeDefaultDirectionMask,
									    0,
									    NULL,
									    NULL,
									    NULL,
									    aBufSize,
									    &aInput,
									    &aOutput,
									    tempBuffer.Buffer_char ());
									    
			::DisposeUnicodeToTextInfo (&aInfo);
			
			if (aOutput > 0)
				{
				
				buffer.Allocate ((uint32) (aOutput + 1));
				
				memcpy (buffer.Buffer (),
						tempBuffer.Buffer (),
						aOutput);
						
				buffer.Buffer_char () [aOutput] = 0;
				
				return (uint32) aOutput;
				
				}
				
			}
			
		}
		
	buffer.Allocate (1);
	
	buffer.Buffer_char () [0] = 0;
	
	return 0;
	
	}
	
static void Assign_SystemEncoding (dng_string &dngString,
							       const char *otherString)
	{ 
	
	TextEncoding aEncoding;
	
	::UpgradeScriptInfoToTextEncoding (smSystemScript,
									   kTextLanguageDontCare,
									   kTextRegionDontCare,
									   NULL,
									   &aEncoding);
									   
	Assign_Multibyte (dngString,
					  otherString,
					  aEncoding);

	}
	
static uint32 Extract_SystemEncoding (const dng_string &dngString,
							   		  dng_memory_data &buffer)
	{ 
	
	TextEncoding aEncoding;
	
	::UpgradeScriptInfoToTextEncoding (smSystemScript,
									   kTextLanguageDontCare,
									   kTextRegionDontCare,
									   NULL,
									   &aEncoding);
									   
	return Extract_Multibyte (dngString,
					   		  buffer,
					   		  aEncoding);

	}
	
static void Assign_JIS_X208_1990 (dng_string &dngString,
							      const char *otherString)
	{
	
	Assign_Multibyte (dngString,
					  otherString,
					  kTextEncodingJIS_X0208_90);

	}

#endif  // TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
#endif  // qMacOS

/*****************************************************************************/

#if qWinOS

static void Assign_Multibyte (dng_string &dngString,
							  const char *otherString,
							  UINT encoding)
	{
	
	// This function contains security-vulnerable code. Do not use.
	// The particular vulnerabilities are:
	// - Converting the return value of strlen() to int may cause overflow.
	// - The computation of aBufChars and of the argument to the dng_memory_data
	//   constructor may overflow. Additionally, there is an implicit
	//   signed-to-unsigned conversion in the call to the dng_memory_data
	//   constructor.
	ThrowNotHardened();
	
	DNG_ASSERT (sizeof (WCHAR) == 2, "WCHAR must be 2 bytes");
	
	int aSize = (int) strlen (otherString);
	
	if (aSize > 0)
		{
		
		int aBufChars = aSize * 3 + 128;
		
		dng_memory_data aBuf ((aBufChars + 1) << 1);
		
		int aResult = ::MultiByteToWideChar (encoding,
											 0,
											 otherString,
											 aSize,
											 (WCHAR *) aBuf.Buffer (),
											 aBufChars);
											 
		if (aResult > 0 && aResult <= aBufChars)
			{
			
			uint16 * aUTF16 = aBuf.Buffer_uint16 ();
			
			aUTF16 [aResult] = 0;
			
			dngString.Set_UTF16 (aUTF16);
		
			return;
			
			}
			
		}
		
	dngString.Clear ();
	
	}

static uint32 Extract_Multibyte (const dng_string &dngString,
							     dng_memory_data &buffer,
							     UINT encoding)
	{
	
	// This function contains security-vulnerable code. Do not use.
	// The particular vulnerabilities are:
	// - Converting the return value of dngString.Get_UTF16() may cause
	//   overflow.
	// - The computation of dBufSize may overflow.
	// - The calls to the dng_memory_data constructor and to buffer.Allocate()
	//   trigger implicit conversions of int to uint32 that may be problematic.
	// - The memcpy() call triggers an implicit conversion of aResult to a
	//   size_t, which may be problematic.
	// - The conversion of aResult to a uint32 in the return statement may be
	//   problematic.
	ThrowNotHardened();
	
	DNG_ASSERT (sizeof (WCHAR) == 2, "WCHAR must be 2 bytes");
	
	dng_memory_data sBuffer;
	
	int aCount = dngString.Get_UTF16 (sBuffer);
	
	int dBufSize = aCount * 2 + 256;
	
	dng_memory_data dBuffer (dBufSize);
	
	int aResult = ::WideCharToMultiByte (encoding,
										 0,
										 (WCHAR *) sBuffer.Buffer (),
										 aCount,
										 dBuffer.Buffer_char (),
										 dBufSize,
										 NULL,
										 NULL);
										 
	if (aResult < 0)
		aResult = 0;
		
	buffer.Allocate (aResult + 1);
	
	memcpy (buffer.Buffer (),
			dBuffer.Buffer (),
			aResult);
			
	buffer.Buffer_char () [aResult] = 0;

	return (uint32) aResult;
	
	}

static void Assign_SystemEncoding (dng_string &dngString,
							       const char *otherString)
	{ 
	
	Assign_Multibyte (dngString,
					  otherString,
					  ::GetACP ());

	}
	
static uint32 Extract_SystemEncoding (const dng_string &dngString,
							   		  dng_memory_data &buffer)
	{ 
	
	return Extract_Multibyte (dngString,
					   		  buffer,
					   		  ::GetACP ());

	}
	
static void Assign_JIS_X208_1990 (dng_string &dngString,
							      const char *otherString)
	{
	
	// From MSDN documentation: 20932 = JIS X 0208-1990 & 0121-1990
	
	const UINT kJIS = 20932;
	
	Assign_Multibyte (dngString,
					  otherString,
					  kJIS);

	}

#endif

/*****************************************************************************/

static bool IsASCII (const char *s)
	{
	
	if (!s)
		{
		
		return true;
		
		}
	
	while (true)
		{
		
		uint8 c = (uint8) *(s++);
		
		if (c == 0)
			{
			
			break;
			
			}
		
		if (c & 0x80)
			{
			
			return false;
			
			}
			
		}
		
	return true;
	
	}

/*****************************************************************************/

dng_string::dng_string ()

	:	fData ()
	
	{
	
	}

/*****************************************************************************/

dng_string::dng_string (const dng_string &s)

	:	fData ()
	
	{
	
	Set (s.Get ());
	
	}

/*****************************************************************************/

dng_string & dng_string::operator= (const dng_string &s)
	{
	
	if (this != &s)
		{
		
		Set (s.Get ());
				
		}
		
	return *this;
	
	}

/*****************************************************************************/

dng_string::~dng_string ()
	{
	
	}
				
/*****************************************************************************/

const char * dng_string::Get () const
	{
	
	if (fData.Buffer ())
		{
		
		return fData.Buffer_char ();
		
		}
		
	return "";
	
	}

/*****************************************************************************/

bool dng_string::IsASCII () const
	{
	
	return ::IsASCII (Get ());
		
	}
		
/*****************************************************************************/

void dng_string::Set (const char *s)
	{
	
	// Measure the new length.
	
	uint32 newLen = (s != NULL ? strlenAsUint32 (s) : 0);
	
	// If it is a NULL string, then clear the buffer.
	
	if (newLen == 0)
		{
		
		fData.Clear ();
		
		}
		
	// Else we need to copy the bytes.
	
	else
		{
	
		uint32 oldLen = Length ();
			
		// We might be setting this string to a sub-string of itself,
		// so don't reallocate the data unless the string is getting
		// longer.
		
		if (newLen > oldLen)
			{
			
			fData.Clear ();
			
			fData.Allocate (SafeUint32Add (newLen, 1));
			
			}
			
		char *d = fData.Buffer_char ();
			
		for (uint32 k = 0; k <= newLen; k++)
			{
			
			d [k] = s [k];
			
			}
			
		}
				
	}

/*****************************************************************************/

void dng_string::Set_ASCII (const char *s)
	{
	
	if (::IsASCII (s))
		{
		
		Set (s);
		
		}
		
	else
		{
	
		Set_SystemEncoding (s);

		}
	
	}
		
/*****************************************************************************/

void dng_string::Set_UTF8 (const char *s)
	{
	
	uint32 len = strlenAsUint32 (s);
	
	const char *sEnd = s + len;
	
	// Worst case expansion is 1-byte characters expanding to
	// replacement character, which requires 3 bytes.
	
	const uint32 destBufferLength = SafeUint32Add (SafeUint32Mult (len, 3), 1);
	dng_memory_data buffer (destBufferLength);
	
	uint8 *d = buffer.Buffer_uint8 ();
	uint8 * const destEnd = d + destBufferLength;
	
	while (s < sEnd)
		{
		
		uint32 aChar = DecodeUTF8 (s, (uint32) (sEnd - s));
		
		if (aChar > 0x7FFFFFFF)
			{
			aChar = kREPLACEMENT_CHARACTER;
			}
			
		#if qDNGValidate
		
		if (aChar == kREPLACEMENT_CHARACTER)
			{
			ReportWarning ("Expected UTF-8 value is not valid UTF-8 (or contains a kREPLACEMENT_CHARACTER)");
			}
		
		#endif
			
		if (aChar < 0x00000080) 
			{
			CheckSpaceLeftInBuffer (d, destEnd, 1);
			*(d++) = (uint8) aChar;
			}
			
		else if (aChar < 0x00000800)
			{
			CheckSpaceLeftInBuffer (d, destEnd, 2);
			*(d++) = (uint8) ((aChar >> 6) | 0x000000C0);
			*(d++) = (uint8) ((aChar & 0x0000003F) | 0x00000080);
			}
			
		else if (aChar < 0x00010000)
			{
			CheckSpaceLeftInBuffer (d, destEnd, 3);
			*(d++) = (uint8) ( (aChar >> 12) | 0x000000E0);
			*(d++) = (uint8) (((aChar >>  6) & 0x0000003F) | 0x00000080);
			*(d++) = (uint8) ( (aChar & 0x0000003F) | 0x00000080);
			}
			
		else if (aChar < 0x00200000)
			{
			CheckSpaceLeftInBuffer (d, destEnd, 4);
			*(d++) = (uint8) ( (aChar >> 18) | 0x000000F0);
			*(d++) = (uint8) (((aChar >> 12) & 0x0000003F) | 0x00000080);
			*(d++) = (uint8) (((aChar >>  6) & 0x0000003F) | 0x00000080);
			*(d++) = (uint8) ( (aChar & 0x0000003F) | 0x00000080);
			}
			
		else if (aChar < 0x04000000)
			{
			CheckSpaceLeftInBuffer (d, destEnd, 5);
			*(d++) = (uint8) ( (aChar >> 24) | 0x000000F8);
			*(d++) = (uint8) (((aChar >> 18) & 0x0000003F) | 0x00000080);
			*(d++) = (uint8) (((aChar >> 12) & 0x0000003F) | 0x00000080);
			*(d++) = (uint8) (((aChar >>  6) & 0x0000003F) | 0x00000080);
			*(d++) = (uint8) ( (aChar & 0x0000003F) | 0x00000080);
			}
			
		else
			{
			CheckSpaceLeftInBuffer (d, destEnd, 6);
			*(d++) = (uint8) ( (aChar >> 30) | 0x000000FC);
			*(d++) = (uint8) (((aChar >> 24) & 0x0000003F) | 0x00000080);
			*(d++) = (uint8) (((aChar >> 18) & 0x0000003F) | 0x00000080);
			*(d++) = (uint8) (((aChar >> 12) & 0x0000003F) | 0x00000080);
			*(d++) = (uint8) (((aChar >>  6) & 0x0000003F) | 0x00000080);
			*(d++) = (uint8) ( (aChar & 0x0000003F) | 0x00000080);
			}
			
		}
		
	CheckSpaceLeftInBuffer (d, destEnd, 1);
	*d = 0;
	
	Set (buffer.Buffer_char ());
	
	}
	
/*****************************************************************************/

uint32 dng_string::Get_SystemEncoding (dng_memory_data &buffer) const
	{
	
	if (IsASCII ())
		{
		
		uint32 len = Length ();
		
		const uint32 destBufferLength = SafeUint32Add (len, 1);
		buffer.Allocate (destBufferLength);

		memcpy (buffer.Buffer (), Get (), destBufferLength);
		
		return len;
		
		}
		
	else
		{
		
		#if qMacOS || qWinOS
		
		return Extract_SystemEncoding (*this, buffer);
		
		#else
		
		// Fallback logic to force the string to ASCII.
		
		dng_string temp (*this);
		
		temp.ForceASCII ();
		
		return temp.Get_SystemEncoding (buffer);
		
		#endif
		
		}
	
	}
		
/*****************************************************************************/

void dng_string::Set_SystemEncoding (const char *s)
	{
	
	if (::IsASCII (s))
		{
		
		Set (s);
		
		}
		
	else
		{
		
		#if qMacOS || qWinOS
	
		Assign_SystemEncoding (*this, s);
		
		#else
		
		// Fallback logic that just grabs the ASCII characters and
		// ignores the non-ASCII characters.
		
		uint32 len = strlenAsUint32 (s);
		
		const uint32 destBufferLength = SafeUint32Add (len, 1);
		dng_memory_data buffer (destBufferLength);
		
		uint8 *d = buffer.Buffer_uint8 ();
		uint8 * const destEnd = d + destBufferLength;
		
		while (*s)
			{
			
			uint8 c = (uint8) *(s++);
			
			if ((c & 0x80) == 0)
				{
				
				CheckSpaceLeftInBuffer (d, destEnd, 1);
				*(d++) = c;
				
				}
				
			}
			
		CheckSpaceLeftInBuffer (d, destEnd, 1);
		*d = 0;
		
		Set (buffer.Buffer_char ());
			
		#endif
	
		}

	}
		
/*****************************************************************************/

bool dng_string::ValidSystemEncoding () const
	{
	
	if (IsASCII ())
		{
		
		return true;
		
		}
		
	dng_memory_data buffer;
	
	Get_SystemEncoding (buffer);
	
	dng_string temp;
	
	temp.Set_SystemEncoding (buffer.Buffer_char ());
	
	return (*this == temp);
	
	}
		
/*****************************************************************************/

void dng_string::Set_JIS_X208_1990 (const char *s)
	{
	
	if (::IsASCII (s))
		{
		
		Set (s);
		
		}
		
	else
		{
		
		#if qMacOS || qWinOS
	
		Assign_JIS_X208_1990 (*this, s);
		
		#else
		
		// Fallback to the ASCII extraction logic.
		
		Set_SystemEncoding (s);
		
		#endif
	
		}
	
	}

/*****************************************************************************/

uint32 dng_string::DecodeUTF8 (const char *&s,
							   uint32 maxBytes,
							   bool *isValid)
	{
	
	static const uint8 gUTF8Bytes [256] =
		{
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
		3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0
		};
		
	if (isValid)
		{
		*isValid = true;
		}
			
	const uint8 *nBuf = (const uint8 *) s;
	
	uint32 aChar = nBuf [0];
	
	uint32 aSize = gUTF8Bytes [aChar];
	
	if (aSize > maxBytes)
		{
		
		s += maxBytes;
		
		if (isValid)
			{
			*isValid = false;
			}
			
		return kREPLACEMENT_CHARACTER;
		
		}
		
	s += aSize;

	for (uint32 extra = 1; extra < aSize; extra++)
		{
		
		if ((nBuf [extra] & 0xC0) != 0x80)
			{
			
			if (isValid)
				{
				*isValid = false;
				}
				
			return kREPLACEMENT_CHARACTER;

			}
		
		}
	
	switch (aSize)
		{
		
		case 0:
			{
			
			s++;		// Don't get stuck in infinite loop
			
			if (isValid)
				{
				*isValid = false;
				}
			
			return kREPLACEMENT_CHARACTER;
			
			}
			
		case 1:
			{
			
			return aChar;
			
			}
			
		case 2:
			{
			
			aChar = ((aChar << 6) + nBuf [1]) - (uint32) 0x00003080UL;
			
			break;
			
			}
			
		case 3:
			{
			
			aChar =  ((((aChar << 6) + nBuf [1])
							   << 6) + nBuf [2]) - (uint32) 0x000E2080UL;
							   
			break;
			
			}
							 
		case 4:
			{
			
			aChar = ((((((aChar << 6) + nBuf [1])
							    << 6) + nBuf [2])
								<< 6) + nBuf [3]) - (uint32) 0x03C82080UL;
								
			break;
			
			}
		}
		
	if (aChar < 0x7F || aChar > 0x0010FFFF)
		{
		
		if (isValid)
			{
			*isValid = false;
			}
		
		return kREPLACEMENT_CHARACTER;
			
		}
	
	return aChar;
	
	}

/*****************************************************************************/

bool dng_string::IsUTF8 (const char *s)
	{
	
	uint32 len = strlenAsUint32 (s);
	
	const char *sEnd = s + len;
	
	while (s < sEnd)
		{
		
		bool isValid = true;
		
		(void) DecodeUTF8 (s, (uint32) (sEnd - s), &isValid);
		
		if (!isValid)
			{
			return false;
			}
			
		}
	
	return true;
	
	}

/*****************************************************************************/

void dng_string::Set_UTF8_or_System (const char *s)
	{
	
	if (::IsASCII (s))
		{
		
		Set (s);
		
		}
		
	else if (IsUTF8 (s))
		{
		
		Set_UTF8 (s);
		
		}
		
	else
		{
		
		Set_SystemEncoding (s);
		
		}
	
	}

/*****************************************************************************/

uint32 dng_string::Get_UTF16 (dng_memory_data &buffer) const
	{
	
	uint32 count = 0;
	
	const char *sPtr = Get ();
	
	while (*sPtr)
		{
		
		uint32 x = DecodeUTF8 (sPtr);
		
		if (x <= 0x0000FFFF ||
			x >  0x0010FFFF)
			{
			
			count = SafeUint32Add (count, 1);
			
			}
			
		else
			{
			
			count = SafeUint32Add (count, 2);
			
			}
			
		}

	const uint32 destBufferLength = SafeUint32Add (count, 1);
	buffer.Allocate (destBufferLength, sizeof (uint16));
	
	uint16 *dPtr = buffer.Buffer_uint16 ();
	uint16 * const destEnd = dPtr + destBufferLength;
	
	sPtr = Get ();
	
	while (*sPtr)
		{
		
		uint32 x = DecodeUTF8 (sPtr);
		
		if (x <= 0x0000FFFF)
			{
			
			CheckSpaceLeftInBuffer (dPtr, destEnd, 1);
			*(dPtr++) = (uint16) x;
			
			}
		
		else if (x > 0x0010FFFF)
			{
			
			CheckSpaceLeftInBuffer (dPtr, destEnd, 1);
			*(dPtr++) = (uint16) kREPLACEMENT_CHARACTER;
			
			}
			
		else
			{
			
			x -= 0x00010000;
			
			CheckSpaceLeftInBuffer (dPtr, destEnd, 2);
			*(dPtr++) = (uint16) ((x >> 10       ) + 0x0000D800);
			*(dPtr++) = (uint16) ((x & 0x000003FF) + 0x0000DC00);
			
			}
			
		}
		
	CheckSpaceLeftInBuffer (dPtr, destEnd, 1);
	*dPtr = 0;
	
	return count;

	}
		
/*****************************************************************************/

void dng_string::Set_UTF16 (const uint16 *s)
	{
	
	if (!s)
		{
		Clear ();
		return;
		}
	
	bool swap = false;
	
	if (s [0] == 0xFFFE)		// Swapped byte order marker
		{
		swap = true;
		s++;
		}
		
	else if (s [0] == 0xFEFF)	// Non-swapped byte order marker
		{
		s++;
		}

	uint32 length16 = 0;
	
	while (s [length16] != 0)
		{
		length16 = SafeUint32Add (length16, 1);
		}
		
	const uint16 *sEnd = s + length16;
	
	const uint32 destBufferSize =
		SafeUint32Add (SafeUint32Mult (length16, 6), 1);
	dng_memory_data buffer (destBufferSize);
	
	uint8 *d = buffer.Buffer_uint8 ();
	uint8 * const destEnd = d + destBufferSize;
	
	while (s < sEnd)
		{
		
		uint32 aChar = *s++;
		
		if (swap)
			{
			aChar = ((aChar << 8) | (aChar >> 8)) & 0x0000FFFF;
			}
			
		if ((aChar >= 0x0000D800) && (aChar <= 0x0000DBFF) && (s < sEnd)) 
			{
				
			uint32 aLow = *s;
				
			if (swap)
				{
				aLow = ((aLow << 8) | (aLow >> 8)) & 0x0000FFFF;
				}
			
			if ((aLow >= 0x0000DC00) && (aLow <= 0x0000DFFF))
				{
				
				aChar = ((aChar - 0x0000D800) << 10) +
					    (aLow - 0x0000DC00) +
					    0x00010000;
				
				s++;
				
				}
				
			}
			
		if (aChar > 0x7FFFFFFF)
			{
			aChar = kREPLACEMENT_CHARACTER;
			}
			
		if (aChar < 0x00000080) 
			{
			CheckSpaceLeftInBuffer (d, destEnd, 1);
			*(d++) = (uint8) aChar;
			}
			
		else if (aChar < 0x00000800)
			{
			CheckSpaceLeftInBuffer (d, destEnd, 2);
			*(d++) = (uint8) ((aChar >> 6) | 0x000000C0);
			*(d++) = (uint8) ((aChar & 0x0000003F) | 0x00000080);
			}
			
		else if (aChar < 0x00010000)
			{
			CheckSpaceLeftInBuffer (d, destEnd, 3);
			*(d++) = (uint8) ( (aChar >> 12) | 0x000000E0);
			*(d++) = (uint8) (((aChar >>  6) & 0x0000003F) | 0x00000080);
			*(d++) = (uint8) ( (aChar & 0x0000003F) | 0x00000080);
			}
			
		else if (aChar < 0x00200000)
			{
			CheckSpaceLeftInBuffer (d, destEnd, 4);
			*(d++) = (uint8) ( (aChar >> 18) | 0x000000F0);
			*(d++) = (uint8) (((aChar >> 12) & 0x0000003F) | 0x00000080);
			*(d++) = (uint8) (((aChar >>  6) & 0x0000003F) | 0x00000080);
			*(d++) = (uint8) ( (aChar & 0x0000003F) | 0x00000080);
			}
			
		else if (aChar < 0x04000000)
			{
			CheckSpaceLeftInBuffer (d, destEnd, 5);
			*(d++) = (uint8) ( (aChar >> 24) | 0x000000F8);
			*(d++) = (uint8) (((aChar >> 18) & 0x0000003F) | 0x00000080);
			*(d++) = (uint8) (((aChar >> 12) & 0x0000003F) | 0x00000080);
			*(d++) = (uint8) (((aChar >>  6) & 0x0000003F) | 0x00000080);
			*(d++) = (uint8) ( (aChar & 0x0000003F) | 0x00000080);
			}
			
		else
			{
			CheckSpaceLeftInBuffer (d, destEnd, 6);
			*(d++) = (uint8) ( (aChar >> 30) | 0x000000FC);
			*(d++) = (uint8) (((aChar >> 24) & 0x0000003F) | 0x00000080);
			*(d++) = (uint8) (((aChar >> 18) & 0x0000003F) | 0x00000080);
			*(d++) = (uint8) (((aChar >> 12) & 0x0000003F) | 0x00000080);
			*(d++) = (uint8) (((aChar >>  6) & 0x0000003F) | 0x00000080);
			*(d++) = (uint8) ( (aChar & 0x0000003F) | 0x00000080);
			}
			
		}
		
	CheckSpaceLeftInBuffer (d, destEnd, 1);
	*d = 0;
	
	Set (buffer.Buffer_char ());
	
	}
				
/*****************************************************************************/

void dng_string::Clear ()
	{
	
	Set (NULL);
	
	}
		
/*****************************************************************************/

void dng_string::Truncate (uint32 maxBytes)
	{
	
	uint32 len = Length ();
	
	if (len > maxBytes)
		{
		
		uint8 *s = fData.Buffer_uint8 ();
		
		// Don't truncate on an extension character.  Extensions characters
		// in UTF-8 have the 0x80 bit set and the 0x40 bit clear.
		
		while (maxBytes > 0 && ((s [maxBytes]) & 0xC0) == 0x80)
			{
			
			maxBytes--;
			
			}
			
		s [maxBytes] = 0;
		
		}
	
	}
		
/*****************************************************************************/

bool dng_string::TrimTrailingBlanks ()
	{
	
	bool didTrim = false;
	
	if (fData.Buffer ())
		{
		
		char *s = fData.Buffer_char ();
		
		uint32 len = strlenAsUint32 (s);
		
		while (len > 0 && s [len - 1] == ' ')
			{
			len--;
			didTrim = true;
			}
			
		s [len] = 0;
		
		}
		
	return didTrim;
		
	}
				
/*****************************************************************************/

bool dng_string::TrimLeadingBlanks ()
	{
	
	bool didTrim = false;
	
	const char *s = Get ();
	
	while (*s == ' ')
		{
		s++;
		didTrim = true;
		}
		
	if (didTrim)
		{
		Set (s);
		}
		
	return didTrim;
		
	}
				
/*****************************************************************************/

bool dng_string::IsEmpty () const
	{
	
	const char *s = Get ();
	
	return *s == 0;
	
	}
		
/*****************************************************************************/

uint32 dng_string::Length () const
	{
	
	const char *s = Get ();
	
	return strlenAsUint32 (s);
	
	}

/*****************************************************************************/

bool dng_string::operator== (const dng_string &s) const
	{
	
	const char *s1 =   Get ();
	const char *s2 = s.Get ();
	
	return strcmp (s1, s2) == 0; 
	
	}

/*****************************************************************************/

bool dng_string::Matches (const char *t,
						  const char *s,
						  bool case_sensitive)
	{
	
	while (*s != 0)
		{
		
		char c1 = *(s++);
		char c2 = *(t++);
		
		if (!case_sensitive)
			{
			c1 = ForceUppercase (c1);
			c2 = ForceUppercase (c2);
			}
			
		if (c1 != c2)
			{
			return false;
			}
			
		}
		
	return (*t == 0);
	
	}

/*****************************************************************************/

bool dng_string::Matches (const char *s,
						  bool case_sensitive) const
	{
	
	return dng_string::Matches (Get (), s, case_sensitive);
	
	}

/*****************************************************************************/

bool dng_string::StartsWith (const char *s,
						     bool case_sensitive) const
	{
	
	const char *t = Get ();
	
	while (*s != 0)
		{
		
		char c1 = *(s++);
		char c2 = *(t++);
		
		if (!case_sensitive)
			{
			c1 = ForceUppercase (c1);
			c2 = ForceUppercase (c2);
			}
			
		if (c1 != c2)
			{
			return false;
			}
			
		}
		
	return true;
	
	}

/*****************************************************************************/

bool dng_string::EndsWith (const char *s,
						   bool case_sensitive) const
	{
	
	uint32 len1 = Length ();
	
	uint32 len2 = strlenAsUint32 (s);
	
	if (len1 < len2)
		{
		return false;
		}
		
	const char *t = Get () + (len1 - len2);
	
	while (*s != 0)
		{
		
		char c1 = *(s++);
		char c2 = *(t++);
		
		if (!case_sensitive)
			{
			c1 = ForceUppercase (c1);
			c2 = ForceUppercase (c2);
			}
			
		if (c1 != c2)
			{
			return false;
			}
			
		}
		
	return true;
	
	}

/*****************************************************************************/

bool dng_string::Contains (const char *s,
						   bool case_sensitive,
						   int32 *match_offset) const
	{
	
	if (match_offset)
		{
		*match_offset = -1;
		}
	
	uint32 len1 = Length ();
	
	uint32 len2 = strlenAsUint32 (s);
	
	if (len1 < len2)
		{
		return false;
		}
		
	uint32 offsets = len1 - len2;
		
	for (uint32 offset = 0; offset <= offsets; offset++)
		{
		
		const char *ss = s;
		const char *tt = Get () + offset;
		
		while (*ss != 0)
			{
			
			char c1 = *(ss++);
			char c2 = *(tt++);
			
			if (!case_sensitive)
				{
				c1 = ForceUppercase (c1);
				c2 = ForceUppercase (c2);
				}
				
			if (c1 != c2)
				{
				goto tryNextOffset;
				}
				
			}
			
		if (match_offset)
			{
			*match_offset = offset;
			}
	
		return true;
		
		tryNextOffset:	;
		
		}
		
	return false;
	
	}
		
/*****************************************************************************/

bool dng_string::Replace (const char *old_string,
						  const char *new_string,
						  bool case_sensitive)
	{
	
	int32 match_offset = -1;
	
	if (Contains (old_string,
				  case_sensitive,
				  &match_offset))
		{
		
		uint32 len1 = Length ();
		
		uint32 len2 = strlenAsUint32 (old_string);
		uint32 len3 = strlenAsUint32 (new_string);
		
		if (len2 == len3)
			{
			
			strncpy (fData.Buffer_char () + match_offset,
					 new_string,
					 len3);
			
			}
			
		else if (len2 > len3)
			{
			
			strncpy (fData.Buffer_char () + match_offset,
					 new_string,
					 len3);
					 
			const char *s = fData.Buffer_char () + match_offset + len2;
				  char *d = fData.Buffer_char () + match_offset + len3;
				  
			uint32 extra = len1 - match_offset - len2 + 1;	// + 1 for NULL termination
			
			for (uint32 j = 0; j < extra; j++)
				{
				*(d++) = *(s++);
				}
			
			}
			
		else
			{
			
			// "len1 - len2" cannot wrap around because we know that if this
			// string contains old_string, len1 >= len2 must hold.
			dng_memory_data tempBuffer (
				SafeUint32Add (SafeUint32Add (len1 - len2, len3), 1));
			
			if (match_offset)
				{
				
				strncpy (tempBuffer.Buffer_char (),
						 fData     .Buffer_char (),
						 match_offset);
						 
				}
				
			if (len3)
				{

				strncpy (tempBuffer.Buffer_char () + match_offset,
						 new_string,
						 len3);
						 
				}
				
			uint32 extra = len1 - match_offset - len2 + 1;	// + 1 for NULL termination
			
			strncpy (tempBuffer.Buffer_char () + match_offset + len3,
					 fData     .Buffer_char () + match_offset + len2,
					 extra);
					 
			Set (tempBuffer.Buffer_char ());

			}
			
		return true;
		
		}
		
	return false;
	
	}
		
/*****************************************************************************/

bool dng_string::TrimLeading (const char *s,
						      bool case_sensitive)
	{
	
	if (StartsWith (s, case_sensitive))
		{
		
		Set (Get () + strlenAsUint32 (s));
		
		return true;
		
		}
	
	return false;
	
	}

/*****************************************************************************/

void dng_string::Append (const char *s)
	{
	
	uint32 len2 = strlenAsUint32 (s);
	
	if (len2)
		{
	
		uint32 len1 = Length ();
		
		dng_memory_data temp (SafeUint32Add (SafeUint32Add (len1, len2), 1));
		
		char *buffer = temp.Buffer_char ();
		
		if (len1)
			{
			memcpy (buffer, Get (), len1);
			}
			
		memcpy (buffer + len1, s, len2 + 1);
		
		Set (buffer);
		
		}
	
	}
		
/*****************************************************************************/

void dng_string::SetUppercase ()
	{
	
	if (fData.Buffer ())
		{
		
		uint32 len = Length ();
		
		char *dPtr = fData.Buffer_char ();
		
		for (uint32 j = 0; j < len; j++)
			{
			
			char c = dPtr [j];
			
			if (c >= 'a' && c <= 'z')
				{
				
				dPtr [j] = c - 'a' + 'A';
				
				}
				
			}
			
		}
		
	}
		
/*****************************************************************************/

void dng_string::SetLowercase ()
	{
	
	if (fData.Buffer ())
		{
		
		uint32 len = Length ();
		
		char *dPtr = fData.Buffer_char ();
		
		for (uint32 j = 0; j < len; j++)
			{
			
			char c = dPtr [j];
			
			if (c >= 'A' && c <= 'Z')
				{
				
				dPtr [j] = c - 'A' + 'a';
				
				}
				
			}
			
		}
		
	}
		
/*****************************************************************************/

void dng_string::SetLineEndings (char ending)
	{
	
	if (fData.Buffer ())
		{
		
		const char *sPtr = fData.Buffer_char ();
		      char *dPtr = fData.Buffer_char ();
		
		while (*sPtr)
			{
			
			char c = *(sPtr++);
			
			char nc = sPtr [0];
			
			if ((c == '\r' && nc == '\n') ||
				(c == '\n' && nc == '\r'))
				{
				
				sPtr++;
				
				if (ending)
					{
					*(dPtr++) = ending;
					}
				
				}
				
			else if (c == '\n' ||
					 c == '\r')
				{
				
				if (ending)
					{
					*(dPtr++) = ending;
					}
				
				}
				
			else
				{
				
				*(dPtr++) = c;
				
				}
				
			}
			
		*dPtr = 0;
		
		}
		
	}
		
/*****************************************************************************/

void dng_string::StripLowASCII ()
	{
	
	if (fData.Buffer ())
		{
		
		const char *sPtr = fData.Buffer_char ();
		      char *dPtr = fData.Buffer_char ();
		
		while (*sPtr)
			{
			
			char c = *(sPtr++);
			
			if (c == '\r' || c == '\n' || (uint8) c >= ' ')
				{
				
				*(dPtr++) = c;
				
				}
							
			}

		*dPtr = 0;
		
		}
		
	}

/*****************************************************************************/

void dng_string::NormalizeAsCommaSeparatedNumbers ()
	{
	
	if (fData.Buffer ())
		{
		
		const char *sPtr = fData.Buffer_char ();
			  char *dPtr = fData.Buffer_char ();
		
		bool commaInserted = false;

		while (*sPtr)
			{
			
			uint32 c = DecodeUTF8 (sPtr);
			
			// Support number formats such as "3", "+3.0", "-3.1416", "314.16e-2",
			// "0.31416E1", but no hex/octal number representations.

			if (isdigit ((int) c) || c == '.' || c == '-' || c == '+' || c == 'e' || c == 'E')
				{
				
				*(dPtr++) = (char) c;
				
				if (commaInserted)
					{

					commaInserted = false;

					}
				
				} 

			else if (!commaInserted) 
				{
				
				*(dPtr++) = ',';

				commaInserted = true;
				
				}
			
			}
		
		*dPtr = 0;
		
		}
	
	}

/******************************************************************************/

// Unicode to low-ASCII strings table.

struct UnicodeToLowASCIIEntry
	{
	uint32 unicode;
	const char *ascii;
	};
	
static const UnicodeToLowASCIIEntry kUnicodeToLowASCII [] =
	{
	{	0x00A0, " "		},
	{	0x00A1, "!"		},
	{	0x00A9, "(C)"	},
	{	0x00AA, "a"		},
	{	0x00AB, "<<"	},
	{	0x00AC, "!"		},
	{	0x00AE, "(R)"	},
	{	0x00B0, "dg"	},
	{	0x00B1, "+-"	},
	{	0x00B7, "."		},
	{	0x00BA, "o"		},
	{	0x00BB, ">>"	},
	{	0x00BF, "?"		},
	{	0x00C0, "A"		},
	{	0x00C1, "A"		},
	{	0x00C2, "A"		},
	{	0x00C3, "A"		},
	{	0x00C4, "A"		},
	{	0x00C5, "A"		},
	{	0x00C6, "AE"	},
	{	0x00C7, "C"		},
	{	0x00C8, "E"		},
	{	0x00C9, "E"		},
	{	0x00CA, "E"		},
	{	0x00CB, "E"		},
	{	0x00CC, "I"		},
	{	0x00CD, "I"		},
	{	0x00CE, "I"		},
	{	0x00CF, "I"		},
	{	0x00D1, "N"		},
	{	0x00D2, "O"		},
	{	0x00D3, "O"		},
	{	0x00D4, "O"		},
	{	0x00D5, "O"		},
	{	0x00D6, "O"		},
	{	0x00D8, "O"		},
	{	0x00D9, "U"		},
	{	0x00DA, "U"		},
	{	0x00DB, "U"		},
	{	0x00DC, "U"		},
	{	0x00DD, "Y"		},
	{	0x00E0, "a"		},
	{	0x00E1, "a"		},
	{	0x00E2, "a"		},
	{	0x00E3, "a"		},
	{	0x00E4, "a"		},
	{	0x00E5, "a"		},
	{	0x00E6, "ae"	},
	{	0x00E7, "c"		},
	{	0x00E8, "e"		},
	{	0x00E9, "e"		},
	{	0x00EA, "e"		},
	{	0x00EB, "e"		},
	{	0x00EC, "i"		},
	{	0x00ED, "i"		},
	{	0x00EE, "i"		},
	{	0x00EF, "i"		},
	{	0x00F1, "n"		},
	{	0x00F2, "o"		},
	{	0x00F3, "o"		},
	{	0x00F4, "o"		},
	{	0x00F5, "o"		},
	{	0x00F6, "o"		},
	{	0x00F7, "/"		},
	{	0x00F8, "o"		},
	{	0x00F9, "u"		},
	{	0x00FA, "u"		},
	{	0x00FB, "u"		},
	{	0x00FC, "u"		},
	{	0x00FD, "y"		},
	{	0x00FF, "y"		},
	{	0x0131, "i"		},
	{	0x0152, "OE"	},
	{	0x0153, "oe"	},
	{	0x0178, "Y"		},
	{	0x2013, "-"		},
	{	0x2014, "-"		},
	{	0x2018, "'"		},
	{	0x2019, "'"		},
	{	0x201A, ","		},
	{	0x201C, "\""	},
	{	0x201D, "\""	},
	{	0x201E, ",,"	},
	{	0x2022, "."		},
	{	0x2026, "..."	},
	{	0x2039, "<"		},
	{	0x203A, ">"		},
	{	0x2044, "/"		},
	{	0x2122, "TM"	},
	{	0x2206, "d"		},
	{	0x2211, "S"		},
	{	0x2260, "!="	},
	{	0x2264, "<="	},
	{	0x2265, ">="	},
	{	0x2318, "#"		},
	{	0xFB01, "fi"	},
	{	0xFB02, "fl"	}
	};
	
/******************************************************************************/

void dng_string::ForceASCII ()
	{
	
	if (!IsASCII ())
		{
		
		uint32 tempBufferSize =
			SafeUint32Add (SafeUint32Mult(Length(), 3), 1);
		dng_memory_data tempBuffer (tempBufferSize);
		
		char *dPtr = tempBuffer.Buffer_char ();
		char * const destEnd = dPtr + tempBufferSize;
		
		const char *sPtr = Get ();
		
		while (*sPtr)
			{
			
			uint32 x = DecodeUTF8 (sPtr);
			
			if (x <= 0x007F)
				{
				
				CheckSpaceLeftInBuffer (dPtr, destEnd, 1);
				*(dPtr++) = (char) x;
				
				}
				
			else
				{
				
				const char *ascii = NULL;
				
				const uint32 kTableEntrys = sizeof (kUnicodeToLowASCII    ) /
									        sizeof (kUnicodeToLowASCII [0]);
				
				for (uint32 entry = 0; entry < kTableEntrys; entry++)
					{
					
					if (kUnicodeToLowASCII [entry] . unicode == x)
						{
						
						ascii = kUnicodeToLowASCII [entry] . ascii;
						
						break;
						
						}
						
					}
					
				if (ascii)
					{
					
					while (*ascii)
						{
						
						CheckSpaceLeftInBuffer (dPtr, destEnd, 1);
						*(dPtr++) = *(ascii++);
						
						}
						
					}
					
				else
					{
					
					CheckSpaceLeftInBuffer (dPtr, destEnd, 1);
					*(dPtr++) ='?';
					
					}
				
				}
				
			}
			
		CheckSpaceLeftInBuffer (dPtr, destEnd, 1);
		*dPtr = 0;
			
		Set (tempBuffer.Buffer_char ());
		
		}
	
	}
	
/******************************************************************************/

static dng_mutex gProtectUCCalls ("gProtectUCCalls");
				
/******************************************************************************/

int32 dng_string::Compare (const dng_string &s) const
	{
	
	#if qMacOS
	#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
	
		// TODO: Needs implementation.
		ThrowProgramError ("Compare() not implemented on iOS");
		return 0;
		
	#else

		{
	
		dng_memory_data aStrA;
		dng_memory_data aStrB;
		
		uint32 aLenA = this->Get_UTF16 (aStrA);
		uint32 aLenB = s    .Get_UTF16 (aStrB);
		
		if (aLenA > 0)
			{
			
			if (aLenB > 0)
				{
				
				// For some Mac OS versions anyway, UCCompareTextDefault is not
				// thread safe.
				
				dng_lock_mutex lockMutex (&gProtectUCCalls);

				UCCollateOptions aOptions = kUCCollateStandardOptions |
											kUCCollatePunctuationSignificantMask;
											   
				SInt32 aOrder = -1;
				
				Boolean aEqual = false;
				
				OSStatus searchStatus = ::UCCompareTextDefault (aOptions,
																aStrA.Buffer_uint16 (),
																aLenA,
																aStrB.Buffer_uint16 (),
																aLenB,
																&aEqual,
																&aOrder);
																
				if (searchStatus == noErr)
					{
					
					if (aEqual || (aOrder == 0))
						{
						return 0;
						}
	
					else
						{
						return (aOrder > 0) ? 1 : -1;
						}
						
					}
					
				else
					{
					
					DNG_REPORT ("UCCompareTextDefault failed");
					
					return -1;
					
					}

				}

			else
				{
				return 1;
				}

			}
			
		else
			{
			
			if (aLenB > 0)
				{
				return -1;
				}
				
			else
				{
				return 0;
				}
				
			}
			
		}
	
	#endif  // TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR

	#elif qWinOS
	
		{

		dng_memory_data aStrA;
		dng_memory_data aStrB;
		
		uint32 aLenA = this->Get_UTF16 (aStrA);
		uint32 aLenB = s    .Get_UTF16 (aStrB);
			
		if (aLenA > 0)
			{
				
			if (aLenB > 0)
				{

				LCID locale = LOCALE_SYSTEM_DEFAULT;

				DWORD aFlags = NORM_IGNOREWIDTH;
				
				int aOrder = ::CompareStringW (locale, 
											   aFlags,
											   (const WCHAR *) aStrA.Buffer_uint16 (), 
											   aLenA,
											   (const WCHAR *) aStrB.Buffer_uint16 (), 
											   aLenB);

				if (aOrder == CSTR_EQUAL)
					{
					return 0;
					}

				else if (aOrder == CSTR_GREATER_THAN)
					{
					return 1;
					} 
				
				else 
					{
					return -1;
					}

				}

			else 
				{
				return 1;
				}

			}

		else 
			{

			if (aLenB > 0) 
				{
				return -1;
				} 
			else
				{
				return 0;
				}

			}
		
		}
		
	#else
	
	// Fallback to a pure Unicode sort order.
	
		{
		
		for (uint32 pass = 0; pass < 2; pass++)
			{
		
			const char *aPtr =   Get ();
			const char *bPtr = s.Get ();
			
			while (*aPtr || *bPtr)
				{
				
				if (!bPtr)
					{
					return 1;
					}
	
				else if (!aPtr)
					{
					return -1;
					}
					
				uint32 a = DecodeUTF8 (aPtr);
				uint32 b = DecodeUTF8 (bPtr);
				
				// Ignore case on first compare pass.
				
				if (pass == 0)
					{
					
					if (a >= (uint32) 'a' && a <= (uint32) 'z')
						{
						a = a - (uint32) 'a' + (uint32) 'A';
						}
						
					if (b >= (uint32) 'a' && b <= (uint32) 'z')
						{
						b = b - (uint32) 'a' + (uint32) 'A';
						}
				
					}
					
				if (b > a)
					{
					return 1;
					}
					
				else if (a < b)
					{
					return -1;
					}
					
				}
				
			}
			
		}
		
	#endif
			
	return 0;
	
	}

/*****************************************************************************/
