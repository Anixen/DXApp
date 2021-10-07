/**
 * 
 * An application is responsible for running the main loop, and holding a variety of managers.
 *
 * @class AppSimpleMSAA
 *
 *
 * @file src/AppSimpleMSAA.h
 * @author Olivier Falconnet
 * @date 20211005 - File creation
 */

#ifndef NXN_APP_SIMPLE_MSAA
#define NXN_APP_SIMPLE_MSAA


#include "../../lib-DXApp/src/App.h"

namespace nxn {


    class AppSimpleMSAA : public App {

    public:
        AppSimpleMSAA                           (std::unique_ptr<DX::DeviceResources> & p_deviceResources);
        ~AppSimpleMSAA                          ();

        virtual void	GetDefaultWindowSize    (int& width, int& height) const;

    protected:
        virtual void    PreInit                 (); // Performs custom steps (e.g. allocating memory) before entering the main loop
        virtual void    PostInit                (); // Performs custom steps (e.g. allocating memory) before entering the main loop
        virtual void    HandleCleanUp           (); // Performs custom steps (e.g. freeing memory) after exiting the main loop

    }; // class AppSimpleMSAA

} // namespace nxn


#endif // !NXN_APP_SIMPLE_MSAA
