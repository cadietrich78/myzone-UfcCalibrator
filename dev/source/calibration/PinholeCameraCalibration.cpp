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

#include <Logger.h>

#include "PinholeCameraCalibration.h"

CPinholeCameraCalibration::CPinholeCameraCalibration()
{
	Create();
}

bool CPinholeCameraCalibration::AddCandidate(const boost::shared_ptr<CPinholeCamera2>& pinholeCamera, boost::shared_ptr<CMarkerGroup> markerGroup)
{
    HEALTH_CHECK(!pinholeCamera, false);
    HEALTH_CHECK(!markerGroup, false);

    CANDIDATE_PINHOLE_CAMERA candidateSolution;

    candidateSolution.m_pinholeCamera.reset(new CPinholeCamera2(*pinholeCamera));

    HEALTH_CHECK(!candidateSolution.m_pinholeCamera, false);

    candidateSolution.m_fitness = markerGroup->GetFitness(candidateSolution.m_pinholeCamera);

    m_candidatePinholeCameraArray.push_back(candidateSolution);

    return true;
}

bool CPinholeCameraCalibration::Update(const boost::shared_ptr<CPinholeCamera2>& pinholeCamera, boost::shared_ptr<CMarkerGroup> markerGroup)
{
    HEALTH_CHECK(!pinholeCamera, false);
    HEALTH_CHECK(!markerGroup, false);

    m_pinholeCamera = pinholeCamera;
    m_markerGroup = markerGroup;

    // DEBUG ONLY! (10-Oct-2015)
    LOG_MESSAGE("START FITNESS: " + my::NumberToString(GetFitness(m_pinholeCamera), 6));

    // THE FIRST APPROACH BASED ON THE FRAIL OPENGL-BASED CAMERA, AND SO FAR, THE ONLY ONE THAT WORKED.
    if (!Evolve())
    {
        LOG_ERROR();

        return false;
    }

    m_markerGroup->SetError(m_pinholeCamera);

    // DEBUG ONLY! (10-Oct-2015)
    LOG_MESSAGE("END FITNESS: " + my::NumberToString(GetFitness(m_pinholeCamera), 6));

    return true;
}

// [1, 8192]
int CPinholeCameraCalibration::GetIterationCount() const
{
    return m_iterationCount;
}

// [1, 8192]
void CPinholeCameraCalibration::SetIterationCount(int iterationCount)
{
    m_iterationCount = iterationCount;
}

// [1, 8192]
int CPinholeCameraCalibration::GetPopulationSize() const
{
    return m_populationSize;
}

// [1, 8192]
void CPinholeCameraCalibration::SetPopulationSize(int populationSize)
{
    m_populationSize = populationSize;
}

// [1, POPULATION SIZE]
int CPinholeCameraCalibration::GetSelectedIndividualsCount() const
{
    return m_selectedIndividualsCount;
}

// [1, POPULATION SIZE]
bool CPinholeCameraCalibration::SetSelectedIndividualsCount(int selectedIndividualsCount)
{
    HEALTH_CHECK(my::IsNull(m_populationSize), false);
    HEALTH_CHECK(selectedIndividualsCount >= m_populationSize, false);

    m_selectedIndividualsCount = selectedIndividualsCount;

    return true;
}

// [0, 1]
double CPinholeCameraCalibration::GetMutationThreshold() const
{
    return m_mutationThreshold;
}

// [0, 1]
void CPinholeCameraCalibration::SetMutationThreshold(double mutationThreshold)
{
    m_mutationThreshold = mutationThreshold;
}

// TODO: (22-Nov-2016) ENABLE CROSSOVER
//// [0, 1]
//double CPinholeCameraCalibration::GetCrossoverThreshold() const
//{
//    return m_crossoverThreshold;
//}

// TODO: (22-Nov-2016) ENABLE CROSSOVER
//// [0, 1]
//void CPinholeCameraCalibration::SetCrossoverThreshold(double crossoverThreshold)
//{
//    m_crossoverThreshold = crossoverThreshold;
//}

bool CPinholeCameraCalibration::IsMaintainingDiversity() const
{
    return m_maintainingDiversity;
}

void CPinholeCameraCalibration::SetMaintainingDiversity(bool maintainingDiversity)
{
    m_maintainingDiversity = maintainingDiversity;
}

