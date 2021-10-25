/**
 * 
 * An application is responsible for running the main loop, and holding a variety of managers.
 *
 * @class AppSimpleTexture_11
 *
 *
 * @file src/AppTest.h
 * @author Olivier Falconnet
 * @date 20211003 - File creation
 */

#ifndef NXN_APP_SIMPLE_TEXTURE
#define NXN_APP_SIMPLE_TEXTURE


#include "App_11.h"

namespace nxn {


    class AppSimpleTexture_11 : public App_11 {

    public:
                        AppSimpleTexture_11     (std::unique_ptr<DX::DeviceResources_11> & p_deviceResources);
                        ~AppSimpleTexture_11    ();

    protected:
        virtual void    PreInit                 (); // Performs custom steps (e.g. allocating memory) before entering the main loop
        virtual void    PostInit                (); // Performs custom steps (e.g. allocating memory) before entering the main loop
        virtual void    HandleCleanUp           (); // Performs custom steps (e.g. freeing memory) after exiting the main loop

    }; // class AppSimpleTexture_11

} // namespace nxn


#endif // !NXN_APP_SIMPLE_TEXTURE
