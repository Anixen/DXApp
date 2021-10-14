/**
 * 
 * An application is responsible for running the main loop, and holding a variety of managers.
 *
 * @class AppSimpleLighting_11
 *
 *
 * @file src/AppSimpleLighting_11.h
 * @author Olivier Falconnet
 * @date 20211004 - File creation
 */

#ifndef NXN_APP_SIMPLE_LIGHTING
#define NXN_APP_SIMPLE_LIGHTING


#include "../../lib-DXApp/src/11/App_11.h"

namespace nxn {


    class AppSimpleLighting_11 : public App_11 {

    public:
        AppSimpleLighting_11            (std::unique_ptr<DX::DeviceResources_11> & p_deviceResources);
        ~AppSimpleLighting_11           ();

    protected:
        virtual void    PreInit         (); // Performs custom steps (e.g. allocating memory) before entering the main loop
        virtual void    PostInit        (); // Performs custom steps (e.g. allocating memory) before entering the main loop
        virtual void    HandleCleanUp   (); // Performs custom steps (e.g. freeing memory) after exiting the main loop

    }; // class AppSimpleLighting_11

} // namespace nxn


#endif // !NXN_APP_SIMPLE_LIGHTING