bool CPinholeCameraCalibration::IsParameterEnabled(PINHOLE_CAMERA_PARAMETER cameraParameter) const
{
    std::map<PINHOLE_CAMERA_PARAMETER, bool>::const_iterator parameterToEnabledIterator = m_parameterToEnabledMap.find(cameraParameter);

    if (parameterToEnabledIterator == m_parameterToEnabledMap.end())
        return false;

    return parameterToEnabledIterator->second;
}

void CPinholeCameraCalibration::SetParameterEnabled(PINHOLE_CAMERA_PARAMETER cameraParameter, bool isEnabled)
{
    m_parameterToEnabledMap[cameraParameter] = isEnabled;
}

double CPinholeCameraCalibration::GetParameterStep(PINHOLE_CAMERA_PARAMETER cameraParameter) const
{
    std::map<PINHOLE_CAMERA_PARAMETER, double>::const_iterator parameterToStepIterator = m_parameterToStepMap.find(cameraParameter);

    if (parameterToStepIterator == m_parameterToStepMap.end())
        return 0;

    return parameterToStepIterator->second;
}

void CPinholeCameraCalibration::SetParameterStep(PINHOLE_CAMERA_PARAMETER cameraParameter, double step)
{
    m_parameterToStepMap[cameraParameter] = step;
}

boost::shared_ptr<CPinholeCamera2> CPinholeCameraCalibration::GetPinholeCamera()
{
    return m_pinholeCamera;
}

void CPinholeCameraCalibration::Clear()
{
	m_pinholeCamera.reset();
	m_markerGroup.reset();
	//m_candidatePinholeCameraArray.clear();
}

// THE FIRST APPROACH BASED ON THE FRAIL OPENGL-BASED CAMERA, AND SO FAR, THE ONLY ONE THAT WORKED.
bool CPinholeCameraCalibration::Evolve()
{
    HEALTH_CHECK(!m_pinholeCamera, false);
    HEALTH_CHECK(!m_markerGroup, false);

    // ALGORITHM PARAMETERS
    HEALTH_CHECK(my::IsNull(m_iterationCount), false);
    HEALTH_CHECK(m_parameterToStepMap.empty(), false);
    // ALGORITHM PARAMETERS
    HEALTH_CHECK(my::IsNull(m_populationSize), false);
    HEALTH_CHECK(my::IsNull(m_selectedIndividualsCount), false);
    HEALTH_CHECK(my::IsNull(m_mutationThreshold), false);
    // TODO: (22-Nov-2016) ENABLE CROSSOVER
    //HEALTH_CHECK(my::IsNull(m_crossoverThreshold), false);
    // DOMAIN PARAMETERS

    double fitness = GetFitness(m_pinholeCamera);

    // FIRST INDIVIDUAL

    CANDIDATE_PINHOLE_CAMERA candidateSolution;

    candidateSolution.m_pinholeCamera.reset(new CPinholeCamera2(*m_pinholeCamera));

    HEALTH_CHECK(!candidateSolution.m_pinholeCamera, false);

    candidateSolution.m_fitness = fitness;

    m_candidatePinholeCameraArray.push_back(candidateSolution);

    // ITERATIONS

    for (int iterationIndex = 0; iterationIndex < m_iterationCount; ++iterationIndex)
    {
        if (!GeneratePopulation())
        {
            LOG_ERROR();

            return false;
        }

        CANDIDATE_PINHOLE_CAMERA highRankedIndividual = m_candidatePinholeCameraArray.front();

        if (fitness > highRankedIndividual.m_fitness)
        {
            fitness = highRankedIndividual.m_fitness;

            m_pinholeCamera.reset(new CPinholeCamera2(*m_candidatePinholeCameraArray.front().m_pinholeCamera));

            HEALTH_CHECK(!m_pinholeCamera, false);

            // (BEGIN OF) DEBUG ONLY! (22-Nov-2016)
            std::string parameterAsString;

            if (highRankedIndividual.m_parameterBitfield & OPTICAL_CENTER_PARAMETER)
                parameterAsString += " optical_center ";
            if (highRankedIndividual.m_parameterBitfield & YAW_PARAMETER)
                parameterAsString += " yaw ";
            if (highRankedIndividual.m_parameterBitfield & PITCH_PARAMETER)
                parameterAsString += " pitch ";
            if (highRankedIndividual.m_parameterBitfield & ROLL_PARAMETER)
                parameterAsString += " roll ";
            if (highRankedIndividual.m_parameterBitfield & FIELD_OF_VIEW_PARAMETER)
                parameterAsString += " field_of_view ";

            LOG_MESSAGE("UPDATED FITNESS: " + my::NumberToString(fitness, 6) + " (" + parameterAsString + ")");
            // (END OF) DEBUG ONLY! (22-Nov-2016)
        }
    }

    m_candidatePinholeCameraArray.clear();

    return true;
}

