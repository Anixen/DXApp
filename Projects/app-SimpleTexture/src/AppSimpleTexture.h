/**
 * 
 * An application is responsible for running the main loop, and holding a variety of managers.
 *
 * @class AppSimpleTexture
 *
 *
 * @file src/AppTest.h
 * @author Olivier Falconnet
 * @date 20210929 - File creation
 */

#ifndef NXN_APP_SIMPLE_TEXTURE
#define NXN_APP_SIMPLE_TEXTURE


#include "../../lib-DXApp/src/App.h"

namespace nxn {


    class AppSimpleTexture : public App {

    public:
        AppSimpleTexture();
        ~AppSimpleTexture();

    protected:
        virtual void    PreInit         (); // Performs custom steps (e.g. allocating memory) before entering the main loop
        virtual void    PostInit        (); // Performs custom steps (e.g. allocating memory) before entering the main loop
        virtual void    HandleCleanUp   (); // Performs custom steps (e.g. freeing memory) after exiting the main loop

    }; // class AppSimpleTexture

} // namespace nxn


#endif // !NXN_APP_SIMPLE_TEXTURE

