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

#if !defined(MARKER_INCLUDED)
#define MARKER_INCLUDED

// (BEGIN OF) DEBUG ONLY! (09-Mar-2016) RENDERING OF DEBUGGING DATA
#include <boost/shared_ptr.hpp>
#include <Texture.h>
// (END OF) DEBUG ONLY! (09-Mar-2016) RENDERING OF DEBUGGING DATA

#include <Object.h>
#include <PropertyObject.h>
#include <Vector3.h>
#include <PinholeCamera2.h>

namespace my
{
    namespace video
    {
        class CMarker
            : public CObject,
            public my::CPropertyObject
        {
        public:
            enum CONSTRAINT
            {
                UNCONSTRAINED,
                // SCREEN COORDINATES CONSTRAINT - VERTICAL MOVEMENT ONLY
                Y_CONSTRAINT
            };

            CMarker();
            CMarker(const CMarker& marker);

            virtual ~CMarker();

            void operator=(const CMarker& marker);

            std::string GetName() const;
            void SetName(std::string name);

            std::string GetIcon() const;
            void SetIcon(std::string icon);

            // (WORLD) POSITION WHERE THE REAL-WORLD MARKER IS (3D)
            const double *GetWorldCoord() const;
            void SetWorldCoord(const double* worldCoord);
            void SetWorldCoord(double x, double y, double z);

            const double *GetWorldCoordError() const;
            void SetWorldCoordError(const double* worldCoordError);
            void SetWorldCoordError(double x, double y, double z);

            // (SCREEN) POSITION WHERE THE MARKER SHOULD BE PROJECTED (2D) [-1, 1]. (1) THE MARKER HAS A WORLD POSITION; (2) IT IS PROJECTED SOMEWHERE UNDER THE CURRENT CAMERA; (3) THIS ATTRIBUTE HOLDS THE COORDINATES WHERE THE MARKER SHOULD BE PROJECTED (THE ACTUAL FEATURE ON THE IMAGE).
            const double *GetScreenCoord() const;
            // (SCREEN) POSITION WHERE THE MARKER SHOULD BE PROJECTED (2D) [-1, 1]. (1) THE MARKER HAS A WORLD POSITION; (2) IT IS PROJECTED SOMEWHERE UNDER THE CURRENT CAMERA; (3) THIS ATTRIBUTE HOLDS THE COORDINATES WHERE THE MARKER SHOULD BE PROJECTED (THE ACTUAL FEATURE ON THE IMAGE).
            virtual void SetScreenCoord(const double* screenCoordz);
            // (SCREEN) POSITION WHERE THE MARKER SHOULD BE PROJECTED (2D) [-1, 1]. (1) THE MARKER HAS A WORLD POSITION; (2) IT IS PROJECTED SOMEWHERE UNDER THE CURRENT CAMERA; (3) THIS ATTRIBUTE HOLDS THE COORDINATES WHERE THE MARKER SHOULD BE PROJECTED (THE ACTUAL FEATURE ON THE IMAGE).
            virtual void SetScreenCoord(double x, double y);

            const double *GetScreenCoordError() const;
            void SetScreenCoordError(const double* screenCoordError);
            void SetScreenCoordError(double x, double y);

            // TRICKY: (27-Jan-2016) NO WINDOW COORDINATE BECAUSE THE MARKER HAS NO KNOWLEDGE ABOUT THE WINDOW SIZE.

            my::CVector3<double> GetWindowCoordError() const;
            void SetWindowCoordError(my::CVector3<double> windowCoordError);
            void SetWindowCoordError(double x, double y);
            
            // TRICKY: (21-Sep-2016) ERROR ON SCREEN COORDINATES
            virtual double GetFitness(boost::shared_ptr<CPinholeCamera2>& pinholeCamera);
            //// TRICKY: (21-Sep-2016) ERROR ON SCREEN COORDINATES
            //virtual double GetFitness(my::CViewProjectionMatrix<double>& viewProjectionMatrix);

            // IT IS NECESSARY TO COMPUTE THE SIZE OF THE MARKER ON SCREEN COORDINATES
            double GetScreenSize() const;
            // IT IS NECESSARY TO COMPUTE THE SIZE OF THE MARKER ON SCREEN COORDINATES
            void SetScreenSize(double markerSize);

            // IT IS NECESSARY TO COMPUTE THE SIZE OF THE MARKER ON SCREEN COORDINATES
            double GetZoomFactor() const;
            // IT IS NECESSARY TO COMPUTE THE SIZE OF THE MARKER ON SCREEN COORDINATES
            virtual void SetZoomFactor(double zoomFactor);

            CONSTRAINT GetConstraint() const;
            void SetConstraint(CONSTRAINT constraint);
            bool SetConstraint(INT32 constraintIndex);

            // DEPRECATED: (27-Jan-2016)
            double GetScreenSpeed() const;
            // DEPRECATED: (27-Jan-2016)
            void SetScreenSpeed(double speed);

            bool IsEnabled() const;
            void SetEnabled(bool enabled);

            // (BEGIN OF) DEBUG ONLY! (09-Mar-2016) RENDERING OF DEBUGGING DATA
            virtual bool IsTextured() const
            {
                return false;
            }

            virtual boost::shared_ptr<CTexture> GetTexture()
            {
                return boost::shared_ptr<CTexture>();
            }
            // (END OF) DEBUG ONLY! (09-Mar-2016) RENDERING OF DEBUGGING DATA

            // IT REFERS TO ANY CHANGES TO THE MARKER SINCE INITIALIZATION
            bool HasChanged() const;
            void SetHasChanged(bool hasChanged);

            virtual std::string ToJson() const;

        private:
            void Create();
            void Copy(const CMarker& marker);

        protected:
            std::string m_name;

            std::string m_icon;

            // (WORLD) POSITION WHERE THE REAL-WORLD MARKER IS (3D)
            double m_worldCoord[3];
            double m_worldCoordError[3];

            // (SCREEN) POSITION WHERE THE MARKER SHOULD BE PROJECTED (2D) [-1, 1]
            double m_screenCoord[2];
            double m_screenCoordError[2];

            my::CVector3<double> m_windowCoordError;

            // IT IS NECESSARY TO COMPUTE THE SIZE OF THE MARKER ON SCREEN COORDINATES
            double m_screenSize;
            // IT IS NECESSARY TO COMPUTE THE SIZE OF THE MARKER ON SCREEN COORDINATES
            double m_zoomFactor;

            CONSTRAINT m_constraint;

            bool m_enabled;

            // IT REFERS TO ANY CHANGES TO THE MARKER SINCE INITIALIZATION
            bool m_hasChanged;

            // DEPRECATED: (27-Jan-2016)
            double m_screenSpeed;
        };
    }; // video
}; // my

#endif // #if !defined(MARKER_INCLUDED)

