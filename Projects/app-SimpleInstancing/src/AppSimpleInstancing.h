/**
 * 
 * An application is responsible for running the main loop, and holding a variety of managers.
 *
 * @class AppSimpleInstancing
 *
 *
 * @file src/AppSimpleInstancing.h
 * @author Olivier Falconnet
 * @date 20211005 - File creation
 */

#ifndef NXN_APP_SIMPLE_INSTANCING
#define NXN_APP_SIMPLE_INSTANCING


#include "../../lib-DXApp/src/App.h"

namespace nxn {


    class AppSimpleInstancing : public App {

    public:
        AppSimpleInstancing                     ();
        ~AppSimpleInstancing                    ();

        virtual void	GetDefaultWindowSize    (int& width, int& height) const;

    protected:
        virtual void    PreInit                 (); // Performs custom steps (e.g. allocating memory) before entering the main loop
        virtual void    PostInit                (); // Performs custom steps (e.g. allocating memory) before entering the main loop
        virtual void    HandleCleanUp           (); // Performs custom steps (e.g. freeing memory) after exiting the main loop

    }; // class AppSimpleInstancing

} // namespace nxn


#endif // !NXN_APP_SIMPLE_INSTANCING