bool CPinholeCameraCalibration::GeneratePopulation()
{
    std::vector<CANDIDATE_PINHOLE_CAMERA> lastCandidateSolutionArray,
        newCandidateSolutionArray;

    // TESTING: (07-Dec-2015) MAINTAINING DIVERSITY (85% FROM THE HIGH RANKED, 15% FROM THE LOW RANKED)

    int highRankedIndividualCount = (int)(0.85 * m_selectedIndividualsCount),
        lowRankedIndividualCount = (int)(0.15 * m_selectedIndividualsCount);

    if (m_candidatePinholeCameraArray.size() > m_selectedIndividualsCount)
    {
        if ((highRankedIndividualCount > 0) &&
            (lowRankedIndividualCount > 0))
        {
            m_candidatePinholeCameraArray.erase(m_candidatePinholeCameraArray.begin() + highRankedIndividualCount, m_candidatePinholeCameraArray.end() - lowRankedIndividualCount);
        }
    }

    lastCandidateSolutionArray = m_candidatePinholeCameraArray;

    // BUG: (??-???-????) DISCARD OLD POPULATION
    m_candidatePinholeCameraArray.clear();
    m_candidatePinholeCameraArray.push_back(lastCandidateSolutionArray.front());

    while (m_candidatePinholeCameraArray.size() < m_populationSize)
    {
        for (std::vector<CANDIDATE_PINHOLE_CAMERA>::iterator candidateSolutionIterator = lastCandidateSolutionArray.begin(); candidateSolutionIterator != lastCandidateSolutionArray.end(); ++candidateSolutionIterator)
        {
            if (IsParameterEnabled(FIELD_OF_VIEW_PARAMETER))
                newCandidateSolutionArray.push_back(GenerateIndividual(FIELD_OF_VIEW_PARAMETER, (*candidateSolutionIterator)));

            if (IsParameterEnabled(OPTICAL_CENTER_PARAMETER))
                newCandidateSolutionArray.push_back(GenerateIndividual(OPTICAL_CENTER_PARAMETER, (*candidateSolutionIterator)));

            if (IsParameterEnabled(YAW_PARAMETER) ||
                IsParameterEnabled(PITCH_PARAMETER) ||
                IsParameterEnabled(ROLL_PARAMETER))
            {
                newCandidateSolutionArray.push_back(GenerateIndividual(YAW_PARAMETER | PITCH_PARAMETER | ROLL_PARAMETER, (*candidateSolutionIterator)));
            }

            if (IsParameterEnabled(FIELD_OF_VIEW_PARAMETER) ||
                IsParameterEnabled(OPTICAL_CENTER_PARAMETER) ||
                IsParameterEnabled(YAW_PARAMETER) ||
                IsParameterEnabled(PITCH_PARAMETER) ||
                IsParameterEnabled(ROLL_PARAMETER))
            {
                newCandidateSolutionArray.push_back(GenerateIndividual(FIELD_OF_VIEW_PARAMETER | OPTICAL_CENTER_PARAMETER | YAW_PARAMETER | PITCH_PARAMETER | ROLL_PARAMETER, (*candidateSolutionIterator)));
            }
        }

        m_candidatePinholeCameraArray.insert(m_candidatePinholeCameraArray.end(), newCandidateSolutionArray.begin(), newCandidateSolutionArray.end());

        lastCandidateSolutionArray = newCandidateSolutionArray;
        newCandidateSolutionArray.clear();
    }

    std::sort(m_candidatePinholeCameraArray.begin(), m_candidatePinholeCameraArray.end());

    return true;
}

