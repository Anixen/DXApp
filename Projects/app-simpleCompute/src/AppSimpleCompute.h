/**
 * 
 * An application is responsible for running the main loop, and holding a variety of managers.
 *
 * @class AppSimpleCompute
 *
 *
 * @file src/AppSimpleCompute.h
 * @author Olivier Falconnet
 * @date 20211005 - File creation
 */

#ifndef NXN_APP_SIMPLE_COMPUTE
#define NXN_APP_SIMPLE_COMPUTE


#include "../../lib-DXApp/src/App.h"

namespace nxn {


    class AppSimpleCompute : public App {

    public:
        AppSimpleCompute                        ();
        ~AppSimpleCompute                       ();

        virtual void	GetDefaultWindowSize    (int& width, int& height) const;

    protected:
        virtual void    PreInit                 (); // Performs custom steps (e.g. allocating memory) before entering the main loop
        virtual void    PostInit                (); // Performs custom steps (e.g. allocating memory) before entering the main loop
        virtual void    HandleCleanUp           (); // Performs custom steps (e.g. freeing memory) after exiting the main loop

    }; // class AppSimpleCoompute

} // namespace nxn


#endif // !NXN_APP_SIMPLE_COMPUTE
