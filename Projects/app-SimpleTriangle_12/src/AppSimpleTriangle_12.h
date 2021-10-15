/**
 * 
 * An application is responsible for running the main loop, and holding a variety of managers.
 *
 * @class AppSimpleTriangle_12
 *
 *
 * @file src/AppSimpleTriangle_12.h
 * @author Olivier Falconnet
 * @date 20211014 - File creation
 */

#ifndef NXN_APP_SIMPLE_TRIANGLE_12
#define NXN_APP_SIMPLE_TRIANGLE_12


#include "App_12.h"

namespace nxn {


    class AppSimpleTriangle_12 : public App_12 {

    public:
        AppSimpleTriangle_12            (std::unique_ptr<DX::DeviceResources_12> & p_deviceResources);
        ~AppSimpleTriangle_12           ();

    protected:
        virtual void    PreInit         (); // Performs custom steps (e.g. allocating memory) before entering the main loop
        virtual void    PostInit        (); // Performs custom steps (e.g. allocating memory) before entering the main loop
        virtual void    HandleCleanUp   (); // Performs custom steps (e.g. freeing memory) after exiting the main loop

    }; // class AppSimpleTriangle_12

} // namespace nxn


#endif // !NXN_APP_SIMPLE_TRIANGLE_12