CPinholeCameraCalibration::CANDIDATE_PINHOLE_CAMERA CPinholeCameraCalibration::GenerateIndividual(int parameterTypeBitfield, const CANDIDATE_PINHOLE_CAMERA& father)
{
    CPinholeCameraCalibration::CANDIDATE_PINHOLE_CAMERA child;

    child.m_pinholeCamera.reset(new CPinholeCamera2(*father.m_pinholeCamera));

    HEALTH_CHECK(!child.m_pinholeCamera, father);

    double mutationThreshold = GetMutationThreshold();

    bool isNew = false;

    while (!isNew)
    {
        if (IsParameterEnabled(FIELD_OF_VIEW_PARAMETER) &&
            (parameterTypeBitfield & FIELD_OF_VIEW_PARAMETER) &&
            (my::Rand<double>() > mutationThreshold))
        {
            child.m_pinholeCamera->SetFieldOfView(child.m_pinholeCamera->GetFieldOfView() + GetStep(FIELD_OF_VIEW_PARAMETER));

            child.m_parameterBitfield |= FIELD_OF_VIEW_PARAMETER;

            isNew = true;
        }

        if ((IsParameterEnabled(YAW_PARAMETER) &&
             (parameterTypeBitfield & YAW_PARAMETER)) ||
            (IsParameterEnabled(PITCH_PARAMETER) &&
             (parameterTypeBitfield & PITCH_PARAMETER)) ||
            (IsParameterEnabled(ROLL_PARAMETER) &&
             (parameterTypeBitfield & ROLL_PARAMETER)))
        {
            // PITCH (TILT)
            if (my::Rand<double>() > mutationThreshold)
            {
                child.m_pinholeCamera->Pitch(GetStep(PITCH_PARAMETER));

                child.m_parameterBitfield |= PITCH_PARAMETER;

                isNew = true;
            }

            // YAW (PAN)
            if (my::Rand<double>() > mutationThreshold)
            {
                child.m_pinholeCamera->Yaw(GetStep(YAW_PARAMETER));

                child.m_parameterBitfield |= YAW_PARAMETER;

                isNew = true;
            }

            // ROLL (HORIZON)
            if (my::Rand<double>() > mutationThreshold)
            {
                child.m_pinholeCamera->Roll(GetStep(ROLL_PARAMETER));

                child.m_parameterBitfield |= ROLL_PARAMETER;

                isNew = true;
            }
        }

        if (IsParameterEnabled(OPTICAL_CENTER_PARAMETER) &&
            (parameterTypeBitfield & OPTICAL_CENTER_PARAMETER))
        {
            // FRONT
            if (my::Rand<double>() > mutationThreshold)
            {
                child.m_pinholeCamera->MoveFront(GetStep(OPTICAL_CENTER_PARAMETER));

                child.m_parameterBitfield |= OPTICAL_CENTER_PARAMETER;

                isNew = true;
            }

            // SIDE
            if (my::Rand<double>() > mutationThreshold)
            {
                child.m_pinholeCamera->MoveSide(GetStep(OPTICAL_CENTER_PARAMETER));

                child.m_parameterBitfield |= OPTICAL_CENTER_PARAMETER;

                isNew = true;
            }

            // UP
            if (my::Rand<double>() > mutationThreshold)
            {
                child.m_pinholeCamera->MoveUp(GetStep(OPTICAL_CENTER_PARAMETER));

                child.m_parameterBitfield |= OPTICAL_CENTER_PARAMETER;

                isNew = true;
            }
        }
    }

    child.m_pinholeCamera->ApplyTransform();
    child.m_fitness = GetFitness(child.m_pinholeCamera);

    return child;

    //CPinholeCameraCalibration::CANDIDATE_PINHOLE_CAMERA child;

    //child.m_pinholeCamera.reset(new CPinholeCamera2(*father.m_pinholeCamera));

    //HEALTH_CHECK(!child.m_pinholeCamera, father);

    //double mutationThreshold = GetMutationThreshold();

    //bool isNew = false;

    //while (!isNew)
    //{
    //    if (IsParameterEnabled(OPTICAL_CENTER_PARAMETER) &&
    //        (parameterTypeBitfield & OPTICAL_CENTER_PARAMETER) &&
    //        (my::Rand<double>() > mutationThreshold))
    //    {
    //        child.m_pinholeCamera->MoveFront(GetStep(OPTICAL_CENTER_PARAMETER));
    //        child.m_pinholeCamera->MoveSide(GetStep(OPTICAL_CENTER_PARAMETER));
    //        child.m_pinholeCamera->MoveUp(GetStep(OPTICAL_CENTER_PARAMETER));

    //        child.m_parameterBitfield |= OPTICAL_CENTER_PARAMETER;

    //        isNew = true;
    //    }

    //    // PITCH (TILT)
    //    if (IsParameterEnabled(PITCH_PARAMETER) &&
    //        (parameterTypeBitfield & PITCH_PARAMETER) &&
    //        (my::Rand<double>() > mutationThreshold))
    //    {
    //        child.m_pinholeCamera->Pitch(GetStep(PITCH_PARAMETER));

    //        child.m_parameterBitfield |= PITCH_PARAMETER;

    //        isNew = true;
    //    }

    //    // YAW (PAN)
    //    if (IsParameterEnabled(YAW_PARAMETER) &&
    //        (parameterTypeBitfield & YAW_PARAMETER) &&
    //        (my::Rand<double>() > mutationThreshold))
    //    {
    //        child.m_pinholeCamera->Yaw(GetStep(YAW_PARAMETER));

    //        child.m_parameterBitfield |= YAW_PARAMETER;

    //        isNew = true;
    //    }

    //    // ROLL (HORIZON)
    //    if (IsParameterEnabled(ROLL_PARAMETER) &&
    //        (parameterTypeBitfield & ROLL_PARAMETER) &&
    //        (my::Rand<double>() > mutationThreshold))
    //    {
    //        child.m_pinholeCamera->Roll(GetStep(ROLL_PARAMETER));

    //        child.m_parameterBitfield |= ROLL_PARAMETER;

    //        isNew = true;
    //    }

    //    if (IsParameterEnabled(FIELD_OF_VIEW_PARAMETER) &&
    //        (parameterTypeBitfield & FIELD_OF_VIEW_PARAMETER) &&
    //        (my::Rand<double>() > mutationThreshold))
    //    {
    //        child.m_pinholeCamera->SetFieldOfView(child.m_pinholeCamera->GetFieldOfView() + GetStep(FIELD_OF_VIEW_PARAMETER));

    //        child.m_parameterBitfield |= FIELD_OF_VIEW_PARAMETER;

    //        isNew = true;
    //    }        
    //}

    //child.m_pinholeCamera->ApplyTransform();
    //child.m_fitness = GetFitness(child.m_pinholeCamera);

    //return child;
}

