/*
 * COPYRIGHT NOTICE, DISCLAIMER, and LICENSE:
 *
 * 
 * For the purposes of this copyright and license, "Contributing Authors"
 * is defined as the following set of individuals:
 *
 *    Carlos Augusto Dietrich (cadietrich@gmail.com)
 *
 * This library is supplied "AS IS".  The Contributing Authors disclaim 
 * all warranties, expressed or implied, including, without limitation, 
 * the warranties of merchantability and of fitness for any purpose. 
 * The Contributing Authors assume no liability for direct, indirect, 
 * incidental, special, exemplary, or consequential damages, which may 
 * result from the use of the this library, even if advised of the 
 * possibility of such damage.
 *
 * Permission is hereby granted to use, copy, modify, and distribute this
 * source code, or portions hereof, for any purpose, without fee, subject
 * to the following restrictions:
 *
 * 1. The origin of this source code must not be misrepresented.
 *
 * 2. Altered versions must be plainly marked as such and must not be 
 *    misrepresented as being the original source.
 *
 * 3. This Copyright notice may not be removed or altered from any source 
 *    or altered source distribution.
 *
 * The Contributing Authors specifically permit, without fee, and 
 * encourage the use of this source code as a component in commercial 
 * products. If you use this source code in a product, acknowledgment 
 * is not required but would be appreciated.
 *
 * 
 * "Software is a process, it's never finished, it's always evolving. 
 * That's its nature. We know our software sucks. But it's shipping! 
 * Next time we'll do better, but even then it will be shitty. 
 * The only software that's perfect is one you're dreaming about. 
 * Real software crashes, loses data, is hard to learn and hard to use. 
 * But it's a process. We'll make it less shitty. Just watch!"
 */

#if !defined(DISTANCE_INCLUDED)
#define DISTANCE_INCLUDED

#include <cfloat>

namespace MyMath 
{
    namespace distance
    {
		// BUG: (07-Oct-2020) ?????
		//template <typename T>
		//T PointToSegment2(const T* p, const T* s0, const T* s1)
		//{
		//	T segment[2] = { 0 },
		//		segmentLength = 0,
		//		s0ToP[2] = { 0 },
		//		dopProduct = 0,
		//		closestPointOnSegment[2] = { 0 },
		//		pToClosestPointOnSegment[2] = { 0 },
		//		distance = 0;

		//	segment[0] = s1[0] - s0[0];
		//	segment[1] = s1[1] - s0[1];

		//	segmentLength = segment[0] * segment[0] + segment[1] * segment[1];

		//	if (segmentLength > (T)1.0e-4)
		//	{
		//		segmentLength = MyMath::SquareRoot(segmentLength);

		//		segment[0] /= segmentLength;
		//		segment[1] /= segmentLength;

		//		s0ToP[0] = p[0] - s0[0];
		//		s0ToP[1] = p[1] - s0[1];

		//		dopProduct = segment[0] * s0ToP[0] + segment[1] * s0ToP[1];

		//		closestPointOnSegment[0] = s0[0] + dopProduct*segment[0];
		//		closestPointOnSegment[1] = s0[1] + dopProduct*segment[1];
		//	}
		//	else
		//	{
		//		closestPointOnSegment[0] = s0[0];
		//		closestPointOnSegment[1] = s0[1];
		//	}

		//	pToClosestPointOnSegment[0] = p[0] - closestPointOnSegment[0];
		//	pToClosestPointOnSegment[1] = p[1] - closestPointOnSegment[1];
		//	
		//	distance = MyMath::SquareRoot(pToClosestPointOnSegment[0] * pToClosestPointOnSegment[0] + pToClosestPointOnSegment[1] * pToClosestPointOnSegment[1]);

		//	return distance;
		//}
		template <typename T>
		T PointToSegment2(const T* p, const T* s0, const T* s1)
		{
			T v[2] = { s1[0] - s0[0], s1[1] - s0[1] },
				w[2] = { p[0] - s0[0], p[1] - s0[1] };

			T c1 = w[0] * v[0] + w[1] * v[1];

			if (c1 <= 0.0)
			{
				/*T*/ v[0]/*dx*/ = p[0] - s0[0],
					v[1]/*dy*/ = p[1] - s0[1];

				return sqrt(v[0] * v[0] + v[1] * v[1]);
			}

			T c2 = v[0] * v[0] + v[1] * v[1];

			if (c2 <= c1)
			{
				/*T*/ v[0]/*dx*/ = p[0] - s1[0],
					v[1]/*dy*/ = p[1] - s1[1];

				return sqrt(v[0] * v[0] + v[1] * v[1]);
			}

#if defined(_DEBUG)
#if defined(_WIN32)
			// -INF, +INF, NaN
			if ((c2 == 0.0) ||
				(_finite(c2) == 0) ||
				// NaN
				(_isnan(c2) != 0))
#else // #if defined(_WIN32)
			if ((c2 == 0.0) ||
				std::isinf(c2) ||
				std::isnan(c2))
#endif // #if defined(_WIN32)
			{
				LOG_ERROR();

				return DBL_MAX;
			}
#endif //#if defined(_DEBUG)

			T b = c1 / c2;

			//T ps[2] = { s0[0] + v[0] * b, s0[1] + v[1] * b };
			w[0] = s0[0] + v[0] * b;
			w[1] = s0[1] + v[1] * b;

			/*T*/ v[0]/*dx*/ = p[0] - w/*ps*/[0],
				v[1]/*dy*/ = p[1] - w/*ps*/[1];

			return sqrt(v[0] * v[0] + v[1] * v[1]);
		}

