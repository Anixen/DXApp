/**
 * 
 * An application is responsible for running the main loop, and holding a variety of managers.
 *
 * @class AppSimpleMSAA_11
 *
 *
 * @file src/AppSimpleMSAA_11.h
 * @author Olivier Falconnet
 * @date 20211005 - File creation
 */

#ifndef NXN_APP_SIMPLE_MSAA_11
#define NXN_APP_SIMPLE_MSAA_11


#include "App_11.h"

namespace nxn {


    class AppSimpleMSAA_11 : public App_11 {

    public:
                        AppSimpleMSAA_11        (std::unique_ptr<DX::DeviceResources_11> & p_deviceResources);
                        ~AppSimpleMSAA_11       ();

        virtual void	GetDefaultWindowSize    (int& width, int& height) const;

    protected:
        virtual void    PreInit                 (); // Performs custom steps (e.g. allocating memory) before entering the main loop
        virtual void    PostInit                (); // Performs custom steps (e.g. allocating memory) before entering the main loop
        virtual void    HandleCleanUp           (); // Performs custom steps (e.g. freeing memory) after exiting the main loop

    }; // class AppSimpleMSAA_11

} // namespace nxn


#endif // !NXN_APP_SIMPLE_MSAA_11
