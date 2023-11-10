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

#if !defined(CALIBRATE_PINHOLE_CAMERA_INCLUDED)
#define CALIBRATE_PINHOLE_CAMERA_INCLUDED

#include <vector>

#include <boost/shared_ptr.hpp>

#include <PinholeCamera2.h>
#include <Marker.h>

#include "MarkerGroup.h"

class CPinholeCameraCalibration
{
    struct CANDIDATE_PINHOLE_CAMERA {
        CANDIDATE_PINHOLE_CAMERA()
            : m_fitness(std::numeric_limits<double>::max()),
            m_pinholeCamera(),
            m_parameterBitfield(0)
        {
        }

        CANDIDATE_PINHOLE_CAMERA(const CANDIDATE_PINHOLE_CAMERA& candidatePinholeCamera)
            : m_fitness(candidatePinholeCamera.m_fitness),
            m_pinholeCamera(candidatePinholeCamera.m_pinholeCamera),
            m_parameterBitfield(candidatePinholeCamera.m_parameterBitfield)
        {
        }

        void operator=(const CANDIDATE_PINHOLE_CAMERA& candidatePinholeCamera)
        {
            m_fitness = candidatePinholeCamera.m_fitness;
            m_pinholeCamera = candidatePinholeCamera.m_pinholeCamera;
            m_parameterBitfield = candidatePinholeCamera.m_parameterBitfield;
        }

        bool operator<(const CANDIDATE_PINHOLE_CAMERA& candidatePinholeCamera) const
        {
            return m_fitness < candidatePinholeCamera.m_fitness;
        }

        bool IsValid() const
        {
            return m_fitness != std::numeric_limits<double>::max();
        }

        double m_fitness;
        boost::shared_ptr<CPinholeCamera2> m_pinholeCamera;
        int m_parameterBitfield;
    };

public:
    enum PINHOLE_CAMERA_PARAMETER {
        OPTICAL_CENTER_PARAMETER = 1,
        // PAN
        YAW_PARAMETER = 2,
        // TILT
        PITCH_PARAMETER = 4,
        // HORIZON
        ROLL_PARAMETER = 8,
        FIELD_OF_VIEW_PARAMETER = 16
    };

    CPinholeCameraCalibration();

    bool AddCandidate(const boost::shared_ptr<CPinholeCamera2>& pinholeCamera, boost::shared_ptr<CMarkerGroup> markerArray);

    bool Update(const boost::shared_ptr<CPinholeCamera2>& pinholeCamera, boost::shared_ptr<CMarkerGroup> markerGroup);

    // [1, 8192]
    int GetIterationCount() const;
    // [1, 8192]
    void SetIterationCount(int iterationCount);

    // [1, 8192]
    int GetPopulationSize() const;
    // [1, 8192]
    void SetPopulationSize(int populationSize);

    // [1, POPULATION SIZE]
    int GetSelectedIndividualsCount() const;
    // [1, POPULATION SIZE]
    bool SetSelectedIndividualsCount(int selectedIndividualsCount);

    // [0, 1]
    double GetMutationThreshold() const;
    // [0, 1]
    void SetMutationThreshold(double mutationThreshold);

    bool IsMaintainingDiversity() const;
    void SetMaintainingDiversity(bool maintainingDiversity);

    bool IsParameterEnabled(PINHOLE_CAMERA_PARAMETER cameraParameter) const;
    void SetParameterEnabled(PINHOLE_CAMERA_PARAMETER cameraParameter, bool isEnabled);

    // OPTICAL_CENTER_PARAMETER (FEET)
    // FIELD_OF_VIEW_PARAMETER (DEGREES)
    // YAW_PARAMETER (DEGREES)
    // PITCH_PARAMETER (DEGREES)
    // ROLL_PARAMETER (DEGREES)
    double GetParameterStep(PINHOLE_CAMERA_PARAMETER cameraParameter) const;
    // OPTICAL_CENTER_PARAMETER (FEET)
    // FIELD_OF_VIEW_PARAMETER (DEGREES)
    // YAW_PARAMETER (DEGREES)
    // PITCH_PARAMETER (DEGREES)
    // ROLL_PARAMETER (DEGREES)
    void SetParameterStep(PINHOLE_CAMERA_PARAMETER cameraParameter, double step);

    // OPTICAL_CENTER_PARAMETER (FEET)
    void SetParameterMaximum(PINHOLE_CAMERA_PARAMETER cameraParameter, double maximum);

    boost::shared_ptr<CPinholeCamera2> GetPinholeCamera();
    
    void Clear();

private:
    bool Evolve();

    bool GeneratePopulation();
    CANDIDATE_PINHOLE_CAMERA GenerateIndividual(int parameterTypeBitfield, const CANDIDATE_PINHOLE_CAMERA& father);

	double GetFitness(boost::shared_ptr<CPinholeCamera2>& pinholeCamera);

    double GetStep(PINHOLE_CAMERA_PARAMETER cameraParameter) const;

    void Create();

protected:
	boost::shared_ptr<CPinholeCamera2> m_pinholeCamera;
	
	boost::shared_ptr<CMarkerGroup> m_markerGroup;

    int m_iterationCount;

    int m_populationSize;
    int m_selectedIndividualsCount;

    double m_mutationThreshold;

    bool m_maintainingDiversity;

    std::map<PINHOLE_CAMERA_PARAMETER, bool> m_parameterToEnabledMap;
    std::map<PINHOLE_CAMERA_PARAMETER, double> m_parameterToStepMap;
    std::map<PINHOLE_CAMERA_PARAMETER, double> m_parameterToMaximumMap;

    std::vector<CANDIDATE_PINHOLE_CAMERA> m_candidatePinholeCameraArray;
};

#endif //#if !defined(CALIBRATE_PINHOLE_CAMERA_INCLUDED)