		// BUG: (07-Oct-2020) ?????
		///**
		//http://paulbourke.net/geometry/pointlineplane/
		//(tested by 1 project)
		//*/
		//template <typename T>
		//T PointToSegment3(const T *p, const T *s0, const T* s1)
		//{
		//	T segment[3] = { 0 },
		//		segmentLength = 0,
		//		s0ToP[3] = { 0 },
		//		dopProduct = 0,
		//		closestPointOnSegment[3] = { 0 },
		//		pToClosestPointOnSegment[3] = { 0 },
		//		distance = 0;

		//	segment[0] = s1[0] - s0[0];
		//	segment[1] = s1[1] - s0[1];
		//	segment[2] = s1[2] - s0[2];

		//	segmentLength = segment[0] * segment[0] + segment[1] * segment[1] + segment[2] * segment[2];

		//	if (segmentLength > (T)1.0e-4)
		//	{
		//		segmentLength = MyMath::SquareRoot(segmentLength);

		//		segment[0] /= segmentLength;
		//		segment[1] /= segmentLength;
		//		segment[2] /= segmentLength;

		//		s0ToP[0] = p[0] - s0[0];
		//		s0ToP[1] = p[1] - s0[1];
		//		s0ToP[2] = p[2] - s0[2];

		//		dopProduct = segment[0] * s0ToP[0] + segment[1] * s0ToP[1] + segment[2] * s0ToP[2];

		//		closestPointOnSegment[0] = s0[0] + dopProduct*segment[0];
		//		closestPointOnSegment[1] = s0[1] + dopProduct*segment[1];
		//		closestPointOnSegment[2] = s0[2] + dopProduct*segment[2];
		//	}
		//	else
		//	{
		//		closestPointOnSegment[0] = s0[0];
		//		closestPointOnSegment[1] = s0[1];
		//		closestPointOnSegment[2] = s0[2];
		//	}

		//	pToClosestPointOnSegment[0] = p[0] - closestPointOnSegment[0];
		//	pToClosestPointOnSegment[1] = p[1] - closestPointOnSegment[1];
		//	pToClosestPointOnSegment[2] = p[2] - closestPointOnSegment[2];

		//	distance = MyMath::SquareRoot(pToClosestPointOnSegment[0] * pToClosestPointOnSegment[0] + pToClosestPointOnSegment[1] * pToClosestPointOnSegment[1] + pToClosestPointOnSegment[2] * pToClosestPointOnSegment[2]);

		//	return distance;
		//}

		//// BUG: (07-Oct-2020) CIRCULAR DEPENDENCY?
		//template <typename T>
		//T PointToSegment(my::CVector3<T> P, my::CVector3<T> SP0, my::CVector3<T> SP1)
		//{
		//	my::CVector3<T> v = SP1 - SP0;
		//	my::CVector3<T> w = P - SP0;

		//	T c1 = w.Dot(v);
		//	if (c1 <= 0)
		//		return P.Distance(SP0);

		//	T c2 = v.Dot(v);
		//	if (c2 <= c1)
		//		return P.Distance(SP1);

		//	T b = c1 / c2;
		//	my::CVector3<T> Pb = SP0 + v * b;
		//	return P.Distance(Pb);
		//}
		template <typename T>
		T PointToSegment3(const T* p, const T* s0, const T* s1)
		{
			T v[3] = { s1[0] - s0[0], s1[1] - s0[1], s1[2] - s0[2] },
				w[3] = { p[0] - s0[0], p[1] - s0[1], p[2] - s0[2] };

			T c1 = w[0] * v[0] + w[1] * v[1] + w[2] * v[2];

			if (c1 <= 0.0)
			{
				/*T*/ v[0]/*dx*/ = p[0] - s0[0],
					v[1]/*dy*/ = p[1] - s0[1],
					v[2]/*dz*/ = p[2] - s0[2];

				return sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
			}

			T c2 = v[0] * v[0] + v[1] * v[1] + v[2] * v[2];

			if (c2 <= c1)
			{
				/*T*/ v[0]/*dx*/ = p[0] - s1[0],
					v[1]/*dy*/ = p[1] - s1[1],
					v[2]/*dz*/ = p[2] - s1[2];

				return sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
			}

#if defined(_DEBUG)
#if defined(_WIN32)
			// -INF, +INF, NaN
			if ((c2 == 0.0) ||
				(_finite(c2) == 0) ||
				// NaN
				(_isnan(c2) != 0))
#else // #if defined(_WIN32)
			if ((c2 == 0.0) ||
				std::isinf(c2) ||
				std::isnan(c2))
#endif // #if defined(_WIN32)
			{
				LOG_ERROR();

				return DBL_MAX;
			}
#endif //#if defined(_DEBUG)

			T b = c1 / c2;

			//T ps[3] = { s0[0] + v[0] * b, s0[1] + v[1] * b, s0[2] + v[2] * b };
			w[0] = s0[0] + v[0] * b;
			w[1] = s0[1] + v[1] * b;
			w[2] = s0[2] + v[2] * b;

			/*T*/ v[0]/*dx*/ = p[0] - w/*ps*/[0],
				v[1]/*dy*/ = p[1] - w/*ps*/[1],
				v[2]/*dz*/ = p[2] - w/*ps*/[2];

			return sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
		}
	} //namespace distance
} //namespace MyMath 

#endif //#if !defined(DISTANCE_INCLUDED)

