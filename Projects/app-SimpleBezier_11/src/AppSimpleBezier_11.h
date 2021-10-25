/**
 * 
 * An application is responsible for running the main loop, and holding a variety of managers.
 *
 * @class AppSimpleBezier_11
 *
 *
 * @file src/AppSimpleBezier_11.h
 * @author Olivier Falconnet
 * @date 20211004 - File creation
 */

#ifndef NXN_APP_SIMPLE_BEZIER
#define NXN_APP_SIMPLE_BEZIER


#include "App_11.h"

namespace nxn {


    class AppSimpleBezier_11 : public App_11 {

    public:
                        AppSimpleBezier_11  (std::unique_ptr<DX::DeviceResources_11> & p_deviceResources);
                        ~AppSimpleBezier_11 ();

    protected:
        virtual void    PreInit             (); // Performs custom steps (e.g. allocating memory) before entering the main loop
        virtual void    PostInit            (); // Performs custom steps (e.g. allocating memory) before entering the main loop
        virtual void    HandleCleanUp       (); // Performs custom steps (e.g. freeing memory) after exiting the main loop

    }; // class AppSimpleBezier_11

} // namespace nxn


#endif // !NXN_APP_SIMPLE_BEZIER
