/**
 * 
 * An application is responsible for running the main loop, and holding a variety of managers.
 *
 * @class AppSimpleMSAA_12
 *
 *
 * @file src/AppSimpleMSAA_12.h
 * @author Olivier Falconnet
 * @date 20211120 - File creation
 */

#ifndef NXN_APP_SIMPLE_MSAA_12
#define NXN_APP_SIMPLE_MSAA_12


#include "App_12.h"

namespace nxn {


    class AppSimpleMSAA_12 : public App_12 {

    public:
                        AppSimpleMSAA_12        (std::unique_ptr<DX::DeviceResources_12> & p_deviceResources);
                        ~AppSimpleMSAA_12       ();

        virtual void	GetDefaultWindowSize    (int& width, int& height) const;

    protected:
        virtual void    PreInit                 (); // Performs custom steps (e.g. allocating memory) before entering the main loop
        virtual void    PostInit                (); // Performs custom steps (e.g. allocating memory) before entering the main loop
        virtual void    HandleCleanUp           (); // Performs custom steps (e.g. freeing memory) after exiting the main loop

    }; // class AppSimpleMSAA_12

} // namespace nxn


#endif // !NXN_APP_SIMPLE_MSAA_12
