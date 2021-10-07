/**
 * 
 * An application is responsible for running the main loop, and holding a variety of managers.
 *
 * @class AppSimpleTriangle
 *
 *
 * @file src/AppSimpleTriangle.h
 * @author Olivier Falconnet
 * @date 20211003 - File creation
 */

#ifndef NXN_APP_SIMPLE_TRIANGLE
#define NXN_APP_SIMPLE_TRIANGLE


#include "../../lib-DXApp/src/App.h"

namespace nxn {


    class AppSimpleTriangle : public App {

    public:
        AppSimpleTriangle               (std::unique_ptr<DX::DeviceResources> & p_deviceResources);
        ~AppSimpleTriangle              ();

    protected:
        virtual void    PreInit         (); // Performs custom steps (e.g. allocating memory) before entering the main loop
        virtual void    PostInit        (); // Performs custom steps (e.g. allocating memory) before entering the main loop
        virtual void    HandleCleanUp   (); // Performs custom steps (e.g. freeing memory) after exiting the main loop

    }; // class AppSimpleTriangle

} // namespace nxn


#endif // !NXN_APP_SIMPLE_TRIANGLE
