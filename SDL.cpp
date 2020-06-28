////
//// Created by ZenYuan on 2020/6/26.
////
//
//#include "common.h"
//
//
//#define SDL_MAIN_HANDLED
//enum KeyPressSurfaces
//{
//    KEY_PRESS_SURFACE_DEFAULT,
//    KEY_PRESS_SURFACE_UP,
//    KEY_PRESS_SURFACE_DOWN,
//    KEY_PRESS_SURFACE_TOTAL
//};
//
//SDL_Window *gWindow = nullptr;
//SDL_Surface *gScreenSurface = nullptr;
//SDL_Event e;
//SDL_Surface *gKeyPressSurface[KEY_PRESS_SURFACE_TOTAL] = {0};
//SDL_Surface *gCurrSurface = nullptr;
//
//SDL_Window *gWindowR = nullptr;
//SDL_Renderer *gRenderer = nullptr;
//SDL_Texture *gTexture = nullptr;
//
//static bool init()
//{
//    bool success = true;
//    if(SDL_Init(SDL_INIT_VIDEO) < 0)
//    {
//        cout<<SDL_GetError()<<endl;
//        success = false;
//    }
//    else
//    {
//        //gWindow = SDL_CreateWindow("yuan", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);
//        gWindowR = SDL_CreateWindow("SDL_Renderer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720, SDL_WINDOW_SHOWN);
//        if(nullptr == gWindow && nullptr == gWindowR)
//        {
//            cout<<SDL_GetError()<<endl;
//            success = false;
//        }
//        else
//        {
//            gScreenSurface = SDL_GetWindowSurface(gWindowR);
//
//            //create renderer for window
//            gRenderer = SDL_CreateRenderer(gWindowR, -1, SDL_RENDERER_ACCELERATED);
//            if(nullptr == gRenderer)
//            {
//                cout<<SDL_GetError()<<endl;
//                success = false;
//            }
//            else
//            {
//                SDL_SetRenderDrawColor(gRenderer, 0xff, 0xff, 0xff, 0xff);
//            }
//        }
//    }
//    return success;
//}
//
//static SDL_Surface* loadSurface(const string& path)
//{
//    SDL_Surface *optimizedSurface = nullptr;
//    char* path_cstr = const_cast<char*>(path.c_str());
//    SDL_Surface* tmpSurface = SDL_LoadBMP(path_cstr);
//    if(nullptr == tmpSurface)
//    {
//        cout<<SDL_GetError()<<endl;
//        return nullptr;
//    }
//    else
//    {
//        //Convert surface to screen surface to avoid per time
//        optimizedSurface = SDL_ConvertSurface(tmpSurface, gScreenSurface->format, 0);
//        SDL_FreeSurface(tmpSurface);
//    }
//    return optimizedSurface;
//}
//
//static bool loadMedia()
//{
//    bool success = true;
//
//    gKeyPressSurface[KEY_PRESS_SURFACE_DEFAULT] = loadSurface("F:\\Project\\Project_clion\\SDL_demo\\resources\\dog.bmp");
//    if(nullptr == gKeyPressSurface[KEY_PRESS_SURFACE_DEFAULT])
//    {
//        success = false;
//    }
//    gKeyPressSurface[KEY_PRESS_SURFACE_UP] = loadSurface("F:\\Project\\Project_clion\\SDL_demo\\resources\\o.bmp");
//    if(nullptr == gKeyPressSurface[KEY_PRESS_SURFACE_UP])
//    {
//        success = false;
//    }
//    gKeyPressSurface[KEY_PRESS_SURFACE_DOWN] = loadSurface("F:\\Project\\Project_clion\\SDL_demo\\resources\\sky.bmp");
//    if(nullptr == gKeyPressSurface[KEY_PRESS_SURFACE_DOWN])
//    {
//        success = false;
//    }
//    return success;
//}
//
//static void close()
//{
//    //Deallocte surface
//    for(int i =0 ; i < KEY_PRESS_SURFACE_TOTAL; i++)
//    {
//        SDL_FreeSurface(gKeyPressSurface[i]);
//        gKeyPressSurface[i] = nullptr;
//    }
//    SDL_FreeSurface(gScreenSurface);
//    gScreenSurface = nullptr;
//
//    //Dsetory window
//    SDL_DestroyWindow(gWindowR);
//    gWindowR = nullptr;
//
//    SDL_DestroyTexture(gTexture);
//    gTexture = nullptr;
//    //Quit SDL subSystem
//    SDL_Quit();
//}
//
//int display()
//{
//    bool quite = false;
//    if(!init())
//    {
//        cout<<"init error\n"<<endl;
//        return 0;
//    }
//    else
//    {
//        if(!loadMedia())
//        {
//            cout<<"loadMedia error\n"<<endl;
//            return 0;
//        }
//        else
//        {
//            SDL_Rect rect;
//            rect.x = 0;
//            rect.y = 0;
//
//            while(!quite)
//            {
//                //Handle
//                while(SDL_PollEvent(&e) != 0)
//                {
//                    if(e.type == SDL_QUIT)
//                    {
//                        quite = true;
//                    }
//                    else if(e.type = SDL_KEYDOWN)
//                    {
//                        switch(e.key.keysym.sym)
//                        {
//                            case SDLK_UP:
//                                gCurrSurface = gKeyPressSurface[KEY_PRESS_SURFACE_DEFAULT];
//                                break;
//                            case SDLK_DOWN:
//                                gCurrSurface = gKeyPressSurface[KEY_PRESS_SURFACE_DOWN];
//                                break;
//                            default:
//                                gCurrSurface = gKeyPressSurface[KEY_PRESS_SURFACE_UP];
//                        }
//                    }
//                }
//                gTexture = SDL_CreateTextureFromSurface(gRenderer, gCurrSurface);
//                //clear screen
//                SDL_RenderClear(gRenderer);
//                //Render texture to screen
//                SDL_RenderCopy(gRenderer, gTexture, nullptr, nullptr);
//                //Update screen
//                SDL_RenderPresent(gRenderer);
//
////                Apply image
////                SDL_BlitScaled(gCurrSurface, nullptr, gScreenSurface, nullptr);
////                Update image
////                SDL_UpdateWindowSurface(gWindow);
//            }
//        }
//    }
//    close();
//}