double CPinholeCameraCalibration::GetFitness(boost::shared_ptr<CPinholeCamera2>& pinholeCamera)
{
	HEALTH_CHECK(!m_markerGroup, 0);

	return m_markerGroup->GetFitness(pinholeCamera);
}

double CPinholeCameraCalibration::GetStep(PINHOLE_CAMERA_PARAMETER cameraParameter) const
{
    double parameterStep = GetParameterStep(cameraParameter);

    return (2.0 * my::Rand<double>() - 1.0) * parameterStep;
}

void CPinholeCameraCalibration::Create()
{
    m_pinholeCamera.reset();
    m_markerGroup.reset();
    m_iterationCount = 64;
    m_populationSize = 756;
    m_selectedIndividualsCount = 96;
    // 30%
    m_mutationThreshold = 0.3;
    // TODO: (22-Nov-2016) ENABLE CROSSOVER
    //m_crossoverThreshold;
    m_maintainingDiversity = true;
    m_parameterToEnabledMap.clear();
    m_parameterToStepMap.clear();
    m_parameterToEnabledMap[OPTICAL_CENTER_PARAMETER] = true;
    m_parameterToStepMap[OPTICAL_CENTER_PARAMETER] = 5.0;
    m_parameterToEnabledMap[YAW_PARAMETER] = true;
    m_parameterToStepMap[YAW_PARAMETER] = 1.0;
    m_parameterToEnabledMap[PITCH_PARAMETER] = true;
    m_parameterToStepMap[PITCH_PARAMETER] = 1.0;
    m_parameterToEnabledMap[FIELD_OF_VIEW_PARAMETER] = true;
    m_parameterToStepMap[ROLL_PARAMETER] = 0.1;
    m_parameterToEnabledMap[FIELD_OF_VIEW_PARAMETER] = true;
    m_parameterToStepMap[FIELD_OF_VIEW_PARAMETER] = 1.0;
    m_candidatePinholeCameraArray.clear();
}

